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

	//建立字体标准标签
	this->fontLabel = std::make_unique<juce::Label>();
}

void TimeValueViewer::setValue(
	double x, double xInBar, double bFromBar, double time, uint8_t beat, uint8_t base, double tempo)
{
	this->x = x;
	this->xInBar = xInBar;
	this->bFromBar = bFromBar;
	this->time = time;
	this->beat = beat;
	this->base = base;
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

//bar+beat
//At timeInBar unit
//At timeInSec unit
//beat: beat/base
//tempo: tempo unit

void TimeValueViewer::getContentSize(int& width, int& height)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	int height_margin = this->sizes.height_timeViewerMargin * screenSize.getHeight();
	int width_margin = this->sizes.width_timeViewerMargin * screenSize.getWidth();
	int height_split = this->sizes.height_timeViewerLineSplit * screenSize.getHeight();
	int width_split = this->sizes.width_timeViewerTextSplit * screenSize.getWidth();

	int height_text = this->sizes.height_timeViewerText * screenSize.getHeight();

	//设置字体
	juce::Font font = this->fontLabel->getFont();
	font.setHeight(height_text);

	//计算文字大小
	juce::String xBarBeatStr(juce::String((uint64_t)std::floor(this->xInBar + 1)) + " + " + juce::String(this->bFromBar + 1, 2));
	juce::String xInBarStr(this->xInBar + 1, 2);
	juce::String timeStr(this->time, 2);
	juce::String beatStr(juce::String((uint64_t)this->beat) + "/" + juce::String((uint64_t)this->base));
	juce::String tempoStr(this->tempo, 2);

	juce::String xInBarLeadStr(this->tr("tip_xInBarLead"));
	juce::String timeLeadStr(this->tr("tip_timeLead"));
	juce::String xInBarUnitStr(this->tr("tip_xInBarUnit"));
	juce::String timeUnitStr(this->tr("tip_timeUnit"));
	juce::String beatLeadStr(this->tr("tip_beatLead"));
	juce::String tempoLeadStr(this->tr("tip_tempoLead"));
	juce::String tempoUnitStr(this->tr("tip_tempoUnit"));

	int width_xBarBeatStr = font.getStringWidth(xBarBeatStr);
	int width_xInBarStr = font.getStringWidth(xInBarStr);
	int width_timeStr = font.getStringWidth(timeStr);
	int width_beatStr = font.getStringWidth(beatStr);
	int width_tempoStr = font.getStringWidth(tempoStr);

	int width_xInBarLeadStr = font.getStringWidth(xInBarLeadStr);
	int width_timeLeadStr = font.getStringWidth(timeLeadStr);
	int width_xInBarUnitStr = font.getStringWidth(xInBarUnitStr);
	int width_timeUnitStr = font.getStringWidth(timeUnitStr);
	int width_beatLeadStr = font.getStringWidth(beatLeadStr);
	int width_tempoLeadStr = font.getStringWidth(tempoLeadStr);
	int width_tempoUnitStr = font.getStringWidth(tempoUnitStr);

	int width_line0 = width_xBarBeatStr;
	int width_line1 = width_xInBarLeadStr + width_xInBarStr + width_xInBarUnitStr + width_split * 2;
	int width_line2 = width_timeLeadStr + width_timeStr + width_timeUnitStr + width_split * 2;
	int width_line3 = width_beatLeadStr + width_beatStr + width_split * 2;
	int width_line4 = width_tempoLeadStr + width_tempoStr + width_tempoUnitStr + width_split * 2;

	//寻找最长行
	int width_lineMax = width_line0;
	width_lineMax = std::max(width_lineMax, width_line1);
	width_lineMax = std::max(width_lineMax, width_line2);
	width_lineMax = std::max(width_lineMax, width_line3);
	width_lineMax = std::max(width_lineMax, width_line4);

	//设置高度
	height = height_margin * 2 + height_text * 5 + height_split * 4;
	width = width_margin * 2 + width_lineMax;
}

void TimeValueViewer::paintContent(juce::Graphics& g, int width, int height)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	int height_margin = this->sizes.height_timeViewerMargin * screenSize.getHeight();
	int width_margin = this->sizes.width_timeViewerMargin * screenSize.getWidth();
	int height_split = this->sizes.height_timeViewerLineSplit * screenSize.getHeight();
	int width_split = this->sizes.width_timeViewerTextSplit * screenSize.getWidth();

	int height_text = this->sizes.height_timeViewerText * screenSize.getHeight();

	//设置字体
	juce::Font font = this->fontLabel->getFont();
	font.setHeight(height_text);
	g.setFont(font);

	//计算文字大小
	juce::String xBarBeatStr(juce::String((uint64_t)std::floor(this->xInBar + 1)) + " + " + juce::String(this->bFromBar + 1, 2));
	juce::String xInBarStr(this->xInBar + 1, 2);
	juce::String timeStr(this->time, 2);
	juce::String beatStr(juce::String((uint64_t)this->beat) + "/" + juce::String((uint64_t)this->base));
	juce::String tempoStr(this->tempo, 2);

	juce::String xInBarLeadStr(this->tr("tip_xInBarLead"));
	juce::String timeLeadStr(this->tr("tip_timeLead"));
	juce::String xInBarUnitStr(this->tr("tip_xInBarUnit"));
	juce::String timeUnitStr(this->tr("tip_timeUnit"));
	juce::String beatLeadStr(this->tr("tip_beatLead"));
	juce::String tempoLeadStr(this->tr("tip_tempoLead"));
	juce::String tempoUnitStr(this->tr("tip_tempoUnit"));

	int width_xBarBeatStr = font.getStringWidth(xBarBeatStr);
	int width_xInBarStr = font.getStringWidth(xInBarStr);
	int width_timeStr = font.getStringWidth(timeStr);
	int width_beatStr = font.getStringWidth(beatStr);
	int width_tempoStr = font.getStringWidth(tempoStr);

	int width_xInBarLeadStr = font.getStringWidth(xInBarLeadStr);
	int width_timeLeadStr = font.getStringWidth(timeLeadStr);
	int width_xInBarUnitStr = font.getStringWidth(xInBarUnitStr);
	int width_timeUnitStr = font.getStringWidth(timeUnitStr);
	int width_beatLeadStr = font.getStringWidth(beatLeadStr);
	int width_tempoLeadStr = font.getStringWidth(tempoLeadStr);
	int width_tempoUnitStr = font.getStringWidth(tempoUnitStr);

	//计算值绘制区域
	juce::Rectangle<int> xBarBeatRect(
		width_margin,
		height_margin + height_text * 0 + height_split * 0,
		width_xBarBeatStr, height_text);
	juce::Rectangle<int> xInBarRect(
		width_margin + width_xInBarLeadStr + width_split * 1,
		height_margin + height_text * 1 + height_split * 1,
		width_xInBarStr, height_text);
	juce::Rectangle<int> timeRect(
		width_margin + width_timeLeadStr + width_split * 1,
		height_margin + height_text * 2 + height_split * 2,
		width_timeStr, height_text);
	juce::Rectangle<int> beatRect(
		width_margin + width_beatLeadStr + width_split * 1,
		height_margin + height_text * 3 + height_split * 3,
		width_beatStr, height_text);
	juce::Rectangle<int> tempoRect(
		width_margin + width_tempoLeadStr + width_split * 1,
		height_margin + height_text * 4 + height_split * 4,
		width_tempoStr, height_text);

	//计算文字提示绘制区域
	juce::Rectangle<int> xInBarLeadRect(
		width_margin,
		height_margin + height_text * 1 + height_split * 1,
		width_xInBarLeadStr, height_text);
	juce::Rectangle<int> timeLeadRect(
		width_margin,
		height_margin + height_text * 2 + height_split * 2,
		width_timeLeadStr, height_text);
	juce::Rectangle<int> beatLeadRect(
		width_margin,
		height_margin + height_text * 3 + height_split * 3,
		width_beatLeadStr, height_text);
	juce::Rectangle<int> tempoLeadRect(
		width_margin,
		height_margin + height_text * 4 + height_split * 4,
		width_tempoLeadStr, height_text);

	juce::Rectangle<int> xInBarUnitRect(
		width_margin + width_xInBarLeadStr + width_xInBarStr + width_split * 2,
		height_margin + height_text * 1 + height_split * 1,
		width_xInBarUnitStr, height_text);
	juce::Rectangle<int> timeUnitRect(
		width_margin + width_timeLeadStr + width_timeStr + width_split * 2,
		height_margin + height_text * 2 + height_split * 2,
		width_timeUnitStr, height_text);
	juce::Rectangle<int> tempoUnitRect(
		width_margin + width_tempoLeadStr + width_tempoStr + width_split * 2,
		height_margin + height_text * 4 + height_split * 4,
		width_tempoUnitStr, height_text);

	//填充背景
	//g.fillAll(this->colors.background_timeViewer);

	//绘制值
	g.setColour(this->colors.value_timeViewer);
	g.drawFittedText(xBarBeatStr, xBarBeatRect, juce::Justification::centred, 1);
	g.drawFittedText(xInBarStr, xInBarRect, juce::Justification::centred, 1);
	g.drawFittedText(timeStr, timeRect, juce::Justification::centred, 1);
	g.drawFittedText(beatStr, beatRect, juce::Justification::centred, 1);
	g.drawFittedText(tempoStr, tempoRect, juce::Justification::centred, 1);

	//绘制文字提示
	g.setColour(this->colors.text_timeViewer);
	g.drawFittedText(xInBarLeadStr, xInBarLeadRect, juce::Justification::centred, 1);
	g.drawFittedText(timeLeadStr, timeLeadRect, juce::Justification::centred, 1);
	g.drawFittedText(beatLeadStr, beatLeadRect, juce::Justification::centred, 1);
	g.drawFittedText(tempoLeadStr, tempoLeadRect, juce::Justification::centred, 1);

	g.drawFittedText(xInBarUnitStr, xInBarUnitRect, juce::Justification::centred, 1);
	g.drawFittedText(timeUnitStr, timeUnitRect, juce::Justification::centred, 1);
	g.drawFittedText(tempoUnitStr, tempoUnitRect, juce::Justification::centred, 1);
}
