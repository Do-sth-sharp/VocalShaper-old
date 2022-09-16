#include "MainMenu.h"
#include <libJModule.h>

std::function<const juce::String(const juce::String&)> MainMenu::tr;

juce::ApplicationCommandManager* commandManager = nullptr;

int 
commandNewProj = -1,
commandOpenProj = -1,
commandShowStartMenu = -1,
commandCloseProj = -1,
commandCloseAllProj = -1,
commandCloseEditor = -1
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
	IDCloseProj,
	IDCloseAllProj,
	IDCloseEditor
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
