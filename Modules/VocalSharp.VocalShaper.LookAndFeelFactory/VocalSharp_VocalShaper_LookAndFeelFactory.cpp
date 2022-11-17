#include "VocalSharp_VocalShaper_LookAndFeelFactory.h"

VocalSharp_VocalShaper_LookAndFeelFactory::VocalSharp_VocalShaper_LookAndFeelFactory()
	:Module()
{
	
}

VocalSharp_VocalShaper_LookAndFeelFactory::~VocalSharp_VocalShaper_LookAndFeelFactory()
{
	
}

bool VocalSharp_VocalShaper_LookAndFeelFactory::init()
{
	if (!jmadf::LoadModule("WuChang.JMADF.LookAndFeelConfigs")) {
		return false;
	}
	this->factory = std::make_unique<LAFFactory>();

	jmadf::RegisterInterface<const std::function<int()>&, juce::LookAndFeel*&>(
		"GetStartMenuButtonLAF",
		[this](const juce::String& caller, const std::function<int()>& fontHeightFunc, juce::LookAndFeel*& laf) {
			laf = this->factory->getStartMenuButtonLAF(caller, fontHeightFunc);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetStartMenuTextEditorLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getStartMenuTextEditorLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetStartMenuListBoxLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getStartMenuListBoxLAF(caller);
		}
	);

	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetMainMenuButtonLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getMainMenuButtonLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetTabCloseButtonLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getTabCloseButtonLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetPlayButtonLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getPlayButtonLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetPlayTextButtonLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getPlayTextButtonLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetToolButtonLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getToolButtonLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&, juce::Colour>(
		"GetStretchableBarLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf, juce::Colour mainColour) {
			laf = this->factory->getStretchableBarLAF(caller, mainColour);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetStatusButtonLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getStatusButtonLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetStatusTextButtonLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getStatusTextButtonLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetStatusRoundButtonLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getStatusRoundButtonLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetStatusFlatTextButtonLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getStatusFlatTextButtonLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetLabelEditorComboBoxLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getLabelEditorComboBoxLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetLabelEditorCodeEditorLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getLabelEditorCodeEditorLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetLabelEditorResultLabelLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getLabelEditorResultLabelLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetLabelEditorButtonLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getLabelEditorButtonLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&,
		const std::function<int()>&, const std::function<float()>&,
		const juce::Colour, const juce::Colour>(
		"GetLabelEditorCallOutBoxLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf, const std::function<int()>& borderSizeFunc, const std::function<float()>& cornerSizeFunc,
			const juce::Colour colorBackground, const juce::Colour colorBorder) {
				laf = this->factory->getLabelEditorCallOutBoxLAF(caller,
					borderSizeFunc, cornerSizeFunc, colorBackground, colorBorder);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&,
		const std::function<int()>&, const std::function<float()>&>(
			"GetLabelEditorBubbleLAF",
			[this](const juce::String& caller, juce::LookAndFeel*& laf, const std::function<int()>& borderSizeFunc, const std::function<float()>& cornerSizeFunc) {
					laf = this->factory->getLabelEditorBubbleLAF(caller,
						borderSizeFunc, cornerSizeFunc);
			}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetTrackViewSMButtonLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getTrackViewSMButtonLAF(caller);
		}
	);
	jmadf::RegisterInterface<juce::LookAndFeel*&>(
		"GetTrackViewLinkButtonLAF",
		[this](const juce::String& caller, juce::LookAndFeel*& laf) {
			laf = this->factory->getTrackViewLinkButtonLAF(caller);
		}
	);

	jmadf::SetUnloadCallback(
		[this](const juce::String& caller) {
			this->factory->close(caller);
		}
	);

	return true;
}

void VocalSharp_VocalShaper_LookAndFeelFactory::destory()
{
	this->factory = nullptr;
}