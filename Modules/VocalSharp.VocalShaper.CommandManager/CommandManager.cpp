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
	//File
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
		.name = "Close Other Project",
		.shortName = this->tr("sn_CloseOtherProject"),
		.description = this->tr("ds_CloseOtherProject"),
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
	this->infoList.add({
		.name = "Copy Path",
		.shortName = this->tr("sn_CopyPath"),
		.description = this->tr("ds_CopyPath"),
		.categoryName = this->tr("cn_File"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Copy Name",
		.shortName = this->tr("sn_CopyName"),
		.description = this->tr("ds_CopyName"),
		.categoryName = this->tr("cn_File"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Copy Full Url",
		.shortName = this->tr("sn_CopyFullUrl"),
		.description = this->tr("ds_CopyFullUrl"),
		.categoryName = this->tr("cn_File"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Open Path In System",
		.shortName = this->tr("sn_OpenPathInSystem"),
		.description = this->tr("ds_OpenPathInSystem"),
		.categoryName = this->tr("cn_File"),
		.defaultKeypresses = {}
		});

	//Edit
	this->infoList.add({
		.name = "Undo",
		.shortName = this->tr("sn_Undo"),
		.description = this->tr("ds_Undo"),
		.categoryName = this->tr("cn_Edit"),
		.defaultKeypresses = {juce::KeyPress('z', juce::ModifierKeys::Flags::ctrlModifier, 0)}
		});
	this->infoList.add({
		.name = "Redo",
		.shortName = this->tr("sn_Redo"),
		.description = this->tr("ds_Redo"),
		.categoryName = this->tr("cn_Edit"),
		.defaultKeypresses = {
			juce::KeyPress('z', juce::ModifierKeys::Flags::ctrlModifier | juce::ModifierKeys::Flags::shiftModifier, 0),
			juce::KeyPress('y', juce::ModifierKeys::Flags::ctrlModifier, 0)}
		});
	this->infoList.add({
		.name = "Cut",
		.shortName = this->tr("sn_Cut"),
		.description = this->tr("ds_Cut"),
		.categoryName = this->tr("cn_Edit"),
		.defaultKeypresses = {juce::KeyPress('x', juce::ModifierKeys::Flags::ctrlModifier, 0)}
		});
	this->infoList.add({
		.name = "Copy",
		.shortName = this->tr("sn_Copy"),
		.description = this->tr("ds_Copy"),
		.categoryName = this->tr("cn_Edit"),
		.defaultKeypresses = {juce::KeyPress('c', juce::ModifierKeys::Flags::ctrlModifier, 0)}
		});
	this->infoList.add({
		.name = "Paste",
		.shortName = this->tr("sn_Paste"),
		.description = this->tr("ds_Paste"),
		.categoryName = this->tr("cn_Edit"),
		.defaultKeypresses = {juce::KeyPress('v', juce::ModifierKeys::Flags::ctrlModifier, 0)}
		});
	this->infoList.add({
		.name = "Clip Board",
		.shortName = this->tr("sn_ClipBoard"),
		.description = this->tr("ds_ClipBoard"),
		.categoryName = this->tr("cn_Edit"),
		.defaultKeypresses = {juce::KeyPress('v', juce::ModifierKeys::Flags::ctrlModifier | juce::ModifierKeys::Flags::shiftModifier, 0)}
		});
	this->infoList.add({
		.name = "Clean Clip Board",
		.shortName = this->tr("sn_CleanClipBoard"),
		.description = this->tr("ds_CleanClipBoard"),
		.categoryName = this->tr("cn_Edit"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Create Copy",
		.shortName = this->tr("sn_CreateCopy"),
		.description = this->tr("ds_CreateCopy"),
		.categoryName = this->tr("cn_Edit"),
		.defaultKeypresses = {juce::KeyPress('d', juce::ModifierKeys::Flags::ctrlModifier, 0)}
		});
	this->infoList.add({
		.name = "Delete",
		.shortName = this->tr("sn_Delete"),
		.description = this->tr("ds_Delete"),
		.categoryName = this->tr("cn_Edit"),
		.defaultKeypresses = {juce::KeyPress::createFromDescription("delete")}
		});
	this->infoList.add({
		.name = "Copy To System",
		.shortName = this->tr("sn_CopyToSystem"),
		.description = this->tr("ds_CopyToSystem"),
		.categoryName = this->tr("cn_Edit"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Paste From System",
		.shortName = this->tr("sn_PasteFromSystem"),
		.description = this->tr("ds_PasteFromSystem"),
		.categoryName = this->tr("cn_Edit"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Select All",
		.shortName = this->tr("sn_SelectAll"),
		.description = this->tr("ds_SelectAll"),
		.categoryName = this->tr("cn_Edit"),
		.defaultKeypresses = {juce::KeyPress('a', juce::ModifierKeys::Flags::ctrlModifier, 0)}
		});

	//View
	this->infoList.add({
		.name = "Show Mixture Editor",
		.shortName = this->tr("sn_ShowMixtureEditor"),
		.description = this->tr("ds_ShowMixtureEditor"),
		.categoryName = this->tr("cn_View"),
		.defaultKeypresses = {}
		});

	//Transport
	this->infoList.add({
		.name = "Play",
		.shortName = this->tr("sn_Play"),
		.description = this->tr("ds_Play"),
		.categoryName = this->tr("cn_Transport"),
		.defaultKeypresses = {juce::KeyPress::createFromDescription("spacebar"),
		juce::KeyPress::createFromDescription("play")}
		});
	this->infoList.add({
		.name = "Stop",
		.shortName = this->tr("sn_Stop"),
		.description = this->tr("ds_Stop"),
		.categoryName = this->tr("cn_Transport"),
		.defaultKeypresses = {juce::KeyPress::createFromDescription("stop")}
		});
	this->infoList.add({
		.name = "Begin",
		.shortName = this->tr("sn_Begin"),
		.description = this->tr("ds_Begin"),
		.categoryName = this->tr("cn_Transport"),
		.defaultKeypresses = {juce::KeyPress::createFromDescription("home"),
		juce::KeyPress::createFromDescription("rewind")}
		});
	this->infoList.add({
		.name = "End",
		.shortName = this->tr("sn_End"),
		.description = this->tr("ds_End"),
		.categoryName = this->tr("cn_Transport"),
		.defaultKeypresses = {juce::KeyPress::createFromDescription("end"),
		juce::KeyPress::createFromDescription("fast forward")}
		});
	this->infoList.add({
		.name = "Loop",
		.shortName = this->tr("sn_Loop"),
		.description = this->tr("ds_Loop"),
		.categoryName = this->tr("cn_Transport"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Follow",
		.shortName = this->tr("sn_Follow"),
		.description = this->tr("ds_Follow"),
		.categoryName = this->tr("cn_Transport"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Back On Stop",
		.shortName = this->tr("sn_BackOnStop"),
		.description = this->tr("ds_BackOnStop"),
		.categoryName = this->tr("cn_Transport"),
		.defaultKeypresses = {}
		});

	//Adsorb
	this->infoList.add({
		.name = "Adsorb 1 Beat",
		.shortName = this->tr("sn_Adsorb1Beat"),
		.description = this->tr("ds_Adsorb1Beat"),
		.categoryName = this->tr("cn_Adsorb"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Adsorb 1/2 Beat",
		.shortName = this->tr("sn_Adsorb1/2Beat"),
		.description = this->tr("ds_Adsorb1/2Beat"),
		.categoryName = this->tr("cn_Adsorb"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Adsorb 1/4 Beat",
		.shortName = this->tr("sn_Adsorb1/4Beat"),
		.description = this->tr("ds_Adsorb1/4Beat"),
		.categoryName = this->tr("cn_Adsorb"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Adsorb 1/6 Beat",
		.shortName = this->tr("sn_Adsorb1/6Beat"),
		.description = this->tr("ds_Adsorb1/6Beat"),
		.categoryName = this->tr("cn_Adsorb"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Adsorb 1/8 Beat",
		.shortName = this->tr("sn_Adsorb1/8Beat"),
		.description = this->tr("ds_Adsorb1/8Beat"),
		.categoryName = this->tr("cn_Adsorb"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Adsorb 1/12 Beat",
		.shortName = this->tr("sn_Adsorb1/12Beat"),
		.description = this->tr("ds_Adsorb1/12Beat"),
		.categoryName = this->tr("cn_Adsorb"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Adsorb 1/16 Beat",
		.shortName = this->tr("sn_Adsorb1/16Beat"),
		.description = this->tr("ds_Adsorb1/16Beat"),
		.categoryName = this->tr("cn_Adsorb"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Adsorb 1/24 Beat",
		.shortName = this->tr("sn_Adsorb1/24Beat"),
		.description = this->tr("ds_Adsorb1/24Beat"),
		.categoryName = this->tr("cn_Adsorb"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Adsorb 1/32 Beat",
		.shortName = this->tr("sn_Adsorb1/32Beat"),
		.description = this->tr("ds_Adsorb1/32Beat"),
		.categoryName = this->tr("cn_Adsorb"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Adsorb Off",
		.shortName = this->tr("sn_AdsorbOff"),
		.description = this->tr("ds_AdsorbOff"),
		.categoryName = this->tr("cn_Adsorb"),
		.defaultKeypresses = {}
		});


	//Grid
	this->infoList.add({
		.name = "Grid 1 Beat",
		.shortName = this->tr("sn_Grid1Beat"),
		.description = this->tr("ds_Grid1Beat"),
		.categoryName = this->tr("cn_Grid"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Grid 1/2 Beat",
		.shortName = this->tr("sn_Grid1/2Beat"),
		.description = this->tr("ds_Grid1/2Beat"),
		.categoryName = this->tr("cn_Grid"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Grid 1/4 Beat",
		.shortName = this->tr("sn_Grid1/4Beat"),
		.description = this->tr("ds_Grid1/4Beat"),
		.categoryName = this->tr("cn_Grid"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Grid 1/6 Beat",
		.shortName = this->tr("sn_Grid1/6Beat"),
		.description = this->tr("ds_Grid1/6Beat"),
		.categoryName = this->tr("cn_Grid"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Grid 1/8 Beat",
		.shortName = this->tr("sn_Grid1/8Beat"),
		.description = this->tr("ds_Grid1/8Beat"),
		.categoryName = this->tr("cn_Grid"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Grid 1/12 Beat",
		.shortName = this->tr("sn_Grid1/12Beat"),
		.description = this->tr("ds_Grid1/12Beat"),
		.categoryName = this->tr("cn_Grid"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Grid 1/16 Beat",
		.shortName = this->tr("sn_Grid1/16Beat"),
		.description = this->tr("ds_Grid1/16Beat"),
		.categoryName = this->tr("cn_Grid"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Grid 1/24 Beat",
		.shortName = this->tr("sn_Grid1/24Beat"),
		.description = this->tr("ds_Grid1/24Beat"),
		.categoryName = this->tr("cn_Grid"),
		.defaultKeypresses = {}
		});
	this->infoList.add({
		.name = "Grid 1/32 Beat",
		.shortName = this->tr("sn_Grid1/32Beat"),
		.description = this->tr("ds_Grid1/32Beat"),
		.categoryName = this->tr("cn_Grid"),
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