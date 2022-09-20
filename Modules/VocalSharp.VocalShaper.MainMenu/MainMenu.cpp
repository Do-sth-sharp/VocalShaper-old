#include "MainMenu.h"
#include <libJModule.h>

std::function<const juce::String(const juce::String&)> MainMenu::tr;

juce::ApplicationCommandManager* commandManager = nullptr;

int 
commandNewProj = -1,
commandOpenProj = -1,
commandShowStartMenu = -1,
commandSaveProj = -1,
commandSaveAllProj = -1,
commandCloseProj = -1,
commandCloseAllProj = -1,
commandCloseEditor = -1
;

int
commandUndo = -1,
commandRedo = -1,
commandCut = -1,
commandCopy = -1,
commandPaste = -1,
commandCutBoard = -1,
commandCleanCutBoard = -1,
commandCreateCopy = -1,
commmandDelete = -1,
commandSelectAll = -1
;

int
commandPlay = -1,
commandStop = -1,
commandBegin = -1,
commandEnd = -1,
commandLoop = -1,
commandFollow = -1,
commandBackOnStop = -1
;

enum GroupID {
	IDFile = 0x01,
	IDEdit,
	IDView,
	IDModify,
	IDProject,
	IDTransport,
	IDAutomatic,
	IDConfig,
	IDMisc
};

enum FileID {
	IDNewProj = 0x00,
	IDOpenProj,
	IDShowStartMenu,
	IDSaveProj,
	IDSaveAllProj,
	IDCloseProj,
	IDCloseAllProj,
	IDCloseEditor
};

enum EditID {
	IDUndo = 0x00,
	IDRedo,
	IDCut,
	IDCopy,
	IDPaste,
	IDCutBoard,
	IDCleanCutBoard,
	IDCreateCopy,
	IDDelete,
	IDSelectAll
};

enum TransportID {
	IDPlay = 0x00,
	IDStop,
	IDBegin,
	IDEnd,
	IDLoop,
	IDFollow,
	IDBackOnStop
};

juce::PopupMenu MainMenu::create()
{
	//获取翻译器
	if (!MainMenu::tr) {
		jmadf::CallInterface<std::function<const juce::String(const juce::String&)>&>(
			"WuChang.JMADF.Translates", "GetFunc",
			MainMenu::tr
			);
	}

	//获取命令
	if (!::commandManager) {
		//获取管理器
		jmadf::CallInterface<juce::ApplicationCommandManager*&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandManager",
			::commandManager
			);

		//获取ID
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"New Project", ::commandNewProj
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Open Project", ::commandOpenProj
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Show Start Menu", ::commandShowStartMenu
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Save Project", ::commandSaveProj
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Save All Project", ::commandSaveAllProj
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Close Project", ::commandCloseProj
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Close All Project", ::commandCloseAllProj
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Close Editor", ::commandCloseEditor
			);

		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Undo", ::commandUndo
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Redo", ::commandRedo
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Cut", ::commandCut
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Copy", ::commandCopy
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Paste", ::commandPaste
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Cut Board", ::commandCutBoard
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Clean Cut Board", ::commandCleanCutBoard
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Create Copy", ::commandCreateCopy
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Delete", ::commmandDelete
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Select All", ::commandSelectAll
			);

		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Play", ::commandPlay
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Stop", ::commandStop
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Begin", ::commandBegin
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"End", ::commandEnd
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Loop", ::commandLoop
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Follow", ::commandFollow
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Back On Stop", ::commandBackOnStop
			);
	}

	juce::PopupMenu menu;

	menu.addSubMenu(MainMenu::tr("mb_File"), MainMenu::createFileMenu());
	menu.addSubMenu(MainMenu::tr("mb_Edit"), MainMenu::createEditMenu());
	menu.addSubMenu(MainMenu::tr("mb_View"), MainMenu::createViewMenu());
	menu.addSubMenu(MainMenu::tr("mb_Modify"), MainMenu::createModifyMenu());
	menu.addSubMenu(MainMenu::tr("mb_Project"), MainMenu::createProjectMenu());
	menu.addSubMenu(MainMenu::tr("mb_Transport"), MainMenu::createTransportMenu());
	menu.addSubMenu(MainMenu::tr("mb_Automatic"), MainMenu::createAutomaticMenu());
	menu.addSubMenu(MainMenu::tr("mb_Config"), MainMenu::createConfigMenu());
	menu.addSubMenu(MainMenu::tr("mb_Misc"), MainMenu::createMiscMenu());

	return menu;
}

void MainMenu::active(int id)
{

}

juce::PopupMenu MainMenu::createFileMenu()
{
	int sectionId = GroupID::IDFile;
	juce::PopupMenu menu;

	menu.addCommandItem(::commandManager, ::commandNewProj);
	menu.addCommandItem(::commandManager, ::commandOpenProj);
	menu.addCommandItem(::commandManager, ::commandShowStartMenu);
	menu.addSeparator();
	menu.addCommandItem(::commandManager, ::commandSaveProj);
	menu.addCommandItem(::commandManager, ::commandSaveAllProj);
	menu.addSeparator();
	menu.addCommandItem(::commandManager, ::commandCloseProj);
	menu.addCommandItem(::commandManager, ::commandCloseAllProj);
	menu.addSeparator();
	menu.addCommandItem(::commandManager, ::commandCloseEditor);

	return menu;
}

juce::PopupMenu MainMenu::createEditMenu()
{
	int sectionId = GroupID::IDEdit;
	juce::PopupMenu menu;

	menu.addCommandItem(::commandManager, ::commandUndo);
	menu.addCommandItem(::commandManager, ::commandRedo);
	menu.addSeparator();
	menu.addCommandItem(::commandManager, ::commandCut);
	menu.addCommandItem(::commandManager, ::commandCopy);
	menu.addCommandItem(::commandManager, ::commandPaste);
	menu.addCommandItem(::commandManager, ::commandCutBoard);
	menu.addCommandItem(::commandManager, ::commandCleanCutBoard);
	menu.addCommandItem(::commandManager, ::commmandDelete);
	menu.addSeparator();
	menu.addCommandItem(::commandManager, ::commandSelectAll);

	return menu;
}

juce::PopupMenu MainMenu::createViewMenu()
{
	int sectionId = GroupID::IDView;
	juce::PopupMenu menu;

	return menu;
}

juce::PopupMenu MainMenu::createModifyMenu()
{
	int sectionId = GroupID::IDModify;
	juce::PopupMenu menu;

	return menu;
}

juce::PopupMenu MainMenu::createProjectMenu()
{
	int sectionId = GroupID::IDProject;
	juce::PopupMenu menu;

	return menu;
}

juce::PopupMenu MainMenu::createTransportMenu()
{
	int sectionId = GroupID::IDTransport;
	juce::PopupMenu menu;

	menu.addCommandItem(::commandManager, ::commandPlay);
	menu.addCommandItem(::commandManager, ::commandStop);
	menu.addCommandItem(::commandManager, ::commandBegin);
	menu.addCommandItem(::commandManager, ::commandEnd);
	menu.addSeparator();
	menu.addCommandItem(::commandManager, ::commandLoop);
	menu.addCommandItem(::commandManager, ::commandFollow);
	menu.addSeparator();
	menu.addCommandItem(::commandManager, ::commandBackOnStop);

	return menu;
}

juce::PopupMenu MainMenu::createAutomaticMenu()
{
	int sectionId = GroupID::IDAutomatic;
	juce::PopupMenu menu;

	return menu;
}

juce::PopupMenu MainMenu::createConfigMenu()
{
	int sectionId = GroupID::IDConfig;
	juce::PopupMenu menu;

	return menu;
}

juce::PopupMenu MainMenu::createMiscMenu()
{
	int sectionId = GroupID::IDMisc;
	juce::PopupMenu menu;

	return menu;
}
