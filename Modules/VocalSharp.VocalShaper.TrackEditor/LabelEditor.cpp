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
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-menu", this->colors.background_menu, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-menu", this->colors.text_menu, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "header-menu", this->colors.header_menu, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-menu-highlight", this->colors.background_menu_highlight, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "text-menu-highlight", this->colors.text_menu_highlight, result
		);

	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "code-error", this->colors.code_error, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "code-comment", this->colors.code_comment, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "code-keyword", this->colors.code_keyword, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "code-operator", this->colors.code_operator, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "code-identifier", this->colors.code_identifier, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "code-integer", this->colors.code_integer, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "code-float", this->colors.code_float, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "code-string", this->colors.code_string, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "code-bracket", this->colors.code_bracket, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "code-punctuation", this->colors.code_punctuation, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "code-preprocessorText", this->colors.code_preprocessorText, result
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

	//建立下拉框样式
	jmadf::CallInterface<juce::LookAndFeel*&>(
		"VocalSharp.VocalShaper.LookAndFeelFactory", "GetLabelEditorComboBoxLAF",
		this->lafs.comboBox
		);
	this->lafs.comboBox->setColour(
		juce::ComboBox::ColourIds::backgroundColourId, this->colors.background_comboBox
	);
	this->lafs.comboBox->setColour(
		juce::ComboBox::ColourIds::textColourId, this->colors.text_comboBox
	);
	this->lafs.comboBox->setColour(
		juce::ComboBox::ColourIds::buttonColourId, this->colors.background_comboBox
	);
	this->lafs.comboBox->setColour(
		juce::ComboBox::ColourIds::arrowColourId, this->colors.text_comboBox
	);
	this->lafs.comboBox->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);
	this->lafs.comboBox->setColour(
		juce::ComboBox::ColourIds::focusedOutlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);
	this->lafs.comboBox->setColour(
		juce::PopupMenu::ColourIds::backgroundColourId, this->colors.background_menu
	);
	this->lafs.comboBox->setColour(
		juce::PopupMenu::ColourIds::textColourId, this->colors.text_menu
	);
	this->lafs.comboBox->setColour(
		juce::PopupMenu::ColourIds::headerTextColourId, this->colors.header_menu
	);
	this->lafs.comboBox->setColour(
		juce::PopupMenu::ColourIds::highlightedBackgroundColourId, this->colors.background_menu_highlight
	);
	this->lafs.comboBox->setColour(
		juce::PopupMenu::ColourIds::highlightedTextColourId, this->colors.text_menu_highlight
	);

	//初始化标签类型下拉框
	this->comboBox = std::make_unique<juce::ComboBox>("Label Type");
	this->comboBox->setLookAndFeel(this->lafs.comboBox);
	this->comboBox->addItemList({ "LUA", "INI", "XML", "JSON" }, 1);
	this->comboBox->setSelectedItemIndex(0);
	this->comboBox->onChange = [this] {this->comboBoxChanged(); };
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

	//建立代码编辑器样式
	jmadf::CallInterface<juce::LookAndFeel*&>(
		"VocalSharp.VocalShaper.LookAndFeelFactory", "GetLabelEditorCodeEditorLAF",
		this->lafs.codeEditor
		);
	this->lafs.codeEditor->setColour(
		juce::CodeEditorComponent::ColourIds::backgroundColourId, this->colors.background_codeEditor
	);
	this->lafs.codeEditor->setColour(
		juce::CodeEditorComponent::ColourIds::highlightColourId, this->colors.background_codeEditor_highlight
	);
	this->lafs.codeEditor->setColour(
		juce::CodeEditorComponent::ColourIds::defaultTextColourId, this->colors.text_codeEditor
	);
	this->lafs.codeEditor->setColour(
		juce::CodeEditorComponent::ColourIds::lineNumberBackgroundId, this->colors.background_codeEditor
	);
	this->lafs.codeEditor->setColour(
		juce::CodeEditorComponent::ColourIds::lineNumberTextId, this->colors.text_codeEditor
	);
	this->lafs.codeEditor->setColour(
		juce::ScrollBar::ColourIds::backgroundColourId, this->colors.background_codeEditor
	);
	this->lafs.codeEditor->setColour(
		juce::ScrollBar::ColourIds::thumbColourId, this->colors.thumb_codeEditor_scroller
	);
	this->lafs.codeEditor->setColour(
		juce::ScrollBar::ColourIds::trackColourId, this->colors.track_codeEditor_scroller
	);
	this->lafs.codeEditor->setColour(
		juce::CaretComponent::ColourIds::caretColourId, this->colors.caret_codeEditor
	);
	this->lafs.codeEditor->setColour(
		juce::PopupMenu::ColourIds::backgroundColourId, this->colors.background_menu
	);
	this->lafs.codeEditor->setColour(
		juce::PopupMenu::ColourIds::textColourId, this->colors.text_menu
	);
	this->lafs.codeEditor->setColour(
		juce::PopupMenu::ColourIds::headerTextColourId, this->colors.header_menu
	);
	this->lafs.codeEditor->setColour(
		juce::PopupMenu::ColourIds::highlightedBackgroundColourId, this->colors.background_menu_highlight
	);
	this->lafs.codeEditor->setColour(
		juce::PopupMenu::ColourIds::highlightedTextColourId, this->colors.text_menu_highlight
	);

	//建立代码调色板
	juce::CodeEditorComponent::ColourScheme codeScheme;
	codeScheme.set("Error", this->colors.code_error);
	codeScheme.set("Comment", this->colors.code_comment);
	codeScheme.set("Keyword", this->colors.code_keyword);
	codeScheme.set("Operator", this->colors.code_operator);
	codeScheme.set("Identifier", this->colors.code_identifier);
	codeScheme.set("Integer", this->colors.code_integer);
	codeScheme.set("Float", this->colors.code_float);
	codeScheme.set("String", this->colors.code_string);
	codeScheme.set("Bracket", this->colors.code_bracket);
	codeScheme.set("Punctuation", this->colors.code_punctuation);
	codeScheme.set("Preprocessor Text", this->colors.code_preprocessorText);

	//初始化编辑器与数据模型
	this->luaTokeniser = std::make_unique<juce::LuaTokeniser>();
	this->iniTokeniser = std::make_unique<juce::LuaTokeniser>();
	this->xmlTokeniser = std::make_unique<juce::XmlTokeniser>();
	this->jsonTokeniser = std::make_unique<juce::CPlusPlusCodeTokeniser>();

	this->document.addListener(this->documentListener.get());

	this->luaLabelEditor
		= std::make_unique<juce::CodeEditorComponent>(
			this->document, this->luaTokeniser.get());
	this->iniLabelEditor
		= std::make_unique<juce::CodeEditorComponent>(
			this->document, this->iniTokeniser.get());
	this->xmlLabelEditor
		= std::make_unique<juce::CodeEditorComponent>(
			this->document, this->xmlTokeniser.get());
	this->jsonLabelEditor
		= std::make_unique<juce::CodeEditorComponent>(
			this->document, this->jsonTokeniser.get());

	this->luaLabelEditor->setLookAndFeel(this->lafs.codeEditor);
	this->luaLabelEditor->setColourScheme(codeScheme);
	this->luaLabelEditor->setLineNumbersShown(false);
	this->iniLabelEditor->setLookAndFeel(this->lafs.codeEditor);
	this->iniLabelEditor->setColourScheme(codeScheme);
	this->iniLabelEditor->setLineNumbersShown(false);
	this->xmlLabelEditor->setLookAndFeel(this->lafs.codeEditor);
	this->xmlLabelEditor->setColourScheme(codeScheme);
	this->xmlLabelEditor->setLineNumbersShown(false);
	this->jsonLabelEditor->setLookAndFeel(this->lafs.codeEditor);
	this->jsonLabelEditor->setColourScheme(codeScheme);
	this->jsonLabelEditor->setLineNumbersShown(false);

	this->addAndMakeVisible(this->luaLabelEditor.get());
	this->addChildComponent(this->iniLabelEditor.get());
	this->addChildComponent(this->xmlLabelEditor.get());
	this->addChildComponent(this->jsonLabelEditor.get());

	//建立结果回显样式
	jmadf::CallInterface<juce::LookAndFeel*&>(
		"VocalSharp.VocalShaper.LookAndFeelFactory", "GetLabelEditorResultLabelLAF",
		this->lafs.label
		);
	this->lafs.label->setColour(
		juce::Label::ColourIds::backgroundColourId, this->colors.background_labelEditorResultLabel
	);
	this->lafs.label->setColour(
		juce::Label::ColourIds::textColourId, this->colors.text_labelEditorResultLabel
	);
	this->lafs.label->setColour(
		juce::Label::ColourIds::backgroundWhenEditingColourId, this->colors.background_labelEditorResultLabel
	);
	this->lafs.label->setColour(
		juce::Label::ColourIds::textWhenEditingColourId, this->colors.text_labelEditorResultLabel
	);
	this->lafs.label->setColour(
		juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);
	this->lafs.label->setColour(
		juce::Label::ColourIds::outlineWhenEditingColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//初始化结果回显
	this->resultLabel = std::make_unique<juce::Label>("Label Result");
	this->resultLabel->setLookAndFeel(this->lafs.label);
	this->resultLabel->setJustificationType(juce::Justification::topLeft);
	this->addAndMakeVisible(this->resultLabel.get());

	//建立确定按钮样式
	jmadf::CallInterface<juce::LookAndFeel*&>(
		"VocalSharp.VocalShaper.LookAndFeelFactory", "GetLabelEditorButtonLAF",
		this->lafs.button
		);
	this->lafs.button->setColour(
		juce::TextButton::ColourIds::buttonColourId, this->colors.background_labelEditorButton);
	this->lafs.button->setColour(
		juce::TextButton::ColourIds::buttonOnColourId, this->colors.background_labelEditorButton);
	this->lafs.button->setColour(
		juce::TextButton::ColourIds::textColourOffId, this->colors.text_labelEditorButton);
	this->lafs.button->setColour(
		juce::TextButton::ColourIds::textColourOnId, this->colors.text_labelEditorButton);
	this->lafs.button->setColour(
		juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromRGBA(0, 0, 0, 0)
	);

	//初始化确定按钮
	this->okButton = std::make_unique<juce::TextButton>(this->tr("bt_Accept"));
	this->okButton->setLookAndFeel(this->lafs.button);
	this->okButton->onClick = [this] {this->acceptAndClose(); };
	this->okButton->setEnabled(false);
	this->addAndMakeVisible(this->okButton.get());
}

LabelEditor::~LabelEditor()
{
	this->luaLabelEditor = nullptr;
	this->iniLabelEditor = nullptr;
	this->xmlLabelEditor = nullptr;
	this->jsonLabelEditor = nullptr;
}

void LabelEditor::setProject(vocalshaper::ProjectProxy* project)
{
	juce::ScopedWriteLock locker(this->projectLock);
	this->project = project;
	this->index = -1;
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
	//获取屏幕属性
	juce::Rectangle<int> screenSize;
	this->screenSizeFunc(this, screenSize);

	//计算控件大小
	int height_marginTop = this->sizes.height_labelEditorMarginTop * screenSize.getHeight();
	int height_marginBottom = this->sizes.height_labelEditorMarginBottom * screenSize.getHeight();
	int width_margin = this->sizes.width_labelEditorMargin * screenSize.getWidth();
	int width_comboBox = this->sizes.width_labelEditorComboBox * screenSize.getWidth();
	int width_button = this->sizes.width_labelEditorButton * screenSize.getWidth();

	int height_inside = this->getHeight() - height_marginTop - height_marginBottom;
	int width_inside = this->getWidth() - width_margin * 2;

	int posY_editorArea = height_marginTop +
		this->positions.posY_labelEditorCodeEditor * height_inside;
	int posY_resultArea = height_marginTop +
		this->positions.posY_labelEditorResultLabel * height_inside;
	int posY_buttonArea = height_marginTop +
		this->positions.posY_labelEditorButton * height_inside;

	int posX_comboBox = width_margin +
		this->positions.posX_labelEditorComboBox * width_inside;
	int posX_button = width_margin +
		this->positions.posX_labelEditorButton * width_inside;

	int height_comboBox = (posY_editorArea - height_marginTop) * this->scales.height_labelEditorComboBox;
	int height_editor = (posY_resultArea - posY_editorArea) * this->scales.height_labelEditorCodeEditor;
	int height_result = (posY_buttonArea - posY_resultArea) * this->scales.height_labelEditorResultLabel;
	int height_button = (this->getHeight() - height_marginBottom - posY_buttonArea) * this->scales.height_labelEditorButton;

	int posY_comboBox = height_marginTop + (posY_editorArea - height_marginTop) / 2 - height_comboBox / 2;
	int posY_editor = posY_editorArea + (posY_resultArea - posY_editorArea) / 2 - height_editor / 2;
	int posY_result = posY_resultArea + (posY_buttonArea - posY_resultArea) / 2 - height_result / 2;
	int posY_button = posY_buttonArea + (this->getHeight() - height_marginBottom - posY_buttonArea) / 2 - height_button / 2;

	//调整控件大小
	this->comboBox->setBounds(posX_comboBox, posY_comboBox,
		width_comboBox, height_comboBox);
	this->luaLabelEditor->setBounds(width_margin, posY_editor,
		width_inside, height_editor);
	this->iniLabelEditor->setBounds(width_margin, posY_editor,
		width_inside, height_editor);
	this->xmlLabelEditor->setBounds(width_margin, posY_editor,
		width_inside, height_editor);
	this->jsonLabelEditor->setBounds(width_margin, posY_editor,
		width_inside, height_editor);
	this->resultLabel->setBounds(width_margin, posY_result,
		width_inside, height_result);
	this->okButton->setBounds(posX_button, posY_button,
		width_button, height_button);
}

void LabelEditor::paint(juce::Graphics& g)
{
	//填充背景
	g.fillAll(this->colors.background_labelEditor);
}

void LabelEditor::comboBoxChanged()
{
	switch (static_cast<vocalshaper::Label::LabelType>(this->comboBox->getSelectedItemIndex()))
	{
	case vocalshaper::Label::LabelType::Lua:
		this->luaLabelEditor->setVisible(true);
		this->iniLabelEditor->setVisible(false);
		this->xmlLabelEditor->setVisible(false);
		this->jsonLabelEditor->setVisible(false);
		break;
	case vocalshaper::Label::LabelType::Ini:
		this->luaLabelEditor->setVisible(false);
		this->iniLabelEditor->setVisible(true);
		this->xmlLabelEditor->setVisible(false);
		this->jsonLabelEditor->setVisible(false);
		break;
	case vocalshaper::Label::LabelType::Xml:
		this->luaLabelEditor->setVisible(false);
		this->iniLabelEditor->setVisible(false);
		this->xmlLabelEditor->setVisible(true);
		this->jsonLabelEditor->setVisible(false);
		break;
	case vocalshaper::Label::LabelType::Json:
		this->luaLabelEditor->setVisible(false);
		this->iniLabelEditor->setVisible(false);
		this->xmlLabelEditor->setVisible(false);
		this->jsonLabelEditor->setVisible(true);
		break;
	}

	this->checkLabelData();
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
			this->currentDataTemp = vocalshaper::LabelDAO::getData(label);
			this->currentTypeTemp = vocalshaper::LabelDAO::getLabelType(label);

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
			//获取数据
			auto strData = this->document.getAllContent();

			//检查状态
			auto label = vocalshaper::ProjectDAO::getLabel(
				this->project->getPtr(), this->index);
			bool couldAcceptFlag = !((static_cast<vocalshaper::Label::LabelType>(this->comboBox->getSelectedItemIndex()) == this->currentTypeTemp) &&
				(strData == this->currentDataTemp));

			//解析结果
			vocalshaper::LabelTemp::LabelData data;

			//执行解析
			auto startTime = juce::Time::getMillisecondCounterHiRes();
			auto result = this->project->getLabelParser()->parseNow(
				strData, data,
				static_cast<vocalshaper::Label::LabelType>(this->comboBox->getSelectedItemIndex()),
				this->time, this->lastTempo, this->lastBeat
			);
			auto elapsedMs = juce::Time::getMillisecondCounterHiRes() - startTime;

			//显示状态
			juce::String labelStr;
			if (result == vocalshaper::LabelTemp::ParseResult::OK) {
				labelStr +=
					(this->tr("tip_LabelParseHead") + " " + juce::String(elapsedMs, 2) + " " + this->tr("tip_LabelParseTail") + "\n");
				this->resultLabel->getLookAndFeel().setColour(
					juce::Label::ColourIds::textColourId, this->colors.text_labelEditorResultLabel
				);
			}
			else if (result == vocalshaper::LabelTemp::ParseResult::TLE) {
				labelStr += (this->tr("tip_LabelParseTimeOut") + "\n");
				this->resultLabel->getLookAndFeel().setColour(
					juce::Label::ColourIds::textColourId, this->colors.text_labelEditorResultLabel_error
				);
				couldAcceptFlag = false;
			}
			else {
				labelStr += (this->tr("tip_LabelParseError") + "\n");
				this->resultLabel->getLookAndFeel().setColour(
					juce::Label::ColourIds::textColourId, this->colors.text_labelEditorResultLabel_error
				);
				couldAcceptFlag = false;
			}

			labelStr += ("time: " + juce::String(data.x) + "\n");
			labelStr += ("tempo: " + juce::String(data.tempo) + "\n");
			labelStr += ("beat: " + juce::String(data.beat) + "\n");
			labelStr += ("auto: " + juce::String(data.autoTempo ? "true" : "false") + "\n");

			this->resultLabel->setText(labelStr, juce::NotificationType::sendNotificationAsync);
			this->okButton->setEnabled(couldAcceptFlag);
		}
	}
}

void LabelEditor::acceptAndClose()
{
	//应用改变
	juce::ScopedReadLock locker(this->projectLock);
	if (this->project) {
		juce::ScopedReadLock projLocker(this->project->getLock());
		if (this->index >= 0 && this->index < vocalshaper::ProjectDAO::labelSize(this->project->getPtr())) {
			vocalshaper::actions::LabelAction::TargetType target;
			target.label = this->index;

			juce::OwnedArray<vocalshaper::actions::ActionBase> actionList({
				new vocalshaper::actions::label::TypeAction(
					target,static_cast<vocalshaper::Label::LabelType>(this->comboBox->getSelectedItemIndex()),
					this->project),
				new vocalshaper::actions::label::DataAction(
					target,this->document.getAllContent(),
					this->project)
				});
			this->project->getProcesser()->processEvents(std::move(actionList));
		}
	}
}

LabelEditorCallOutBox::LabelEditorCallOutBox(juce::Component* parent)
	: parent(parent)
{
	//以下获取界面属性
	bool result = false;
	//color
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "background-labelEditor", this->colors.background_labelEditor, result
		);
	jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
		"WuChang.JMADF.LookAndFeelConfigs", "GetColor",
		"main", "color", "border-labelEditor", this->colors.border_labelEditor, result
		);

	//size
	//position
	//scale

	//构建呼出框样式
	jmadf::CallInterface<juce::LookAndFeel*&,
		const std::function<int()>&, const std::function<float()>&,
		const juce::Colour, const juce::Colour>(
			"VocalSharp.VocalShaper.LookAndFeelFactory", "GetLabelEditorCallOutBoxLAF",
			this->lafs.callOutBox,
			[this] {return this->borderSize; }, [this] {return this->cornerSize; },
			this->colors.background_labelEditor, this->colors.border_labelEditor
			);

	//初始化控件
	this->labelEditor = std::make_unique<LabelEditor>();
}

void LabelEditorCallOutBox::setProject(vocalshaper::ProjectProxy* project)
{
	this->labelEditor->setProject(project);
}

void LabelEditorCallOutBox::setLabelIndex(int index)
{
	this->labelEditor->setLabelIndex(index);
}

void LabelEditorCallOutBox::resize(int width, int height)
{
	this->labelEditor->setSize(width, height);
}

void LabelEditorCallOutBox::setArrowSize(float arrowWidth)
{
	this->arrowWidth = arrowWidth;
	if (this->callback) {
		this->callback->setArrowSize(arrowWidth);
	}
}

void LabelEditorCallOutBox::setBorderSize(int borderSize)
{
	this->borderSize = borderSize;
}

void LabelEditorCallOutBox::setCornerSize(float cornerSize)
{
	this->cornerSize = cornerSize;
}

void LabelEditorCallOutBox::showAt(juce::Rectangle<int> rect)
{
	if (!this->callback) {
		this->callback = std::make_unique<LabelEditorCallOutBoxCallback>(this);
	}
	this->callback->showAt(rect);
}

void LabelEditorCallOutBox::close()
{
	if (this->callback) {
		this->callback->close();
	}
}

LabelEditorCallOutBox::LabelEditorCallOutBoxCallback::LabelEditorCallOutBoxCallback(LabelEditorCallOutBox* manager)
	: manager(manager), parent(manager->parent)
{
	//初始化控件
	this->callOutBox = std::make_unique<juce::CallOutBox>(
		*(manager->labelEditor.get()), juce::Rectangle<int>({ 0, 0, 0, 0 }), nullptr);
	this->callOutBox->setLookAndFeel(manager->lafs.callOutBox);
	this->callOutBox->setArrowSize(manager->arrowWidth);
	//this->callOutBox->setDismissalMouseClicksAreAlwaysConsumed(true);
}

void LabelEditorCallOutBox::LabelEditorCallOutBoxCallback::setArrowSize(float arrowWidth)
{
	this->callOutBox->setArrowSize(arrowWidth);
}

void LabelEditorCallOutBox::LabelEditorCallOutBoxCallback::showAt(juce::Rectangle<int> rect)
{
	//获取控件所在窗口
	auto window = parent->getTopLevelComponent();
	auto windowSize = window->getScreenBounds();

	//计算屏幕显示位置
	auto screenBounds = parent->getScreenBounds();
	juce::Rectangle<int> placeInScreen = rect;
	placeInScreen.setPosition(rect.getX() + screenBounds.getX(), rect.getY() + screenBounds.getY());

	//更新位置并显示
	this->callOutBox->updatePosition(placeInScreen, windowSize);
	this->callOutBox->setVisible(true);
	this->callOutBox->enterModalState(true, this, false);
}

void LabelEditorCallOutBox::LabelEditorCallOutBoxCallback::close()
{
	this->callOutBox->dismiss();
}

void LabelEditorCallOutBox::LabelEditorCallOutBoxCallback::modalStateFinished(int)
{
	manager->callback.release();
}
