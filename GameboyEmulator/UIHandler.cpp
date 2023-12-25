#include "UIHandler.h"



//static var defs
std::vector<UIWindow> UIHandler::Windows;


HINSTANCE UIHandler::hInstance;
HINSTANCE UIHandler::hPrevInstance;
PSTR UIHandler::lpCmdLine;
int UIHandler::nCmdShow;




void UIHandler::Initialize(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    UIHandler::hInstance = hInstance;
    UIHandler::hPrevInstance = hPrevInstance;
    UIHandler::lpCmdLine = lpCmdLine;
    UIHandler::nCmdShow = nCmdShow;
}

LRESULT UIHandler::MainEventHandler(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{

    for (int i = 0; i < UIHandler::Windows.size(); i++) {
        if (UIHandler::Windows[i].Window == Handle || UIHandler::Windows[i].Window == NULL) {

            return UIHandler::Windows[i].EventHandler(Handle, Message, WParam, LParam);
        }
    }
}



void UIHandler::AddWindow(const wchar_t* windowname, const wchar_t* windowtype, std::function<LRESULT CALLBACK(HWND, UINT, WPARAM, LPARAM)> WindowEvents)
{



    UIWindow window;


    window.apptype = windowtype;
    window.windowName = windowname;
    window.EventHandler = WindowEvents;

    window.WindowClass.style = 0;
    window.WindowClass.lpfnWndProc = &MainEventHandler;
    window.WindowClass.cbClsExtra = 0;
    window.WindowClass.cbWndExtra = 0;
    window.WindowClass.hInstance = hInstance;
    window.WindowClass.hIcon = NULL;
    window.WindowClass.hCursor = 0;
    window.WindowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
    window.WindowClass.lpszMenuName = NULL;
    window.WindowClass.lpszClassName = window.apptype;
    RegisterClass(&window.WindowClass);

 
    Windows.push_back(window);

    // Let's create the main window

    
    Windows[Windows.size() - 1].Window = CreateWindow(window.apptype, window.windowName, WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_SIZEBOX | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 0, 0, 800, 600, NULL, NULL, hInstance, NULL);


    DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
   
   // HWND  View1 = CreateWindow(toLPCWSTR("STATIC"), NULL, Style, 0, 0, 800, 600, Window, NULL, hInstance, NULL);


}
void UIHandler::DestroyUIWindow(const wchar_t* windowname) {

    for (int i = 0; i < Windows.size(); i++) {
        if (!wcscmp(windowname, Windows[i].windowName)) {

            DestroyWindow(Windows[i].Window);
            UnregisterClass(Windows[i].apptype, hInstance);
            Windows.erase(Windows.begin() + i);

            break;
        }
    }

}

UIWindow* UIHandler::GetWindow(const wchar_t* windowname) {
    for (int i = 0; i < Windows.size(); i++) {
        if (!wcscmp(windowname, Windows[i].windowName)) {
            return &Windows[i];
        }
    }
    return NULL;
}

void UIHandler::DestroyResources()
{
    for (int i = 0; i < Windows.size(); i++) {
        UnregisterClass(Windows[i].windowName, hInstance);
    }
  
}

void UIHandler::UIShowWindow(HWND Windowinstance) {
    ShowWindow(Windowinstance, nCmdShow);
}