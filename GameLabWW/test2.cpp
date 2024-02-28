#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

char cursor(int x, int y) {
    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    char buf[2]; COORD c = { x,y }; DWORD num_read;
    if (
        !ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))

        return '\0';
    else
        return buf[0];

}

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
    setcolor(5, 0);
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
    setcolor(4, 7);
    gotoxy(x, y);
    printf("^");
}

void erase_bullet(int x, int y)
{
    setcolor(0, 0);
    gotoxy(x, y);
    printf(" ");
}

void draw_star(int x, int y)
{
    setcolor(6, 0);
    gotoxy(x, y);
    printf("*");
}

void scoreboard(int x, int y, int score)
{
    setcolor(7, 4);
    gotoxy(70, 0);
    printf("SCORE : %d", score);
}
int main()
{
    setcursor(false);
    int score = 0;
    int x = 38, y = 20;
    int weight = 80 - 7;
    char ch = ' ', direction = ' ', status = ' ';
    int bullets[5] = { 0 }, bulletX[5] = { 0 }, bulletY[5] = { 0 };
    draw_ship(x, y);

    scoreboard(70, 0, score);

    srand(time(NULL));
    for (int j = 1; j <= 20; j++)
    {
        int starX = rand() % (70 - 10 + 1) + 10;
        int starY = rand() % (5 - 2 + 1) + 2;
        draw_star(starX, starY);
    }

    do
    {
        if (_kbhit())
        {
            ch = _getch();
            if (ch == 'a') { direction = ch; }
            if (ch == 'd') { direction = ch; }
            if (ch == 's') { direction = ch; }
            if (status != 1 && ch == ' ') {
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
        if (direction == 'd' && x < weight) { erase_ship(x, y); draw_ship(++x, y); }

        for (int i = 0; i < 5; i++) {
            if (bullets[i] == 1) {
                erase_bullet(bulletX[i], bulletY[i]);
                if (bulletY[i] > 0) {
                    draw_bullet(bulletX[i], --bulletY[i]);
                    if (cursor(bulletX[i], bulletY[i] - 1) == '*')
                    {
                        Beep(3000, 45);
                        score++;
                        scoreboard(70, 0, score);
                        int starX = rand() % (70 - 10 + 1) + 10;
                        int starY = rand() % (5 - 2 + 1) + 2;
                        draw_star(starX, starY);
                    }
                }
                else {
                    bullets[i] = 0;
                }
            }
        }

        Sleep(100);
    } while (ch != 'x');

    return 0;
}
