#include "EditorComponent.h"
#include <libJModule.h>

EditorComponent::EditorComponent()
	: EditorBase()
{
	//获取屏幕属性接口
	this->screenSizeFunc =
		jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSize"
			);

	//获取命令管理器
	jmadf::CallInterface<juce::ApplicationCommandManager*&>(
		"VocalSharp.VocalShaper.CommandManager", "GetCommandManager",
		this->commandManager
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
	this->addChildEditor(this->topEditor.get());
	this->bottomEditor = std::make_unique<BottomEditor>();
	this->addChildEditor(this->bottomEditor.get());

	//注册编辑器方法
	{
		auto setCurrentTrackFunc = [this](int trackID) {this->wannaSetCurrentTrack(trackID); };
		auto refreshTotalLengthFunc = [this] {this->wannaRefreshTotalLength(); };
		auto setCurrentPositionFunc = [this](double currentTime) {this->wannaSetCurrentPosition(currentTime); };
		auto setLoopRangeFunc = [this](double startTime, double endTime) {this->wannaSetLoopRange(startTime, endTime); };
		auto setHorizontalViewPortFunc = [this](double startTime, double endTime) {this->wannaSetHorizontalViewPort(startTime, endTime); };
		auto setVerticalViewPortFunc = [this](double bottomPitch, double topPitch) {this->wannaSetVerticalViewPort(bottomPitch, topPitch); };
		auto setHViewPortFunc = [this](double startTime, double endTime) {this->wannaChangeHViewPort(startTime, endTime); };
		auto setVViewPortFunc = [this](double bottomPer, double topPer) {this->wannaChangeVViewPort(bottomPer, topPer); };

		this->setMethods(
			setCurrentTrackFunc, refreshTotalLengthFunc, setCurrentPositionFunc,
			setLoopRangeFunc, setHorizontalViewPortFunc, setVerticalViewPortFunc);
		this->setTrackViewMethods(setHViewPortFunc, setVViewPortFunc);
	}

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
			this->setProject(project);
		}
	);

	//添加监听器
	this->initProjectListener();

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
	if (this->bottomEditor) {
		return this->bottomEditor->isVisible();
	}
	return false;
}

void EditorComponent::setProjectCallback(const vocalshaper::ProjectProxy* ptr)
{
	this->setTotalLength(this->countProjectTime(this->getProject()));
}

void EditorComponent::undo()
{
	juce::ScopedReadLock locker(this->getProjLock());
	return this->getProject()->getProcesser()->undo();
}

void EditorComponent::redo()
{
	juce::ScopedReadLock locker(this->getProjLock());
	return this->getProject()->getProcesser()->redo();
}

void EditorComponent::cut()
{
	jmadf::CallInterface<void>(
		"VocalSharp.VocalShaper.ClipBoard", "SendCopy");
	jmadf::CallInterface<void>(
		"VocalSharp.VocalShaper.ClipBoard", "SendDelete");
}

void EditorComponent::copy()
{
	jmadf::CallInterface<void>(
		"VocalSharp.VocalShaper.ClipBoard", "SendCopy");
}

void EditorComponent::paste()
{
	jmadf::CallInterface<void>(
		"VocalSharp.VocalShaper.ClipBoard", "SendPaste");
}

void EditorComponent::clipBoard()
{
	juce::StringArray clipList;
	jmadf::CallInterface<juce::StringArray&>(
		"VocalSharp.VocalShaper.ClipBoard", "GetList",
		clipList);

	int result = -1;
	//TODO 显示剪贴板

	if (result > -1) {
		jmadf::CallInterface<int>(
			"VocalSharp.VocalShaper.ClipBoard", "SendPasteWithIndex", result);
	}
}

void EditorComponent::cleanClipBoard()
{
	jmadf::CallInterface<void>(
		"VocalSharp.VocalShaper.ClipBoard", "Clean");
}

void EditorComponent::createCopy()
{
	jmadf::CallInterface<void>(
		"VocalSharp.VocalShaper.ClipBoard", "SendCopy");
	jmadf::CallInterface<void>(
		"VocalSharp.VocalShaper.ClipBoard", "SendPaste");
}

void EditorComponent::delete_()
{
	jmadf::CallInterface<void>(
		"VocalSharp.VocalShaper.ClipBoard", "SendDelete");
}

void EditorComponent::copyToSystem()
{
	jmadf::CallInterface<void>(
		"VocalSharp.VocalShaper.ClipBoard", "SendCopyToSystem");
}

void EditorComponent::pasteFromSystem()
{
	jmadf::CallInterface<void>(
		"VocalSharp.VocalShaper.ClipBoard", "SendPasteFromSystem");
}

void EditorComponent::selectAll()
{
	jmadf::CallInterface<void>(
		"VocalSharp.VocalShaper.ClipBoard", "SendSelectAll");
}

bool EditorComponent::couldUndo()
{
	juce::ScopedReadLock locker(this->getProjLock());
	if (this->getProject()) {
		auto ptrProcesser = this->getProject()->getProcesser();
		if (ptrProcesser) {
			return ptrProcesser->couldUndo();
		}
	}
	return false;
}

bool EditorComponent::couldRedo()
{
	juce::ScopedReadLock locker(this->getProjLock());
	if (this->getProject()) {
		auto ptrProcesser = this->getProject()->getProcesser();
		if (ptrProcesser) {
			return ptrProcesser->couldRedo();
		}
	}
	return false;
}

bool EditorComponent::couldCut()
{
	bool result = false;
	jmadf::CallInterface<bool&>(
		"VocalSharp.VocalShaper.ClipBoard", "CouldCopyAndDelete", result);
	return result;
}

bool EditorComponent::couldCopy()
{
	bool result = false;
	jmadf::CallInterface<bool&>(
		"VocalSharp.VocalShaper.ClipBoard", "CouldCopyAndDelete", result);
	return result;
}

bool EditorComponent::couldPaste()
{
	bool result = false;
	jmadf::CallInterface<bool&>(
		"VocalSharp.VocalShaper.ClipBoard", "CouldPaste", result);
	return result;
}

bool EditorComponent::couldCleanClipBoard()
{
	int size = 0;
	jmadf::CallInterface<int&>(
		"VocalSharp.VocalShaper.ClipBoard", "GetSize",
		size
		);
	return size > 0;
}

bool EditorComponent::couldCreateCopy()
{
	bool result1 = false, result2 = false;
	jmadf::CallInterface<bool&>(
		"VocalSharp.VocalShaper.ClipBoard", "CouldCopyAndDelete", result1);
	jmadf::CallInterface<bool&>(
		"VocalSharp.VocalShaper.ClipBoard", "CouldPaste", result2);
	return result1 && result2;
}

bool EditorComponent::couldDelete()
{
	bool result = false;
	jmadf::CallInterface<bool&>(
		"VocalSharp.VocalShaper.ClipBoard", "CouldCopyAndDelete", result);
	return result;
}

bool EditorComponent::couldCopyToSystem()
{
	bool result = false;
	jmadf::CallInterface<bool&>(
		"VocalSharp.VocalShaper.ClipBoard", "CouldCopyToSystem", result);
	return result;
}

bool EditorComponent::couldPasteFromSystem()
{
	bool result = false;
	jmadf::CallInterface<bool&>(
		"VocalSharp.VocalShaper.ClipBoard", "CouldPasteFromSystem", result);
	return result;
}

bool EditorComponent::couldSelectAll()
{
	bool result = false;
	jmadf::CallInterface<bool&>(
		"VocalSharp.VocalShaper.ClipBoard", "CouldCopyAndDelete", result);
	return result;
}

void EditorComponent::lastTrack()
{
	juce::ScopedReadLock locker1(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock locker2(this->getProject()->getLock());
		int trackSize = ::vocalshaper::ProjectDAO::trackSize(
			this->getProject()->getPtr());
		if (this->getTrackID() > 0 && this->getTrackID() < trackSize) {
			this->setTrackID(this->getTrackID() - 1);
		}
	}
}

void EditorComponent::nextTrack()
{
	juce::ScopedReadLock locker1(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock locker2(this->getProject()->getLock());
		int trackSize = ::vocalshaper::ProjectDAO::trackSize(
			this->getProject()->getPtr());
		if (this->getTrackID() >= 0 && this->getTrackID() < trackSize - 1) {
			this->setTrackID(this->getTrackID() + 1);
		}
	}
}

void EditorComponent::switchTrack()
{
	int result = this->bottomEditor->switchTrack();
	juce::ScopedReadLock locker1(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock locker2(this->getProject()->getLock());
		int trackSize = ::vocalshaper::ProjectDAO::trackSize(
			this->getProject()->getPtr());
		if (result > 0 && this->getTrackID() <= trackSize) {
			this->setTrackID(result - 1);
		}
		else {
			this->setTrackID(-1);
		}
	}
}

bool EditorComponent::couldLastTrack()
{
	juce::ScopedReadLock locker1(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock locker2(this->getProject()->getLock());
		int trackSize = ::vocalshaper::ProjectDAO::trackSize(this->getProject()->getPtr());
		if (this->getTrackID() > 0 && this->getTrackID() < trackSize) {
			return true;
		}
	}
	return false;
}

bool EditorComponent::couldNextTrack()
{
	juce::ScopedReadLock locker1(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock locker2(this->getProject()->getLock());
		int trackSize = ::vocalshaper::ProjectDAO::trackSize(this->getProject()->getPtr());
		if (this->getTrackID() >= 0 && this->getTrackID() < trackSize - 1) {
			return true;
		}
	}
	return false;
}

bool EditorComponent::couldSwitchTrack()
{
	if (!this->bottomEditor) {
		return false;
	}
	if (!this->bottomEditor->isVisible()) {
		return false;
	}
	juce::ScopedReadLock locker1(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock locker2(this->getProject()->getLock());
		int trackSize = ::vocalshaper::ProjectDAO::trackSize(
			this->getProject()->getPtr());
		if (trackSize > 0) {
			return true;
		}
	}
	return false;
}

void EditorComponent::wannaSetCurrentTrack(int trackID)
{
	juce::ScopedReadLock locker1(this->getProjLock());
	if (this->getProject()) {
		juce::ScopedReadLock locker2(this->getProject()->getLock());
		int trackSize = ::vocalshaper::ProjectDAO::trackSize(this->getProject()->getPtr());
		if (trackID >= -1 && trackID < trackSize) {
			this->setTrackID(trackID);
		}
	}
}

void EditorComponent::wannaRefreshTotalLength()
{
	juce::ScopedReadLock locker(this->getProjLock());
	this->setTotalLength(this->countProjectTime(this->getProject()));
}

void EditorComponent::wannaSetCurrentPosition(double currentTime)
{
	//TODO 同步至播放控制器
	//begin test
	this->setCurrentPosition(currentTime);
	//end test
}

void EditorComponent::wannaSetLoopRange(double startTime, double endTime)
{
	//TODO 同步至播放控制器
	//begin test
	this->setLoopRange(startTime, endTime);
	//end test
}

void EditorComponent::wannaSetHorizontalViewPort(double startTime, double endTime)
{
	juce::ScopedReadLock locker1(this->getProjLock());
	juce::ScopedReadLock locker2(this->getProject()->getLock());
	auto project = this->getProject()->getPtr();
	if (!project) {
		return;
	}
	if (startTime < endTime) {
		this->setHorizontalViewPort(startTime, endTime);
	}
}

void EditorComponent::wannaSetVerticalViewPort(double bottomPitch, double topPitch)
{
	if (bottomPitch >= 0 && bottomPitch <= 127 && topPitch >= 0 && topPitch <= 127) {
		if (bottomPitch < topPitch - 1) {
			//至少要有一个音符的高度
			this->setVerticalViewPort(bottomPitch, topPitch);
		}
	}
}

void EditorComponent::wannaChangeHViewPort(double startTime, double endTime)
{
	/*if (this->startTimeTemp == startTime && this->endTimeTemp == endTime) {
		return;
	}*/
	juce::ScopedReadLock locker1(this->getProjLock());
	juce::ScopedReadLock locker2(this->getProject()->getLock());
	auto project = this->getProject()->getPtr();
	if (!project) {
		return;
	}
	if (startTime < endTime) {
		this->setHViewPort(startTime, endTime);
	}
}

void EditorComponent::wannaChangeVViewPort(double bottomTrack, double topTrack)
{
	/*if (this->bottomVTrackTemp == bottomTrack && this->topVTrackTemp == topTrack) {
		return;
	}*/
	if (bottomTrack > topTrack) {
		this->setVViewPort(bottomTrack, topTrack);
	}
}

void EditorComponent::listenTrackSizeChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
{
	juce::ScopedReadLock locker(this->getProjLock());
	if (this->getProject() != action.getProxy()) {
		return;
	}
	auto messageManager = juce::MessageManager::getInstance();
	if (!messageManager) {
		return;
	}
	if (action.getBaseType() == vocalshaper::actions::ActionBase::Type::Project) {
		if (action.getActionType() == vocalshaper::actions::ProjectAction::Actions::AddTrack) {
			auto data = reinterpret_cast<const vocalshaper::actions::project::AddTrackAction::DataType*>(action.getData());
			if (type == vocalshaper::actions::ActionBase::UndoType::Perform) {
				int index = data->index;
				messageManager->callAsync([this, index] {this->wannaSetCurrentTrack(index); });
			}
			else if (type == vocalshaper::actions::ActionBase::UndoType::Undo) {
				if (data->index == this->getTrackID()) {
					juce::ScopedReadLock projLock(action.getProxy()->getLock());
					int size = vocalshaper::ProjectDAO::trackSize(action.getProxy()->getPtr());
					int nextID = data->index - 1;
					if (size > 0 && nextID < 0) {
						nextID = 0;
					}
					int index = nextID;
					messageManager->callAsync([this, index] {this->wannaSetCurrentTrack(index); });
				}
				else {
					int index = this->getTrackID();
					messageManager->callAsync([this, index] {this->wannaSetCurrentTrack(index); });
				}
			}
		}
		else if (action.getActionType() == vocalshaper::actions::ProjectAction::Actions::RemoveTrack) {
			auto data = reinterpret_cast<const vocalshaper::actions::project::RemoveTrackAction::DataType*>(action.getData());
			if (type == vocalshaper::actions::ActionBase::UndoType::Perform) {
				if (data->index == this->getTrackID()) {
					juce::ScopedReadLock projLock(action.getProxy()->getLock());
					int size = vocalshaper::ProjectDAO::trackSize(action.getProxy()->getPtr());
					int nextID = data->index - 1;
					if (size > 0 && nextID < 0) {
						nextID = 0;
					}
					int index = nextID;
					messageManager->callAsync([this, index] {this->wannaSetCurrentTrack(index); });
				}
				else {
					int index = this->getTrackID();
					messageManager->callAsync([this, index] {this->wannaSetCurrentTrack(index); });
				}
			}
			else if (type == vocalshaper::actions::ActionBase::UndoType::Undo) {
				int index = data->index;
				messageManager->callAsync([this, index] {this->wannaSetCurrentTrack(index); });
			}
		}
	}
}

void EditorComponent::listenProjectLengthChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
{
	juce::ScopedReadLock locker(this->getProjLock());
	if (this->getProject() != action.getProxy()) {
		return;
	}
	auto messageManager = juce::MessageManager::getInstance();
	if (!messageManager) {
		return;
	}
	{
		juce::ScopedReadLock projLocker(this->getProject()->getLock());
		double totalLength = EditorComponent::countProjectTime(this->getProject());
		double bar =
			this->getProject()->getBeat()->getBarAtTime(std::floor(totalLength));
		bar = std::max(std::floor(bar) + 4, 20.);
		double totalTime =
			this->getProject()->getBeat()->getTimeAtBar(bar);
		if (totalTime != this->totalTimeTemp || totalLength != this->totalLengthTemp) {
			this->totalTimeTemp = totalTime;
			this->totalLengthTemp = totalLength;
			messageManager->callAsync([this] {this->wannaRefreshTotalLength(); });
		}
	}
}

void EditorComponent::listenAllChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
{
	juce::ScopedReadLock locker(this->getProjLock());
	if (this->getProject() != action.getProxy()) {
		return;
	}
	auto messageManager = juce::MessageManager::getInstance();
	if (!messageManager) {
		return;
	}
	if (this->commandManager) {
		auto ptrCM = this->commandManager;
		messageManager->callAsync([ptrCM] {ptrCM->commandStatusChanged(); });
	}
}

void EditorComponent::listenCurrentPositionChange(double currentTime)
{
	auto messageManager = juce::MessageManager::getInstance();
	if (!messageManager) {
		return;
	}
	messageManager->callAsync([this, currentTime] {this->setCurrentPosition(currentTime); });
}

void EditorComponent::listenFollowStateChange(bool followState)
{
	auto messageManager = juce::MessageManager::getInstance();
	if (!messageManager) {
		return;
	}
	messageManager->callAsync([this, followState] {this->setFollowState(followState); });
}

void EditorComponent::listenLoopRangeChange(double startTime, double endTime)
{
	auto messageManager = juce::MessageManager::getInstance();
	if (!messageManager) {
		return;
	}
	messageManager->callAsync([this, startTime, endTime] {this->setLoopRange(startTime, endTime); });
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

double EditorComponent::countProjectTime(vocalshaper::ProjectProxy* ptr)
{
	if (!ptr) {
		return 0.;
	}

	juce::ScopedReadLock locker(ptr->getLock());
	auto project = ptr->getPtr();
	if (!project) {
		return 0.;
	}

	return vocalshaper::CountTime::count(project);
}

double EditorComponent::countTrackTime(vocalshaper::ProjectProxy* ptr, int trackID)
{
	if (!ptr) {
		return 0.;
	}

	juce::ScopedReadLock locker(ptr->getLock());
	auto project = ptr->getPtr();
	if (!project) {
		return 0.;
	}

	if (trackID < 0 || trackID >= vocalshaper::ProjectDAO::trackSize(project)) {
		return 0.;
	}
	auto track = vocalshaper::ProjectDAO::getTrack(project, trackID);
	if (!track) {
		return 0.;
	}

	return vocalshaper::CountTime::count(track);
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

	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"View Mode", [this] {this->setEditMode(false); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Edit Mode", [this] {this->setEditMode(true); }
	);

	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Tool 1", [this] {this->setToolID(1); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Tool 2", [this] {this->setToolID(2); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Tool 3", [this] {this->setToolID(3); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Tool 4", [this] {this->setToolID(4); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Tool 5", [this] {this->setToolID(5); }
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Last Track", [this] {
			this->lastTrack();
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Next Track", [this] {
			this->nextTrack();
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<void(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFunction",
		"Switch Track", [this] {
			this->switchTrack();
		}
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

	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"View Mode", [this]()->int {
			int flag = 0;
			if (!this->getEditMode()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Edit Mode", [this]()->int {
			int flag = 0;
			if (this->getEditMode()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);

	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Tool 1", [this]()->int {
			int flag = 0;
			if (!this->getEditMode()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			if (this->getToolID() == 1) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Tool 2", [this]()->int {
			int flag = 0;
			if (!this->getEditMode()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			if (this->getToolID() == 2) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Tool 3", [this]()->int {
			int flag = 0;
			if (!this->getEditMode()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			if (this->getToolID() == 3) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Tool 4", [this]()->int {
			int flag = 0;
			if (!this->getEditMode()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			if (this->getToolID() == 4) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Tool 5", [this]()->int {
			int flag = 0;
			if (!this->getEditMode()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			if (this->getToolID() == 5) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Last Track", [this]()->int {
			int flag = 0;
			if (!this->couldLastTrack ()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Next Track", [this]()->int {
			int flag = 0;
			if (!this->couldNextTrack()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Switch Track", [this]()->int {
			int flag = 0;
			if (!this->couldSwitchTrack()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isDisabled;
			}
			return flag;
		}
	);
}

void EditorComponent::initProjectListener()
{
	//侦听轨道数量变化
	jmadf::CallInterface<const vocalshaper::actions::ActionBase::RuleFunc&>(
		"VocalSharp.VocalShaper.CallbackReactor", "AddActionRules",
		[this](const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
		{this->listenTrackSizeChange(action, type); });

	//侦听工程长度变化
	jmadf::CallInterface<const vocalshaper::actions::ActionBase::RuleFunc&>(
		"VocalSharp.VocalShaper.CallbackReactor", "AddActionRules",
		[this](const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
		{this->listenProjectLengthChange(action, type); });

	//侦听全部变化
	jmadf::CallInterface<const vocalshaper::actions::ActionBase::RuleFunc&>(
		"VocalSharp.VocalShaper.CallbackReactor", "AddActionRules",
		[this](const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
		{this->listenAllChange(action, type); });

	//TODO 侦听播放状态变化
}
