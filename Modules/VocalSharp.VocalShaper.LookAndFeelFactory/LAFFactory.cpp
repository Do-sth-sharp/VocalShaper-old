#include "LAFFactory.h"
#include "StartMenuButtonLAF.h"

juce::LookAndFeel* LAFFactory::getStartMenuButtonLAF(int fontHeight)
{
	if (!this->lafs.startMenuButton) {
		this->lafs.startMenuButton =
			std::make_unique<StartMenuButtonLAF>(fontHeight);
	}
	return this->lafs.startMenuButton.get();
}

juce::LookAndFeel* LAFFactory::getStartMenuTextEditorLAF()
{
	if (!this->lafs.startMenuTextEditor) {
		this->lafs.startMenuTextEditor =
			std::make_unique<juce::LookAndFeel_V4>();
	}
	return this->lafs.startMenuTextEditor.get();
}
