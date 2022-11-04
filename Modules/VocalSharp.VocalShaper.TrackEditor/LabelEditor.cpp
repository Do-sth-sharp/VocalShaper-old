#include "LabelEditor.h"
#include <libJModule.h>

LabelEditor::LabelEditor()
	: Component("Label Editor")
{
	//获取屏幕属性接口
	this->screenSizeFunc =
		jmadf::GetInterface<juce::Component*, juce::Rectangle<int>&>(
			"WuChang.JMADF.Device", "GetScreenSize"
			);

	//获取翻译器
	jmadf::CallInterface<std::function<const juce::String(const juce::String&)>&>(
		"WuChang.JMADF.Translates", "GetFunc",
		this->tr
		);

    //以下获取界面属性
    bool result = false;
    //color
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-labelEditor", this->colors.background_labelEditor, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-comboBox", this->colors.text_comboBox, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-comboBox", this->colors.background_comboBox, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-codeEditor", this->colors.text_codeEditor, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-codeEditor", this->colors.background_codeEditor, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-codeEditor-highlight", this->colors.text_codeEditor_highlight, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-codeEditor-highlight", this->colors.background_codeEditor_highlight, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "caret-codeEditor", this->colors.caret_codeEditor, result
        );
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "thumb-codeEditor-scroller", this->colors.thumb_codeEditor_scroller, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "track-codeEditor-scroller", this->colors.track_codeEditor_scroller, result
		);
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-labelEditorResultLabel", this->colors.text_labelEditorResultLabel, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-labelEditorResultLabel-error", this->colors.text_labelEditorResultLabel_error, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-labelEditorResultLabel", this->colors.background_labelEditorResultLabel, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-labelEditorButton", this->colors.text_labelEditorButton, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-labelEditorButton", this->colors.background_labelEditorButton, result
        );

    //size
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-labelEditorMarginTop", this->sizes.height_labelEditorMarginTop, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-labelEditorMarginBottom", this->sizes.height_labelEditorMarginBottom, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-labelEditorMargin", this->sizes.width_labelEditorMargin, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-labelEditorComboBox", this->sizes.width_labelEditorComboBox, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-labelEditorButton", this->sizes.width_labelEditorButton, result
        );
    //position
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "position", "posY-labelEditorCodeEditor", this->positions.posY_labelEditorCodeEditor, result
        );
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "position", "posY-labelEditorResultLabel", this->positions.posY_labelEditorResultLabel, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "position", "posY-labelEditorButton", this->positions.posY_labelEditorButton, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "position", "posX-labelEditorComboBox", this->positions.posX_labelEditorComboBox, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
		"main", "position", "posX-labelEditorButton", this->positions.posX_labelEditorButton, result
		);
    //scale
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "height-labelEditorComboBox", this->scales.height_labelEditorComboBox, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "height-labelEditorCodeEditor", this->scales.height_labelEditorCodeEditor, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "height-labelEditorResultLabel", this->scales.height_labelEditorResultLabel, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "height-labelEditorButton", this->scales.height_labelEditorButton, result
        );

	//初始化标签类型下拉框
	this->comboBox = std::make_unique<juce::ComboBox>("Label Type");
	this->comboBox->addItemList({ "LUA", "INI", "XML", "JSON" }, 1);
	this->comboBox->setSelectedItemIndex(0);
	this->comboBox->onChange = [this] {this->checkLabelData(); };
	this->addAndMakeVisible(this->comboBox.get());

	//侦听文档变化
	struct DocumentListener final : public juce::CodeDocument::Listener
	{
		DocumentListener() = delete;
		explicit DocumentListener(LabelEditor* _this) : _this(_this) {};
		void codeDocumentTextInserted(const juce::String& newText, int insertIndex) override
		{
			if (_this) {
				_this->checkLabelData();
			}
		};
		void codeDocumentTextDeleted(int startIndex, int endIndex) override
		{
			if (_this) {
				_this->checkLabelData();
			}
		};

	private:
		LabelEditor* _this = nullptr;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DocumentListener)
	};
	this->documentListener = std::unique_ptr<juce::CodeDocument::Listener>(new DocumentListener(this));

	//初始化编辑器与数据模型
	this->luaTokeniser = std::make_unique<juce::LuaTokeniser>();
	this->iniTokeniser = std::make_unique<juce::LuaTokeniser>();
	this->xmlTokeniser = std::make_unique<juce::XmlTokeniser>();
	this->jsonTokeniser = std::make_unique<juce::CPlusPlusCodeTokeniser>();

	this->document.addListener(this->documentListener.get());
	this->labelEditor
		= std::make_unique<juce::CodeEditorComponent>(
			this->document, this->luaTokeniser.get());
	this->labelEditor->setLineNumbersShown(false);
	this->addAndMakeVisible(this->labelEditor.get());

	//初始化结果回显
	this->resultLabel = std::make_unique<juce::Label>("Label Result");
	this->addAndMakeVisible(this->resultLabel.get());

	//初始化确定按钮
	this->okButton = std::make_unique<juce::TextButton>(this->tr("bt_Accept"));
	this->okButton->onClick = [this] {this->acceptAndClose(); };
	this->addAndMakeVisible(this->okButton.get());
}

void LabelEditor::setProject(vocalshaper::ProjectProxy* project)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->project = project;
	this->refreshLabel();
}

void LabelEditor::setLabelIndex(int index)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->index = index;
	this->refreshLabel();
}

void LabelEditor::resized()
{

}

void LabelEditor::refreshLabel()
{
	//加载标签数据并检查
	juce::ScopedReadLock locker(this->projectLock);
	if (this->project) {
		juce::ScopedReadLock projLocker(this->project->getLock());
		if (this->index >= 0 && this->index < vocalshaper::ProjectDAO::labelSize(this->project->getPtr())) {
			//获取当前标签
			auto label = vocalshaper::ProjectDAO::getLabel(
				this->project->getPtr(), this->index);
			this->time = vocalshaper::LabelDAO::getPosition(label);

			//计算上一标签曲速与节拍
			if (this->index > 0) {
				auto lastLabel = vocalshaper::ProjectDAO::getLabel(
					this->project->getPtr(), this->index - 1);
				double lastTime = vocalshaper::LabelDAO::getPosition(lastLabel);

				this->lastTempo = this->project->getTempo()->getTempData(
					this->project->getTempo()->selectLabelBy_x(lastTime)).Ts;
				
				this->lastBeat = this->project->getBeat()->getBeatAtTime(lastTime);
			}

			//更新数据
			this->comboBox->setSelectedItemIndex(
				static_cast<int>(vocalshaper::LabelDAO::getLabelType(label)));
			this->document.replaceAllContent(vocalshaper::LabelDAO::getData(label));
		}
	}
}

void LabelEditor::checkLabelData()
{
	//检查标签数据
	juce::ScopedReadLock locker(this->projectLock);
	if (this->project) {
		juce::ScopedReadLock projLocker(this->project->getLock());
		if (this->index >= 0 && this->index < vocalshaper::ProjectDAO::labelSize(this->project->getPtr())) {
			vocalshaper::LabelTemp::LabelData data;

			//执行解析
			auto startTime = juce::Time::getMillisecondCounterHiRes();
			bool result = this->project->getLabelParser()->parseNow(
				this->document.getAllContent(),
				data, static_cast<vocalshaper::Label::LabelType>(this->comboBox->getSelectedItemIndex()),
				this->time, this->lastTempo, this->lastBeat
			);
			auto elapsedMs = juce::Time::getMillisecondCounterHiRes() - startTime;

			//显示状态
			juce::String labelStr;
			if (result) {
				labelStr +=
					(this->tr("tip_LabelParseHead") + " " + juce::String(elapsedMs, 2) + " " + this->tr("tip_LabelParseTail") + "\n");
				this->resultLabel->getLookAndFeel().setColour(
					juce::Label::ColourIds::textColourId, this->colors.text_labelEditorResultLabel
				);
			}
			else {
				labelStr += (this->tr("tip_LabelParseError") + "\n");
				this->resultLabel->getLookAndFeel().setColour(
					juce::Label::ColourIds::textColourId, this->colors.text_labelEditorResultLabel_error
				);
			}

			labelStr += ("time: " + juce::String(data.x) + "\n");
			labelStr += ("tempo: " + juce::String(data.tempo) + "\n");
			labelStr += ("beat: " + juce::String(data.beat) + "\n");
			labelStr += ("autoTempo: " + juce::String(data.autoTempo ? "true" : "false") + "\n");

			this->resultLabel->setText(labelStr, juce::NotificationType::sendNotificationAsync);
		}
	}
}

void LabelEditor::acceptAndClose()
{
	//应用改变
	juce::ScopedWriteLock locker(this->projectLock);
	if (this->project) {
		juce::ScopedReadLock projLocker(this->project->getLock());
		if (this->index >= 0 && this->index < vocalshaper::ProjectDAO::labelSize(this->project->getPtr())) {
			vocalshaper::actions::LabelAction::TargetType target;
			target.label = this->index;

			juce::OwnedArray<vocalshaper::actions::ActionBase> actionList = {
				new vocalshaper::actions::label::TypeAction(
					target,static_cast<vocalshaper::Label::LabelType>(this->comboBox->getSelectedItemIndex()),
					this->project),
				new vocalshaper::actions::label::DataAction(
					target,this->document.getAllContent(),
					this->project)
			};
			this->project->getProcesser()->processEvents(std::move(actionList));
		}
	}
	this->project = nullptr;
	this->setVisible(false);
}
