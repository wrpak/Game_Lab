// สุ่มตัวอักษร และ สี

#include <windows.h>
#include <time.h>
#include <stdio.h>


#define screen_x 80
#define screen_y 25

HANDLE wHnd;
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD bufferSize = { screen_x,screen_y };
COORD characterPos = { 0,0 };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };

int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}

void fill_data_to_buffer()
{
	srand(time(NULL));
	for (int y = 0; y < screen_y; ++y) {
		for (int x = 0; x < screen_x; ++x) {
			char randomChar = 'A' + rand() % 26; // สุ่มตัวอักษร A-Z
			int randomColor = rand() % 256; // สุ่มค่าสี 0-255

			consoleBuffer[x + screen_x * y].Char.AsciiChar = randomChar;
			consoleBuffer[x + screen_x * y].Attributes = randomColor;
			
		}
	}
}

void fill_buffer_to_console()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos,
		&windowSize);
}

int main()
{
	setConsole(screen_x, screen_y);

	for (int i = 0; i < 10000; ++i) { // วนลูปแสดงผล 10,000 รอบ
		fill_data_to_buffer();
		fill_buffer_to_console();
		Sleep(50);
	}
	
		return 0;
}