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

	SaveState save_state;

	LRESULT CALLBACK MainWindowEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);

	enum {
		OPEN_FILE,
		OPEN_DEBUGGER,
		EMULATION_RESET,
		SAVE_SLOT,
		LOAD_SLOT,
	};

	void OnCommand(WPARAM WParam);
	
	void MenuHandler(HWND Handle);

	HMENU hMenu;
	HMENU hDebug;
	HMENU hEmulation;
	HMENU hSaveState;
	HMENU Emulation_Reset;
	HMENU Load_SLOT;
	HMENU Save_SLOT;
	HMENU hFileMenu;
};


