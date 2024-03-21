#pragma once
#include <windows.h>
#include <CommCtrl.h>
#include <objbase.h>
#include <string>
#include <iostream>
#include <functional>
#include <vector>

struct UIWindow {
	WNDCLASS WindowClass;
	HWND Window = NULL;
	std::function<LRESULT CALLBACK(HWND, UINT, WPARAM, LPARAM)> EventHandler;
	LPCWSTR windowName;
	LPCWSTR apptype;
};


//main ui handler for all things ui
class UIHandler
{
public:


	static HINSTANCE hInstance;
	static HINSTANCE hPrevInstance;
	static PSTR lpCmdLine;
	static int nCmdShow;

	



	static LRESULT CALLBACK MainEventHandler(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);
	

	static void Initialize(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);

	static void AddWindow(const wchar_t* windowname, const wchar_t* windowtype, std::function<LRESULT CALLBACK(HWND, UINT, WPARAM, LPARAM)> EventHandler);

	static UIWindow* GetWindow(const wchar_t* windowname);

	static void DestroyUIWindow(const wchar_t* windowname);

	static void UIShowWindow(HWND Windowinstance);


	static void DestroyResources();
private:

	static std::vector<UIWindow> Windows;

};

