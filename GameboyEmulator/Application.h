#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdio>
#include "Debugger.h"
#include "UIHandler.h"

class Application
{

public:
	Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
private:

	//main vars
	gb::Gameboy gameboy;
	Debugger* debugger;

	LRESULT CALLBACK MainWindowEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);

	enum {
		OPEN_FILE,
		OPEN_DEBUGGER,
		EMULATION_RESET
	};

	void OnCommand(WPARAM WParam);
	
	void MenuHandler(HWND Handle);

	HMENU hMenu;
	HMENU hDebug;
	HMENU hEmulation;
	HMENU Emulation_Reset;
	HMENU hFileMenu;
};


