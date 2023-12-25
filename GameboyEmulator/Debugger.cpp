#include "Debugger.h"
Debugger::Debugger(gb::Gameboy* gameBoy)
{

	BreakPoints = std::vector<BreakPoint>(0xffff, { 0, 0 });
	AddresstoInstruction = std::vector<uint16_t>(0xffff, 0);
	gameboy = gameBoy;
	GenerateRomText();

	logstream.open("Execution_log.txt");


}
void Debugger::StartWindow()
{
	UIWindow* window = UIHandler::GetWindow(L"Debug");


	if (window == NULL) {
		UIHandler::AddWindow(L"Debug", L"SFML App", [this](HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam) { return EventPoll(Handle, Message, WParam, LParam); });

		window = UIHandler::GetWindow(L"Debug");

		SetWindowPos(window->Window, HWND_TOP, 0.5, 0.5, 800, 600, 0);
	}
	else {


		LPRECT rect = new RECT();
		GetWindowRect(window->Window, rect);
		SetWindowPos(window->Window, HWND_TOP, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, 0);

		free(rect);
	}
	
}
std::string Debugger::GetReference(gb::CPU_INFO::Instruction& instruction)
{
	std::stringstream output;

	for (int i = 0; i < instruction.Operands.size(); i++) {
		switch (instruction.Operands[i].name) {
			case gb::CPU_INFO::REF_A:
				output << "A";
				break;
			case gb::CPU_INFO::REF_C:
				output << "C";
				break;
			case gb::CPU_INFO::REF_B:
				output << "B";
				break;
			case gb::CPU_INFO::REF_D:
				output << "D";
				break;
			case gb::CPU_INFO::REF_E:
				output << "E";
				break;
			case gb::CPU_INFO::REF_H:
				output << "H";
				break;
			case gb::CPU_INFO::REF_L:
				output << "C";
				break;
			case gb::CPU_INFO::REF_$HL:
				
				if (instruction.instructionType == gb::CPU_INFO::INST_ldi_$HL$_A || instruction.instructionType == gb::CPU_INFO::INST_ldi_A_$HL$) {
					output << "(HL+)";
				}
				else if(instruction.instructionType == gb::CPU_INFO::INST_ldd_$HL$_A || instruction.instructionType == gb::CPU_INFO::INST_ldd_A_$HL$) {
					output << "(HL-)";
				}
				else {
					output << "(HL)";
				}
				break;
			case gb::CPU_INFO::REF_0:
			case gb::CPU_INFO::REF_1:
			case gb::CPU_INFO::REF_2:
			case gb::CPU_INFO::REF_3:
			case gb::CPU_INFO::REF_4:
			case gb::CPU_INFO::REF_5:
			case gb::CPU_INFO::REF_6:
			case gb::CPU_INFO::REF_7:
				output << instruction.Operands[i].name - gb::CPU_INFO::REF_0;
				break;
			case gb::CPU_INFO::REF_n8:
				output << std::hex << std::setfill('0') << std::setw(2) << D1;
				break;
			case gb::CPU_INFO::REF_a16:
				output << "$" << std::hex << std::setfill('0') << std::setw(4) << A16;
				break;
			case gb::CPU_INFO::REF_HL:
				output << "HL";
				break;
			case gb::CPU_INFO::REF_SP:
				output << "SP";
				break;
			case gb::CPU_INFO::REF_BC:
				output << "BC";
				break;
			case gb::CPU_INFO::REF_DE:
				output << "DE";
				break;
			case gb::CPU_INFO::REF_AF:
				output << "AF";
				break;
			case gb::CPU_INFO::REF_e8:
				output << (int)D1;
				break;
			case gb::CPU_INFO::REF_n16:
				output << std::hex << std::setfill('0') << std::setw(4) << A16;
				break;
			case gb::CPU_INFO::REF_NZ:
				output << "NZ";
				break;
			case gb::CPU_INFO::REF_Z:
				output << "Z";
				break;
			case gb::CPU_INFO::REF_NC:
				output << "NC";
				break;
			case gb::CPU_INFO::REF_$a16:
				output << "(" << std::hex << std::setfill('0') << std::setw(4) << A16 << ")";
				break;
			case gb::CPU_INFO::REF_$BC:
				output << "(" << std::hex << std::setfill('0') << std::setw(4) << "BC" << ")";
				break;
			case gb::CPU_INFO::REF_$DE:
				output << "(DE)";
				break;
			case gb::CPU_INFO::REF_$a8:
				output << "(ff00+" << std::hex << std::setfill('0') << std::setw(2) << D1 << ")";
				break;
			case gb::CPU_INFO::REF_$C:
				output << "(C)";
				break;
			case gb::CPU_INFO::REF_$00:
			case gb::CPU_INFO::REF_$08:
			case gb::CPU_INFO::REF_$10:
			case gb::CPU_INFO::REF_$18:
			case gb::CPU_INFO::REF_$20:
			case gb::CPU_INFO::REF_$28:
			case gb::CPU_INFO::REF_$30:
			case gb::CPU_INFO::REF_$38:
				output << std::hex << std::setfill('0') << std::setw(2) <<  8 * (instruction.Operands[i].name - gb::CPU_INFO::REF_$00);
				break;





		}




		if (instruction.Operands.size() > 1 && i == 0) {
			output << ",";
		}
	}
	return output.str();
	
}

void Debugger::GenerateRomText()
{
	uint16_t i = 0; 
	while (i < 0x8000) {


		AddresstoInstruction[i] = RomText.size();

		InstructionData inst;
		
		inst.Address = i;

		uint8_t opcode = gameboy->bus->ReadByte(i);

		gb::CPU_INFO::Instruction instruction = gb::CPU_INFO::unprefixed[opcode];

		inst.bytes[0] = opcode;

		inst.size = instruction.Size;
		i++;
		if (instruction.Size == 2) {
			D1 = gameboy->bus->ReadByte(i);
			inst.bytes[1] = D1;
			i++;
			
		}
		if (instruction.Size == 3) {
			A16 = gameboy->bus->ReadWord(i);


			inst.bytes[1] = D1;
			inst.bytes[2] = D2;

			i += 2;
		}
		if (instruction.mnemonic == "PREFIX") {
			
			opcode = gameboy->bus->ReadByte(i);
			inst.bytes[1] = opcode;

			instruction = gb::CPU_INFO::cbprefixed[opcode];
		}
		inst.mnemonic = instruction.mnemonic;
		
		inst.operand = GetReference(instruction);

		RomText.push_back(inst);
	}
}



std::wstring Debugger::GetRegisterString()
{
	std::wstringstream stream;

	stream << "af=" << std::hex << std::setfill(L'0') << std::setw(4) << gameboy->cpu->Registers.AF << "\n";
	stream << "bc=" << std::hex << std::setfill(L'0') << std::setw(4) << gameboy->cpu->Registers.BC << "\n";
	stream << "de=" << std::hex << std::setfill(L'0') << std::setw(4) << gameboy->cpu->Registers.DE << "\n";
	stream << "hl=" << std::hex << std::setfill(L'0') << std::setw(4) << gameboy->cpu->Registers.HL << "\n";
	stream << "sp=" << std::hex << std::setfill(L'0') << std::setw(4) << gameboy->cpu->Registers.SP << "\n";
	stream << "pc=" << std::hex << std::setfill(L'0') << std::setw(4) << gameboy->cpu->Registers.PC << "\n";

	if (gameboy->cpu->Registers.zero) {
		stream << L'☑' << "zero\n";
	}
	else {
		stream << L'☐' << "zero\n";
	}
	if (gameboy->cpu->Registers.Subtract) {
		stream << L'☑' << "Subtract\n";
	}
	else {
		stream << L'☐' << "Subtract\n";
	}

	if (gameboy->cpu->Registers.Carry) {
		stream << L'☑' << "Carry\n";
	}
	else {
		stream << L'☐' << "Carry\n";
	}
	if (gameboy->cpu->Registers.Half_Carry) {
		stream << L'☑' << "Half_Carry\n";
	}
	else {
		stream << L'☐' << "Half_Carry\n";
	}

	return stream.str();
}


LRESULT CALLBACK Debugger::EventPoll(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{

	switch (Message)
	{
		// Quit when we close the main window


	case WM_COMMAND:


		

		if(HIWORD(WParam) == BN_CLICKED){

			if ((HWND)LParam == StopStartButton) {
				if (RunGb) {
					RunGb = false;

					SetWindowText(StopStartButton, L"Start");
				}
				else {

					RunGb = true;
					Step = true;
					SetWindowText(StopStartButton, L"STOP");
				}
			}
			else if ((HWND)LParam == BreakPointButton) {

				int indexsel = SendMessage(DisAssembleView, LB_GETCURSEL, 0, 0);

				if (indexsel != LB_ERR) {


					LPTSTR toreceive = new wchar_t[256];

					SendMessage(DisAssembleView, LB_GETTEXT, indexsel, (LPARAM)toreceive);

					BreakPoints[RomText[indexsel].Address].InstructionIndex = indexsel;

					if (BreakPoints[RomText[indexsel].Address].Active) {
						toreceive[0] = L' ';
						BreakPoints[RomText[indexsel].Address].Active = false;
					}
					else {
					
						BreakPoints[RomText[indexsel].Address].Active = true;
						toreceive[0] = L'֍';
					}
				
					SendMessage(DisAssembleView, LB_DELETESTRING, indexsel, 0);
					SendMessage(DisAssembleView, LB_INSERTSTRING, indexsel, (LPARAM)toreceive);


				}
			}
			else if ((HWND)LParam == StepThrough) {
				Step = true;
			}
			else if ((HWND)LParam == GoBack1000) {

				if (States.size() > 0) {
				
					*gameboy = *States[States.size() - 1];

					States.pop_back();
				
				}
		

			}
		}

		break;

	case WM_SIZE:

		SetWindowPos(DisAssembleView, HWND_TOP, 0, 0, LOWORD(LParam) - 100, 500, 0);

		break;

	case WM_PAINT: {
		

		LPRECT rect = new RECT();
		GetWindowRect(Handle, rect);

		int win_width = rect->right - rect->left;
		int win_height = rect->bottom - rect->top;

		free(rect);

		// Get DC for window
		hdc = BeginPaint(Handle, &ps);

		// Create an off-screen DC for double-buffering
		hdcMem = CreateCompatibleDC(hdc);
		hbmMem = CreateCompatibleBitmap(hdc, win_width, win_height);

		hOld = SelectObject(hdcMem, hbmMem);

		// Draw into hdcMem here


		// Transfer the off-screen DC to the screen
		BitBlt(hdc, 0, 0, win_width, win_height, hdcMem, 0, 0, SRCCOPY);

		// Free-up the off-screen DC
		SelectObject(hdcMem, hOld);

		DeleteObject(hbmMem);
		DeleteDC(hdcMem);
		EndPaint(Handle, &ps);
		break;
	}


	case WM_CREATE:
	{


		

		wchar_t Word[128];



		
		if (DisAssembleView != NULL) {
			break;
		}

		DisAssembleView = CreateWindow(WC_LISTBOX, L"Hello", LBS_STANDARD | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 0, 0, 400, 500, Handle, NULL, UIHandler::hInstance, NULL);
		for (int i = 0; i < RomText.size(); i++) {


			
		
			size_t* PtCharConverted = new size_t[1];
			mbstowcs_s(PtCharConverted, Word, RomText[i].GetAsString().c_str(), 128);

			SendMessage(DisAssembleView, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)Word);
		}
		SendMessage(DisAssembleView, LB_GETSEL, (WPARAM)2, (LPARAM)0);

		HFONT newfont = CreateFontA(
			20,
			10,
			0,
			0,
			0,
			false,
			false,
			false,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			CLEARTYPE_QUALITY,
			FF_MODERN,
			"My weird font test"
		);

		COLORREF ref = 0x000000FF;
		
		SendMessage(DisAssembleView, WM_SETFONT, (WPARAM)newfont, true);
		
		RegisterView = CreateWindow(TEXT("STATIC"), TEXT("INFO"), WS_CHILDWINDOW | WS_VISIBLE | WS_THICKFRAME, 10, 10, 50, 30, Handle, NULL, GetModuleHandle(NULL), NULL);

		StopStartButton = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"STOP",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			700,         // x position 
			200,         // y position 
			100,        // Button width
			100,        // Button height
			Handle,     // Parent window
			NULL,       // No menu.
			(HINSTANCE)GetWindowLongPtr(Handle, GWLP_HINSTANCE),
			NULL);      // 
		BreakPointButton = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"Break Point",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			700,         // x position 
			200,         // y position 
			100,        // Button width
			100,        // Button height
			Handle,     // Parent window
			NULL,       // No menu.
			(HINSTANCE)GetWindowLongPtr(Handle, GWLP_HINSTANCE),
			NULL);      // 
		StepThrough = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"Step",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			700,         // x position 
			200,         // y position 
			100,        // Button width
			100,        // Button height
			Handle,     // Parent window
			NULL,       // No menu.
			(HINSTANCE)GetWindowLongPtr(Handle, GWLP_HINSTANCE),
			NULL);      // 
		GoBack1000 = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"Back 1000",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			700,         // x position 
			500,         // y position 
			100,        // Button width
			20,        // Button height
			Handle,     // Parent window
			NULL,       // No menu.
			(HINSTANCE)GetWindowLongPtr(Handle, GWLP_HINSTANCE),
			NULL);      // 


		break;
	}

	
	case WM_CLOSE:
		
		DestroyWindow(DisAssembleView);
		DisAssembleView = NULL;
		UIHandler::DestroyUIWindow(L"Debug");


	default:
		return DefWindowProc(Handle, Message, WParam, LParam);
	}
	return 0;

}

void Debugger::Update()
{
	UIWindow* window = UIHandler::GetWindow(L"Debug");

	if (window != NULL) {
		

		
		if (RunGb) {
			SetWindowText(StopStartButton, L"Stop");
		}
		else {

			SetWindowText(StopStartButton, L"Start");
		}

		LPRECT rectp = new RECT();
		GetWindowRect(window->Window, rectp);

		BitBlt(hdc, 0, 0, rectp->right - rectp->left, rectp->bottom - rectp->top, hdcMem, 0, 0, SRCCOPY);


		RECT rect = *rectp;
		free(rectp);
		SetWindowPos(RegisterView, NULL, rect.right - rect.left - 120, 0, 100, 200, NULL);
		SetWindowPos(StopStartButton, NULL, rect.right - rect.left - 120, 200, 100, 100, NULL);
		SetWindowPos(BreakPointButton, NULL, rect.right - rect.left - 120, 300, 100, 100, NULL);
		SetWindowPos(StepThrough, NULL, rect.right - rect.left - 120, 400, 100, 100, NULL);
		SetWindowPos(GoBack1000, NULL, rect.right - rect.left - 120, 500, 100, 20, NULL);

		SetWindowText(RegisterView, GetRegisterString().c_str());
	
		SendMessage(window->Window, WM_PAINT, 0, 0);


		
	}

	if (Step) {
		gameboy->Clock();
		Step = false;

		SendMessage(DisAssembleView, LB_SETCURSEL, AddresstoInstruction[gameboy->cpu->Registers.PC], 0);
	}
	if (RunGb) {
		while(gameboy->ppu->ReadLY() != 144) {

			
			if (BreakPoints[gameboy->cpu->Registers.PC].Active) {
				RunGb = false;
				SendMessage(DisAssembleView, LB_SETCURSEL, BreakPoints[gameboy->cpu->Registers.PC].InstructionIndex, 0);
				break;
			}
			//logstream << std::hex << std::setfill('0') << std::setw(4) << gameboy->cpu->Registers.PC << " ";
			gameboy->Clock();

			//logstream << gameboy->cpu->LoadedInstruction.mnemonic + "\n";

			//if (logstream.tellp() > 0xfffff) {
				//logstream.seekp(0);
			//}
			
			/*
			if (gameboy->CurrentClock % 70000 == 0) {
				States.push_back(gameboy->CopyState());
				if (States.size() > numberofstates) {
					delete States[0];
					States.erase(States.begin());
				}
			}
			
			*/
		}

		while (gameboy->ppu->ReadLY() == 144) {

			if (BreakPoints[gameboy->cpu->Registers.PC].Active) {
				RunGb = false;
				SendMessage(DisAssembleView, LB_SETCURSEL, BreakPoints[gameboy->cpu->Registers.PC].InstructionIndex, 0);
				break;
			}
			gameboy->Clock();

		}
	}
	





	/*
	* 	textRenderer.setCharacterSize(11);
	elements = (1.0f / Zoom * 600);



	std::string Current;

	int highest = 0;
	for (int i = 0; i < elements; i++) {
		Current += RomText[i + Index] + "\n";

		if (RomText[i + Index].size() > highest) {
			highest = RomText[i + Index].size();
		}
	}
	textRenderer.setPosition(sf::Vector2f(0, 0));
	textRenderer.setString(Current);

	window.draw(textRenderer);

	//draw register info
	Current = GetRegisterString();
	textRenderer.setString(Current);
	textRenderer.setCharacterSize(30);
	textRenderer.setPosition(sf::Vector2f(500, 0));
	window.draw(textRenderer);
	*/

}

std::string Debugger::InstructionData::GetAsString()
{
	std::stringstream str;

	str << " ";
	if (Address < 0x4000) {
		str << "ROM0:";
	}
	else {
		str << "ROM1:";
	}
	str << std::uppercase << std::hex << std::setfill('0') << std::setw(4) << Address;

	for (int i = 0; i < size; i++) {
		str << " ";
		str << std::hex << std::setfill('0') << std::setw(2) << (int)bytes[i];
	}

	str << std::setfill(' ') << std::setw(30 + mnemonic.length() - str.tellp());
	str << mnemonic;


	if (operand.size() > 0) {
		str << std::setfill(' ') << std::setw(36 + operand.length() - str.tellp());
		str << operand;
	}
	return str.str();
}
