#include<stdio.h>
#include<windows.h>
#include<conio.h>

void gotoxy(int x, int y);
void draw_ship(int x, int y);
void erase_ship(int x, int y);
void setcursor(int visable);
void setcolor(int fg, int bg);
void draw_bullet(int round);
void erase_bullet(int round);

struct Bullet {
    int active;
    int x, y;
}
Bullet bullet[5];

int main() {
    char ch = '\0';
    char before = ' ';
    int x = 38, y = 20;
    int bullet_stat = 0;
    setcursor(0);
    draw_ship(x, y);
    do {
        if (_kbhit()) {
            ch = _getch();
            fflush(stdin);
            if (ch == ' ') {
                for (int i = 0; i < 5; i++)
                    if (bullet[i].active == 0) {
                        bullet[i].active = 1;
                        bullet[i].x = x + 3;
                        bullet[i].y = 20;
                        break;
                    }
            }
            if (ch == 'a')
                before = 'a';
            else if (ch == 'd')
                before = 'd';
            else if (ch == 's')
                before = 's';
        }

        if (ch != 's') {
            //erase zone
            for (int i = 0; i < 5; i++)
                if (bullet[i].active == 1)
                    erase_bullet(i);
            erase_ship(x, y);

            if ((ch == 'a' || before == 'a') && x > 0)
                x--;
            else if ((ch == 'd' || before == 'd') && x < 80)
                x++;

            //draw zone
            for (int i = 0; i < 5; i++)
                if (bullet[i].active == 1)
                    draw_bullet(i);
            draw_ship(x, y);

        }
        Sleep(10);
    } while (ch != 'x');

    return 0;
}


void gotoxy(int x, int y) {
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void draw_ship(int x, int y) {
    gotoxy(x, y);
    setcolor(2, 4);
    printf(" <-0-> ");
}

void erase_ship(int x, int y) {
    gotoxy(x, y);
    setcolor(0, 0);
    printf("       ");
}

void setcursor(int visible)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = 20;
    SetConsoleCursorInfo(console, &lpCursor);
}

void setcolor(int fg, int bg) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

void draw_bullet(int round) {
    if (bullet[round].active == 0)
        return;
    int x = bullet[round].x;
    int y = bullet[round].y;
    gotoxy(x, y);
    setcolor(7, 7);
    printf(" ");
}

void erase_bullet(int round) {
    int x = bullet[round].x;
    int y = bullet[round].y;
    gotoxy(x, y);
    setcolor(0, 0);
    printf(" ");
    if (y == 0) {
        bullet[round].active = 0;
    }
    else bullet[round].y--;
}