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
	this->acm = std::make_unique<Manager>(this);

	//初始化命令列表
	this->infoList.add({
		.name = "New Project",
		.shortName = this->tr("sn_NewProject"),
		.description = this->tr("ds_NewProject"),
		.categoryName = this->tr("cn_File"),
		.defaultKeypresses = {juce::KeyPress('n', juce::ModifierKeys::Flags::ctrlModifier, 0)}
		});
	this->infoList.add({
		.name = "Open Project",
		.shortName = this->tr("sn_OpenProject"),
		.description = this->tr("ds_OpenProject"),
		.categoryName = this->tr("cn_File"),
		.defaultKeypresses = {juce::KeyPress('o', juce::ModifierKeys::Flags::ctrlModifier, 0)}
		});
	this->infoList.add({
		.name = "Show Start Menu",
		.shortName = this->tr("sn_ShowStartMenu"),
		.description = this->tr("ds_ShowStartMenu"),
		.categoryName = this->tr("cn_File"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Save Project",
		.shortName = this->tr("sn_SaveProject"),
		.description = this->tr("ds_SaveProject"),
		.categoryName = this->tr("cn_File"),
		.defaultKeypresses = {juce::KeyPress('s', juce::ModifierKeys::Flags::ctrlModifier, 0)}
		});
	this->infoList.add({
		.name = "Save All Project",
		.shortName = this->tr("sn_SaveAllProject"),
		.description = this->tr("ds_SaveAllProject"),
		.categoryName = this->tr("cn_File"),
		.defaultKeypresses = {juce::KeyPress('s', juce::ModifierKeys::Flags::ctrlModifier | juce::ModifierKeys::Flags::shiftModifier, 0)}
		});
	this->infoList.add({
		.name = "Close Project",
		.shortName = this->tr("sn_CloseProject"),
		.description = this->tr("ds_CloseProject"),
		.categoryName = this->tr("cn_File"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Close All Project",
		.shortName = this->tr("sn_CloseAllProject"),
		.description = this->tr("ds_CloseAllProject"),
		.categoryName = this->tr("cn_File"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Close Editor",
		.shortName = this->tr("sn_CloseEditor"),
		.description = this->tr("ds_CloseEditor"),
		.categoryName = this->tr("cn_File"),
		.defaultKeypresses = {}
		});


	//应用命令列表
	this->acm->registerAllCommandsForTarget(this);
}

void CommandManager::addCommandFunc(const juce::String& name, const CommandFunction& func, const juce::String& caller)
{
	int commandID = this->getCommandID(name);
	if (commandID == -1) {
		return;
	}

	juce::ScopedWriteLock locker(this->funcLock);
	this->funcList.insert(std::make_pair(commandID, std::make_pair(caller, func)));
}

void CommandManager::addFlagFunc(const juce::String& name, const FlagFunction& func, const juce::String& caller)
{
	int commandID = this->getCommandID(name);
	if (commandID == -1) {
		return;
	}

	juce::ScopedWriteLock locker(this->flagLock);
	this->flagList.insert(std::make_pair(commandID, std::make_pair(caller, func)));
}

int CommandManager::getCommandID(const juce::String& name)
{
	int index = -1;
	for (int i = 0; i < this->infoList.size(); i++) {
		if (this->infoList.getReference(i).name == name) {
			index = i + 1;
			break;
		}
	}
	return index;
}

juce::ApplicationCommandManager* CommandManager::getManager()
{
	return this->acm.get();
}

void CommandManager::release(const juce::String& caller)
{
	juce::ScopedWriteLock locker1(this->funcLock), locker2(this->flagLock);
	for (auto it = this->funcList.begin(); it != this->funcList.end();) {
		if ((*it).second.first == caller) {
			it = this->funcList.erase(it);
			continue;
		}
		it++;
	}
	for (auto it = this->flagList.begin(); it != this->flagList.end();) {
		if ((*it).second.first == caller) {
			it = this->flagList.erase(it);
			continue;
		}
		it++;
	}
}

juce::ApplicationCommandTarget* CommandManager::getNextCommandTarget()
{
	return nullptr;
}

void CommandManager::getAllCommands(juce::Array<juce::CommandID>& commands)
{
	commands.clear();
	for (int i = 0; i < this->infoList.size(); i++) {
		commands.add(i + 1);
	}
}

void CommandManager::getCommandInfo(
	juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
	if (commandID >= 1 && commandID <= this->infoList.size()) {
		juce::ScopedReadLock locker(this->flagLock);
		auto& info = this->infoList.getReference(commandID - 1);
		result.commandID = commandID;
		result.shortName = info.shortName;
		result.description = info.description;
		result.categoryName = info.categoryName;
		result.defaultKeypresses = info.defaultKeypresses;

		//查询flag
		auto it = this->flagList.find(commandID);
		result.flags = (it == this->flagList.end()) ? 0 : (*it).second.second();
	}
}

bool CommandManager::perform(const juce::ApplicationCommandTarget::InvocationInfo& info)
{
	juce::ScopedReadLock locker(this->funcLock);
	auto it = this->funcList.find(info.commandID);
	if (it == this->funcList.end()) {
		return false;
	}

	(*it).second.second();
	return true;
}

//

CommandManager::Manager::Manager(juce::ApplicationCommandTarget* target)
	:ApplicationCommandManager(),
	target(target)
{}

juce::ApplicationCommandTarget* CommandManager::Manager::getFirstCommandTarget(juce::CommandID)
{
	return this->target;
}