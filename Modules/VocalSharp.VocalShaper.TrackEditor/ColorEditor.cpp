#include "ColorEditor.h"
#include "TrackView.h"

ColorEditor::ColorEditor()
	: Component("Color Editor")
{

}

ColorEditorCallOutBox::ColorEditorCallOutBox(TrackView* parent)
	: parent(parent)
{
	//以下获取界面属性
	bool result = false;
	//color

	//size
	//position
	//scale

	//构建呼出框样式

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

