#include "MainMenu.h"
#include <libJModule.h>

std::function<const juce::String(const juce::String&)> MainMenu::tr;

constexpr int idFile = 0x01;
constexpr int idEdit = 0x02;
constexpr int idView = 0x03;
constexpr int idModify = 0x04;
constexpr int idProject = 0x05;
constexpr int idTransport = 0x06;
constexpr int idAutomatic = 0x07;
constexpr int idConfig = 0x08;
constexpr int idMisc = 0x09;

juce::PopupMenu MainMenu::create()
{
	//获取翻译器
	if (!MainMenu::tr) {
		jmadf::CallInterface<std::function<const juce::String(const juce::String&)>&>(
			"WuChang.JMADF.Translates", "GetFunc",
			MainMenu::tr
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
	int sectionId = idFile;
	juce::PopupMenu menu;

	return menu;
}

juce::PopupMenu MainMenu::createEditMenu()
{
	int sectionId = idEdit;
	juce::PopupMenu menu;

	return menu;
}

juce::PopupMenu MainMenu::createViewMenu()
{
	int sectionId = idView;
	juce::PopupMenu menu;

	return menu;
}

juce::PopupMenu MainMenu::createModifyMenu()
{
	int sectionId = idModify;
	juce::PopupMenu menu;

	return menu;
}

juce::PopupMenu MainMenu::createProjectMenu()
{
	int sectionId = idProject;
	juce::PopupMenu menu;

	return menu;
}

juce::PopupMenu MainMenu::createTransportMenu()
{
	int sectionId = idTransport;
	juce::PopupMenu menu;

	return menu;
}

juce::PopupMenu MainMenu::createAutomaticMenu()
{
	int sectionId = idAutomatic;
	juce::PopupMenu menu;

	return menu;
}

juce::PopupMenu MainMenu::createConfigMenu()
{
	int sectionId = idConfig;
	juce::PopupMenu menu;

	return menu;
}

juce::PopupMenu MainMenu::createMiscMenu()
{
	int sectionId = idMisc;
	juce::PopupMenu menu;

	return menu;
}
