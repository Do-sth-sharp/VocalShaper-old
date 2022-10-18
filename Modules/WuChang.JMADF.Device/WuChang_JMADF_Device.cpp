#include "WuChang_JMADF_Device.h"

bool WuChang_JMADF_Device::init()
{
	jmadf::RegisterInterface<juce::Component*, juce::Rectangle<int>&>(
		"GetScreenSize",
		[this](const juce::String&, juce::Component* component, juce::Rectangle<int>& result) {
			//获取屏幕
			auto ptrScreen = WuChang_JMADF_Device::getCurrentScreen(component);
			if (!ptrScreen) {
				return;
			}

			//如果未改变，则返回缓存
			if (ptrScreen == this->ptrCurrentDisplay && ptrScreen->scale == this->currentScale) {
				result = this->sizeTemp;
				return;
			}

			//获取基础屏幕属性
			static juce::Rectangle<int> rect, rectTotal;
			rect = ptrScreen->userArea;
			rectTotal = ptrScreen->totalArea;
			static int width = 0, height = 0, widthT = 0, heightT = 0;
			width = rect.getWidth();
			height = rect.getHeight();
			widthT = rectTotal.getWidth();
			heightT = rectTotal.getHeight();

			//校正屏幕宽高，使长边为宽
			if (width < height) {
				int temp = width;
				width = height;
				height = temp;
			}
			if (widthT < heightT) {
				int temp = widthT;
				widthT = heightT;
				heightT = temp;
			}

			//计算当前屏幕宽高比
			static double pNow = 9.0 / 16.0, pTotal = 9.0 / 16.0;
			pNow = static_cast<double>(height) / static_cast<double>(width);
			pTotal = static_cast<double>(heightT) / static_cast<double>(widthT);

			//和标准宽高比比较并计算缩放比例
			constexpr double proportion = 816.0 / 1536.0;
			constexpr double prop4Scale = 1080.0 / 1920.0;
			const double scaleM = 1.25 / 1920;
			const double scaleN = 1.0 / ((pTotal >= prop4Scale) ? widthT : (heightT / prop4Scale));
			const double scaleMN = scaleN / scaleM;

			//矫正异形宽高比
			if (pNow > proportion) {
				height = width * proportion;
			}
			else if (pNow < proportion) {
				width = height / proportion;
			}

			//设定返回值
			result.setWidth(width * scaleMN);
			result.setHeight(height * scaleMN);

			//记录缓存
			this->sizeTemp = result;
			this->currentScale = ptrScreen->scale;
			this->ptrCurrentDisplay = ptrScreen;
		}
	);
	jmadf::RegisterInterface<juce::Component*, juce::Rectangle<int>&>(
		"GetScreenSizeTruth",
		[](const juce::String&, juce::Component* component, juce::Rectangle<int>& result) {
			result = WuChang_JMADF_Device::getScreenSize(component);
		}
	);
	jmadf::RegisterInterface<juce::Component*, double&>(
		"GetScreenScale",
		[](const juce::String&, juce::Component* component, double& result) {
			result = WuChang_JMADF_Device::getScreenScale(component);
		}
	);
	return true;
}

void WuChang_JMADF_Device::destory()
{
}

const juce::Rectangle<int> WuChang_JMADF_Device::getScreenSize(juce::Component* component)
{
	const juce::Displays::Display* display = WuChang_JMADF_Device::getCurrentScreen(component);
	if (display == nullptr) {
		return juce::Rectangle<int>();
	}
	return display->userArea;
}

const double WuChang_JMADF_Device::getScreenScale(juce::Component* component)
{
	const juce::Displays::Display* display = WuChang_JMADF_Device::getCurrentScreen(component);
	if (display == nullptr) {
		return 1;
	}
	return display->scale;
}

const juce::Displays::Display* WuChang_JMADF_Device::getCurrentScreen(juce::Component* component)
{
	return juce::Desktop::getInstance().getDisplays().getDisplayForRect(component->getScreenBounds());
}
