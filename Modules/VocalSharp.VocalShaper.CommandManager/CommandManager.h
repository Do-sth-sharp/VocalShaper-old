#pragma once
#include <JuceHeader.h>

class CommandManager final 
	: public juce::ApplicationCommandTarget
{
public:
	CommandManager();
	~CommandManager() override = default;

	using CommandFunction = std::function<void(void)>;
	using FlagFunction = std::function<int(void)>;
	void addCommandFunc(const juce::String& name, const CommandFunction& func, const juce::String& caller);
	void addFlagFunc(const juce::String& name, const FlagFunction& func, const juce::String& caller);
	int getCommandID(const juce::String& name);
	juce::ApplicationCommandManager* getManager();
	void release(const juce::String& caller);

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

	class Manager : public juce::ApplicationCommandManager
	{
	public:
		explicit Manager(juce::ApplicationCommandTarget* target);
		juce::ApplicationCommandTarget* getFirstCommandTarget(juce::CommandID commandID) override;

	private:
		juce::ApplicationCommandTarget* target = nullptr;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Manager)
	};

	juce::Array<CommandInfo> infoList;
	std::map<int, std::pair<juce::String, CommandFunction>> funcList;
	std::map<int, std::pair<juce::String, FlagFunction>> flagList;
	std::unique_ptr<Manager> acm = nullptr;
	std::function<const juce::String(const juce::String&)> tr;
	juce::ReadWriteLock funcLock, flagLock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CommandManager)
};