#include "EditorComponent.h"
#include <libJModule.h>

EditorComponent::EditorComponent()
	: Component("Editor")
{
	//获取屏幕属性接口
	this->screenSizeFunc =
		jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSize"
			);

	//以下获取命令ID
	this->initCommandID();

	//以下注册命令回调
	this->initCommandFunction();

	//以下注册Flag获取函数
	this->initCommandFlagHook();

	//以下获取界面属性
	bool result = false;
	//color
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background", this->colors.background, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "stretchableBar", this->colors.stretchableBar, result
		);

	//size
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "size", "height-stretchableBar", this->sizes.height_stretchableBar, result
		);

	//position
	//scale
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-top-max", this->scales.height_top_max, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-top-min", this->scales.height_top_min, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-top-prefer", this->scales.height_top_prefer, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-bottom-max", this->scales.height_bottom_max, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-bottom-min", this->scales.height_bottom_min, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "scale", "height-bottom-prefer", this->scales.height_bottom_prefer, result
		);

	//resource

	//创建编辑器
	this->topEditor = std::make_unique<TopEditor>();
	this->addChildComponent(this->topEditor.get());
	this->bottomEditor = std::make_unique<BottomEditor>();
	this->addChildComponent(this->bottomEditor.get());

	//创建分割器
	this->slManager = std::make_unique<juce::StretchableLayoutManager>();

	//建立拖动条样式
	jmadf::CallInterface<juce::LookAndFeel*&, juce::Colour>(
		"VocalSharp.VocalShaper.LookAndFeelFactory", "GetStretchableBarLAF",
		this->lafs.stretchableBar, this->colors.stretchableBar
		);

	//创建拖动条
	this->slBar = std::make_unique<juce::StretchableLayoutResizerBar>(
		this->slManager.get(),
		1, false);
	this->slBar->setLookAndFeel(this->lafs.stretchableBar);
	this->addChildComponent(this->slBar.get());

	//添加项目回调
	jmadf::CallInterface<const std::function<void(const vocalshaper::ProjectProxy*)>&>(
		"VocalSharp.VocalShaper.ProjectHub", "AddNotice",
		[this](const vocalshaper::ProjectProxy* project) {
			this->projectChanged(project);
		}
	);

	//显示模式
	this->setTrackOpen(false);
}

EditorComponent::~EditorComponent()
{

}

void EditorComponent::setTrackOpen(bool trackOpen)
{
	this->topEditor->setVisible(true);
	this->slBar->setVisible(trackOpen);
	this->bottomEditor->setVisible(trackOpen);

	this->trackOpenStateChanged = true;
	this->resized();
}

bool EditorComponent::isTrackOpen()
{
	return this->bottomEditor->isVisible();
}

void EditorComponent::projectChanged(const vocalshaper::ProjectProxy* ptr)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->project = const_cast<vocalshaper::ProjectProxy*>(ptr);
	this->topEditor->projectChanged(ptr);
	this->bottomEditor->projectChanged(ptr);
}

void EditorComponent::undo()
{
	juce::ScopedReadLock locker(this->projectLock);
	return this->project->getProcesser()->undo();
}

void EditorComponent::redo()
{
	juce::ScopedReadLock locker(this->projectLock);
	return this->project->getProcesser()->redo();
}

void EditorComponent::cut()
{

}

void EditorComponent::copy()
{

}

void EditorComponent::paste()
{

}

void EditorComponent::clipBoard()
{

}

void EditorComponent::cleanClipBoard()
{

}

void EditorComponent::createCopy()
{

}

void EditorComponent::delete_()
{

}

void EditorComponent::copyToSystem()
{

}

void EditorComponent::pasteFromSystem()
{

}

void EditorComponent::selectAll()
{

}

bool EditorComponent::couldUndo()
{
	juce::ScopedReadLock locker(this->projectLock);
	if (this->project) {
		auto ptrProcesser = this->project->getProcesser();
		if (ptrProcesser) {
			return ptrProcesser->couldUndo();
		}
	}
	return false;
}

bool EditorComponent::couldRedo()
{
	juce::ScopedReadLock locker(this->projectLock);
	if (this->project) {
		auto ptrProcesser = this->project->getProcesser();
		if (ptrProcesser) {
			return ptrProcesser->couldRedo();
		}
	}
	return false;
}

bool EditorComponent::couldCut()
{
	return false;
}

bool EditorComponent::couldCopy()
{
	return false;
}

bool EditorComponent::couldPaste()
{
	return false;
}

bool EditorComponent::couldCleanClipBoard()
{
	return false;
}

bool EditorComponent::couldCreateCopy()
{
	return false;
}

bool EditorComponent::couldDelete()
{
	return false;
}

bool EditorComponent::couldCopyToSystem()
{
	return false;
}

bool EditorComponent::couldPasteFromSystem()
{
	juce::String str = juce::SystemClipboard::getTextFromClipboard();
	juce::var var;
	return juce::JSON::parse(str, var).wasOk();
}

bool EditorComponent::couldSelectAll()
{
	return false;
}

void EditorComponent::resized()
{
	//获取屏幕相关属性
	bool sizeChanged = false;
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);
	if (this->screenSize != screenSize) {
		this->screenSize = screenSize;
		sizeChanged = true;
	}

	//计算控件大小
	int height_slBar = this->sizes.height_stretchableBar * screenSize.getHeight();

	//调整控件大小
	if (this->bottomEditor->isVisible()) {
		//音符窗开
		juce::Component* components[] = {
			this->topEditor.get(),
			this->slBar.get(),
			this->bottomEditor.get()
		};
		if (sizeChanged || this->trackOpenStateChanged) {
			this->slManager->setItemLayout(0,
				-this->scales.height_top_min,
				-this->scales.height_top_max,
				-this->scales.height_top_prefer);
			this->slManager->setItemLayout(1,
				height_slBar, height_slBar, height_slBar);
			this->slManager->setItemLayout(2,
				-this->scales.height_bottom_min,
				-this->scales.height_bottom_max,
				-this->scales.height_bottom_prefer);

			this->trackOpenStateChanged = false;
		}
		this->slManager->layOutComponents(
			components, 3,
			0, 0, this->getWidth(), this->getHeight(),
			true, true
		);
	}
	else {
		//音符窗关
		juce::Component* components[] = {
			this->topEditor.get()
		};
		if (sizeChanged || this->trackOpenStateChanged) {
			this->slManager->setItemLayout(0, -1.0, -1.0, -1.0);

			this->trackOpenStateChanged = false;
		}
		this->slManager->layOutComponents(
			components, 1,
			0, 0, this->getWidth(), this->getHeight(),
			true, true
		);
	}
	
}

void EditorComponent::paint(juce::Graphics& g)
{

}

void EditorComponent::initCommandID()
{

}

void EditorComponent::initCommandFunction()
{
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Show Mixture Editor", [this] {
			this->setTrackOpen(!this->isTrackOpen());
		}
	);

	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Undo", [this] {this->undo(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Redo", [this] {this->redo(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Cut", [this] {this->cut(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Copy", [this] {this->copy(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Paste", [this] {this->paste(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Clip Board", [this] {this->clipBoard(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Clean Clip Board", [this] {this->cleanClipBoard(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Create Copy", [this] {this->createCopy(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Delete", [this] {this->delete_(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Copy To System", [this] {this->copyToSystem(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Paste From System", [this] {this->pasteFromSystem(); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Select All", [this] {this->selectAll(); }
	);
}

void EditorComponent::initCommandFlagHook()
{
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Show Mixture Editor", [this]()->int {
			int flag = 0;
			if (this->isTrackOpen()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);

	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Undo", [this]()->int {
			int flag = 0;
			if (!this->couldUndo()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Redo", [this]()->int {
			int flag = 0;
			if (!this->couldRedo()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Cut", [this]()->int {
			int flag = 0;
			if (!this->couldCut()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Copy", [this]()->int {
			int flag = 0;
			if (!this->couldCopy()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Paste", [this]()->int {
			int flag = 0;
			if (!this->couldPaste()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Clean Clip Board", [this]()->int {
			int flag = 0;
			if (!this->couldCleanClipBoard()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Create Copy", [this]()->int {
			int flag = 0;
			if (!this->couldCreateCopy()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Delete", [this]()->int {
			int flag = 0;
			if (!this->couldDelete()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Copy To System", [this]()->int {
			int flag = 0;
			if (!this->couldCopyToSystem()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Paste From System", [this]()->int {
			int flag = 0;
			if (!this->couldPasteFromSystem()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Select All", [this]()->int {
			int flag = 0;
			if (!this->couldSelectAll()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
}
