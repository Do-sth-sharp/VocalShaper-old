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