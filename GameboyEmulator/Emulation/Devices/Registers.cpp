#include "Registers.h"
namespace gb {
	void Registers::Init()
	{
		Type = REGISTERS_TYPE;
		StateSize = sizeof(Registers) - sizeof(Device);
		deviceState = (uint8_t*)this + sizeof(Device);

		Bus->SetMemoryMap(REGISTERS_TYPE, P1_JOYP, IF - P1_JOYP);
		Bus->SetMemoryMap(REGISTERS_TYPE, IE, 1);
	}
	uint8_t Registers::Read(uint16_t Address)
	{

		uint8_t ReturnValue = 0;
		switch (Address) {

		case P1_JOYP:

			if (!joypad_register.SelectButtons) {
				joypad_register.A_Right = !Bus->CurrentIOstate.ControllerInput.A;
				joypad_register.B_Left = !Bus->CurrentIOstate.ControllerInput.B;
				joypad_register.Select_Up = !Bus->CurrentIOstate.ControllerInput.Select;
				joypad_register.Start_Down = !Bus->CurrentIOstate.ControllerInput.Start;
			}
			if (!joypad_register.Selectdpad) {
				joypad_register.A_Right = !Bus->CurrentIOstate.ControllerInput.Right;
				joypad_register.B_Left = !Bus->CurrentIOstate.ControllerInput.Left;
				joypad_register.Select_Up = !Bus->CurrentIOstate.ControllerInput.Up;
				joypad_register.Start_Down = !Bus->CurrentIOstate.ControllerInput.Down;
			}
			ReturnValue = joypad_register.Register;
			break;

		case SB:
			break;

		case SC:
			break;
		case DIV:
			break;
		case TIMA:
			break;
		case TMA:
			break;
		case TAC:
			break;
		case IF:
			ReturnValue = interupt_register.IF.DATA;
			break;
		case NR10:

		case NR11:
			break;

		case NR12:
			break;
		case NR13:
			break;
		case NR14:
			break;
		case NR21:
			break;
		case NR22:
			break;
		case NR23:
			break;
		case NR24:
			break;
		case NR30:
			break;
		case NR31:
			break;
		case NR32:
			break;
		case NR33:
			break;
		case NR34:
			break;
		case NR41:
			break;
		case NR42:
			break;
		case NR43:
			break;
		case NR44:
			break;
		case NR50:
			break;
		case NR51:
			break;
		case NR52:
			break;
		case LCDC:
			break;
		case STAT:
			break;
		case SCY:
			break;
		case SCX:
			break;
		case LY:
			break;
		case LYC:
			break;
		case DMA:
			break;
		case BGP:
			break;
		case OBP0:
			break;
		case OBP1:
			break;
		case WY:
			break;
		case WX:
			break;
		case KEY1:
			break;
		case VBK:
			break;
		case HDMA1:
			break;
		case HDMA2:
			break;
		case HDMA3:
			break;
		case HDMA4:
			break;
		case HDMA5:
			break;
		case RP:
			break;
		case BCPS_BGPI:
			break;
		case BCPD_BGPD:
			break;
		case OCPS_OBPI:
			break;
		case OCPD_OBPD:
			break;
		case OPRI:
			break;
		case SVBK:
			break;
		case PCM12:
			break;
		case PCM34:
			break;
		case IE:
			ReturnValue = interupt_register.IE.DATA;
			break;
		}
		return ReturnValue;
	}
	void Registers::Write(uint16_t Address, uint8_t Data)
	{
		//this is more of a guide most of these will be implemented in the devices them selves
		switch (Address) {

		case P1_JOYP:

			joypad_register.Register = Data;
			break;

		case SB:
			break;

		case SC:
			break;
		case DIV:
			break;
		case TIMA:
			break;
		case TMA:
			break;
		case TAC:
			break;
		case IF:
			interupt_register.IF.DATA = Data;
			break;
		case NR10:

		case NR11:
			break;

		case NR12:
			break;
		case NR13:
			break;
		case NR14:
			break;
		case NR21:
			break;
		case NR22:
			break;
		case NR23:
			break;
		case NR24:
			break;
		case NR30:
			break;
		case NR31:
			break;
		case NR32:
			break;
		case NR33:
			break;
		case NR34:
			break;
		case NR41:
			break;
		case NR42:
			break;
		case NR43:
			break;
		case NR44:
			break;
		case NR50:
			break;
		case NR51:
			break;
		case NR52:
			break;
		case LCDC:
			break;
		case STAT:
			break;
		case SCY:
			break;
		case SCX:
			break;
		case LY:
			break;
		case LYC:
			break;
		case DMA:
			break;
		case BGP:
			break;
		case OBP0:
			break;
		case OBP1:
			break;
		case WY:
			break;
		case WX:
			break;
		case KEY1:
			break;
		case VBK:
			break;
		case HDMA1:
			break;
		case HDMA2:
			break;
		case HDMA3:
			break;
		case HDMA4:
			break;
		case HDMA5:
			break;
		case RP:
			break;
		case BCPS_BGPI:
			break;
		case BCPD_BGPD:
			break;
		case OCPS_OBPI:
			break;
		case OCPD_OBPD:
			break;
		case OPRI:
			break;
		case SVBK:
			break;
		case PCM12:
			break;
		case PCM34:
			break;
		case IE:
			interupt_register.IE.DATA = Data;
			break;
		}
	}
}