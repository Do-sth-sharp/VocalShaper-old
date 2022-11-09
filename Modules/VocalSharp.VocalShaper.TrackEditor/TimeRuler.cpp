#include "TimeRuler.h"
#include <libJModule.h>

TimeRuler::TimeRuler(std::function<void(double, double)> wheelChangeMethod,
	std::function<void(double, double)> wheelChangeWithCtrlMethod)
	: EditorBase() , wheelChangeMethod(wheelChangeMethod),
	wheelChangeWithCtrlMethod(wheelChangeWithCtrlMethod)
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
		"main", "color", "background-timeViewer", this->colors.background_timeViewer, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "border-timeViewer", this->colors.border_timeViewer, result
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
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-timeRuler-labelBottomMargin", this->sizes.height_timeRuler_labelBottomMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-timeRuler-label", this->sizes.height_timeRuler_label, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-labelEditor", this->sizes.width_labelEditor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-labelEditor", this->sizes.height_labelEditor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-labelEditorCalloutArrow", this->sizes.width_labelEditorCalloutArrow, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-labelEditorCalloutBorder", this->sizes.width_labelEditorCalloutBorder, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-labelEditorCalloutCorner", this->sizes.width_labelEditorCalloutCorner, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-timeViewerBorder", this->sizes.width_timeViewerBorder, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-timeViewerCorner", this->sizes.width_timeViewerCorner, result
		);

	//position
	//scale
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-timeRuler-barText", this->scales.height_timeRuler_barText, result
		);

	//resource

	//建立标签编辑器
	this->labelEditor = std::make_unique<LabelEditorCallOutBox>(this);

	//建立时间预览器样式
	jmadf::CallInterface<juce::LookAndFeel*&,
		const std::function<int()>&, const std::function<float()>&>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetLabelEditorBubbleLAF",
			this->lafs.timeViewerBubble,
			[this] {return this->timeViewerBorderSize; },
			[this] {return this->timeViewerCornerSize; });
	this->lafs.timeViewerBubble->setColour(
		juce::BubbleComponent::ColourIds::backgroundColourId, this->colors.background_timeViewer
	);
	this->lafs.timeViewerBubble->setColour(
		juce::BubbleComponent::ColourIds::outlineColourId, this->colors.border_timeViewer
	);

	//建立时间预览器
	this->timeValue = std::make_unique<TimeValueViewer>(this);
	this->timeValue->setLookAndFeel(this->lafs.timeViewerBubble);

	//监听标签变化
	jmadf::CallInterface<const vocalshaper::actions::ActionBase::RuleFunc&>(
		"VocalSharp.VocalShaper.CallbackReactor", "AddActionRules",
		[this](const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
		{this->listenLabelChange(action, type); });
}

void TimeRuler::resized()
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	int width_labelEditor = this->sizes.width_labelEditor * screenSize.getWidth();
	int height_labelEditor = this->sizes.height_labelEditor * screenSize.getHeight();
	float width_labelEditorArrow = this->sizes.width_labelEditorCalloutArrow * screenSize.getWidth();
	int width_labelEditorBorder = this->sizes.width_labelEditorCalloutBorder * screenSize.getWidth();
	float width_labelEditorCorner = this->sizes.width_labelEditorCalloutCorner * screenSize.getWidth();
	int width_timeValueBorder = this->sizes.width_timeViewerBorder * screenSize.getWidth();
	float width_timeValueCorner = this->sizes.width_timeViewerCorner * screenSize.getWidth();

	//调整大小
	this->labelEditor->resize(width_labelEditor, height_labelEditor);
	this->labelEditor->setArrowSize(width_labelEditorArrow);
	this->labelEditor->setBorderSize(width_labelEditorBorder);
	this->labelEditor->setCornerSize(width_labelEditorCorner);

	this->timeViewerBorderSize = width_timeValueBorder;
	this->timeViewerCornerSize = width_timeValueCorner;
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
		juce::ScopedReadLock projLocker(this->getProjLock());
		if (this->getProject()) {
			juce::ScopedReadLock proxyLocker(this->getProject()->getLock());
			//计算分辨率
			double startTime = 0.;
			double endTime = 0.;
			std::tie(startTime, endTime) = this->getHViewPort();
			double totalLength = endTime - startTime;
			double ppb = this->getWidth() / totalLength;

			//判断是否绘制节拍刻度
			bool beatLineOn = ppb > width_lineMinSpace;

			//计算实际绘制的第一个和最后一个节拍
			double trueStartBar = std::floor(
				this->getProject()->getBeat()->getBarAtTime(startTime));
			double trueStartTime 
				= this->getProject()->getBeat()->getTimeAtBar(trueStartBar);
			double trueEndTime = std::ceil(endTime);

			//根据缓存绘制刻度线和文字
			float lastBarTextEndPos = 0.f;				//上一个拍号序号结束位置
			float firstBarTextPos = this->getWidth();	//第一个拍号序号位置
			for (auto t = trueStartTime; t <= trueEndTime; ) {
				//计算线位置
				float pos = (t - startTime) * ppb;

				//判断是否为小节线
				double bar = std::floor(this->getProject()->getBeat()->getBarAtTime(t));
				double beatAtBar = this->getProject()->getBeat()->getTimeAtBar(bar);
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

				//向下增加一个节拍的距离
				auto beatBase = this->getProject()->getBeat()->getBaseAtTime(t);
				t += 1. / (beatBase / 4.);
			}

			//判断并绘制前端小节序号
			{
				//生成文字并计算大小
				auto startBar = std::floor(
					this->getProject()->getBeat()->getBarAtTime(std::floor(startTime)));
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
				double currentTime = this->getCurrentPosition();
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

			//绘制标签
			{
				//计算大小
				float height_label = this->sizes.height_timeRuler_label * screenSize.getHeight();
				float width_label = height_label;
				float height_labelBottomMargin = this->sizes.height_timeRuler_labelBottomMargin * screenSize.getHeight();

				//遍历并绘制标签
				int labelSize = vocalshaper::ProjectDAO::labelSize(
					this->getProject()->getPtr());
				for (int i = 0; i < labelSize; i++) {
					auto label = vocalshaper::ProjectDAO::getLabel(
						this->getProject()->getPtr(), i);
					if (label) {
						float labelPos = (vocalshaper::LabelDAO::getPosition(label) - startTime) * ppb;

						if ((labelPos + width_label / 2) >= 0 && (labelPos - width_label / 2) <= this->getWidth()) {
							if (!this->getEditMode() || (i == this->labelEditingIndex)) {
								g.setColour(this->colors.timeRuler_label_off);
							}
							else {
								g.setColour(this->colors.timeRuler_label_on);
							}

							g.fillEllipse(
								labelPos - width_label / 2, this->getHeight() - height_labelBottomMargin - height_label,
								width_label, height_label);
						}
					}
				}

				//绘制编辑中的标签
				{
					float labelPos = (this->labelEditingTime - startTime) * ppb;
					if ((labelPos + width_label / 2) >= 0 && (labelPos - width_label / 2) <= this->getWidth()) {
						g.setColour(this->colors.timeRuler_label_off);
						g.fillEllipse(
							labelPos - width_label / 2, this->getHeight() - height_labelBottomMargin - height_label,
							width_label, height_label);
					}
				}
			}

			//判断并绘制选区
			{
				//判断选区有效
				double loopStartTime = 0., loopEndTime = 0.;
				std::tie(loopStartTime, loopEndTime) = this->getLoopRange();
				if (loopEndTime > loopStartTime ||
					this->rulerState == RulerState::LoopSP ||
					this->rulerState == RulerState::LoopEP) {
					//计算选区位置
					float width_loopJudgeArea = this->sizes.width_timeRuler_loopJudgeArea * screenSize.getWidth();
					float startLoopPos = (loopStartTime - startTime) * ppb;
					float endLoopPos = (loopEndTime - startTime) * ppb;
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

void TimeRuler::projectChangedCallback(const vocalshaper::ProjectProxy* ptr)
{
	this->labelEditor->setProject(const_cast<vocalshaper::ProjectProxy*>(ptr));
	this->timeValue->close();
	this->repaint();
}

void TimeRuler::setEditModeCallback(bool editMode)
{
	this->repaint();
}

void TimeRuler::trackChangedCallback(int trackID)
{
	this->repaint();
}

void TimeRuler::setTotalLengthCallback(double totalLength)
{
	juce::ScopedWriteLock locker(this->getProjLock());
	if (this->getProject()) {
		double bar =
			this->getProject()->getBeat()->getBarAtTime(std::floor(totalLength));
		bar = std::max(std::floor(bar) + 4, 20.);
		this->totalTime =
			this->getProject()->getBeat()->getTimeAtBar(bar);
		this->timeValue->close();
		this->repaint();
	}
}

void TimeRuler::setCurrentPositionCallback(double currentTime)
{
	this->repaint();
}

void TimeRuler::setLoopRangeCallback(double startTime, double endTime)
{
	this->repaint();
}

void TimeRuler::setAdsorbCallback(vocalshaper::AdsorbState state)
{
	this->repaint();
}

void TimeRuler::setGridCallback(vocalshaper::GridState state)
{
	this->repaint();
}

void TimeRuler::setHViewPortCallback(double startTime, double endTime)
{
	this->timeValue->close();
	this->repaint();
}

void TimeRuler::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& details)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//获取鼠标指针属性
	float posX = event.position.getX();
	float posY = event.position.getY();

	//计算delta
	double delta = details.deltaY / 20.;

	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock projDataLocker(this->getProject()->getLock());

		//计算分辨率
		double startTime = 0.;
		double endTime = 0.;
		std::tie(startTime, endTime) = this->getHViewPort();
		double totalLength = endTime - startTime;
		double ppb = this->getWidth() / totalLength;

		//在常规模式下
		if (this->rulerState == RulerState::Normal) {
			//计算时间
			double per = posX / (double)this->getWidth();

			//ctrl按下
			if (event.mods.isCtrlDown()) {
				this->wheelChangeWithCtrlMethod(per, delta);
			}
			else {
				this->wheelChangeMethod(per, delta);
			}
		}

		//关闭时间值预览
		this->timeValue->close();
	}
}

void TimeRuler::mouseDown(const juce::MouseEvent& event)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//获取鼠标指针属性
	float posX = event.position.getX();
	float posY = event.position.getY();

	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock projDataLocker(this->getProject()->getLock());

		//计算分辨率
		double startTime = 0.;
		double endTime = 0.;
		std::tie(startTime, endTime) = this->getHViewPort();
		double totalLength = endTime - startTime;
		double ppb = this->getWidth() / totalLength;

		//在常规模式下
		if (this->rulerState == RulerState::Normal) {
			//左键按下
			if (event.mods.isLeftButtonDown()) {
				//标签更改
				if (this->getEditMode()) {
					//计算标签大小
					float height_label = this->sizes.height_timeRuler_label * screenSize.getHeight();
					float width_label = height_label;
					float height_labelBottomMargin = this->sizes.height_timeRuler_labelBottomMargin * screenSize.getHeight();

					//判断鼠标位置
					if (posY >= (this->getHeight() - height_labelBottomMargin - height_label) &&
						posY <= (this->getHeight() - height_labelBottomMargin)) {
						//选择标签
						int labelIndex = -1;
						double labelTime = -1;
						int labelSize = vocalshaper::ProjectDAO::labelSize(
							this->getProject()->getPtr());
						for (int i = labelSize; i >= 0; i--) {
							auto label = vocalshaper::ProjectDAO::getLabel(
								this->getProject()->getPtr(), i);
							if (label) {
								float labelPos = (vocalshaper::LabelDAO::getPosition(label) - startTime) * ppb;

								if ((labelPos + width_label / 2) >= posX && (labelPos - width_label / 2) <= posX) {
									labelIndex = i;
									labelTime = vocalshaper::LabelDAO::getPosition(label);
									break;
								}
							}
						}

						//如果标签已选择
						if (labelIndex >= 0 && labelIndex < labelSize) {
							//更改状态缓存
							this->labelEditingIndex = labelIndex;
							this->labelEditingTime = labelTime;
							this->timePressed = startTime + posX / ppb;
							this->rulerState = RulerState::Label;

							//关闭时间值预览
							this->timeValue->close();

							//更改鼠标并刷新
							this->setMouseCursor(juce::MouseCursor::DraggingHandCursor);
							this->repaint();
							return;
						}
					}
					
				}

				//选区更改
				double loopStartTime = 0., loopEndTime = 0.;
				std::tie(loopStartTime, loopEndTime) = this->getLoopRange();
				if (loopEndTime > loopStartTime) {
					//计算选区位置
					float width_loopJudgeArea = this->sizes.width_timeRuler_loopJudgeArea * screenSize.getWidth();
					float startLoopPos = (loopStartTime - startTime) * ppb;
					float endLoopPos = (loopEndTime - startTime) * ppb;
					float startLoopJudgePos = startLoopPos - width_loopJudgeArea;
					float endLoopJudgePos = endLoopPos + width_loopJudgeArea;

					//判断选区
					if (posX >= startLoopJudgePos && posX <= startLoopPos) {
						//更改状态缓存
						this->rulerState = RulerState::LoopSP;

						{
							//计算时间
							double x = loopStartTime;
							double xInBar = this->getProject()->getBeat()->getBarAtTime(x);
							double time = this->getProject()->getTempo()->get_t(x);
							uint8_t beat = this->getProject()->getBeat()->getBeatAtTime(x);
							uint8_t base = this->getProject()->getBeat()->getBaseAtTime(x);
							double xFromBar 
								= (x - this->getProject()->getBeat()->getTimeAtBar(std::floor(xInBar))) * (base / 4.);
							double tempo = this->getProject()->getTempo()->get_T(x);

							//设置预览位置
							this->timeValue->setValue(x, xInBar, xFromBar, time, beat, base, tempo);
							this->timeValue->showAt(juce::Point<int>(
								(x - startTime) * ppb, this->getHeight()));
						}

						//更改鼠标并刷新
						this->setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
						this->repaint();
						return;
					}
					else if (posX >= endLoopPos && posX <= endLoopJudgePos) {
						//更改状态缓存
						this->rulerState = RulerState::LoopEP;

						{
							//计算时间
							double x = loopEndTime;
							double xInBar = this->getProject()->getBeat()->getBarAtTime(x);
							double time = this->getProject()->getTempo()->get_t(x);
							uint8_t beat = this->getProject()->getBeat()->getBeatAtTime(x);
							uint8_t base = this->getProject()->getBeat()->getBaseAtTime(x);
							double xFromBar
								= (x - this->getProject()->getBeat()->getTimeAtBar(std::floor(xInBar))) * (base / 4.);
							double tempo = this->getProject()->getTempo()->get_T(x);

							//设置预览位置
							this->timeValue->setValue(x, xInBar, xFromBar, time, beat, base, tempo);
							this->timeValue->showAt(juce::Point<int>(
								(x - startTime) * ppb, this->getHeight()));
						}

						//更改鼠标并刷新
						this->setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
						this->repaint();
						return;
					}
				}

				//播放指针更改
				{
					//更改状态缓存
					double time = startTime + posX / ppb;
					this->timePressed = time;

					time = vocalshaper::adsorb(time, this->getAdsorb());

					if (time < 0) { time = 0; }
					if (time > this->totalTime) { time = totalTime; }
					this->rulerState = RulerState::Cursor;

					//发送更改
					this->setCurrentPositionMethod(time);

					//如果在选区外，则取消选区
					if (loopEndTime > loopStartTime) {
						if (time < loopStartTime || time > loopEndTime) {
							this->setLoopRangeMethod(-1, -1);
						}
					}

					{
						//计算时间
						double x = time;
						double xInBar = this->getProject()->getBeat()->getBarAtTime(x);
						double time = this->getProject()->getTempo()->get_t(x);
						uint8_t beat = this->getProject()->getBeat()->getBeatAtTime(x);
						uint8_t base = this->getProject()->getBeat()->getBaseAtTime(x);
						double xFromBar
							= (x - this->getProject()->getBeat()->getTimeAtBar(std::floor(xInBar))) * (base / 4.);
						double tempo = this->getProject()->getTempo()->get_T(x);

						//设置预览位置
						this->timeValue->setValue(x, xInBar, xFromBar, time, beat, base, tempo);
						this->timeValue->showAt(juce::Point<int>(
							(x - startTime) * ppb, this->getHeight()));
					}

					//更改鼠标并刷新
					this->setMouseCursor(juce::MouseCursor::NormalCursor);
					this->repaint();
					return;
				}
			}
		}
	}
}

void TimeRuler::mouseMove(const juce::MouseEvent& event)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//获取鼠标指针属性
	float posX = event.position.getX();
	float posY = event.position.getY();

	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock projDataLocker(this->getProject()->getLock());

		//计算分辨率
		double startTime = 0.;
		double endTime = 0.;
		std::tie(startTime, endTime) = this->getHViewPort();
		double totalLength = endTime - startTime;
		double ppb = this->getWidth() / totalLength;

		//选区
		double loopStartTime = 0., loopEndTime = 0.;
		std::tie(loopStartTime, loopEndTime) = this->getLoopRange();

		switch (this->rulerState)
		{
		case RulerState::Normal:
		{
			//标签覆盖
			if (this->getEditMode()) {
				//计算标签大小
				float height_label = this->sizes.height_timeRuler_label * screenSize.getHeight();
				float width_label = height_label;
				float height_labelBottomMargin = this->sizes.height_timeRuler_labelBottomMargin * screenSize.getHeight();

				//判断鼠标位置
				if (posY >= (this->getHeight() - height_labelBottomMargin - height_label) &&
					posY <= (this->getHeight() - height_labelBottomMargin)) {
					//选择标签
					int labelIndex = -1;
					int labelSize = vocalshaper::ProjectDAO::labelSize(
						this->getProject()->getPtr());
					for (int i = labelSize; i >= 0; i--) {
						auto label = vocalshaper::ProjectDAO::getLabel(
							this->getProject()->getPtr(), i);
						if (label) {
							float labelPos = (vocalshaper::LabelDAO::getPosition(label) - startTime) * ppb;

							if ((labelPos + width_label / 2) >= posX && (labelPos - width_label / 2) <= posX) {
								labelIndex = i;
								break;
							}
						}
					}

					//如果标签已选择
					if (labelIndex >= 0 && labelIndex < labelSize) {
						{
							//计算时间
							auto label = vocalshaper::ProjectDAO::getLabel(
								this->getProject()->getPtr(), labelIndex);
							double x = vocalshaper::LabelDAO::getPosition(label);
							double xInBar = this->getProject()->getBeat()->getBarAtTime(x);
							double time = this->getProject()->getTempo()->get_t(x);
							uint8_t beat = this->getProject()->getBeat()->getBeatAtTime(x);
							uint8_t base = this->getProject()->getBeat()->getBaseAtTime(x);
							double xFromBar
								= (x - this->getProject()->getBeat()->getTimeAtBar(std::floor(xInBar))) * (base / 4.);
							double tempo = this->getProject()->getTempo()->get_T(x);

							//设置预览位置
							this->timeValue->setValue(x, xInBar, xFromBar, time, beat, base, tempo);
							this->timeValue->showAt(juce::Point<int>(
								(x - startTime) * ppb, this->getHeight()));
						}

						//更改鼠标并刷新
						this->setMouseCursor(juce::MouseCursor::PointingHandCursor);
						this->repaint();
						return;
					}
				}

			}

			//选区覆盖
			if (loopEndTime > loopStartTime) {
				//计算选区位置
				float width_loopJudgeArea = this->sizes.width_timeRuler_loopJudgeArea * screenSize.getWidth();
				float startLoopPos = (loopStartTime - startTime) * ppb;
				float endLoopPos = (loopEndTime - startTime) * ppb;
				float startLoopJudgePos = startLoopPos - width_loopJudgeArea;
				float endLoopJudgePos = endLoopPos + width_loopJudgeArea;

				//判断选区
				if (posX >= startLoopJudgePos && posX <= startLoopPos) {
					{
						//计算时间
						double x = loopStartTime;
						double xInBar = this->getProject()->getBeat()->getBarAtTime(x);
						double time = this->getProject()->getTempo()->get_t(x);
						uint8_t beat = this->getProject()->getBeat()->getBeatAtTime(x);
						uint8_t base = this->getProject()->getBeat()->getBaseAtTime(x);
						double xFromBar
							= (x - this->getProject()->getBeat()->getTimeAtBar(std::floor(xInBar))) * (base / 4.);
						double tempo = this->getProject()->getTempo()->get_T(x);

						//设置预览位置
						this->timeValue->setValue(x, xInBar, xFromBar, time, beat, base, tempo);
						this->timeValue->showAt(juce::Point<int>(
							startLoopPos, this->getHeight()));
					}

					//更改鼠标并刷新
					this->setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
					this->repaint();
					return;
				}
				else if (posX >= endLoopPos && posX <= endLoopJudgePos) {
					{
						//计算时间
						double x = loopEndTime;
						double xInBar = this->getProject()->getBeat()->getBarAtTime(x);
						double time = this->getProject()->getTempo()->get_t(x);
						uint8_t beat = this->getProject()->getBeat()->getBeatAtTime(x);
						uint8_t base = this->getProject()->getBeat()->getBaseAtTime(x);
						double xFromBar
							= (x - this->getProject()->getBeat()->getTimeAtBar(std::floor(xInBar))) * (base / 4.);
						double tempo = this->getProject()->getTempo()->get_T(x);

						//设置预览位置
						this->timeValue->setValue(x, xInBar, xFromBar, time, beat, base, tempo);
						this->timeValue->showAt(juce::Point<int>(
							endLoopPos, this->getHeight()));
					}

					//更改鼠标并刷新
					this->setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
					this->repaint();
					return;
				}
			}

			{
				//计算时间
				double x = startTime + posX / ppb;
				double xInBar = this->getProject()->getBeat()->getBarAtTime(x);
				double time = this->getProject()->getTempo()->get_t(x);
				uint8_t beat = this->getProject()->getBeat()->getBeatAtTime(x);
				uint8_t base = this->getProject()->getBeat()->getBaseAtTime(x);
				double xFromBar
					= (x - this->getProject()->getBeat()->getTimeAtBar(std::floor(xInBar))) * (base / 4.);
				double tempo = this->getProject()->getTempo()->get_T(x);

				//设置预览位置
				this->timeValue->setValue(x, xInBar, xFromBar, time, beat, base, tempo);
				this->timeValue->showAt(juce::Point<int>(
					posX, this->getHeight()));
			}

			//更改鼠标并刷新
			this->setMouseCursor(juce::MouseCursor::NormalCursor);
			this->repaint();
			break;
		}
		case RulerState::LoopSP:
		{
			//计算时间
			double time = startTime + posX / ppb;
			time = vocalshaper::adsorb(time, this->getAdsorb());
			if (time < 0) { time = 0; }
			if (time > this->totalTime) { time = totalTime; }
			if (time > loopEndTime) { time = loopEndTime; }

			//发送更改
			this->setLoopRangeMethod(time, loopEndTime);

			//指针跟随
			if (this->getCurrentPosition() < time ||
				this->getCurrentPosition() > loopEndTime) {
				this->setCurrentPositionMethod(time);
			}

			{
				//计算时间
				double x = time;
				double xInBar = this->getProject()->getBeat()->getBarAtTime(x);
				double t = this->getProject()->getTempo()->get_t(x);
				uint8_t beat = this->getProject()->getBeat()->getBeatAtTime(x);
				uint8_t base = this->getProject()->getBeat()->getBaseAtTime(x);
				double xFromBar
					= (x - this->getProject()->getBeat()->getTimeAtBar(std::floor(xInBar))) * (base / 4.);
				double tempo = this->getProject()->getTempo()->get_T(x);

				//设置预览位置
				this->timeValue->setValue(x, xInBar, xFromBar, t, beat, base, tempo);
				this->timeValue->showAt(juce::Point<int>(
					(time - startTime)* ppb, this->getHeight()));
			}

			//刷新
			this->repaint();
			break;
		}
		case RulerState::LoopEP:
		{
			//计算时间
			double time = startTime + posX / ppb;
			time = vocalshaper::adsorb(time, this->getAdsorb());
			if (time < 0) { time = 0; }
			if (time > this->totalTime) { time = totalTime; }
			if (time < loopStartTime) { time = loopStartTime; }

			//发送更改
			this->setLoopRangeMethod(loopStartTime, time);

			//指针跟随
			if (this->getCurrentPosition() > time ||
				this->getCurrentPosition() < loopStartTime) {
				this->setCurrentPositionMethod(time);
			}

			{
				//计算时间
				double x = time;
				double xInBar = this->getProject()->getBeat()->getBarAtTime(x);
				double t = this->getProject()->getTempo()->get_t(x);
				uint8_t beat = this->getProject()->getBeat()->getBeatAtTime(x);
				uint8_t base = this->getProject()->getBeat()->getBaseAtTime(x);
				double xFromBar
					= (x - this->getProject()->getBeat()->getTimeAtBar(std::floor(xInBar))) * (base / 4.);
				double tempo = this->getProject()->getTempo()->get_T(x);

				//设置预览位置
				this->timeValue->setValue(x, xInBar, xFromBar, t, beat, base, tempo);
				this->timeValue->showAt(juce::Point<int>(
					(time - startTime) * ppb, this->getHeight()));
			}

			//刷新
			this->repaint();
			break;
		}
		case RulerState::Label:
		{
			//计算时间
			double time = startTime + posX / ppb;
			time = vocalshaper::adsorb(time, this->getAdsorb());

			//限制时间
			{
				double timeMin = 0, timeMax = this->totalTime;
				int labelSize = vocalshaper::ProjectDAO::labelSize(
					this->getProject()->getPtr());
				if (this->labelEditingIndex > 0) {
					auto lastLabel = vocalshaper::ProjectDAO::getLabel(
						this->getProject()->getPtr(), this->labelEditingIndex - 1);
					if (lastLabel) {
						timeMin = vocalshaper::LabelDAO::getPosition(lastLabel);
					}
				}
				if (this->labelEditingIndex < labelSize - 1) {
					auto nextLabel = vocalshaper::ProjectDAO::getLabel(
						this->getProject()->getPtr(), this->labelEditingIndex + 1);
					if (nextLabel) {
						timeMax = vocalshaper::LabelDAO::getPosition(nextLabel);
					}
				}
				if (time < timeMin) { time = timeMin; }
				if (time > timeMax) { time = timeMax; }
			}

			//更新缓存
			this->labelEditingTime = time;

			//关闭时间值预览
			this->timeValue->close();

			//刷新
			this->repaint();
			break;
		}
		case RulerState::Cursor:
		{
			//计算时间
			double posTime = startTime + posX / ppb;
			double time = vocalshaper::adsorb(posTime, this->getAdsorb());
			if (time < 0) { time = 0; }
			if (time > this->totalTime) { time = totalTime; }

			//发送更改
			this->setCurrentPositionMethod(time);

			//选区跟随或建立
			if (loopEndTime > loopStartTime) {

				if (time < loopStartTime) {
					this->setLoopRangeMethod(time, loopEndTime);
				}
				else if (time > loopEndTime) {
					this->setLoopRangeMethod(loopStartTime, time);
				}
			}
			else {
				if (posTime != this->timePressed) {
					this->setLoopRangeMethod(
						std::min(time, vocalshaper::adsorb(this->timePressed, this->getAdsorb())),
						std::max(time, vocalshaper::adsorb(this->timePressed, this->getAdsorb())));
				}
			}

			{
				//计算时间
				double x = time;
				double xInBar = this->getProject()->getBeat()->getBarAtTime(x);
				double time = this->getProject()->getTempo()->get_t(x);
				uint8_t beat = this->getProject()->getBeat()->getBeatAtTime(x);
				uint8_t base = this->getProject()->getBeat()->getBaseAtTime(x);
				double xFromBar
					= (x - this->getProject()->getBeat()->getTimeAtBar(std::floor(xInBar))) * (base / 4.);
				double tempo = this->getProject()->getTempo()->get_T(x);

				//设置预览位置
				this->timeValue->setValue(x, xInBar, xFromBar, time, beat, base, tempo);
				this->timeValue->showAt(juce::Point<int>(
					(x - startTime) * ppb, this->getHeight()));
			}

			//刷新
			this->repaint();
			break;
		}
		}
	}
}

void TimeRuler::mouseDrag(const juce::MouseEvent& event)
{
	this->mouseMove(event);
}

void TimeRuler::mouseUp(const juce::MouseEvent& event)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//获取鼠标指针属性
	float posX = event.position.getX();
	float posY = event.position.getY();

	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock projDataLocker(this->getProject()->getLock());

		//计算标签大小
		float height_label = this->sizes.height_timeRuler_label * screenSize.getHeight();
		float width_label = height_label;
		float height_labelBottomMargin = this->sizes.height_timeRuler_labelBottomMargin * screenSize.getHeight();

		//计算分辨率
		double startTime = 0.;
		double endTime = 0.;
		std::tie(startTime, endTime) = this->getHViewPort();
		double totalLength = endTime - startTime;
		double ppb = this->getWidth() / totalLength;

		//循环范围
		double loopStartTime = 0., loopEndTime = 0.;
		std::tie(loopStartTime, loopEndTime) = this->getLoopRange();

		//左键
		if (event.mods.isLeftButtonDown()) {
			switch (this->rulerState)
			{
			case RulerState::Label:
			{
				if (this->getEditMode()) {
					//计算时间
					double time = startTime + posX / ppb;

					//展开面板或更新标签
					if (time == this->timePressed) {
						int labelSize = vocalshaper::ProjectDAO::labelSize(
							this->getProject()->getPtr());
						if (this->labelEditingIndex >= 0 && this->labelEditingIndex < labelSize) {
							//获取标签
							auto label = vocalshaper::ProjectDAO::getLabel(
								this->getProject()->getPtr(), this->labelEditingIndex);
							float labelSelectedPos = -1;
							if (label) {
								labelSelectedPos = (vocalshaper::LabelDAO::getPosition(label) - startTime) * ppb;
							}

							//显示标签编辑面板
							juce::Rectangle<int> labelRect(
								labelSelectedPos - width_label / 2, this->getHeight() - height_labelBottomMargin - height_label,
								width_label, height_label
							);
							this->showLabelEditor(labelEditingIndex, labelRect);
						}
					}
					else {
						//限制时间
						time = vocalshaper::adsorb(time, this->getAdsorb());
						{
							double timeMin = 0, timeMax = this->totalTime;
							int labelSize = vocalshaper::ProjectDAO::labelSize(
								this->getProject()->getPtr());
							if (this->labelEditingIndex > 0) {
								auto lastLabel = vocalshaper::ProjectDAO::getLabel(
									this->getProject()->getPtr(), this->labelEditingIndex - 1);
								if (lastLabel) {
									timeMin = vocalshaper::LabelDAO::getPosition(lastLabel);
								}
							}
							if (this->labelEditingIndex < labelSize - 1) {
								auto nextLabel = vocalshaper::ProjectDAO::getLabel(
									this->getProject()->getPtr(), this->labelEditingIndex + 1);
								if (nextLabel) {
									timeMax = vocalshaper::LabelDAO::getPosition(nextLabel);
								}
							}
							if (time < timeMin) { time = timeMin; }
							if (time > timeMax) { time = timeMax; }
						}

						//编辑事件
						using ActionType = vocalshaper::actions::label::PositionAction;
						ActionType::TargetType target;
						target.label = this->labelEditingIndex;
						auto action = std::make_unique<ActionType>(
							target, time, this->getProject()
							);

						//发送事件
						this->getProject()->getProcesser()->processEvent(std::move(action));
					}

					//刷新
					this->repaint();
					break;
				}
			}
			}

			//重置缓存
			this->rulerState = RulerState::Normal;
			this->timePressed = -1;
			this->labelEditingIndex = -1;
			this->labelEditingTime = -1;
		}
		else if (event.mods.isRightButtonDown()) {
			if (this->rulerState == RulerState::Normal) {
				//标签覆盖
				int labelSelectedIndex = -1;
				float labelSelectedPos = -1.f;
				if (this->getEditMode()) {
					//判断鼠标位置
					if (posY >= (this->getHeight() - height_labelBottomMargin - height_label) &&
						posY <= (this->getHeight() - height_labelBottomMargin)) {
						//选择标签
						int labelSize = vocalshaper::ProjectDAO::labelSize(
							this->getProject()->getPtr());
						for (int i = labelSize; i >= 0; i--) {
							auto label = vocalshaper::ProjectDAO::getLabel(
								this->getProject()->getPtr(), i);
							if (label) {
								float labelPos = (vocalshaper::LabelDAO::getPosition(label) - startTime) * ppb;

								if ((labelPos + width_label / 2) >= posX && (labelPos - width_label / 2) <= posX) {
									labelSelectedIndex = i;
									labelSelectedPos = labelPos;
									break;
								}
							}
						}
					}
				}

				//右键菜单
				juce::PopupMenu menu;
				menu.addItem(1, this->tr("bt_TimeUnselect"),
					(loopEndTime > loopStartTime));
				menu.addSeparator();
				menu.addItem(2, this->tr("bt_AddLabel"),
					(labelSelectedIndex == -1) && this->getEditMode());
				menu.addItem(3, this->tr("bt_EditLabel"),
					(labelSelectedIndex > -1) && this->getEditMode());
				menu.addItem(4, this->tr("bt_RemoveLabel"),
					(labelSelectedIndex > -1) && this->getEditMode());

				//显示右键菜单
				int result = menu.show();

				//结果判断
				switch (result)
				{
				case 1:
				{
					//取消选区
					this->setLoopRangeMethod(-1, -1);
					break;
				}
				case 2:
				{
					//计算时间
					double time = startTime + posX / ppb;
					time = vocalshaper::adsorb(time, this->getAdsorb());

					int labelSize = vocalshaper::ProjectDAO::labelSize(
						this->getProject()->getPtr());
					int newIndex = labelSize;
					for (int i = labelSize; i >= 0; i--) {
						auto label = vocalshaper::ProjectDAO::getLabel(
							this->getProject()->getPtr(), i);
						if (label) {
							//获取label时间
							double labelTime = vocalshaper::LabelDAO::getPosition(label);

							//计算label索引
							if (labelTime > time) {
								newIndex = i;
							}
						}
					}

					//建立标签
					auto label = new vocalshaper::Label;
					vocalshaper::LabelDAO::setPosition(label, time);

					//编辑事件
					using ActionType = vocalshaper::actions::project::AddLabelAction;
					ActionType::TargetType target{};
					auto action = std::make_unique<ActionType>(
						target, newIndex, label, this->getProject()
						);

					//发送事件
					this->getProject()->getProcesser()->processEvent(std::move(action));

					//刷新
					this->repaint();
					break;
				}
				case 3:
				{
					//显示标签编辑面板
					juce::Rectangle<int> labelRect(
						labelSelectedPos - width_label / 2, this->getHeight() - height_labelBottomMargin - height_label,
						width_label, height_label
					);
					this->showLabelEditor(labelSelectedIndex, labelRect);
					break;
				}
				case 4:
				{
					//编辑事件
					using ActionType = vocalshaper::actions::project::RemoveLabelAction;
					ActionType::TargetType target{};
					auto action = std::make_unique<ActionType>(
						target, labelSelectedIndex, nullptr, this->getProject()
						);

					//发送事件
					auto ptrAction = action.get();
					this->getProject()->getProcesser()->processEvent(std::move(action));

					//刷新
					this->repaint();
					break;
				}
				}
			}
		}

		//设置鼠标并刷新
		{
			//标签覆盖
			if (this->getEditMode()) {
				//计算标签大小
				float height_label = this->sizes.height_timeRuler_label * screenSize.getHeight();
				float width_label = height_label;
				float height_labelBottomMargin = this->sizes.height_timeRuler_labelBottomMargin * screenSize.getHeight();

				//判断鼠标位置
				if (posY >= (this->getHeight() - height_labelBottomMargin - height_label) &&
					posY <= (this->getHeight() - height_labelBottomMargin)) {
					//选择标签
					int labelIndex = -1;
					int labelSize = vocalshaper::ProjectDAO::labelSize(
						this->getProject()->getPtr());
					for (int i = labelSize; i >= 0; i--) {
						auto label = vocalshaper::ProjectDAO::getLabel(
							this->getProject()->getPtr(), i);
						if (label) {
							float labelPos = (vocalshaper::LabelDAO::getPosition(label) - startTime) * ppb;

							if ((labelPos + width_label / 2) >= posX && (labelPos - width_label / 2) <= posX) {
								labelIndex = i;
								break;
							}
						}
					}

					//如果标签已选择
					if (labelIndex >= 0 && labelIndex < labelSize) {
						//关闭时间值预览
						this->timeValue->close();

						//更改鼠标并刷新
						this->setMouseCursor(juce::MouseCursor::PointingHandCursor);
						this->repaint();
						return;
					}
				}
			}

			//选区覆盖
			if (loopEndTime > loopStartTime) {
				//计算选区位置
				float width_loopJudgeArea = this->sizes.width_timeRuler_loopJudgeArea * screenSize.getWidth();
				float startLoopPos = (loopStartTime - startTime) * ppb;
				float endLoopPos = (loopEndTime - startTime) * ppb;
				float startLoopJudgePos = startLoopPos - width_loopJudgeArea;
				float endLoopJudgePos = endLoopPos + width_loopJudgeArea;

				//判断选区
				if (posX >= startLoopJudgePos && posX <= startLoopPos) {
					{
						//计算时间
						double x = loopStartTime;
						double xInBar = this->getProject()->getBeat()->getBarAtTime(x);
						double time = this->getProject()->getTempo()->get_t(x);
						uint8_t beat = this->getProject()->getBeat()->getBeatAtTime(x);
						uint8_t base = this->getProject()->getBeat()->getBaseAtTime(x);
						double xFromBar
							= (x - this->getProject()->getBeat()->getTimeAtBar(std::floor(xInBar))) * (base / 4.);
						double tempo = this->getProject()->getTempo()->get_T(x);

						//设置预览位置
						this->timeValue->setValue(x, xInBar, xFromBar, time, beat, base, tempo);
						this->timeValue->showAt(juce::Point<int>(
							(x - startTime) * ppb, this->getHeight()));
					}

					//更改鼠标并刷新
					this->setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
					this->repaint();
					return;
				}
				else if (posX >= endLoopPos && posX <= endLoopJudgePos) {
					{
						//计算时间
						double x = loopEndTime;
						double xInBar = this->getProject()->getBeat()->getBarAtTime(x);
						double time = this->getProject()->getTempo()->get_t(x);
						uint8_t beat = this->getProject()->getBeat()->getBeatAtTime(x);
						uint8_t base = this->getProject()->getBeat()->getBaseAtTime(x);
						double xFromBar
							= (x - this->getProject()->getBeat()->getTimeAtBar(std::floor(xInBar))) * (base / 4.);
						double tempo = this->getProject()->getTempo()->get_T(x);

						//设置预览位置
						this->timeValue->setValue(x, xInBar, xFromBar, time, beat, base, tempo);
						this->timeValue->showAt(juce::Point<int>(
							(x - startTime) * ppb, this->getHeight()));
					}

					//更改鼠标并刷新
					this->setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
					this->repaint();
					return;
				}
			}

			{
				//计算时间
				double x = startTime + posX / ppb;
				double xInBar = this->getProject()->getBeat()->getBarAtTime(x);
				double time = this->getProject()->getTempo()->get_t(x);
				uint8_t beat = this->getProject()->getBeat()->getBeatAtTime(x);
				uint8_t base = this->getProject()->getBeat()->getBaseAtTime(x);
				double xFromBar
					= (x - this->getProject()->getBeat()->getTimeAtBar(std::floor(xInBar))) * (base / 4.);
				double tempo = this->getProject()->getTempo()->get_T(x);

				//设置预览位置
				this->timeValue->setValue(x, xInBar, xFromBar, time, beat, base, tempo);
				this->timeValue->showAt(juce::Point<int>(
					posX, this->getHeight()));
			}

			//更改鼠标并刷新
			this->setMouseCursor(juce::MouseCursor::NormalCursor);
			this->repaint();
		}	
	}

	//超范围强制关闭
	if (posY < 0 || posY > this->getHeight()) {
		this->timeValue->close();
		this->setMouseCursor(juce::MouseCursor::NormalCursor);
	}
}

void TimeRuler::mouseExit(const juce::MouseEvent& event)
{
	if (this->rulerState != RulerState::Normal) {
		this->rulerState = RulerState::Normal;
		this->timePressed = -1;
		this->labelEditingIndex = -1;
		this->labelEditingTime = -1;
		this->setMouseCursor(juce::MouseCursor::NormalCursor);
		this->repaint();
	}
	this->timeValue->close();
}

void TimeRuler::mouseDoubleClick(const juce::MouseEvent& event)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//获取鼠标指针属性
	float posX = event.position.getX();
	float posY = event.position.getY();

	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock projDataLocker(this->getProject()->getLock());

		//计算分辨率
		double startTime = 0.;
		double endTime = 0.;
		std::tie(startTime, endTime) = this->getHViewPort();
		double totalLength = endTime - startTime;
		double ppb = this->getWidth() / totalLength;

		//左键
		if (event.mods.isLeftButtonDown()) {
			if (this->getEditMode()) {
				//计算标签大小
				float height_label = this->sizes.height_timeRuler_label * screenSize.getHeight();
				float width_label = height_label;
				float height_labelBottomMargin = this->sizes.height_timeRuler_labelBottomMargin * screenSize.getHeight();

				//计算时间
				double time = startTime + posX / ppb;
				time = vocalshaper::adsorb(time, this->getAdsorb());

				int labelSize = vocalshaper::ProjectDAO::labelSize(
					this->getProject()->getPtr());
				int newIndex = labelSize;
				for (int i = labelSize; i >= 0; i--) {
					auto label = vocalshaper::ProjectDAO::getLabel(
						this->getProject()->getPtr(), i);
					if (label) {
						//获取label时间
						double labelTime = vocalshaper::LabelDAO::getPosition(label);

						//计算label索引
						if (labelTime > time) {
							newIndex = i;
						}

						//获取label位置并确保指针不在标签上
						float labelPos = (labelTime - startTime) * ppb;
						if (posY >= (this->getHeight() - height_labelBottomMargin - height_label) &&
							posY <= (this->getHeight() - height_labelBottomMargin)) {
							if ((labelPos + width_label / 2) >= posX && (labelPos - width_label / 2) <= posX) {
								return;
							}
						}
					}
				}

				//建立标签
				auto label = new vocalshaper::Label;
				vocalshaper::LabelDAO::setPosition(label, time);
				//begin test
				/*juce::String labelData("beat = 3");
				vocalshaper::LabelDAO::setData(label, labelData);*/
				//end test

				//编辑事件
				using ActionType = vocalshaper::actions::project::AddLabelAction;
				ActionType::TargetType target{};
				auto action = std::make_unique<ActionType>(
					target, newIndex, label, this->getProject()
					);

				//发送事件
				this->getProject()->getProcesser()->processEvent(std::move(action));

				//关闭时间值预览
				this->timeValue->close();

				//刷新
				this->repaint();
			}
		}
	}
}

void TimeRuler::listenLabelChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
{
	juce::ScopedReadLock projLocker(this->getProjLock());
	if (this->getProject() != action.getProxy()) {
		return;
	}

	//获取消息管理器
	auto messageManager = juce::MessageManager::getInstance();
	if (!messageManager) {
		return;
	}

	if (action.getBaseType() == vocalshaper::actions::ActionBase::Type::Project) {
		if (
			action.getActionType() == vocalshaper::actions::ProjectAction::Actions::AddLabel ||
			action.getActionType() == vocalshaper::actions::ProjectAction::Actions::RemoveLabel) {

			//刷新控件
			messageManager->callAsync([this] {this->labelEditor->close(); this->repaint(); });

			//关闭时间值预览
			messageManager->callAsync([this] {this->timeValue->close(); });
		}
	}
	else if (action.getBaseType() == vocalshaper::actions::ActionBase::Type::Label) {
		//刷新控件
		messageManager->callAsync([this] {this->labelEditor->close(); this->repaint(); });

		//关闭时间值预览
		messageManager->callAsync([this] {this->timeValue->close(); });
	}
}

void TimeRuler::showLabelEditor(int labelIndex, juce::Rectangle<int> place)
{
	//设置编辑器标签号
	this->labelEditor->setLabelIndex(labelIndex);

	//显示控件
	this->labelEditor->showAt(place);
}
