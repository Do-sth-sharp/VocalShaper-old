#pragma once
#include <JuceHeader.h>
#include <libVocalShaper.h>

class TrackView;

class ColorHistory final
{
public:
	ColorHistory(const juce::String& path = "/.data/colorHistory.json");

	bool load();
	bool save();

	int getSizeUsed();
	int getSizeTotal();

	const juce::Array<juce::Colour>& getList();
	void acceptColor(juce::Colour color);

private:
	juce::Array<juce::Colour> colorList;
	const juce::String path = "/.data/colorHistory.json";
	int totalSize = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorHistory);
};

class ColorEditor final : public juce::Component
{
public:
	explicit ColorEditor(TrackView* parent);

	void resized() override;
	void paint(juce::Graphics& g) override;

	void setCurrentColor(juce::Colour currentColor);
	void setSingerColor(bool hasSingerColor, juce::Colour color = juce::Colour());

private:
	void mouseDown(const juce::MouseEvent& event) override;
	void mouseUp(const juce::MouseEvent& event) override;
	void mouseMove(const juce::MouseEvent& event) override;
	void mouseDrag(const juce::MouseEvent& event) override;

private:
	struct Colors final
	{
		juce::Colour background_colorEditor;

		juce::Colour text_colorEditorButton;
		juce::Colour background_colorEditorButton;

		juce::Colour splitLine_colorEditor;

		juce::Colour text_colorEditorTitle;

		juce::Colour colorEditorHistoryDefaultColor;

		juce::Colour themeColor0;
		juce::Colour themeColor1;
		juce::Colour themeColor2;
		juce::Colour themeColor3;
		juce::Colour themeColor4;
		juce::Colour themeColor5;
		juce::Colour themeColor6;
	}colors;//界面颜色
	struct Size final
	{
		double height_colorEditorMarginTop;
		double height_colorEditorMarginBottom;
		double width_colorEditorMarginLeft;
		double width_colorEditorMarginRight;

		double width_colorEditorButton;

		double width_colorEditorSplitLine;
		double height_colorEditorSplitLine;

		double width_colorEditorSplit;

		double height_colorEditorTitleFont;
		double width_colorEditorTitleLeftMargin;

		double width_colorEditorColorTemplate;
		double height_colorEditorColorTemplate;
		double width_colorEditorColorTemplateCorner;

		double width_colorEditorColorTemplateSplit;
		double height_colorEditorColorTemplateSplit;

		double width_colorEditorColorSpace;
		double width_colorEditorHueSpace;
		double width_colorEditorColorSpaceSplit;
		double height_colorEditorColorSpace;
	}sizes;//控件大小
	struct Positions final
	{
		double posY_colorEditorButton;

		double posX_colorEditorButton;

		double posX_colorEditorSplit;
		double posY_colorEditorSplitLine;

		double posY_colorEditorLeftSplit1;
		double posY_colorEditorLeftSplit2;
		double posY_colorEditorLeftSplit3;
	}positions;//控件位置
	struct Scale final
	{
		double height_colorEditorButton;
	}scales;//控件缩放
	struct LookAndFeels final
	{
		std::unique_ptr<juce::LookAndFeel> button;
	}lafs;//控件样式

	std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;
	std::function<const juce::String(const juce::String&)> tr;

	class ColourSpaceView final : public juce::Component
	{
	public:
		ColourSpaceView(ColorEditor& cs);

		void setCurrentColor(float hue, float sat, float bri);

		void paint(juce::Graphics& g) override;

		void mouseDown(const juce::MouseEvent& e) override;
		void mouseDrag(const juce::MouseEvent& e) override;

		void resized() override;

	private:
		ColorEditor& owner;
		float h = 0.f, s = 0.f, v = 0.f;
		juce::Image colours;

		struct ColourSpaceMarker final : public juce::Component
		{
		public:
			ColourSpaceMarker();
			void paint(juce::Graphics& g) override;

			void setPos(float pX, float pY);

		private:
			struct Colors final
			{
				juce::Colour colorEditorColorSpaceCursor;
				juce::Colour colorEditorColorSpaceCursorBorder;
			}colors;//界面颜色
			struct Size final
			{
				double width_colorEditorColorSpaceCursor;
				double width_colorEditorColorSpaceCursorLine;
				double width_colorEditorColorSpaceCursorBorder;
			}sizes;//控件大小

			std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

			float pX = 0.f, pY = 0.f;
		};

		ColourSpaceMarker marker;

		void updateMarker();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColourSpaceView)
	};

	class HueSelectorComp final : public juce::Component
	{
	public:
		HueSelectorComp(ColorEditor& cs);

		void setCurrentColor(float hue, float sat, float bri);

		void paint(juce::Graphics& g) override;
		void resized() override;

		void mouseDown(const juce::MouseEvent& e) override;
		void mouseDrag(const juce::MouseEvent& e) override;

		void updateMarker();

	private:
		struct Size final
		{
			double width_colorEditorHueSpaceCorner;
		}sizes;//控件大小

		std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

		ColorEditor& owner;
		float h = 0.f, s = 0.f, v = 0.f;

		struct HueSelectorMarker : public juce::Component
		{
		public:
			HueSelectorMarker();
			void paint(juce::Graphics& g) override;

			void setPos(float pY);

		private:
			struct Colors final
			{
				juce::Colour colorEditorHueSpaceCursor;
				juce::Colour colorEditorHueSpaceCursorBorder;
			}colors;//界面颜色
			struct Size final
			{
				double width_colorEditorHueSpaceCursorLine;
				double width_colorEditorHueSpaceCursorBorder;
			}sizes;//控件大小

			std::function<void(juce::Component*, juce::Rectangle<int>&)> screenSizeFunc;

			float pY = 0.f;
		};

		HueSelectorMarker marker;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HueSelectorComp)
	};

	friend class ColourSpaceView;
	friend class HueSelectorComp;

	std::unique_ptr<juce::TextButton> okButton = nullptr;
	std::unique_ptr<ColorHistory> history = nullptr;

	std::unique_ptr<ColourSpaceView> colorSpace = nullptr;
	std::unique_ptr<HueSelectorComp> hueView = nullptr;

	TrackView* parent = nullptr;

	juce::Colour currentColor;
	float hue = 0.f, sat = 0.f, bri = 0.f;
	bool hasSinger = false;
	juce::Colour singerColor;

	int templateXSize = 6;

	void selectColor(float hue, float sat, float bri);
	void selectColor(juce::Colour color);
	void refreshWidgets();
	void acceptAndClose();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorEditor)
};

class ColorEditorCallOutBox final
{
public:
	ColorEditorCallOutBox() = delete;
	explicit ColorEditorCallOutBox(TrackView* parent);

	void resize(int width, int height);
	void setArrowSize(float arrowWidth);
	void setBorderSize(int borderSize);
	void setCornerSize(float cornerSize);

	void showAt(juce::Rectangle<int> rect);
	void close();

	void setCurrentColor(juce::Colour currentColour);
	void setSingerColor(bool hasSingerColor, juce::Colour color = juce::Colour());

private:
	class ColorEditorCallOutBoxCallback final
		: public juce::ModalComponentManager::Callback
	{
	public:
		ColorEditorCallOutBoxCallback() = delete;
		explicit ColorEditorCallOutBoxCallback(ColorEditorCallOutBox* manager);

		void setArrowSize(float arrowWidth);

		void showAt(juce::Rectangle<int> rect);
		void close();

	public:
		void modalStateFinished(int) override;

	private:
		std::unique_ptr<juce::CallOutBox> callOutBox = nullptr;

		TrackView* parent = nullptr;
		ColorEditorCallOutBox* manager = nullptr;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorEditorCallOutBoxCallback)
	};

private:
	struct Colors final
	{
		juce::Colour background_colorEditor;
		juce::Colour border_colorEditor;
	}colors;//界面颜色
	struct LookAndFeels final
	{
		std::unique_ptr<juce::LookAndFeel> callOutBox;
	}lafs;//控件样式

	friend class ColorEditorCallOutBoxCallback;

	TrackView* parent = nullptr;
	float arrowWidth = 16.f;
	int borderSize = 20;
	float cornerSize = 9.f;

	std::unique_ptr<ColorEditor> colorEditor = nullptr;
	std::unique_ptr<ColorEditorCallOutBoxCallback> callback = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorEditorCallOutBox)
};