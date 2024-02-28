//66010737_วรพร_Lab6

#include<stdio.h>
#include<windows.h>
#include<conio.h>

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void setcursor(boolean visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

void draw_ship(int x, int y)
{
	setcolor(2, 4);
	gotoxy(x, y);
	printf(" <-0-> ");
}

void erase_ship(int x, int y)
{
	setcolor(0, 0);
	gotoxy(x, y);
	printf("       ");
}

void draw_bullet(int x, int y)
{
	setcolor(3, 0);
	gotoxy(x, y);
	printf("*");
}

void erase_bullet(int x, int y)
{
	setcolor(0, 0);
	gotoxy(x, y);
	printf(" ");
}

int main()
{
	setcursor(0);
	int x = 38, y = 20;
	char ch = ' ', direction = ' ';
	int bx, by, i;
	int bullet = 0;
	draw_ship(x, y);
	draw_bullet(x, y);
	do {
		if (_kbhit()) {
			ch = _getch();
			if (ch == 'a') { direction = ch; }
			if (ch == 'd') { direction = ch; }
			if (ch == 's') { direction = ch; }
			if (bullet != 1 && ch == ' ') { bullet = 1; bx = x + 3; by = y - 1; }
			fflush(stdin);
			if (ch == 'w') { direction = ch; }
		}

		if (direction == 'a' && x > 0) { erase_ship(x, y); draw_ship(--x, y); }
		if (direction == 'd' && x < 73) { erase_ship(x, y); draw_ship(++x, y); }
		if (direction == 's' && y < 25 - 1) { erase_ship(x, y); draw_ship(x, ++y); }
		if (direction == 'w' && y > 0) { erase_ship(x, y); draw_ship(x, --y); }
		if (bullet == 1) {
			erase_bullet(bx, by);
			if (by <= 0) { bullet = 0; }
			else { draw_bullet(bx, --by); }
		}
		Sleep(100);
	} while (ch != 'x');
	return 0;
}




