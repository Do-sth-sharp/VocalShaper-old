#include "MainWindow.h"
#include "libJModule.h"

#if JUCE_WINDOWS
#include <Windows.h>
#include <ShlObj.h>
#endif

MainWindow::MainWindow(juce::String name)
    : DocumentWindow(name,
        juce::Desktop::getInstance().getDefaultLookAndFeel()
        .findColour(juce::ResizableWindow::backgroundColourId),
        DocumentWindow::allButtons)
{
    this->setUsingNativeTitleBar(true);
    this->setContentOwned(new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
    this->setFullScreen(true);
#else
    this->setResizable(true, true);
    this->centreWithSize(getWidth(), getHeight());
    this->setFullScreen(true);
#endif

    size_t iconSize = 0;
    void* iconPtr = nullptr;
    juce::String iconPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
        .getParentDirectory().getFullPathName() + "/rc/logo.bmp";
	
    jmadf::CallInterface<const juce::String&, std::pair<size_t&, void*&>>(
        "WuChang.JMADF.DynamicRC",
        "GetRC",
        iconPath, std::pair<size_t&, void*&>(iconSize, iconPtr)
        );
    this->setIcon(juce::ImageFileFormat::loadFrom(iconPtr, iconSize));
    void* wHandle = this->getWindowHandle();
	
	//在WINDOWS平台上使用WIN32_API修改图标
#if JUCE_WINDOWS
    {
        HWND hWnd = (HWND)wHandle;
        HDC hDC = ::GetDC(hWnd);

        tagBITMAPFILEHEADER bfh = *(tagBITMAPFILEHEADER*)iconPtr;
        tagBITMAPINFOHEADER bih = *(tagBITMAPINFOHEADER*)((size_t)iconPtr + sizeof(tagBITMAPFILEHEADER));
        RGBQUAD rgb = *(RGBQUAD*)((size_t)iconPtr + sizeof(tagBITMAPFILEHEADER) + sizeof(tagBITMAPINFOHEADER));
		
        BITMAPINFO bi{};
        bi.bmiColors[0] = rgb;
        bi.bmiHeader = bih;
		
        char* pPixels = (char*)((size_t)iconPtr + bfh.bfOffBits);
        char* ppvBits = NULL;
        HBITMAP hBitmap = ::CreateDIBSection(hDC, &bi, DIB_RGB_COLORS, (void**)&ppvBits, NULL, 0);
		
        if (hBitmap) {
            ::SetDIBits(hDC, hBitmap, 0, bih.biHeight, pPixels, &bi, DIB_RGB_COLORS);

            HBITMAP hbmMask = ::CreateCompatibleBitmap(hDC, bih.biWidth, bih.biHeight);
            if (hbmMask) {
                ICONINFO ii = { 0 };
                ii.fIcon = TRUE;
                ii.hbmColor = hBitmap;
                ii.hbmMask = hbmMask;

                HICON hIcon = ::CreateIconIndirect(&ii);
                if (hIcon) {
                    ::SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
                    ::DestroyIcon(hIcon);
                }
                ::DeleteObject(hbmMask);
            }
            ::DeleteObject(hBitmap);
        }
        ::ReleaseDC(hWnd, hDC);
    }
#endif
	
}

void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}
