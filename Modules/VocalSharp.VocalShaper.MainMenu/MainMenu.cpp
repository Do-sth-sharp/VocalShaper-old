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
commandClipBoard = -1,
commandCleanClipBoard = -1,
commandCreateCopy = -1,
commandDelete = -1,
commandCopyToSystem = -1,
commandPasteFromSystem = -1,
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

int
commandAdsorb1Beat = -1,
commandAdsorb1_2Beat = -1,
commandAdsorb1_4Beat = -1,
commandAdsorb1_6Beat = -1,
commandAdsorb1_8Beat = -1,
commandAdsorb1_12Beat = -1,
commandAdsorb1_16Beat = -1,
commandAdsorb1_24Beat = -1,
commandAdsorb1_32Beat = -1,
commandAdsorbOff = -1
;

int
commandGrid1Beat = -1,
commandGrid1_2Beat = -1,
commandGrid1_4Beat = -1,
commandGrid1_6Beat = -1,
commandGrid1_8Beat = -1,
commandGrid1_12Beat = -1,
commandGrid1_16Beat = -1,
commandGrid1_24Beat = -1,
commandGrid1_32Beat = -1
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
	IDClipBoard,
	IDCleanClipBoard,
	IDCreateCopy,
	IDDelete,
	IDCopyToSystem,
	IDPasteFromSystem,
	IDSelectAll
};

enum ViewID {
	IDAdsorb = 0x00,
	IDGrid
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

enum AdsorbID {
	IDAdsorb1Beat = 0x00,
	IDAdsorb1_2Beat,
	IDAdsorb1_4Beat,
	IDAdsorb1_6Beat,
	IDAdsorb1_8Beat,
	IDAdsorb1_12Beat,
	IDAdsorb1_16Beat,
	IDAdsorb1_24Beat,
	IDAdsorb1_32Beat,
	IDAdsorbOff
};

enum GridID {
	IDGrid1Beat = 0x00,
	IDGrid1_2Beat,
	IDGrid1_4Beat,
	IDGrid1_6Beat,
	IDGrid1_8Beat,
	IDGrid1_12Beat,
	IDGrid1_16Beat,
	IDGrid1_24Beat,
	IDGrid1_32Beat
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
		//File
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

		//Edit
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
			"Clip Board", ::commandClipBoard
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Clean Clip Board", ::commandCleanClipBoard
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Create Copy", ::commandCreateCopy
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Delete", ::commandDelete
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Copy To System", ::commandCopyToSystem
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Paste From System", ::commandPasteFromSystem
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Select All", ::commandSelectAll
			);

		//Transport
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

		//Adsorb
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Adsorb 1 Beat", ::commandAdsorb1Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Adsorb 1/2 Beat", ::commandAdsorb1_2Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Adsorb 1/4 Beat", ::commandAdsorb1_4Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Adsorb 1/6 Beat", ::commandAdsorb1_6Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Adsorb 1/8 Beat", ::commandAdsorb1_8Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Adsorb 1/12 Beat", ::commandAdsorb1_12Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Adsorb 1/16 Beat", ::commandAdsorb1_16Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Adsorb 1/24 Beat", ::commandAdsorb1_24Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Adsorb 1/32 Beat", ::commandAdsorb1_32Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Adsorb Off", ::commandAdsorbOff
			);

		//Grid
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Grid 1 Beat", ::commandGrid1Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Grid 1/2 Beat", ::commandGrid1_2Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Grid 1/4 Beat", ::commandGrid1_4Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Grid 1/6 Beat", ::commandGrid1_6Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Grid 1/8 Beat", ::commandGrid1_8Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Grid 1/12 Beat", ::commandGrid1_12Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Grid 1/16 Beat", ::commandGrid1_16Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Grid 1/24 Beat", ::commandGrid1_24Beat
			);
		jmadf::CallInterface<const juce::String&, int&>(
			"VocalSharp.VocalShaper.CommandManager", "GetCommandID",
			"Grid 1/32 Beat", ::commandGrid1_32Beat
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
	menu.addCommandItem(::commandManager, ::commandClipBoard);
	menu.addCommandItem(::commandManager, ::commandCleanClipBoard);
	menu.addCommandItem(::commandManager, ::commandDelete);
	menu.addSeparator();
	menu.addCommandItem(::commandManager, ::commandCopyToSystem);
	menu.addCommandItem(::commandManager, ::commandPasteFromSystem);
	menu.addSeparator();
	menu.addCommandItem(::commandManager, ::commandSelectAll);

	return menu;
}

juce::PopupMenu MainMenu::createViewMenu()
{
	int sectionId = GroupID::IDView;
	juce::PopupMenu menu;

	menu.addSubMenu(MainMenu::tr("mb_Adsorb"), MainMenu::createAdsorbMenu());
	menu.addSubMenu(MainMenu::tr("mb_Grid"), MainMenu::createGridMenu());

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

juce::PopupMenu MainMenu::createAdsorbMenu()
{
	int sectionId = ViewID::IDAdsorb << 8; 
	juce::PopupMenu menu;

	menu.addCommandItem(::commandManager, ::commandAdsorb1Beat);
	menu.addCommandItem(::commandManager, ::commandAdsorb1_2Beat);
	menu.addCommandItem(::commandManager, ::commandAdsorb1_4Beat);
	menu.addCommandItem(::commandManager, ::commandAdsorb1_6Beat);
	menu.addCommandItem(::commandManager, ::commandAdsorb1_8Beat);
	menu.addCommandItem(::commandManager, ::commandAdsorb1_12Beat);
	menu.addCommandItem(::commandManager, ::commandAdsorb1_16Beat);
	menu.addCommandItem(::commandManager, ::commandAdsorb1_24Beat);
	menu.addCommandItem(::commandManager, ::commandAdsorb1_32Beat);
	menu.addCommandItem(::commandManager, ::commandAdsorbOff);

	return menu;
}

juce::PopupMenu MainMenu::createGridMenu()
{
	int sectionId = ViewID::IDGrid << 8;
	juce::PopupMenu menu;

	menu.addCommandItem(::commandManager, ::commandGrid1Beat);
	menu.addCommandItem(::commandManager, ::commandGrid1_2Beat);
	menu.addCommandItem(::commandManager, ::commandGrid1_4Beat);
	menu.addCommandItem(::commandManager, ::commandGrid1_6Beat);
	menu.addCommandItem(::commandManager, ::commandGrid1_8Beat);
	menu.addCommandItem(::commandManager, ::commandGrid1_12Beat);
	menu.addCommandItem(::commandManager, ::commandGrid1_16Beat);
	menu.addCommandItem(::commandManager, ::commandGrid1_24Beat);
	menu.addCommandItem(::commandManager, ::commandGrid1_32Beat);

	return menu;
}
