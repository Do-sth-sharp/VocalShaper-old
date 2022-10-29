#include "HScroller.h"
#include <libJModule.h>

HScroller::HScroller()
	: ScrollerBase(false)
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
		"main", "color", "cursor", this->colors.cursor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "horizontalScroller-loopBlock", this->colors.horizontalScroller_loopBlock, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "horizontalScroller-labelBlock", this->colors.horizontalScroller_labelBlock, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-beat-min", this->sizes.width_beat_min, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-beat-max", this->sizes.width_beat_max, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-horizontalScroller-loopBlock", this->sizes.height_horizontalScroller_loopBlock, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-horizontalScroller-loopBlockTopMargin", this->sizes.height_horizontalScroller_loopBlockTopMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-horizontalScroller-loopBlock", this->sizes.height_horizontalScroller_labelBlock, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-horizontalScroller-loopBlockTopMargin", this->sizes.height_horizontalScroller_labelBlockTopMargin, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-horizontalScroller-loopBlock", this->sizes.width_horizontalScroller_labelBlock, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "width-cursor", this->sizes.width_cursor, result
		);

	//position
	//scale
	//resource
}

void HScroller::limitSize(double& sp, double& ep, double nailPer)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	{
		juce::ScopedReadLock tempLocker(this->tempLock);
		if (this->ptrTemp) {
			//计算时间轴最大最小大小
			int beatMaxWidth = this->sizes.width_beat_max * screenSize.getWidth();
			int beatMinWidth = this->sizes.width_beat_min * screenSize.getWidth();

			double lengthSize = this->ptrTemp->projectLengthTemp;

			int maxSize = lengthSize * beatMaxWidth;
			int minSize = lengthSize * beatMinWidth;

			//计算最大最小delta
			double deltaMax = this->getWidth() / (double)minSize;
			double deltaMin = this->getWidth() / (double)maxSize;

			//根据delta限制滑块大小
			double delta = ep - sp;
			if (nailPer == 0.) {
				if (sp < 0.) { sp = 0.; delta = ep - sp; }
				if (ep > 1.) { ep = 1.; delta = ep - sp; }
				if (delta > deltaMax) { ep = sp + deltaMax; }
				if (delta < deltaMin) { ep = sp + deltaMin; }
			}
			else if (nailPer == 1.) {
				if (sp < 0.) { sp = 0.; delta = ep - sp; }
				if (ep > 1.) { ep = 1.; delta = ep - sp; }
				if (delta > deltaMax) { sp = ep - deltaMax; }
				if (delta < deltaMin) { sp = ep - deltaMin; }
			}
			else {
				if (delta > 1.) { delta = 1.; }
				if (sp < 0.) { sp = 0.; ep = delta; }
				if (ep > 1.) { ep = 1.; sp = 1 - delta; }

				double nail = sp + delta * nailPer;
				if (delta > deltaMax) { sp = nail - deltaMax * nailPer; ep = sp + deltaMax; }
				if (delta < deltaMin) { sp = nail - deltaMin * nailPer; ep = sp + deltaMin; }
			}
		}
	}
}

void HScroller::paintPreView(juce::Graphics& g, int width, int height)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	{
		juce::ScopedReadLock tempLocker(this->tempLock);
		juce::ScopedReadLock proxyLocker(this->project->getLock());
		if (this->ptrTemp && this->project) {
			//计算指针位置
			float curPos
				= width * (this->ptrTemp->currentPositionTemp / this->ptrTemp->projectLengthTemp);

			//计算指针宽度
			float width_cursor = this->sizes.width_cursor * screenSize.getWidth();

			//绘制指针
			juce::Rectangle<float> cursorRect(
				curPos - width_cursor / 2, 0,
				width_cursor, height
				);
			g.setColour(this->colors.cursor);
			g.fillRect(cursorRect);

			//判断选区是否有效
			if (this->ptrTemp->loopET > this->ptrTemp->loopST) {
				//计算色块区域大小
				int loopRectPosY = this->sizes.height_horizontalScroller_loopBlockTopMargin * screenSize.getHeight();
				int height_loopRect = this->sizes.height_horizontalScroller_loopBlock * screenSize.getHeight();

				float loopST = (this->ptrTemp->loopST / this->ptrTemp->projectLengthTemp) * width;
				float loopET = (this->ptrTemp->loopET / this->ptrTemp->projectLengthTemp) * width;

				//绘制色块区域
				juce::Rectangle<float> rectLoopBlock(
					loopST, loopRectPosY,
					loopET - loopST, height_loopRect
				);
				g.setColour(this->colors.horizontalScroller_loopBlock);
				g.fillRect(rectLoopBlock);
			}

			//绘制标签色块
			{
				//计算色块大小
				int labelRectPosY = this->sizes.height_horizontalScroller_labelBlockTopMargin * screenSize.getHeight();
				int height_labelRect = this->sizes.height_horizontalScroller_labelBlock * screenSize.getHeight();
				float width_labelRect = this->sizes.width_horizontalScroller_labelBlock * screenSize.getWidth();

				//绘制色块
				int labelSize = vocalshaper::ProjectDAO::labelSize(this->project->getPtr());
				for (int i = 0; i < labelSize; i++) {
					auto label = vocalshaper::ProjectDAO::getLabel(this->project->getPtr(), i);
					if (label) {
						float labelPos
							= width * (vocalshaper::LabelDAO::getPosition(label) / this->ptrTemp->projectLengthTemp);
						juce::Rectangle<float> rectLabel(
							labelPos - width_labelRect / 2, labelRectPosY,
							width_labelRect, height_labelRect
						);
						g.setColour(this->colors.horizontalScroller_labelBlock);
						g.fillRect(rectLabel);
					}
				}
			}
		}
	}
}

void HScroller::noticeChange(double sp, double ep)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	{
		juce::ScopedReadLock tempLocker(this->tempLock);
		if (this->ptrTemp) {
			//计算时间
			double lengthSize = this->ptrTemp->projectLengthTemp;
			double startTime = sp * lengthSize;
			double endTime = ep * lengthSize;

			//发送更改
			this->setHViewPortMethod(startTime, endTime);
		}
	}
}

void HScroller::refreshSizeOnProjectLengthChanged(
	double lastLength, double length, double& sp, double& ep)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算时间
	double startTime = sp * lastLength;
	double endTime = ep * lastLength;

	//计算缩放
	sp = startTime / length;
	ep = endTime / length;
}

void HScroller::refreshSizeOnResized(int lastSize, int size, double& sp, double& ep)
{
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	{
		juce::ScopedReadLock tempLocker(this->tempLock);
		if (this->ptrTemp) {
			//计算时间
			double startTime = sp * this->ptrTemp->projectLengthTemp;
			double endTime = ep * this->ptrTemp->projectLengthTemp;

			//计算分辨率并以维持起始位置和分辨率为目的调整
			double ratio = (endTime - startTime) / lastSize;
			endTime = startTime + ratio * size;

			//计算缩放
			sp = startTime / this->ptrTemp->projectLengthTemp;
			ep = endTime / this->ptrTemp->projectLengthTemp;
		}
	}
}
