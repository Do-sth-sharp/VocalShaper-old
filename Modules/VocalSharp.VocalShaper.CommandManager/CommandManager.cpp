#include "CommandManager.h"
#include <libJModule.h>

CommandManager::CommandManager()
	:ApplicationCommandTarget()
{
	//获取翻译器
	jmadf::CallInterface<std::function<const juce::String(const juce::String&)>&>(
		"WuChang.JMADF.Translates", "GetFunc",
		this->tr
		);

	//创建管理器
	this->acm = std::make_unique<juce::ApplicationCommandManager>();

	//初始化命令列表

	//应用命令列表
	this->acm->registerAllCommandsForTarget(this);
}

void CommandManager::addCommandFunc(const juce::String& name, CommandFunction func)
{
	int commandID = this->getCommandID(name);
	if (commandID == -1) {
		return;
	}

	this->funcList.insert(std::make_pair(commandID, func));
}

int CommandManager::getCommandID(const juce::String& name)
{
	int index = -1;
	for (int i = 0; i < this->infoList.size(); i++) {
		if (this->infoList.getReference(i).name == name) {
			index = i;
			break;
		}
	}
	return index;
}

juce::ApplicationCommandManager* CommandManager::getManager()
{
	return this->acm.get();
}

juce::ApplicationCommandTarget* CommandManager::getNextCommandTarget()
{
	return nullptr;
}

void CommandManager::getAllCommands(juce::Array<juce::CommandID>& commands)
{
	commands.clear();
	for (int i = 0; i < this->infoList.size(); i++) {
		commands.add(i);
	}
}

void CommandManager::getCommandInfo(
	juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
	if (commandID >= 0 && commandID < this->infoList.size()) {
		auto& info = this->infoList.getReference(commandID);
		result.commandID = commandID;
		result.shortName = info.shortName;
		result.description = info.description;
		result.categoryName = info.categoryName;
		result.defaultKeypresses = info.defaultKeypresses;
	}
}

bool CommandManager::perform(const juce::ApplicationCommandTarget::InvocationInfo& info)
{
	auto it = this->funcList.find(info.commandID);
	if (it == this->funcList.end()) {
		return false;
	}

	(*it).second();
	return true;
}