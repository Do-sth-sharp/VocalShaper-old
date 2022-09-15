#pragma once
#include <JuceHeader.h>

class CommandManager final 
	: public juce::ApplicationCommandTarget
{
public:
	CommandManager();
	~CommandManager() = default;

	using CommandFunction = std::function<void(void)>;
	void addCommandFunc(const juce::String& name, CommandFunction func);
	int getCommandID(const juce::String& name);
	juce::ApplicationCommandManager* getManager();

protected:
	juce::ApplicationCommandTarget* getNextCommandTarget() override;
	void getAllCommands(juce::Array<juce::CommandID>& commands) override;
	void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
	bool perform(const juce::ApplicationCommandTarget::InvocationInfo& info) override;

private:
	struct CommandInfo {
		juce::String name;
		juce::String shortName;
		juce::String description;
		juce::String categoryName;
		juce::Array<juce::KeyPress> defaultKeypresses;
	};

	juce::Array<CommandInfo> infoList;
	std::map<int, CommandFunction> funcList;
	std::unique_ptr<juce::ApplicationCommandManager> acm = nullptr;
	std::function<const juce::String(const juce::String&)> tr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CommandManager)
};