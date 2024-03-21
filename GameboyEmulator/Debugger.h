#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "UIHandler.h"
#include <fstream>
#include "Emulation/Gameboy.h"
class Debugger
{
public:

	Debugger(gb::Gameboy* gameBoy);

	void StartWindow();

	gb::Gameboy* gameboy;


	std::vector<SaveState> States;
	int numberofstates = 16;
	struct InstructionData {
		uint16_t Address;
		std::string mnemonic;
		std::string operand;
		int size;
		uint8_t bytes[3];

		

		std::string GetAsString();


	};
	std::vector <InstructionData> RomText;
	
	union {
		struct {
			uint16_t D1 : 8;
			uint16_t D2 : 8;
		};

		uint16_t A16;
	};

	std::string GetReference(gb::CPU_INFO::Instruction& instruction);

	void GenerateRomText();



	std::ofstream logstream;

	struct BreakPoint {
		uint16_t InstructionIndex = 0;
		bool Active = false;
	};

	std::vector<BreakPoint> BreakPoints;
	
	std::vector<uint16_t> AddresstoInstruction;

	std::wstring GetRegisterString();

	HWND DisAssembleView;
	HWND RegisterView;
	HWND StopStartButton;
	HWND BreakPointButton;
	HWND StepThrough;
	HWND GoBack1000;
	HDC         hdcMem;
	HBITMAP     hbmMem;
	HANDLE      hOld;
	PAINTSTRUCT ps;
	HDC         hdc;

	bool RunGb = true;

	bool Step = false;
	//currernt instruction that is selected
	int InstructionSelected;

	LRESULT CALLBACK EventPoll(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);
	void Update();
};

