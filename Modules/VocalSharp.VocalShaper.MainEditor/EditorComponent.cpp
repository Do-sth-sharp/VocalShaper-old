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

	//注册编辑器方法
	{
		auto setCurrentTrackFunc = [this](int trackID) {this->setCurrentTrack(trackID); };
		auto refreshTotalLengthFunc = [this] {this->refreshTotalLength(); };
		auto setCurrentPositionFunc = [this](double currentTime) {this->setCurrentPosition(currentTime); };
		auto setLoopRangeFunc = [this](double startTime, double endTime) {this->setLoopRange(startTime, endTime); };
		auto setHorizontalViewPortFunc = [this](double startTime, double endTime) {this->setHorizontalViewPort(startTime, endTime); };
		auto setVerticalViewPortFunc = [this](double bottomPitch, double topPitch) {this->setVerticalViewPort(bottomPitch, topPitch); };

		this->topEditor->setMethods(
			setCurrentTrackFunc, refreshTotalLengthFunc, setCurrentPositionFunc,
			setLoopRangeFunc, setHorizontalViewPortFunc, setVerticalViewPortFunc);
		this->bottomEditor->setMethods(
			setCurrentTrackFunc, refreshTotalLengthFunc, setCurrentPositionFunc,
			setLoopRangeFunc, setHorizontalViewPortFunc, setVerticalViewPortFunc);
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
			this->projectChanged(project);
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

void EditorComponent::projectChanged(const vocalshaper::ProjectProxy* ptr)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->project = const_cast<vocalshaper::ProjectProxy*>(ptr);
	this->topEditor->projectChanged(ptr);
	this->bottomEditor->projectChanged(ptr);
	this->totalLengthChanged(this->countProjectTime(this->project));
}

void EditorComponent::trackChanged(int trackID)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->trackID = trackID;
	this->topEditor->trackChanged(trackID);
	this->bottomEditor->trackChanged(trackID);
}

void EditorComponent::totalLengthChanged(double totalLength)
{
	if (this->topEditor) {
		this->topEditor->setTotalLength(totalLength);
	}
	if (this->bottomEditor) {
		this->bottomEditor->setTotalLength(totalLength);
	}
}

void EditorComponent::currentPositionChanged(double currentTime)
{
	if (this->topEditor) {
		this->topEditor->setCurrentPosition(currentTime);
	}
	if (this->bottomEditor) {
		this->bottomEditor->setCurrentPosition(currentTime);
	}
}

void EditorComponent::followStateChanged(bool followState)
{
	if (this->topEditor) {
		this->topEditor->setFollowState(followState);
	}
	if (this->bottomEditor) {
		this->bottomEditor->setFollowState(followState);
	}
}

void EditorComponent::loopRangeChanged(double startTime, double endTime)
{
	if (this->topEditor) {
		this->topEditor->setLoopRange(startTime, endTime);
	}
	if (this->bottomEditor) {
		this->bottomEditor->setLoopRange(startTime, endTime);
	}
}

void EditorComponent::horizontalViewPortChanged(double startTime, double endTime)
{
	if (this->topEditor) {
		this->topEditor->setHorizontalViewPort(startTime, endTime);
	}
	if (this->bottomEditor) {
		this->bottomEditor->setHorizontalViewPort(startTime, endTime);
	}
}

void EditorComponent::verticalViewPortChanged(double bottomPitch, double topPitch)
{
	//轨道面板的横竖显示范围具有独立性
	if (this->topEditor) {
		this->topEditor->setVerticalViewPort(bottomPitch, topPitch);
	}
	if (this->bottomEditor) {
		this->bottomEditor->setVerticalViewPort(bottomPitch, topPitch);
	}
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
	juce::OwnedArray<vocalshaper::SerializableProjectStructure> temp;
	if (this->topEditor->isActive()) {
		temp = this->topEditor->getCut();
	}
	else if (this->bottomEditor->isActive()) {
		temp = this->bottomEditor->getCut();
	}
	jmadf::CallInterface<juce::OwnedArray<::vocalshaper::SerializableProjectStructure>&&>(
		"VocalSharp.VocalShaper.ClipBoard", "CopyAndCut",
		std::move(temp));
}

void EditorComponent::copy()
{
	juce::OwnedArray<vocalshaper::SerializableProjectStructure> temp;
	if (this->topEditor->isActive()) {
		temp = this->topEditor->getCopy();
	}
	else if (this->bottomEditor->isActive()) {
		temp = this->bottomEditor->getCopy();
	}
	jmadf::CallInterface<juce::OwnedArray<::vocalshaper::SerializableProjectStructure>&&>(
		"VocalSharp.VocalShaper.ClipBoard", "CopyAndCut",
		std::move(temp));
}

void EditorComponent::paste()
{
	juce::OwnedArray<vocalshaper::SerializableProjectStructure> temp;
	jmadf::CallInterface<juce::OwnedArray<::vocalshaper::SerializableProjectStructure>&>(
		"VocalSharp.VocalShaper.ClipBoard", "Paste",
		temp);
	if (this->topEditor->isActive()) {
		this->topEditor->wannaPaste(std::move(temp));
	}
	else if (this->bottomEditor->isActive()) {
		this->bottomEditor->wannaPaste(std::move(temp));
	}
}

void EditorComponent::clipBoard()
{
	juce::StringArray clipList;
	jmadf::CallInterface<juce::StringArray&>(
		"VocalSharp.VocalShaper.ClipBoard", "GetList",
		clipList);

	int result = -1;
	if (this->topEditor->isActive()) {
		result = this->topEditor->showClipBoard(clipList);
	}
	else if (this->bottomEditor->isActive()) {
		result = this->bottomEditor->showClipBoard(clipList);
	}

	if (result > -1) {
		juce::OwnedArray<vocalshaper::SerializableProjectStructure> temp;
		jmadf::CallInterface<juce::OwnedArray<::vocalshaper::SerializableProjectStructure>&, int>(
			"VocalSharp.VocalShaper.ClipBoard", "PasteItem",
			temp, result);
		if (this->topEditor->isActive()) {
			this->topEditor->wannaPaste(std::move(temp));
		}
		else if (this->bottomEditor->isActive()) {
			this->bottomEditor->wannaPaste(std::move(temp));
		}
	}
}

void EditorComponent::cleanClipBoard()
{
	jmadf::CallInterface<void>(
		"VocalSharp.VocalShaper.ClipBoard", "Clean");
}

void EditorComponent::createCopy()
{
	if (this->topEditor->isActive()) {
		this->topEditor->wannaCopy();
	}
	else if (this->bottomEditor->isActive()) {
		this->bottomEditor->wannaCopy();
	}
}

void EditorComponent::delete_()
{
	if (this->topEditor->isActive()) {
		this->topEditor->wannaDelete();
	}
	else if (this->bottomEditor->isActive()) {
		this->bottomEditor->wannaDelete();
	}
}

void EditorComponent::copyToSystem()
{
	juce::OwnedArray<vocalshaper::SerializableProjectStructure> temp;
	if (this->topEditor->isActive()) {
		temp = this->topEditor->getCopy();
	}
	else if (this->bottomEditor->isActive()) {
		temp = this->bottomEditor->getCopy();
	}
	else {
		juce::ScopedReadLock locker1(this->projectLock);
		juce::ScopedReadLock locker2(this->project->getLock());
		temp.add(vocalshaper::ProjectCopier::copy(this->project->getPtr()));
	}
	{
		//判断是否多元素
		bool arrayMode = false;
		if (temp.size() > 1) {
			arrayMode = true;
		}

		//多元素添加列表头
		juce::String tempS;
		if (arrayMode) {
			tempS += "[\n";
		}
		for (auto o : temp) {
			//Json序列化
			juce::String tempStr;
			if (vocalshaper::files::vsp3::ProtoConverter::serilazeToJson(o, tempStr, true)) {
				tempS += tempStr;
				//多元素加分隔
				if (arrayMode) {
					tempS += "\n,\n";
				}
			}
		}
		//多元素添尾
		if (arrayMode) {
			tempS.dropLastCharacters(2);
			tempS += "]";
		}
		juce::SystemClipboard::copyTextToClipboard(tempS);
	}
}

void EditorComponent::pasteFromSystem()
{
	juce::String str = juce::SystemClipboard::getTextFromClipboard();
	juce::StringArray clipList;
	{
		juce::var var;
		if (!juce::JSON::parse(str, var).wasOk()) {
			return;
		}
		if (var.isArray()) {
			auto array = var.getArray();
			for (auto& i : *array) {
				clipList.add(juce::JSON::toString(i, true));
			}
		}
		else {
			clipList.add(str);
		}
	}
	if (this->topEditor->isActive()) {
		this->topEditor->wannaPaste(clipList);
	}
	else if (this->bottomEditor->isActive()) {
		this->bottomEditor->wannaPaste(clipList);
	}
}

void EditorComponent::selectAll()
{
	if (this->topEditor->isActive()) {
		this->topEditor->wannaSelectAll();
	}
	else if (this->bottomEditor->isActive()) {
		this->bottomEditor->wannaSelectAll();
	}
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
	return this->topEditor->isActive() || this->bottomEditor->isActive();
}

bool EditorComponent::couldCopy()
{
	return this->topEditor->isActive() || this->bottomEditor->isActive();
}

bool EditorComponent::couldPaste()
{
	if (!this->topEditor->isActive() && !this->bottomEditor->isActive()) {
		return false;
	}
	int size = 0;
	jmadf::CallInterface<int&>(
		"VocalSharp.VocalShaper.ClipBoard", "GetSize",
		size
		);
	return size > 0;
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
	return this->topEditor->isActive() || this->bottomEditor->isActive();
}

bool EditorComponent::couldDelete()
{
	return this->topEditor->isActive() || this->bottomEditor->isActive();
}

bool EditorComponent::couldCopyToSystem()
{
	//return this->topEditor->isActive() || this->bottomEditor->isActive();
	return true;
}

bool EditorComponent::couldPasteFromSystem()
{
	juce::String str = juce::SystemClipboard::getTextFromClipboard();
	juce::var var;
	return juce::JSON::parse(str, var).wasOk();
}

bool EditorComponent::couldSelectAll()
{
	return this->topEditor->isActive() || this->bottomEditor->isActive();
}

void EditorComponent::lastTrack()
{
	juce::ScopedReadLock locker1(this->projectLock);
	if (this->project) {
		juce::ScopedReadLock locker2(this->project->getLock());
		int trackSize = ::vocalshaper::ProjectDAO::trackSize(this->project->getPtr());
		if (this->trackID > 0 && this->trackID < trackSize) {
			this->trackChanged(this->trackID - 1);
		}
	}
}

void EditorComponent::nextTrack()
{
	juce::ScopedReadLock locker1(this->projectLock);
	if (this->project) {
		juce::ScopedReadLock locker2(this->project->getLock());
		int trackSize = ::vocalshaper::ProjectDAO::trackSize(this->project->getPtr());
		if (this->trackID >= 0 && this->trackID < trackSize - 1) {
			this->trackChanged(this->trackID + 1);
		}
	}
}

void EditorComponent::switchTrack()
{
	int result = this->bottomEditor->switchTrack();
	juce::ScopedReadLock locker1(this->projectLock);
	if (this->project) {
		juce::ScopedReadLock locker2(this->project->getLock());
		int trackSize = ::vocalshaper::ProjectDAO::trackSize(this->project->getPtr());
		if (result > 0 && this->trackID <= trackSize) {
			this->trackChanged(result - 1);
		}
		else {
			this->trackChanged(-1);
		}
	}
}

bool EditorComponent::couldLastTrack()
{
	juce::ScopedReadLock locker1(this->projectLock);
	if (this->project) {
		juce::ScopedReadLock locker2(this->project->getLock());
		int trackSize = ::vocalshaper::ProjectDAO::trackSize(this->project->getPtr());
		if (this->trackID > 0 && this->trackID < trackSize) {
			return true;
		}
	}
	return false;
}

bool EditorComponent::couldNextTrack()
{
	juce::ScopedReadLock locker1(this->projectLock);
	if (this->project) {
		juce::ScopedReadLock locker2(this->project->getLock());
		int trackSize = ::vocalshaper::ProjectDAO::trackSize(this->project->getPtr());
		if (this->trackID >= 0 && this->trackID < trackSize - 1) {
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
	juce::ScopedReadLock locker1(this->projectLock);
	if (this->project) {
		juce::ScopedReadLock locker2(this->project->getLock());
		int trackSize = ::vocalshaper::ProjectDAO::trackSize(this->project->getPtr());
		if (trackSize > 0) {
			return true;
		}
	}
	return false;
}

void EditorComponent::setCurrentTrack(int trackID)
{
	juce::ScopedReadLock locker1(this->projectLock);
	if (this->project) {
		juce::ScopedReadLock locker2(this->project->getLock());
		int trackSize = ::vocalshaper::ProjectDAO::trackSize(this->project->getPtr());
		if (trackID >= -1 && trackID < trackSize) {
			this->trackChanged(trackID);
		}
	}
}

void EditorComponent::refreshTotalLength()
{
	juce::ScopedReadLock locker(this->projectLock);
	this->totalLengthChanged(this->countProjectTime(this->project));
}

void EditorComponent::setCurrentPosition(double currentTime)
{
	//TODO 同步至播放控制器
	//begin test
	this->currentPositionChanged(currentTime);
	//end test
}

void EditorComponent::setLoopRange(double startTime, double endTime)
{
	//TODO 同步至播放控制器
	//begin test
	this->loopRangeChanged(startTime, endTime);
	//end test
}

void EditorComponent::setHorizontalViewPort(double startTime, double endTime)
{
	juce::ScopedReadLock locker1(this->projectLock);
	juce::ScopedReadLock locker2(this->project->getLock());
	auto project = this->project->getPtr();
	if (!project) {
		return;
	}
	if (startTime < endTime) {
		this->horizontalViewPortChanged(startTime, endTime);
	}
}

void EditorComponent::setVerticalViewPort(double bottomPitch, double topPitch)
{
	if (bottomPitch >= 0 && bottomPitch <= 127 && topPitch >= 0 && topPitch <= 127) {
		if (bottomPitch < topPitch - 1) {
			//至少要有一个音符的高度
			this->verticalViewPortChanged(bottomPitch, topPitch);
		}
	}
}

void EditorComponent::setAdsorb(vocalshaper::AdsorbState state)
{
	this->topEditor->setAdsorb(state);
	this->bottomEditor->setAdsorb(state);
}

void EditorComponent::setGrid(vocalshaper::GridState state)
{
	this->topEditor->setGrid(state);
	this->bottomEditor->setGrid(state);
}

bool EditorComponent::isEditMode()
{
	return this->editModeFlag;
}

void EditorComponent::setEditMode(bool editMode)
{
	this->editModeFlag = editMode;
	this->topEditor->setEditMode(editMode);
	this->bottomEditor->setEditMode(editMode);
}

uint8_t EditorComponent::getToolID()
{
	return this->toolID;
}

void EditorComponent::setToolID(uint8_t toolID)
{
	this->toolID = toolID;
}

void EditorComponent::listenTrackSizeChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
{
	juce::ScopedReadLock locker(this->projectLock);
	if (this->project != action.getProxy()) {
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
				messageManager->callAsync([this, index] {this->setCurrentTrack(index); });
			}
			else if (type == vocalshaper::actions::ActionBase::UndoType::Undo) {
				if (data->index == this->trackID) {
					juce::ScopedReadLock projLock(action.getProxy()->getLock());
					int size = vocalshaper::ProjectDAO::trackSize(action.getProxy()->getPtr());
					int nextID = data->index - 1;
					if (size > 0 && nextID < 0) {
						nextID = 0;
					}
					int index = nextID;
					messageManager->callAsync([this, index] {this->setCurrentTrack(index); });
				}
				else {
					int index = this->trackID;
					messageManager->callAsync([this, index] {this->setCurrentTrack(index); });
				}
			}
		}
		else if (action.getActionType() == vocalshaper::actions::ProjectAction::Actions::RemoveTrack) {
			auto data = reinterpret_cast<const vocalshaper::actions::project::RemoveTrackAction::DataType*>(action.getData());
			if (type == vocalshaper::actions::ActionBase::UndoType::Perform) {
				if (data->index == this->trackID) {
					juce::ScopedReadLock projLock(action.getProxy()->getLock());
					int size = vocalshaper::ProjectDAO::trackSize(action.getProxy()->getPtr());
					int nextID = data->index - 1;
					if (size > 0 && nextID < 0) {
						nextID = 0;
					}
					int index = nextID;
					messageManager->callAsync([this, index] {this->setCurrentTrack(index); });
				}
				else {
					int index = this->trackID;
					messageManager->callAsync([this, index] {this->setCurrentTrack(index); });
				}
			}
			else if (type == vocalshaper::actions::ActionBase::UndoType::Undo) {
				int index = data->index;
				messageManager->callAsync([this, index] {this->setCurrentTrack(index); });
			}
		}
	}
}

void EditorComponent::listenProjectLengthChange(const vocalshaper::actions::ActionBase& action, vocalshaper::actions::ActionBase::UndoType type)
{
	juce::ScopedReadLock locker(this->projectLock);
	if (this->project != action.getProxy()) {
		return;
	}
	auto messageManager = juce::MessageManager::getInstance();
	if (!messageManager) {
		return;
	}
	double totalLength = EditorComponent::countProjectTime(this->project);
	if (totalLength != this->totalLengthTemp) {
		this->totalLengthTemp = totalLength;
		messageManager->callAsync([this] {this->refreshTotalLength(); });
	}
}

void EditorComponent::listenCurrentPositionChange(double currentTime)
{
	auto messageManager = juce::MessageManager::getInstance();
	if (!messageManager) {
		return;
	}
	messageManager->callAsync([this, currentTime] {this->currentPositionChanged(currentTime); });
}

void EditorComponent::listenFollowStateChange(bool followState)
{
	auto messageManager = juce::MessageManager::getInstance();
	if (!messageManager) {
		return;
	}
	messageManager->callAsync([this, followState] {this->followStateChanged(followState); });
}

void EditorComponent::listenLoopRangeChange(double startTime, double endTime)
{
	auto messageManager = juce::MessageManager::getInstance();
	if (!messageManager) {
		return;
	}
	messageManager->callAsync([this, startTime, endTime] {this->loopRangeChanged(startTime, endTime); });
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
			if (!this->isEditMode()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);
	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Edit Mode", [this]()->int {
			int flag = 0;
			if (this->isEditMode()) {
				flag |= juce::ApplicationCommandInfo::CommandFlags::isTicked;
			}
			return flag;
		}
	);

	jmadf::CallInterface<const juce::String&, const std::function<int(void)>&>(
		"VocalSharp.VocalShaper.CommandManager", "RegisterFlagHook",
		"Tool 1", [this]()->int {
			int flag = 0;
			if (!this->isEditMode()) {
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
			if (!this->isEditMode()) {
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
			if (!this->isEditMode()) {
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
			if (!this->isEditMode()) {
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
			if (!this->isEditMode()) {
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

	//TODO 侦听播放状态变化
}
