#include "ColorEditor.h"
#include "TrackView.h"
#include <libJModule.h>
#include "ColorEditorCallOutBoxLAF.h"

ColorHistory::ColorHistory(const juce::String& path)
	: path(path)
{
	//获取设置
	{
		juce::var* config = nullptr;
		bool ok = false;
		jmadf::CallInterface<const juce::String&, juce::var*&, bool&>(
			"WuChang.JMADF.Configs", "GetReference",
			"config", config, ok
			);
		if (ok && config->isObject()) {
			this->totalSize = (*config)["ColorEditorHistoryMaxCount"];
		}
	}

	this->load();
}

bool ColorHistory::load()
{
	this->colorList.clear();
	juce::File hisFile(jmadf::GetModuleInfo()->path + this->path);
	if (hisFile.existsAsFile()) {
		juce::var dataTemp = juce::JSON::parse(hisFile);
		if (dataTemp.isArray()) {
			auto& array = *(dataTemp.getArray());
			for (auto& i : array) {
				if (i.isString()) {
					this->colorList.add(juce::Colour::fromString(i.toString()));
				}
			}

			//干掉多余的
			if (this->colorList.size() > this->getSizeTotal()) {
				this->colorList.resize(this->getSizeTotal());
			}
			return true;
		}
	}
	return false;
}

bool ColorHistory::save()
{
	juce::StringArray array;
	for (auto& i : this->colorList) {
		array.add(i.toString());
	}

	juce::File hisFile(jmadf::GetModuleInfo()->path + this->path);
	juce::var dataTemp(array);
	hisFile.replaceWithText(
		juce::JSON::toString(dataTemp)
	);
	return true;
}

int ColorHistory::getSizeUsed()
{
	return this->colorList.size();
}

int ColorHistory::getSizeTotal()
{
	return this->totalSize;
}

const juce::Array<juce::Colour>& ColorHistory::getList()
{
	return this->colorList;
}

void ColorHistory::acceptColor(juce::Colour color)
{
	//如果已存在
	auto index = this->colorList.indexOf(color);
	if (index >= 0 && index < this->colorList.size()) {
		this->colorList.move(index, 0);
		return;
	}

	//插入在头部
	this->colorList.insert(0, color);

	//干掉多余的
	if (this->colorList.size() > this->getSizeTotal()) {
		this->colorList.resize(this->getSizeTotal());
	}

	//保存
	this->save();
}

ColorEditor::ColourSpaceView::ColourSpaceView(ColorEditor& cs)
	: owner(cs)
{
	//添加标记控件
	this->addAndMakeVisible(this->marker);
	this->setMouseCursor(juce::MouseCursor::CrosshairCursor);
}

void ColorEditor::ColourSpaceView::setCurrentColor(float hue, float sat, float bri)
{
	float lastHue = this->h;

	this->h = hue;
	this->s = sat;
	this->v = bri;

	if (lastHue != this->h)
	{
		this->colours = {};
		this->repaint();
	}

	this->updateMarker();
}

void ColorEditor::ColourSpaceView::paint(juce::Graphics& g)
{
	if (this->colours.isNull()) {
		auto width = getWidth() / 2;
		auto height = getHeight() / 2;
		this->colours = juce::Image(juce::Image::RGB, width, height, false);

		juce::Image::BitmapData pixels(this->colours, juce::Image::BitmapData::writeOnly);

		for (int y = 0; y < height; ++y) {
			auto val = 1.0f - (float)y / (float)height;

			for (int x = 0; x < width; ++x) {
				auto sat = (float)x / (float)width;
				pixels.setPixelColour(x, y, juce::Colour(h, sat, val, 1.0f));
			}
		}
	}

	g.setOpacity(1.0f);
	g.drawImageTransformed(this->colours,
		juce::RectanglePlacement(juce::RectanglePlacement::stretchToFit)
		.getTransformToFit(this->colours.getBounds().toFloat(),
			this->getLocalBounds().toFloat()),
		false);
}

void ColorEditor::ColourSpaceView::mouseDown(const juce::MouseEvent& e)
{
	this->mouseDrag(e);
}

void ColorEditor::ColourSpaceView::mouseDrag(const juce::MouseEvent& e)
{
	auto posX = e.x;
	auto posY = e.y;

	if (posX < 0) { posX = 0; }
	if (posX > this->getWidth()) { posX = this->getWidth(); }
	if (posY < 0) { posY = 0; }
	if (posY > this->getHeight()) { posY = this->getHeight(); }

	auto sat = (float)posX / (float)this->getWidth();
	auto val = 1.0f - (float)posY / (float)this->getHeight();

	this->owner.selectColor(this->h, sat, val);
}

void ColorEditor::ColourSpaceView::resized()
{
	this->colours = {};
	this->marker.setBounds(0, 0, this->getWidth(), this->getHeight());
	this->updateMarker();
}

void ColorEditor::ColourSpaceView::updateMarker()
{
	this->marker.setPos(this->s, 1.0f - this->v);
}

ColorEditor::ColourSpaceView::ColourSpaceMarker::ColourSpaceMarker()
{
	this->setInterceptsMouseClicks(false, false);

	//获取屏幕属性接口
	this->screenSizeFunc =
		jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSize"
			);

	//以下获取界面属性
	bool result = false;
	//color
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "colorEditorColorSpaceCursor", this->colors.colorEditorColorSpaceCursor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "colorEditorColorSpaceCursorBorder", this->colors.colorEditorColorSpaceCursorBorder, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorColorSpaceCursor", this->sizes.width_colorEditorColorSpaceCursor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorColorSpaceCursorLine", this->sizes.width_colorEditorColorSpaceCursorLine, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorColorSpaceCursorBorder", this->sizes.width_colorEditorColorSpaceCursorBorder, result
		);
}

void ColorEditor::ColourSpaceView::ColourSpaceMarker::paint(juce::Graphics& g)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	float posX = this->pX * this->getWidth();
	float posY = this->pY * this->getHeight();

	float width_cursor = this->sizes.width_colorEditorColorSpaceCursor * screenSize.getWidth();
	float width_cursorLine = this->sizes.width_colorEditorColorSpaceCursorLine * screenSize.getWidth();
	float width_cursorBorder = this->sizes.width_colorEditorColorSpaceCursorBorder * screenSize.getWidth();

	//绘制指针体
	juce::Rectangle<float> rectCursor(
		posX - width_cursor / 2, posY - width_cursor / 2,
		width_cursor, width_cursor
	);
	g.setColour(this->colors.colorEditorColorSpaceCursor);
	g.drawEllipse(rectCursor, width_cursorLine);

	//绘制指针边框
	juce::Rectangle<float> rectCursorBorderInside(
		posX - width_cursor / 2 + width_cursorLine / 2,
		posY - width_cursor / 2 + width_cursorLine / 2,
		width_cursor - width_cursorLine, width_cursor - width_cursorLine
	);
	juce::Rectangle<float> rectCursorBorderOutside(
		posX - width_cursor / 2 - width_cursorLine / 2,
		posY - width_cursor / 2 - width_cursorLine / 2,
		width_cursor + width_cursorLine, width_cursor + width_cursorLine
	);
	g.setColour(this->colors.colorEditorColorSpaceCursorBorder);
	g.drawEllipse(rectCursorBorderInside, width_cursorBorder);
	g.drawEllipse(rectCursorBorderOutside, width_cursorBorder);
}

void ColorEditor::ColourSpaceView::ColourSpaceMarker::setPos(float pX, float pY)
{
	this->pX = pX;
	this->pY = pY;
	this->repaint();
}

ColorEditor::HueSelectorComp::HueSelectorComp(ColorEditor& cs)
	: owner(cs)
{
	//获取屏幕属性接口
	this->screenSizeFunc =
		jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSize"
			);

	//以下获取界面属性
	bool result = false;
	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorHueSpaceCorner", this->sizes.width_colorEditorHueSpaceCorner, result
		);

	//添加标记控件
	this->addAndMakeVisible(this->marker);
	this->setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void ColorEditor::HueSelectorComp::setCurrentColor(float hue, float sat, float bri)
{
	this->h = hue;
	this->s = sat;
	this->v = bri;

	this->repaint();
	this->updateMarker();
}

void ColorEditor::HueSelectorComp::paint(juce::Graphics& g)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	float width_corner = this->sizes.width_colorEditorHueSpaceCorner * screenSize.getWidth();

	//计算渐变
	juce::ColourGradient cg;
	cg.isRadial = false;
	cg.point1.setXY(0.0f, (float)0);
	cg.point2.setXY(0.0f, (float)this->getHeight());

	for (float i = 0.0f; i <= 1.0f; i += 0.02f) {
		cg.addColour(i, juce::Colour(i, 1.0f, 1.0f, 1.0f));
	}

	//绘制渐变
	g.setGradientFill(cg);
	g.fillRoundedRectangle(this->getLocalBounds().toFloat(), width_corner);
}

void ColorEditor::HueSelectorComp::resized()
{
	this->marker.setBounds(0, 0, this->getWidth(), this->getHeight());
	this->updateMarker();
}

void ColorEditor::HueSelectorComp::mouseDown(const juce::MouseEvent& e)
{
	this->mouseDrag(e);
}

void ColorEditor::HueSelectorComp::mouseDrag(const juce::MouseEvent& e)
{
	auto posY = e.y;

	if (posY < 0) { posY = 0; }
	if (posY > this->getHeight()) { posY = this->getHeight(); }

	auto hue = (float)posY / (float)this->getHeight();
	this->owner.selectColor(hue, this->s, this->v);
}

void ColorEditor::HueSelectorComp::updateMarker()
{
	this->marker.setPos(this->h);
}

ColorEditor::HueSelectorComp::HueSelectorMarker::HueSelectorMarker()
{
	this->setInterceptsMouseClicks(false, false);

	//获取屏幕属性接口
	this->screenSizeFunc =
		jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSize"
			);

	//以下获取界面属性
	bool result = false;
	//color
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "colorEditorHueSpaceCursor", this->colors.colorEditorHueSpaceCursor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "colorEditorHueSpaceCursorBorder", this->colors.colorEditorHueSpaceCursorBorder, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorHueSpaceCursorLine", this->sizes.width_colorEditorHueSpaceCursorLine, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorHueSpaceCursorBorder", this->sizes.width_colorEditorHueSpaceCursorBorder, result
		);
}

void ColorEditor::HueSelectorComp::HueSelectorMarker::paint(juce::Graphics& g)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	float posX = 0.5 * this->getWidth();
	float posY = this->pY * this->getHeight();

	float width_cursorLine = this->sizes.width_colorEditorHueSpaceCursorLine * screenSize.getWidth();
	float width_cursorBorder = this->sizes.width_colorEditorHueSpaceCursorBorder * screenSize.getWidth();
	float width_cursor = this->getWidth() - width_cursorLine - width_cursorBorder;

	//绘制指针体
	juce::Rectangle<float> rectCursor(
		posX - width_cursor / 2, posY - width_cursor / 2,
		width_cursor, width_cursor
	);
	g.setColour(this->colors.colorEditorHueSpaceCursor);
	g.drawEllipse(rectCursor, width_cursorLine);

	//绘制指针边框
	juce::Rectangle<float> rectCursorBorderInside(
		posX - width_cursor / 2 + width_cursorLine / 2,
		posY - width_cursor / 2 + width_cursorLine / 2,
		width_cursor - width_cursorLine, width_cursor - width_cursorLine
	);
	juce::Rectangle<float> rectCursorBorderOutside(
		posX - width_cursor / 2 - width_cursorLine / 2,
		posY - width_cursor / 2 - width_cursorLine / 2,
		width_cursor + width_cursorLine, width_cursor + width_cursorLine
	);
	g.setColour(this->colors.colorEditorHueSpaceCursorBorder);
	g.drawEllipse(rectCursorBorderInside, width_cursorBorder);
	g.drawEllipse(rectCursorBorderOutside, width_cursorBorder);
}

void ColorEditor::HueSelectorComp::HueSelectorMarker::setPos(float pY)
{
	this->pY = pY;
	this->repaint();
}

ColorEditor::ColorEditor(TrackView* parent)
	: Component("Color Editor"),
	parent(parent)
{
	//获取屏幕属性接口
	this->screenSizeFunc =
		jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSize"
			);

	//获取翻译器
	jmadf::CallInterface<std::function<const juce::String(const juce::String&)>&>(
		"WuChang.JMADF.Translates", "GetFunc",
		this->tr
		);

	//获取设置
	{
		juce::var* config = nullptr;
		bool ok = false;
		jmadf::CallInterface<const juce::String&, juce::var*&, bool&>(
			"WuChang.JMADF.Configs", "GetReference",
			"config", config, ok
			);
		if (ok && config->isObject()) {
			this->templateXSize = (*config)["ColorEditorTemplateXSize"];
		}
	}

	//以下获取界面属性
	bool result = false;
	//color
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-colorEditor", this->colors.background_colorEditor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-colorEditorButton", this->colors.text_colorEditorButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-colorEditorButton", this->colors.background_colorEditorButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "splitLine-colorEditor", this->colors.splitLine_colorEditor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-colorEditorTitle", this->colors.text_colorEditorTitle, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "colorEditorHistoryDefaultColor", this->colors.colorEditorHistoryDefaultColor, result
		);

	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "themeColor0", this->colors.themeColor0, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "themeColor1", this->colors.themeColor1, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "themeColor2", this->colors.themeColor2, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "themeColor3", this->colors.themeColor3, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "themeColor4", this->colors.themeColor4, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "themeColor5", this->colors.themeColor5, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "themeColor6", this->colors.themeColor6, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-colorEditorMarginTop", this->sizes.height_colorEditorMarginTop, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-colorEditorMarginBottom", this->sizes.height_colorEditorMarginBottom, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorMarginLeft", this->sizes.width_colorEditorMarginLeft, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorMarginRight", this->sizes.width_colorEditorMarginRight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorButton", this->sizes.width_colorEditorButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorSplitLine", this->sizes.width_colorEditorSplitLine, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-colorEditorSplitLine", this->sizes.height_colorEditorSplitLine, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorSplit", this->sizes.width_colorEditorSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-colorEditorTitleFont", this->sizes.height_colorEditorTitleFont, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorTitleLeftMargin", this->sizes.width_colorEditorTitleLeftMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorColorTemplate", this->sizes.width_colorEditorColorTemplate, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-colorEditorColorTemplate", this->sizes.height_colorEditorColorTemplate, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorColorTemplateCorner", this->sizes.width_colorEditorColorTemplateCorner, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorColorTemplateSplit", this->sizes.width_colorEditorColorTemplateSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-colorEditorColorTemplateSplit", this->sizes.height_colorEditorColorTemplateSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorColorSpace", this->sizes.width_colorEditorColorSpace, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorHueSpace", this->sizes.width_colorEditorHueSpace, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-colorEditorColorSpaceSplit", this->sizes.width_colorEditorColorSpaceSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-colorEditorColorSpace", this->sizes.height_colorEditorColorSpace, result
		);
	//position
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "position", "posY-colorEditorButton", this->positions.posY_colorEditorButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "position", "posX-colorEditorButton", this->positions.posX_colorEditorButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "position", "posX-colorEditorSplit", this->positions.posX_colorEditorSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "position", "posY-colorEditorSplitLine", this->positions.posY_colorEditorSplitLine, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "position", "posY-colorEditorLeftSplit1", this->positions.posY_colorEditorLeftSplit1, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "position", "posY-colorEditorLeftSplit2", this->positions.posY_colorEditorLeftSplit2, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "position", "posY-colorEditorLeftSplit3", this->positions.posY_colorEditorLeftSplit3, result
		);
	//scale
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-colorEditorButton", this->scales.height_colorEditorButton, result
		);

	//建立确定按钮样式
	this->lafs.button = std::unique_ptr<juce::LookAndFeel>(new juce::LookAndFeel_V4);
	this->lafs.button->setColour(
		juce::TextButton::ColourIds::buttonColourId, this->colors.background_colorEditorButton);
	this->lafs.button->setColour(
		juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_colorEditorButton);
	this->lafs.button->setColour(
		juce::TextButton::ColourIds::textColourOffId, this->colors.text_colorEditorButton);
	this->lafs.button->setColour(
		juce::TextButton::ColourIds::textColourOnId, this->colors.text_colorEditorButton);
	this->lafs.button->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//初始化确定按钮
	this->okButton = std::make_unique<juce::TextButton>(this->tr("bt_Accept"));
	this->okButton->setLookAndFeel(this->lafs.button.get());
	this->okButton->setMouseCursor(juce::MouseCursor::PointingHandCursor);
	this->okButton->onClick = [this] {this->acceptAndClose(); };
	this->okButton->setEnabled(false);
	this->addAndMakeVisible(this->okButton.get());

	//建立历史
	this->history = std::make_unique<ColorHistory>();

	//建立颜色控件
	this->colorSpace = std::make_unique<ColorEditor::ColourSpaceView>(*this);
	this->addAndMakeVisible(this->colorSpace.get());

	this->hueView = std::make_unique<ColorEditor::HueSelectorComp>(*this);
	this->addAndMakeVisible(this->hueView.get());
}

void ColorEditor::resized()
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	int height_marginTop = this->sizes.height_colorEditorMarginTop * screenSize.getHeight();
	int height_marginBottom = this->sizes.height_colorEditorMarginBottom * screenSize.getHeight();
	int width_marginLeft = this->sizes.width_colorEditorMarginLeft * screenSize.getWidth();
	int width_marginRight = this->sizes.width_colorEditorMarginRight * screenSize.getWidth();

	int height_inside = this->getHeight() - height_marginTop - height_marginBottom;
	int width_inside = this->getWidth() - width_marginLeft - width_marginRight;

	int width_button = this->sizes.width_colorEditorButton * screenSize.getWidth();

	int posY_buttonArea = height_marginTop +
		this->positions.posY_colorEditorButton * height_inside;

	int posX_button = width_marginLeft +
		this->positions.posX_colorEditorButton * width_inside;

	int height_button = (this->getHeight() - height_marginBottom - posY_buttonArea) * this->scales.height_colorEditorButton;

	int posY_button = posY_buttonArea + (this->getHeight() - height_marginBottom - posY_buttonArea) / 2 - height_button / 2;

	float posX_splitLine = width_marginLeft + width_inside * this->positions.posX_colorEditorSplit;
	float posY_splitLine = height_marginTop + height_inside * this->positions.posY_colorEditorSplitLine;

	float width_splitLine = this->sizes.width_colorEditorSplitLine * screenSize.getWidth();
	float height_splitLine = this->sizes.height_colorEditorSplitLine * screenSize.getHeight();

	float width_split = this->sizes.width_colorEditorSplit * screenSize.getWidth();

	int width_colorSpace = this->sizes.width_colorEditorColorSpace * screenSize.getWidth();
	int width_hueViewer = this->sizes.width_colorEditorHueSpace * screenSize.getWidth();
	int width_colorSpaceSplit = this->sizes.width_colorEditorColorSpaceSplit * screenSize.getWidth();
	int height_colorSpace = this->sizes.height_colorEditorColorSpace * screenSize.getHeight();

	//左右绘图区
	juce::Rectangle<float> rectLeftArea(
		width_marginLeft, height_marginTop,
		posX_splitLine - width_split / 2 - width_marginLeft,
		posY_buttonArea - height_marginTop
	);
	juce::Rectangle<float> rectRightArea(
		posX_splitLine + width_split / 2, height_marginTop,
		this->getWidth() - width_marginRight - (posX_splitLine + width_split / 2),
		posY_buttonArea - height_marginTop
	);

	//计算右侧控件位置
	int posX_colorSpace = rectRightArea.getX();
	int posY_colorSpace = rectRightArea.getY();
	int posX_hueViewer = posX_colorSpace + width_colorSpace + width_colorSpaceSplit;

	//调整控件大小
	this->okButton->setBounds(posX_button, posY_button, width_button, height_button);
	this->colorSpace->setBounds(posX_colorSpace, posY_colorSpace, width_colorSpace, height_colorSpace);
	this->hueView->setBounds(posX_hueViewer, posY_colorSpace, width_hueViewer, height_colorSpace);
}

void ColorEditor::paint(juce::Graphics& g)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	int height_marginTop = this->sizes.height_colorEditorMarginTop * screenSize.getHeight();
	int height_marginBottom = this->sizes.height_colorEditorMarginBottom * screenSize.getHeight();
	int width_marginLeft = this->sizes.width_colorEditorMarginLeft * screenSize.getWidth();
	int width_marginRight = this->sizes.width_colorEditorMarginRight * screenSize.getWidth();

	int height_inside = this->getHeight() - height_marginTop - height_marginBottom;
	int width_inside = this->getWidth() - width_marginLeft - width_marginRight;

	int posY_buttonArea = height_marginTop +
		this->positions.posY_colorEditorButton * height_inside;

	float posX_splitLine = width_marginLeft + width_inside * this->positions.posX_colorEditorSplit;
	float posY_splitLine = height_marginTop + height_inside * this->positions.posY_colorEditorSplitLine;

	float width_splitLine = this->sizes.width_colorEditorSplitLine * screenSize.getWidth();
	float height_splitLine = this->sizes.height_colorEditorSplitLine * screenSize.getHeight();

	float width_split = this->sizes.width_colorEditorSplit * screenSize.getWidth();

	float height_titleFont = this->sizes.height_colorEditorTitleFont * screenSize.getHeight();

	float width_titleMarginLeft = this->sizes.width_colorEditorTitleLeftMargin * screenSize.getWidth();

	//左右绘图区
	juce::Rectangle<float> rectLeftArea(
		width_marginLeft, height_marginTop,
		posX_splitLine - width_split / 2 - width_marginLeft,
		posY_buttonArea - height_marginTop
	);
	juce::Rectangle<float> rectRightArea(
		posX_splitLine + width_split / 2, height_marginTop,
		this->getWidth() - width_marginRight - (posX_splitLine + width_split / 2),
		posY_buttonArea - height_marginTop
	);

	//计算左侧区域控件大小
	float posY_themeTitle = rectLeftArea.getY();
	float posY_themeArea = rectLeftArea.getY() + rectLeftArea.getHeight() * this->positions.posY_colorEditorLeftSplit1;
	float posY_historyTitle = rectLeftArea.getY() + rectLeftArea.getHeight() * this->positions.posY_colorEditorLeftSplit2;
	float posY_historyArea = rectLeftArea.getY() + rectLeftArea.getHeight() * this->positions.posY_colorEditorLeftSplit3;

	//计算左侧色块大小
	float width_colorTemplate = this->sizes.width_colorEditorColorTemplate * screenSize.getWidth();
	float height_colorTemplate = this->sizes.height_colorEditorColorTemplate * screenSize.getHeight();
	float width_colorTemplateSplit = this->sizes.width_colorEditorColorTemplateSplit * screenSize.getWidth();
	float height_colorTemplateSplit = this->sizes.height_colorEditorColorTemplateSplit * screenSize.getHeight();
	float width_colorTemplateCorner = this->sizes.width_colorEditorColorTemplateCorner * screenSize.getWidth();

	float colorTotalWidth = width_colorTemplate * this->templateXSize + width_colorTemplateSplit * (this->templateXSize - 1);
	float colorTemplatePosX = rectLeftArea.getX() + rectLeftArea.getWidth() / 2 - colorTotalWidth / 2;
	float themeColorTemplatePosY = posY_themeArea;
	float historyColorTemplatePosY = posY_historyArea;

	//计算字体
	juce::Font fontTitle = g.getCurrentFont().withHeight(height_titleFont);

	//绘制基准线
	//begin test
	/*{
		g.setColour(juce::Colours::black);
		g.drawRect(rectLeftArea, 1.f);
		g.drawRect(rectRightArea, 1.f);
	}*/
	//end test

	//绘制分割线
	{
		juce::Rectangle<float> rectSplitLine(
			posX_splitLine - width_splitLine / 2, posY_splitLine,
			width_splitLine, height_splitLine
		);
		g.setColour(this->colors.splitLine_colorEditor);
		g.fillRect(rectSplitLine);
	}

	//绘制左侧文字
	{
		juce::String strThemeTitle(this->tr("lb_ThemeColor"));
		juce::String strHistoryTitle(this->tr("lb_HistoryColor"));

		juce::Rectangle<int> rectThemeTitle(
			rectLeftArea.getX() + width_titleMarginLeft, posY_themeTitle,
			rectLeftArea.getWidth() - width_titleMarginLeft,
			posY_themeArea - posY_themeTitle
		);
		juce::Rectangle<int> rectHistoryTitle(
			rectLeftArea.getX() + width_titleMarginLeft, posY_historyTitle,
			rectLeftArea.getWidth() - width_titleMarginLeft,
			posY_historyArea - posY_historyTitle
		);

		g.setFont(fontTitle);
		g.setColour(this->colors.text_colorEditorTitle);
		g.drawFittedText(strThemeTitle, rectThemeTitle,
			juce::Justification::centredLeft, 1);
		g.drawFittedText(strHistoryTitle, rectHistoryTitle,
			juce::Justification::centredLeft, 1);
	}

	//绘制主题颜色块
	{
		juce::Rectangle<float> rect11(
			colorTemplatePosX + width_colorTemplate * 0 + width_colorTemplateSplit * 0,
			themeColorTemplatePosY + height_colorTemplate * 0 + height_colorTemplateSplit * 0,
			width_colorTemplate, height_colorTemplate
		);
		juce::Rectangle<float> rect12(
			colorTemplatePosX + width_colorTemplate * 1 + width_colorTemplateSplit * 1,
			themeColorTemplatePosY + height_colorTemplate * 0 + height_colorTemplateSplit * 0,
			width_colorTemplate, height_colorTemplate
		);

		juce::Rectangle<float> rect21(
			colorTemplatePosX + width_colorTemplate * 0 + width_colorTemplateSplit * 0,
			themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
			width_colorTemplate, height_colorTemplate
		);
		juce::Rectangle<float> rect22(
			colorTemplatePosX + width_colorTemplate * 1 + width_colorTemplateSplit * 1,
			themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
			width_colorTemplate, height_colorTemplate
		);
		juce::Rectangle<float> rect23(
			colorTemplatePosX + width_colorTemplate * 2 + width_colorTemplateSplit * 2,
			themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
			width_colorTemplate, height_colorTemplate
		);
		juce::Rectangle<float> rect24(
			colorTemplatePosX + width_colorTemplate * 3 + width_colorTemplateSplit * 3,
			themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
			width_colorTemplate, height_colorTemplate
		);
		juce::Rectangle<float> rect25(
			colorTemplatePosX + width_colorTemplate * 4 + width_colorTemplateSplit * 4,
			themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
			width_colorTemplate, height_colorTemplate
		);
		juce::Rectangle<float> rect26(
			colorTemplatePosX + width_colorTemplate * 5 + width_colorTemplateSplit * 5,
			themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
			width_colorTemplate, height_colorTemplate
		);

		if (this->hasSinger) {
			g.setColour(this->singerColor);
			g.fillRoundedRectangle(rect11, width_colorTemplateCorner);
			g.setColour(this->colors.themeColor0);
			g.fillRoundedRectangle(rect12, width_colorTemplateCorner);
		}
		else {
			g.setColour(this->colors.themeColor0);
			g.fillRoundedRectangle(rect11, width_colorTemplateCorner);
		}
		
		g.setColour(this->colors.themeColor1);
		g.fillRoundedRectangle(rect21, width_colorTemplateCorner);
		g.setColour(this->colors.themeColor2);
		g.fillRoundedRectangle(rect22, width_colorTemplateCorner);
		g.setColour(this->colors.themeColor3);
		g.fillRoundedRectangle(rect23, width_colorTemplateCorner);
		g.setColour(this->colors.themeColor4);
		g.fillRoundedRectangle(rect24, width_colorTemplateCorner);
		g.setColour(this->colors.themeColor5);
		g.fillRoundedRectangle(rect25, width_colorTemplateCorner);
		g.setColour(this->colors.themeColor6);
		g.fillRoundedRectangle(rect26, width_colorTemplateCorner);
	}

	//绘制历史颜色块
	{
		auto& historyArray = this->history->getList();
		int totalSize = this->history->getSizeTotal();

		for (int i = 0; i < totalSize; i++) {
			int col = i % this->templateXSize;
			int row = i / this->templateXSize;

			juce::Rectangle<float> colorRect(
				colorTemplatePosX + width_colorTemplate * col + width_colorTemplateSplit * col,
				historyColorTemplatePosY + height_colorTemplate * row + height_colorTemplateSplit * row,
				width_colorTemplate, height_colorTemplate
			);

			if (i < historyArray.size()) {
				g.setColour(historyArray.getReference(i));
			}
			else {
				g.setColour(this->colors.colorEditorHistoryDefaultColor);
			}

			g.fillRoundedRectangle(colorRect, width_colorTemplateCorner);
		}
	}
}

void ColorEditor::setCurrentColor(juce::Colour currentColor)
{
	this->currentColor = currentColor;
	this->selectColor(currentColor);
}

void ColorEditor::setSingerColor(bool hasSingerColor, juce::Colour color)
{
	this->hasSinger = hasSingerColor;
	this->singerColor = color;
	this->repaint();
}

void ColorEditor::mouseDown(const juce::MouseEvent& event)
{
	this->mouseUp(event);
}

void ColorEditor::mouseUp(const juce::MouseEvent& event)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	int height_marginTop = this->sizes.height_colorEditorMarginTop * screenSize.getHeight();
	int height_marginBottom = this->sizes.height_colorEditorMarginBottom * screenSize.getHeight();
	int width_marginLeft = this->sizes.width_colorEditorMarginLeft * screenSize.getWidth();
	int width_marginRight = this->sizes.width_colorEditorMarginRight * screenSize.getWidth();

	int height_inside = this->getHeight() - height_marginTop - height_marginBottom;
	int width_inside = this->getWidth() - width_marginLeft - width_marginRight;

	int posY_buttonArea = height_marginTop +
		this->positions.posY_colorEditorButton * height_inside;

	float posX_splitLine = width_marginLeft + width_inside * this->positions.posX_colorEditorSplit;
	float posY_splitLine = height_marginTop + height_inside * this->positions.posY_colorEditorSplitLine;

	float width_split = this->sizes.width_colorEditorSplit * screenSize.getWidth();

	//左右绘图区
	juce::Rectangle<float> rectLeftArea(
		width_marginLeft, height_marginTop,
		posX_splitLine - width_split / 2 - width_marginLeft,
		posY_buttonArea - height_marginTop
	);

	//计算左侧区域控件大小
	float posY_themeTitle = rectLeftArea.getY();
	float posY_themeArea = rectLeftArea.getY() + rectLeftArea.getHeight() * this->positions.posY_colorEditorLeftSplit1;
	float posY_historyTitle = rectLeftArea.getY() + rectLeftArea.getHeight() * this->positions.posY_colorEditorLeftSplit2;
	float posY_historyArea = rectLeftArea.getY() + rectLeftArea.getHeight() * this->positions.posY_colorEditorLeftSplit3;

	//计算左侧色块大小
	float width_colorTemplate = this->sizes.width_colorEditorColorTemplate * screenSize.getWidth();
	float height_colorTemplate = this->sizes.height_colorEditorColorTemplate * screenSize.getHeight();
	float width_colorTemplateSplit = this->sizes.width_colorEditorColorTemplateSplit * screenSize.getWidth();
	float height_colorTemplateSplit = this->sizes.height_colorEditorColorTemplateSplit * screenSize.getHeight();
	float width_colorTemplateCorner = this->sizes.width_colorEditorColorTemplateCorner * screenSize.getWidth();

	float colorTotalWidth = width_colorTemplate * this->templateXSize + width_colorTemplateSplit * (this->templateXSize - 1);
	float colorTemplatePosX = rectLeftArea.getX() + rectLeftArea.getWidth() / 2 - colorTotalWidth / 2;
	float themeColorTemplatePosY = posY_themeArea;
	float historyColorTemplatePosY = posY_historyArea;

	//判断是否在关注区
	{
		juce::Rectangle<float> rectThemeArea(
			rectLeftArea.getX(), posY_themeArea,
			rectLeftArea.getWidth(), posY_historyTitle - posY_themeArea
		);
		juce::Rectangle<float> rectHistoryArea(
			rectLeftArea.getX(), posY_historyArea,
			rectLeftArea.getWidth(), rectLeftArea.getBottom() - posY_historyArea
		);

		if (rectThemeArea.contains(event.position)) {
			juce::Rectangle<float> rect11(
				colorTemplatePosX + width_colorTemplate * 0 + width_colorTemplateSplit * 0,
				themeColorTemplatePosY + height_colorTemplate * 0 + height_colorTemplateSplit * 0,
				width_colorTemplate, height_colorTemplate
			);
			juce::Rectangle<float> rect12(
				colorTemplatePosX + width_colorTemplate * 1 + width_colorTemplateSplit * 1,
				themeColorTemplatePosY + height_colorTemplate * 0 + height_colorTemplateSplit * 0,
				width_colorTemplate, height_colorTemplate
			);

			juce::Rectangle<float> rect21(
				colorTemplatePosX + width_colorTemplate * 0 + width_colorTemplateSplit * 0,
				themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
				width_colorTemplate, height_colorTemplate
			);
			juce::Rectangle<float> rect22(
				colorTemplatePosX + width_colorTemplate * 1 + width_colorTemplateSplit * 1,
				themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
				width_colorTemplate, height_colorTemplate
			);
			juce::Rectangle<float> rect23(
				colorTemplatePosX + width_colorTemplate * 2 + width_colorTemplateSplit * 2,
				themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
				width_colorTemplate, height_colorTemplate
			);
			juce::Rectangle<float> rect24(
				colorTemplatePosX + width_colorTemplate * 3 + width_colorTemplateSplit * 3,
				themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
				width_colorTemplate, height_colorTemplate
			);
			juce::Rectangle<float> rect25(
				colorTemplatePosX + width_colorTemplate * 4 + width_colorTemplateSplit * 4,
				themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
				width_colorTemplate, height_colorTemplate
			);
			juce::Rectangle<float> rect26(
				colorTemplatePosX + width_colorTemplate * 5 + width_colorTemplateSplit * 5,
				themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
				width_colorTemplate, height_colorTemplate
			);

			if (this->hasSinger) {
				if (rect11.contains(event.position)) {
					this->selectColor(this->singerColor);
					return;
				}
				else if (rect12.contains(event.position)) {
					this->selectColor(this->colors.themeColor0);
					return;
				}
			}
			else {
				if (rect11.contains(event.position)) {
					this->selectColor(this->colors.themeColor0);
					return;
				}
			}

			if (rect21.contains(event.position)) {
				this->selectColor(this->colors.themeColor1);
				return;
			}
			else if (rect22.contains(event.position)) {
				this->selectColor(this->colors.themeColor2);
				return;
			}
			else if (rect23.contains(event.position)) {
				this->selectColor(this->colors.themeColor3);
				return;
			}
			else if (rect24.contains(event.position)) {
				this->selectColor(this->colors.themeColor4);
				return;
			}
			else if (rect25.contains(event.position)) {
				this->selectColor(this->colors.themeColor5);
				return;
			}
			else if (rect26.contains(event.position)) {
				this->selectColor(this->colors.themeColor6);
				return;
			}
		}
		else if (rectHistoryArea.contains(event.position)) {
			auto& historyArray = this->history->getList();
			int totalSize = this->history->getSizeTotal();

			for (int i = 0; i < historyArray.size(); i++) {
				int col = i % this->templateXSize;
				int row = i / this->templateXSize;

				juce::Rectangle<float> colorRect(
					colorTemplatePosX + width_colorTemplate * col + width_colorTemplateSplit * col,
					historyColorTemplatePosY + height_colorTemplate * row + height_colorTemplateSplit * row,
					width_colorTemplate, height_colorTemplate
				);

				if (colorRect.contains(event.position)) {
					this->selectColor(historyArray.getReference(i));
					return;
				}
			}
		}
	}
}

void ColorEditor::mouseMove(const juce::MouseEvent& event)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	int height_marginTop = this->sizes.height_colorEditorMarginTop * screenSize.getHeight();
	int height_marginBottom = this->sizes.height_colorEditorMarginBottom * screenSize.getHeight();
	int width_marginLeft = this->sizes.width_colorEditorMarginLeft * screenSize.getWidth();
	int width_marginRight = this->sizes.width_colorEditorMarginRight * screenSize.getWidth();

	int height_inside = this->getHeight() - height_marginTop - height_marginBottom;
	int width_inside = this->getWidth() - width_marginLeft - width_marginRight;

	int posY_buttonArea = height_marginTop +
		this->positions.posY_colorEditorButton * height_inside;

	float posX_splitLine = width_marginLeft + width_inside * this->positions.posX_colorEditorSplit;
	float posY_splitLine = height_marginTop + height_inside * this->positions.posY_colorEditorSplitLine;

	float width_split = this->sizes.width_colorEditorSplit * screenSize.getWidth();

	//左右绘图区
	juce::Rectangle<float> rectLeftArea(
		width_marginLeft, height_marginTop,
		posX_splitLine - width_split / 2 - width_marginLeft,
		posY_buttonArea - height_marginTop
	);

	//计算左侧区域控件大小
	float posY_themeTitle = rectLeftArea.getY();
	float posY_themeArea = rectLeftArea.getY() + rectLeftArea.getHeight() * this->positions.posY_colorEditorLeftSplit1;
	float posY_historyTitle = rectLeftArea.getY() + rectLeftArea.getHeight() * this->positions.posY_colorEditorLeftSplit2;
	float posY_historyArea = rectLeftArea.getY() + rectLeftArea.getHeight() * this->positions.posY_colorEditorLeftSplit3;

	//计算左侧色块大小
	float width_colorTemplate = this->sizes.width_colorEditorColorTemplate * screenSize.getWidth();
	float height_colorTemplate = this->sizes.height_colorEditorColorTemplate * screenSize.getHeight();
	float width_colorTemplateSplit = this->sizes.width_colorEditorColorTemplateSplit * screenSize.getWidth();
	float height_colorTemplateSplit = this->sizes.height_colorEditorColorTemplateSplit * screenSize.getHeight();
	float width_colorTemplateCorner = this->sizes.width_colorEditorColorTemplateCorner * screenSize.getWidth();

	float colorTotalWidth = width_colorTemplate * this->templateXSize + width_colorTemplateSplit * (this->templateXSize - 1);
	float colorTemplatePosX = rectLeftArea.getX() + rectLeftArea.getWidth() / 2 - colorTotalWidth / 2;
	float themeColorTemplatePosY = posY_themeArea;
	float historyColorTemplatePosY = posY_historyArea;

	//判断是否在关注区
	{
		juce::Rectangle<float> rectThemeArea(
			rectLeftArea.getX(), posY_themeArea,
			rectLeftArea.getWidth(), posY_historyTitle - posY_themeArea
		);
		juce::Rectangle<float> rectHistoryArea(
			rectLeftArea.getX(), posY_historyArea,
			rectLeftArea.getWidth(), rectLeftArea.getBottom() - posY_historyArea
		);

		if (rectThemeArea.contains(event.position)) {
			juce::Rectangle<float> rect11(
				colorTemplatePosX + width_colorTemplate * 0 + width_colorTemplateSplit * 0,
				themeColorTemplatePosY + height_colorTemplate * 0 + height_colorTemplateSplit * 0,
				width_colorTemplate, height_colorTemplate
			);
			juce::Rectangle<float> rect12(
				colorTemplatePosX + width_colorTemplate * 1 + width_colorTemplateSplit * 1,
				themeColorTemplatePosY + height_colorTemplate * 0 + height_colorTemplateSplit * 0,
				width_colorTemplate, height_colorTemplate
			);

			juce::Rectangle<float> rect21(
				colorTemplatePosX + width_colorTemplate * 0 + width_colorTemplateSplit * 0,
				themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
				width_colorTemplate, height_colorTemplate
			);
			juce::Rectangle<float> rect22(
				colorTemplatePosX + width_colorTemplate * 1 + width_colorTemplateSplit * 1,
				themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
				width_colorTemplate, height_colorTemplate
			);
			juce::Rectangle<float> rect23(
				colorTemplatePosX + width_colorTemplate * 2 + width_colorTemplateSplit * 2,
				themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
				width_colorTemplate, height_colorTemplate
			);
			juce::Rectangle<float> rect24(
				colorTemplatePosX + width_colorTemplate * 3 + width_colorTemplateSplit * 3,
				themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
				width_colorTemplate, height_colorTemplate
			);
			juce::Rectangle<float> rect25(
				colorTemplatePosX + width_colorTemplate * 4 + width_colorTemplateSplit * 4,
				themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
				width_colorTemplate, height_colorTemplate
			);
			juce::Rectangle<float> rect26(
				colorTemplatePosX + width_colorTemplate * 5 + width_colorTemplateSplit * 5,
				themeColorTemplatePosY + height_colorTemplate * 1 + height_colorTemplateSplit * 1,
				width_colorTemplate, height_colorTemplate
			);

			bool inRow1 = false;
			if (this->hasSinger) {
				inRow1 = rect11.contains(event.position) || rect12.contains(event.position);
			}
			else {
				inRow1 = rect11.contains(event.position);
			}

			bool inRow2 =
				rect21.contains(event.position) ||
				rect22.contains(event.position) ||
				rect23.contains(event.position) ||
				rect24.contains(event.position) ||
				rect25.contains(event.position) ||
				rect26.contains(event.position);

			if (inRow1 || inRow2) {
				this->setMouseCursor(juce::MouseCursor::PointingHandCursor);
			}
			else {
				this->setMouseCursor(juce::MouseCursor::NormalCursor);
			}
			return;
		}
		else if (rectHistoryArea.contains(event.position)) {
			auto& historyArray = this->history->getList();
			int totalSize = this->history->getSizeTotal();

			for (int i = 0; i < historyArray.size(); i++) {
				int col = i % this->templateXSize;
				int row = i / this->templateXSize;

				juce::Rectangle<float> colorRect(
					colorTemplatePosX + width_colorTemplate * col + width_colorTemplateSplit * col,
					historyColorTemplatePosY + height_colorTemplate * row + height_colorTemplateSplit * row,
					width_colorTemplate, height_colorTemplate
				);

				if (colorRect.contains(event.position)) {
					this->setMouseCursor(juce::MouseCursor::PointingHandCursor);
					return;
				}
			}

			this->setMouseCursor(juce::MouseCursor::NormalCursor);
			return;
		}
		else {
			this->setMouseCursor(juce::MouseCursor::NormalCursor);
			return;
		}
	}
}

void ColorEditor::mouseDrag(const juce::MouseEvent& event)
{
	this->mouseMove(event);
}

void ColorEditor::selectColor(float hue, float sat, float bri)
{
	this->hue = hue;
	this->sat = sat;
	this->bri = bri;
	this->refreshWidgets();
}

void ColorEditor::selectColor(juce::Colour color)
{
	auto h = color.getHue();
	auto s = color.getSaturation();
	auto v = color.getBrightness();
	this->selectColor(h, s, v);
}

void ColorEditor::refreshWidgets()
{
	this->okButton->setEnabled(
		juce::Colour::fromHSV(this->hue, this->sat, this->bri, 1.f) != this->currentColor);
	this->colorSpace->setCurrentColor(this->hue, this->sat, this->bri);
	this->hueView->setCurrentColor(this->hue, this->sat, this->bri);
	//TODO 刷新控件当前颜色显示
}

void ColorEditor::acceptAndClose()
{
	this->history->acceptColor(
		juce::Colour::fromHSV(this->hue, this->sat, this->bri, 1.f));
	if (this->parent) {
		this->parent->sendColor(
			juce::Colour::fromHSV(this->hue, this->sat, this->bri, 1.f));
	}
}

ColorEditorCallOutBox::ColorEditorCallOutBox(TrackView* parent)
	: parent(parent)
{
	//以下获取界面属性
	bool result = false;
	//color
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-colorEditor", this->colors.background_colorEditor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "border-colorEditor", this->colors.border_colorEditor, result
		);

	//size
	//position
	//scale

	//构建呼出框样式
	this->lafs.callOutBox = std::unique_ptr<juce::LookAndFeel>(new ColorEditorCallOutBoxLAF(
		[this] {return this->borderSize; }, [this] {return this->cornerSize; },
		this->colors.background_colorEditor, this->colors.border_colorEditor
	));

	//初始化控件
	this->colorEditor = std::make_unique<ColorEditor>(parent);
}

void ColorEditorCallOutBox::resize(int width, int height)
{
	this->colorEditor->setSize(width, height);
}

void ColorEditorCallOutBox::setArrowSize(float arrowWidth)
{
	this->arrowWidth = arrowWidth;
	if (this->callback) {
		this->callback->setArrowSize(arrowWidth);
	}
}

void ColorEditorCallOutBox::setBorderSize(int borderSize)
{
	this->borderSize = borderSize;
}

void ColorEditorCallOutBox::setCornerSize(float cornerSize)
{
	this->cornerSize = cornerSize;
}

void ColorEditorCallOutBox::showAt(juce::Rectangle<int> rect)
{
	if (!this->callback) {
		this->callback = std::make_unique<ColorEditorCallOutBoxCallback>(this);
	}
	this->callback->showAt(rect);
}

void ColorEditorCallOutBox::close()
{
	if (this->callback) {
		this->callback->close();
	}
}

void ColorEditorCallOutBox::setCurrentColor(juce::Colour currentColour)
{
	this->colorEditor->setCurrentColor(currentColour);
}

void ColorEditorCallOutBox::setSingerColor(bool hasSingerColor, juce::Colour color)
{
	this->colorEditor->setSingerColor(hasSingerColor, color);
}

ColorEditorCallOutBox::ColorEditorCallOutBoxCallback::ColorEditorCallOutBoxCallback(ColorEditorCallOutBox* manager)
	: manager(manager), parent(manager->parent)
{
	//初始化控件
	this->callOutBox = std::make_unique<juce::CallOutBox>(
		*(manager->colorEditor.get()), juce::Rectangle<int>({ 0, 0, 0, 0 }), nullptr);
	this->callOutBox->setLookAndFeel(manager->lafs.callOutBox.get());
	this->callOutBox->setArrowSize(manager->arrowWidth);
	//this->callOutBox->setDismissalMouseClicksAreAlwaysConsumed(true);
}

void ColorEditorCallOutBox::ColorEditorCallOutBoxCallback::setArrowSize(float arrowWidth)
{
	this->callOutBox->setArrowSize(arrowWidth);
}

void ColorEditorCallOutBox::ColorEditorCallOutBoxCallback::showAt(juce::Rectangle<int> rect)
{
	//获取控件所在窗口
	auto window = parent->getTopLevelComponent();
	auto windowSize = window->getScreenBounds();

	//计算屏幕显示位置
	auto screenBounds = parent->getScreenBounds();
	juce::Rectangle<int> placeInScreen = rect;
	placeInScreen.setPosition(rect.getX() + screenBounds.getX(), rect.getY() + screenBounds.getY());

	//更新位置并显示
	this->callOutBox->updatePosition(placeInScreen, windowSize);
	this->callOutBox->setVisible(true);
	this->callOutBox->enterModalState(true, this, false);
}

void ColorEditorCallOutBox::ColorEditorCallOutBoxCallback::close()
{
	this->callOutBox->dismiss();
}

void ColorEditorCallOutBox::ColorEditorCallOutBoxCallback::modalStateFinished(int)
{
	manager->callback.release();
}

