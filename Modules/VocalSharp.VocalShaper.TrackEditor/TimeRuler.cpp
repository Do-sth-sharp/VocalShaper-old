#include "TimeRuler.h"
#include <libJModule.h>

TimeRuler::TimeRuler()
	:EditorBase()
{
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
		"main", "color", "background-timeRuler", this->colors.background_timeRuler, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-timeRuler", this->colors.text_timeRuler, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "timeRuler-loopArea", this->colors.timeRuler_loopArea, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "timeRuler-loopJudgeArea", this->colors.timeRuler_loopJudgeArea, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "timeRuler-label-on", this->colors.timeRuler_label_on, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "timeRuler-label-off", this->colors.timeRuler_label_off, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "border", this->colors.border, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "cursor", this->colors.cursor, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-borderTop", this->sizes.height_borderTop, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-cursor", this->sizes.width_cursor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-timeRuler-barLine", this->sizes.height_timeRuler_barLine, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-timeRuler-beatLine", this->sizes.height_timeRuler_beatLine, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-timeRuler-barLine", this->sizes.width_timeRuler_barLine, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-timeRuler-beatLine", this->sizes.width_timeRuler_beatLine, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-timeRuler-minLineSpace", this->sizes.width_timeRuler_minLineSpace, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-timeRuler-barTextLeftMargin", this->sizes.width_timeRuler_barTextLeftMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-timeRuler-barTextRightMargin", this->sizes.width_timeRuler_barTextRightMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-timeRuler-loopJudgeArea", this->sizes.width_timeRuler_loopJudgeArea, result
		);

	//position
	//scale
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-timeRuler-barText", this->scales.height_timeRuler_barText, result
		);

	//resource
}

void TimeRuler::resized()
{

}

void TimeRuler::paint(juce::Graphics& g)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//填充背景
	g.fillAll(this->colors.background_timeRuler);

	//计算控件大小
	int height_borderTop = this->sizes.height_borderTop * screenSize.getHeight();

	//绘制标尺上边框
	juce::Rectangle<int> rectRulerHeadTopBorder(
		0, 0,
		this->getWidth(), height_borderTop
	);
	g.setColour(this->colors.border);
	g.fillRect(rectRulerHeadTopBorder);

	//计算线条大小与宽度
	int height_barLine = this->sizes.height_timeRuler_barLine * screenSize.getHeight();
	int height_beatLine = this->sizes.height_timeRuler_beatLine * screenSize.getHeight();
	float width_barLine = this->sizes.width_timeRuler_barLine * screenSize.getWidth();
	float width_beatLine = this->sizes.width_timeRuler_beatLine * screenSize.getWidth();
	float width_lineMinSpace = this->sizes.width_timeRuler_minLineSpace * screenSize.getWidth();

	int posY_barLine = this->getHeight() - height_barLine;
	int posY_beatLine = this->getHeight() - height_beatLine;

	float height_barText = posY_barLine * this->scales.height_timeRuler_barText;

	float width_barTextLeftMargin = this->sizes.width_timeRuler_barTextLeftMargin * screenSize.getWidth();
	float width_barTextRightMargin = this->sizes.width_timeRuler_barTextRightMargin * screenSize.getWidth();

	//计算字体
	juce::Font barTextFont = g.getCurrentFont();
	barTextFont.setHeight(height_barText);

	//根据数据绘图
	{
		juce::ScopedReadLock projLocker(this->projectLock);
		if (this->project) {
			juce::ScopedReadLock proxyLocker(this->project->getLock());
			//获取曲线量化
			auto curveQuant
				= vocalshaper::ProjectDAO::getCurveQuantification(this->project->getPtr());

			//计算分辨率
			double startTime = this->startTime;
			double endTime = this->endTime;
			double totalLength = endTime - startTime;
			double ppb = this->getWidth() / totalLength;

			//判断是否绘制节拍刻度
			bool beatLineOn = ppb > width_lineMinSpace;

			//计算实际绘制的第一个和最后一个节拍
			double trueStartTime = std::ceil(startTime);
			double trueEndTime = std::floor(endTime);

			//根据缓存绘制刻度线和文字
			float lastBarTextEndPos = 0.f;				//上一个拍号序号结束位置
			float firstBarTextPos = this->getWidth();	//第一个拍号序号位置
			for (auto t = trueStartTime; t <= trueEndTime; t++) {
				//计算线位置
				float pos = (t - startTime) * ppb;

				//判断是否为小节线
				double bar = std::floor(this->project->getBeat()->getBarAtTime(t));
				double beatAtBar = this->project->getBeat()->getTimeAtBar(bar);
				bool isBarLine = (beatAtBar == t);

				//绘制线
				if (isBarLine) {
					//绘小节线
					juce::Rectangle<float> rectLine(
						pos - width_barLine / 2, posY_barLine,
						width_barLine, height_barLine
					);
					g.setColour(this->colors.text_timeRuler);
					g.fillRect(rectLine);

					//如果序号位置无遮挡
					if (pos >= lastBarTextEndPos) {
						//是第一个序号，记录位置
						if (lastBarTextEndPos == 0.f) {
							firstBarTextPos = pos;
						}

						//计算序号文字及文字大小
						juce::String barText((int64_t)(bar + 1));
						float width_barText = barTextFont.getStringWidthFloat(barText);

						//计算序号绘制位置
						lastBarTextEndPos = pos + width_barTextLeftMargin + width_barText + width_barTextRightMargin;
						juce::Rectangle<int> rectBarText(
							pos + width_barTextLeftMargin, 0,
							width_barText, posY_barLine
							);

						//绘制序号
						g.setFont(barTextFont);
						g.setColour(this->colors.text_timeRuler);
						g.drawFittedText(barText, rectBarText, juce::Justification::centred, 1);
					}
				}
				else {
					if (beatLineOn) {
						//绘节拍线
						juce::Rectangle<float> rectLine(
							pos - width_beatLine / 2, posY_beatLine,
							width_beatLine, height_beatLine
						);
						g.setColour(this->colors.text_timeRuler);
						g.fillRect(rectLine);
					}
				}
			}

			//判断并绘制前端小节序号
			{
				//生成文字并计算大小
				auto startBar = std::floor(this->project->getBeat()->getBarAtTime(std::floor(startTime)));
				juce::String frontBarText = "<" + juce::String((int64_t)(startBar + 1));
				float width_barText = barTextFont.getStringWidthFloat(frontBarText);

				//如果有足够的空间绘制
				if (width_barTextLeftMargin + width_barText + width_barTextRightMargin <= firstBarTextPos) {
					//计算序号绘制位置
					juce::Rectangle<int> rectBarText(
						width_barTextLeftMargin, 0,
						width_barText, posY_barLine
					);

					//绘制序号
					g.setFont(barTextFont);
					g.setColour(this->colors.text_timeRuler);
					g.drawFittedText(frontBarText, rectBarText, juce::Justification::centred, 1);
				}
			}

			//判断并绘制播放指针
			{
				double currentTime = this->currentTime;
				if (currentTime >= startTime && currentTime <= endTime) {
					//计算指针位置
					float width_cursor = this->sizes.width_cursor * screenSize.getWidth();
					float pos = (currentTime - startTime) * ppb;

					//绘制指针
					juce::Rectangle<float> rectCursor(
						pos - width_cursor / 2, 0,
						width_cursor, this->getHeight()
					);
					g.setColour(this->colors.cursor);
					g.fillRect(rectCursor);
				}
			}

			//TODO 绘制标签
			{

			}

			//TODO 绘制编辑中的标签
			{

			}

			//判断并绘制选区
			{
				//判断选区有效
				if (this->loopEndTime > this->loopStartTime) {
					//计算选区位置
					float width_loopJudgeArea = this->sizes.width_timeRuler_loopJudgeArea * screenSize.getWidth();
					float startLoopPos = ((this->loopStartTime - startTime) / totalLength) * this->getWidth();
					float endLoopPos= ((this->loopEndTime - startTime) / totalLength) * this->getWidth();
					float startLoopJudgePos = startLoopPos - width_loopJudgeArea;
					float endLoopJudgePos = endLoopPos + width_loopJudgeArea;

					//如果选区与预览区重合
					if (endLoopJudgePos > 0 && startLoopJudgePos < this->getWidth()) {
						//绘选区
						juce::Rectangle<float> rectLoopArea(
							startLoopPos, 0,
							endLoopPos - startLoopPos, this->getHeight()
						);
						g.setColour(this->colors.timeRuler_loopArea);
						g.fillRect(rectLoopArea);

						//绘判定区
						juce::Rectangle<float> rectLeftJudgeArea(
							startLoopJudgePos, 0,
							width_loopJudgeArea, this->getHeight()
						);
						juce::Rectangle<float> rectRightJudgeArea(
							endLoopPos, 0,
							width_loopJudgeArea, this->getHeight()
						);
						g.setColour(this->colors.timeRuler_loopJudgeArea);
						g.fillRect(rectLeftJudgeArea);
						g.fillRect(rectRightJudgeArea);
					}
				}
			}
		}
	}

}

void TimeRuler::projectChanged(const vocalshaper::ProjectProxy* ptr)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->project = const_cast<vocalshaper::ProjectProxy*>(ptr);
	this->repaint();
}

void TimeRuler::setEditMode(bool editMode)
{
	this->editModeFlag = editMode;
}

void TimeRuler::setToolID(uint8_t toolID)
{
	this->toolID = toolID;
}

void TimeRuler::trackChanged(int trackID)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->trackID = trackID;
	this->repaint();
}

void TimeRuler::setTotalLength(double totalLength)
{
}

void TimeRuler::setCurrentPosition(double currentTime)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->currentTime = currentTime;
	this->repaint();
}

void TimeRuler::setLoopRange(double startTime, double endTime)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->loopStartTime = startTime;
	this->loopEndTime = endTime;
	this->repaint();
}

void TimeRuler::setAdsorb(vocalshaper::AdsorbState state)
{
	this->adsorb = state;
	this->repaint();
}

void TimeRuler::setGrid(vocalshaper::GridState state)
{
	this->grid = state;
	this->repaint();
}

void TimeRuler::setHViewPort(double startTime, double endTime)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->startTime = startTime;
	this->endTime = endTime;
	this->repaint();
}

void TimeRuler::setVViewPort(double bottomTrack, double topTrack)
{
}
