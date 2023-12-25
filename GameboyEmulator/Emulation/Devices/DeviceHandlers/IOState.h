#pragma once
#include <iostream>

#define LCDWIDTH 160
#define LCDHEIGHT 144

struct IOState {


	uint32_t* Buffers[2];

	bool BufferSelect = 0;

	//current buffer to render to
	uint32_t* LCDscreen;


	IOState() { Buffers[0] = new uint32_t[LCDWIDTH * LCDHEIGHT]; Buffers[1] = new uint32_t[LCDWIDTH * LCDHEIGHT]; LCDscreen = Buffers[0]; }

	//called when render is complete
	void SwapBuffer() { if (BufferSelect == 1) BufferSelect = 0; else BufferSelect = 1; LCDscreen = Buffers[BufferSelect]; }

	void WritePixel(uint32_t Color, int x, int y) { LCDscreen[LCDWIDTH * y + x] = Color; }

	

	struct {

		//Controller Input
		bool A, B, Select, Start, Up, Down, Right, Left;
	} ControllerInput;
private:


};