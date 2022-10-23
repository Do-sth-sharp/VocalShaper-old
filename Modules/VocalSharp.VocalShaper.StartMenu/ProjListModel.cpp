#include "ProjListModel.h"
#include <libJModule.h>

ProjListModel::ProjListModel()
{
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
        "main", "color", "background-list", this->colors.background_list, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "background-listItem", this->colors.background_listItem, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-listItem-name", this->colors.text_listItem_name, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-listItem-time", this->colors.text_listItem_time, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "text-listItem-path", this->colors.text_listItem_path, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, juce::Colour&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetColor",
        "main", "color", "light-listItemStarted", this->colors.light_listItemStarted, result
        );

    //size
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-fontListItem-name", this->sizes.height_fontListItem_name, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-fontListItem-time", this->sizes.height_fontListItem_time, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-fontListItem-path", this->sizes.height_fontListItem_path, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "width-listItem-margin", this->sizes.width_listItem_margin, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-listItem-margin", this->sizes.height_listItem_margin, result
        );
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "size", "height-listItem-border", this->sizes.height_listItem_border, result
        );
    //scale
    jmadf::CallInterface<const juce::String&, const juce::String&, const juce::String&, double&, bool&>(
        "WuChang.JMADF.LookAndFeelConfigs", "GetNumber",
        "main", "scale", "height-listItemStarted-light", this->scales.height_listItemStarted_light, result
        );

    this->getSizeFunc =
        jmadf::GetInterface<int&>(
            "VocalSharp.VocalShaper.ProjectHistory", "GetSize"
            );
    this->getNameFunc =
        jmadf::GetInterface<int, juce::String&>(
            "VocalSharp.VocalShaper.ProjectHistory", "GetName"
            );
    this->getPathFunc =
        jmadf::GetInterface<int, juce::String&>(
            "VocalSharp.VocalShaper.ProjectHistory", "GetPath"
            );
    this->getTimeFunc =
        jmadf::GetInterface<int, juce::String&>(
            "VocalSharp.VocalShaper.ProjectHistory", "GetTime"
            );
}

void ProjListModel::setScreenSize(juce::Rectangle<int> screenSize)
{
    this->screenSize = screenSize;
}

void ProjListModel::setClickFunc(const std::function<void(int, const juce::String&, const juce::String&)>& onLeftClick,
    const std::function<void(int, const juce::String&, const juce::String&)>& onRightClick)
{
    this->onLeftClick = onLeftClick;
    this->onRightClick = onRightClick;
}

void ProjListModel::setCheckOpenendFunc(const std::function<bool(const juce::String&, const juce::String&)>& checkOpenedFunc)
{
    this->checkOpenedFunc = checkOpenedFunc;
}

int ProjListModel::getNumRows()
{
    int size = 0;
    this->getSizeFunc(size);
    return size;
}

void ProjListModel::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool /*rowIsSelected*/)
{
    //绘制背景
    g.setColour(this->colors.background_listItem);
    g.fillRoundedRectangle(0, 0, width, height, 6.0f);
    g.setColour(this->colors.background_list);
    g.drawRoundedRectangle(0, 0, width, height,
        6.0f, this->sizes.height_listItem_border * this->screenSize.getHeight());

    //计算文字区域
    juce::String nameStr, pathStr, timeStr;
    this->getNameFunc(rowNumber, nameStr);
    this->getPathFunc(rowNumber, pathStr);
    this->getTimeFunc(rowNumber, timeStr);
    if (nameStr.isEmpty()) {
        nameStr = this->tr("lb_NoData");
    }
    if (pathStr.isEmpty()) {
        pathStr = this->tr("lb_NoData");
    }
    if (timeStr.isEmpty()) {
        timeStr = this->tr("lb_NoData");
    }
    juce::Font timeFont = g.getCurrentFont();
    timeFont.setHeight(
        this->sizes.height_fontListItem_time * this->screenSize.getHeight()
    );
    int marginWidth = this->sizes.width_listItem_margin * this->screenSize.getWidth();
    int marginHeight = this->sizes.height_listItem_margin * this->screenSize.getHeight();
    int splitPosX = width - marginWidth
        - timeFont.getStringWidth(timeStr);
    int splitPosY = marginHeight + (height - marginHeight * 2)
        * (this->sizes.height_fontListItem_name / (this->sizes.height_fontListItem_name + this->sizes.height_fontListItem_path));
    int lightHeight = (height - marginHeight - splitPosY) * this->scales.height_listItemStarted_light;
    int lightPosX = width - marginWidth - lightHeight;
    int lightPosY = (height - marginHeight) - (height - marginHeight - splitPosY) / 2 - lightHeight / 2;

    //绘制名称
    g.setColour(this->colors.text_listItem_name);
    juce::Font nameFont = g.getCurrentFont();
    nameFont.setHeight(
        this->sizes.height_fontListItem_name * this->screenSize.getHeight()
    );
    g.setFont(nameFont);
    juce::Rectangle<int> nameRect(
        marginWidth, marginHeight,
        splitPosX - marginWidth, splitPosY - marginHeight
    );
    g.drawFittedText(nameStr, nameRect,
        juce::Justification::centredLeft, 1, 1.0f);

    //绘制路径
    g.setColour(this->colors.text_listItem_path);
    juce::Font pathFont = g.getCurrentFont();
    pathFont.setHeight(
        this->sizes.height_fontListItem_path * this->screenSize.getHeight()
    );
    g.setFont(pathFont);
    juce::Rectangle<int> pathRect(
        marginWidth, splitPosY,
        width - marginWidth * 3 - lightHeight, height - marginHeight - splitPosY
    );
    g.drawFittedText(pathStr, pathRect,
        juce::Justification::centredLeft, 1, 1.0f);

    //绘制时间
    g.setColour(this->colors.text_listItem_time);
    g.setFont(timeFont);
    juce::Rectangle<int> timeRect(
        splitPosX, marginHeight,
        width - marginWidth - splitPosX, splitPosY - marginHeight
    );
    g.drawFittedText(timeStr, timeRect,
        juce::Justification::centredRight, 1, 1.0f);

    //如果已打开，绘色块
    if (this->checkOpenedFunc(nameStr, pathStr)) {
        g.setColour(this->colors.light_listItemStarted);
        g.fillEllipse(lightPosX, lightPosY, lightHeight, lightHeight);
    }
}

juce::MouseCursor ProjListModel::getMouseCursorForRow(int /*row*/)
{
    return juce::MouseCursor::PointingHandCursor;
}

void ProjListModel::listBoxItemClicked(int row, const juce::MouseEvent& event)
{
    if (event.mods == juce::ModifierKeys::leftButtonModifier) {
        juce::String nameStr, pathStr;
        this->getNameFunc(row, nameStr);
        this->getPathFunc(row, pathStr);
        this->onLeftClick(row, nameStr, pathStr);
    }
    else if (event.mods == juce::ModifierKeys::rightButtonModifier) {
        juce::String nameStr, pathStr;
        this->getNameFunc(row, nameStr);
        this->getPathFunc(row, pathStr);
        this->onRightClick(row, nameStr, pathStr);
    }
}
