#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

#define screen_x 80
#define screen_y 25
#define scount 20 //จำนวนดาว

HANDLE rHnd;
HANDLE wHnd;
DWORD fdwMode;
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD characterPos = { 0, 0 };
COORD bufferSize = { screen_x, screen_y };
SMALL_RECT windowSize = { 0, 0, screen_x - 1, screen_y - 1 };
COORD ship = { 0, 0 };
COORD star[scount];
int life = 10; //กำหนดชีวิตยาน


int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}

int setMode()
{
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT |
		ENABLE_MOUSE_INPUT;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}

char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))

		return '\0';
	else
		return buf[0];

}

void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

//**
void draw_ship_to_buffer(int color)
{
	consoleBuffer[ship.X - 2 + screen_x * ship.Y].Char.AsciiChar = '<';
	consoleBuffer[ship.X - 2 + screen_x * ship.Y].Attributes = color;

	consoleBuffer[ship.X - 1 + screen_x * ship.Y].Char.AsciiChar = '-';
	consoleBuffer[ship.X - 1 + screen_x * ship.Y].Attributes = color;

	consoleBuffer[ship.X + screen_x * ship.Y].Char.AsciiChar = '0';
	consoleBuffer[ship.X + screen_x * ship.Y].Attributes = color;

	consoleBuffer[ship.X + 1 + screen_x * ship.Y].Char.AsciiChar = '-';
	consoleBuffer[ship.X + 1 + screen_x * ship.Y].Attributes = color;

	consoleBuffer[ship.X + 2 + screen_x * ship.Y].Char.AsciiChar = '>';
	consoleBuffer[ship.X + 2 + screen_x * ship.Y].Attributes = color;

}

void fill_buffer_to_console()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos,
		&windowSize);
}

//**
void clear_buffer()
{
	for (int y = 0; y < screen_y; y++) {
		for (int x = 0; x < screen_x; x++) {
			consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
			consoleBuffer[x + screen_x * y].Attributes = 0;
		}
	}
}

//**
void init_star()
{
	for (int i = 0; i < scount; i++)
	{
		star[i].X = rand() % 80;
		star[i].Y = rand() % 25;
	}
}

//**
void fill_star_to_buffer()
{
	for (int i = 0; i < screen_x; i++) {
		if (star[i].X != 0 && star[i].Y != 0)
			
			consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = '*';
			consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 6 ;
	}
}

void star_fall()
 {
	int i;
	for (i = 0; i < scount; i++) {
		if (star[i].Y >= screen_y - 1) {
			star[i].X = rand() % screen_x ; // สุ่มตำแหน่งใหม่ในแนวนอน X
			star[i].Y = 1; // เริ่มต้นเคลื่อนที่ในแนวตั้ง Y ใหม่

		}
		else {
			star[i] = { star[i].X, star[i].Y + 1 };
		}
	}
}

//**
void checkstartcollision(int x, int y) //เช็คการชนของยานแล้วลบค่าชีวิต
{
	for (int i = 0; i < scount; i++)
	{
		if (star[i].X - 0  <= x && x < star[i].X + 2 && star[i].Y == y)
		{
			life--;
			star[i].X = rand() % 80;
			star[i].Y = rand() % 25;
		}
	}
}

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}


int main()
{
	setcursor(0);
	int shipcolor = 5;
	bool play = true;
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	srand(time(NULL));
	setConsole(screen_x, screen_y);
	setMode();
	init_star();


	while (play)
	{
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead; ++i) {
				if (eventBuffer[i].EventType == KEY_EVENT &&
					eventBuffer[i].Event.KeyEvent.bKeyDown == true) { //ตรวจสอบการกด keyborad
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) { //ปุ่ม esc
						play = false;
					}
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == 'C') { // ใช้ C ตัวใหญ่ ใช้ ตัวเล้กไม่ได้ ง๊งงง
						shipcolor = rand() % 256;
					}

				}
				else if (eventBuffer[i].EventType == MOUSE_EVENT) {
					ship.X = eventBuffer[i].Event.MouseEvent.dwMousePosition.X; //พิกัดแกน x ของเม้า
					ship.Y = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y; //พิกัดแกน y ของเม้า
					if (eventBuffer[i].Event.MouseEvent.dwButtonState & //สถานะการกดปุ่มเม้า
						FROM_LEFT_1ST_BUTTON_PRESSED) { //กดซ้าย
						shipcolor = rand() % 256;
					}	
				}
			}
			delete[] eventBuffer;
		}

		star_fall();
		clear_buffer();
		checkstartcollision(ship.X, ship.Y);
		if (life == 0)
		{
			play = false;
		}

		fill_star_to_buffer();
		draw_ship_to_buffer(shipcolor);

		fill_buffer_to_console();

		gotoxy(0, 0);
		printf("Life : %d", life);

		Sleep(100);
	}
	return 0;
}