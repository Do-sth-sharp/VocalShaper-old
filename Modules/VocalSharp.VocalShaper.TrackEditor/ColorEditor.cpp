#include "ColorEditor.h"
#include "TrackView.h"
#include <libJModule.h>
#include "ColorEditorCallOutBoxLAF.h"

ColorEditor::ColorEditor()
	: Component("Color Editor")
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
	//this->okButton->onClick = [this] {this->acceptAndClose(); };
	this->okButton->setEnabled(false);
	this->addAndMakeVisible(this->okButton.get());
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

	//调整控件大小
	this->okButton->setBounds(posX_button, posY_button, width_button, height_button);
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
}

void ColorEditor::setCurrentColor(juce::Colour currentColor)
{
	this->currentColor = currentColor;
	//TODO 设置颜色为当前颜色并重置按钮状态
}

void ColorEditor::setSingerColor(bool hasSingerColor, juce::Colour color)
{
	this->hasSinger = hasSingerColor;
	this->singerColor = color;
	this->repaint();
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
	this->colorEditor = std::make_unique<ColorEditor>();
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

