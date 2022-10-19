#include "VocalSharp_VocalShaper_ThemeManager.h"

bool VocalSharp_VocalShaper_ThemeManager::init()
{
	if (!(
		jmadf::LoadModule("WuChang.JMADF.GlobalConfigs")
		)) {
		return false;
	}
	if (
		!jmadf::CheckInterface<const juce::String&, juce::var*&, bool&>(
			"WuChang.JMADF.GlobalConfigs", "GetReference")
		) {
		jmadf::RaiseException("@WuChang.JMADF.GlobalConfigs:Bad Interfaces!");
		return false;
	}

	//获取当前主题设置
	juce::String theme;
	{
		juce::var* config = nullptr;
		bool ok = false;
		jmadf::CallInterface<const juce::String&, juce::var*&, bool&>(
			"WuChang.JMADF.GlobalConfigs", "GetReference",
			"config", config, ok
			);

		if (ok && (config != nullptr)) {
			if ((*config)["Theme"].isString()) {
				theme = (*config)["Theme"].toString();
			}
		}
	}

	//判断主题是否在列表中
	auto themeList = jmadf::GetAllModulesInGroup("VocalShaper.Themes");
	if (!themeList.contains(theme)) {
		//设置一个默认的主题
		if (themeList.size() == 0) {
			jmadf::RaiseException("No available themes!");
			return false;
		}
		theme = themeList[0];
	}

	//获取要应用的模块列表
	juce::Array<juce::File> fileList;
	{
		auto ptrInfo = jmadf::GetModuleInfo();
		if (!ptrInfo) {
			jmadf::RaiseException("Can't get module info!");
			return false;
		}
		juce::String lafPath = ptrInfo->path + "/.laf";
		juce::File lafDir(lafPath);
		fileList = 
			lafDir.findChildFiles(juce::File::findFiles, false, "*.json", juce::File::FollowSymlinks::noCycles);
	}

	//更改每个模块的配置
	for (auto& file : fileList) {
		//建立文件流
		juce::FileOutputStream stream(file);
		stream.setPosition(0);
		stream.truncate();

		//生成json数据
		auto parentObject = new juce::DynamicObject;
		auto object = new juce::DynamicObject;
		parentObject->setProperty("module", theme);
		parentObject->setProperty("class", file.getFileNameWithoutExtension());
		object->setProperty("parent", juce::var(parentObject));
		juce::JSON::writeToStream(stream, juce::var(object), false);
	}

	return true;
}

void VocalSharp_VocalShaper_ThemeManager::destory()
{
}