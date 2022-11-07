#include "TimeValueViewer.h"
#include <libJModule.h>

TimeValueViewer::TimeValueViewer(juce::Component* parent)
	: BubbleComponent(), parent(parent)
{
	//只允许在下侧出现
	this->setAllowedPlacement(juce::BubbleComponent::BubblePlacement::below);

	//显示在顶层
	this->setAlwaysOnTop(true);
	this->addToDesktop(juce::ComponentPeer::StyleFlags::windowIsTemporary);

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
		"main", "color", "background-timeViewer", this->colors.background_timeViewer, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-timeViewer", this->colors.text_timeViewer, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "value-timeViewer", this->colors.value_timeViewer, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-timeViewerMargin", this->sizes.width_timeViewerMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-timeViewerMargin", this->sizes.height_timeViewerMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-timeViewerLineSplit", this->sizes.height_timeViewerLineSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-timeViewerText", this->sizes.height_timeViewerText, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-timeViewerTextSplit", this->sizes.width_timeViewerTextSplit, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-timeViewerArrow", this->sizes.height_timeViewerArrow, result
		);
	//position
	//scale
}

void TimeValueViewer::setValue(double x, double xInBar, double time, uint8_t beat, double tempo)
{
	this->x = x;
	this->xInBar = xInBar;
	this->time = time;
	this->beat = beat;
	this->tempo = tempo;
	this->repaint();
}

void TimeValueViewer::showAt(juce::Point<int> pos)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	int height_arrow = this->sizes.height_timeViewerArrow * screenSize.getHeight();

	//获取parent位置
	auto screenPos = parent->getScreenBounds().getTopLeft();
	juce::Point<int> placeInScreen = pos;
	placeInScreen.setXY(pos.getX() + screenPos.getX(), pos.getY() + screenPos.getY());

	this->setPosition(placeInScreen, height_arrow);
	this->setVisible(true);
}

void TimeValueViewer::close()
{
	this->setVisible(false);
}

void TimeValueViewer::getContentSize(int& width, int& height)
{

}

void TimeValueViewer::paintContent(juce::Graphics& g, int width, int height)
{

}
