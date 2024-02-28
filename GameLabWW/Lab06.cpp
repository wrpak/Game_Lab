#include <stdio.h>
#include <windows.h>
#include <conio.h>

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
    setcolor(3, 3);
    gotoxy(x, y);
    printf(" ");
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
    char ch = ' ', direction = ' ', shot = ' ';
    int bullets[5] = { 0 }; // อาเรย์เก็บสถานะกระสุน
    int bulletX[5] = { 0 }, bulletY[5] = { 0 }; // อาเรย์เก็บพิกัดของกระสุน
    draw_ship(x, y);

    do
    {
        if (_kbhit())
        {
            ch = _getch();
            if (ch == 'a') { direction = ch; }
            if (ch == 'd') { direction = ch; }
            if (ch == 's') { direction = ch; }
            if (shot != 1 && ch == ' ') {
                for (int i = 0; i < 5; i++) {
                    if (bullets[i] == 0) {
                        bullets[i] = 1;
                        bulletX[i] = x + 3;
                        bulletY[i] = y - 1;
                        break;
                    }
                }
            }
            fflush(stdin);

        }
        if (direction == 'a' && x > 0) { erase_ship(x, y); draw_ship(--x, y); }
        if (direction == 'd' && x < 73) { erase_ship(x, y); draw_ship(++x, y); }

        for (int i = 0; i < 5; i++) {
            if (bullets[i] == 1) {
                erase_bullet(bulletX[i], bulletY[i]);
                if (bulletY[i] > 0) {
                    draw_bullet(bulletX[i], --bulletY[i]);
                }
                else {
                    bullets[i] = 0;
                }
            }
        }

        Sleep(20);
    } while (ch != 'x');

    return 0;
}
