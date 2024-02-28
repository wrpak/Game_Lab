#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include<time.h>

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

void draw_star(int x, int y)
{
    setcolor(6, 0);
    gotoxy(x, y);
    printf("*");
}

void erase_star(int x, int y)
{
    setcolor(0, 0);
    gotoxy(x, y);
    printf(" ");
}

void draw_score(int score)
{
    setcolor(7, 0);
    gotoxy(70, 0);
    printf("score: %d" ,score);
}

void erase_score()
{
    setcolor(0, 0);
    gotoxy(77, 0);
    printf("         ");
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


int main()
{
    setcursor(0);
    int x = 38, y = 20, score = 0;
    char ch = ' ', direction = ' ', shot = ' ';
    int bullets[5] = { 0 }; // อาเรย์เก็บสถานะกระสุน
    int bulletX[5] = { 0 }, bulletY[5] = { 0 }; // อาเรย์เก็บพิกัดของกระสุน
    int starX, starY, starN = 0; // พิกัดดาว
    srand(time(NULL));
    draw_ship(x, y);

    do
    {
        draw_score(score);
        if (_kbhit())
        {
            ch = _getch();
            if (ch == 'a') { direction = ch; }
            if (ch == 'd') { direction = ch; }
            if (ch == 's') { direction = ch; }
            if (shot != 1 && ch == ' ') 
            {
                for (int i = 0; i < 5; i++) 
                {
                    if (bullets[i] == 0) {
                        bullets[i] = 1;
                        bulletX[i] = x + 3;
                        bulletY[i] = y - 1;
                        break;
                    }
                }
                fflush(stdin);
            }
        }

            // ship
            if (direction == 'a' && x > 0) { erase_ship(x, y); draw_ship(--x, y); }
            if (direction == 'd' && x < 73) { erase_ship(x, y); draw_ship(++x, y); }

            // bullet
            for (int i = 0; i < 5; i++)
            {
                if (bullets[i] == 1)
                {
                    erase_bullet(bulletX[i], bulletY[i]);
                    if (bulletY[i] > 0) { draw_bullet(bulletX[i], --bulletY[i]); }
                    else { bullets[i] = 0; }
                }
            }

            // star
            if (starN < 20) 
            {
                starX = (rand() % 61) + 10; //เว้นขอบข้าง 10
                starY = (rand() % 4) + 2; //เว้นขอบบน 2
                draw_star(starX, starY);
                starN += 1; // เพื่มดาวใหม่
            }

            for (int i = 0; i < 5; i++)
            {
                if (cursor(bulletX[i], bulletY[i] - 1 ) == '*') // ตรวจการชน
                {
                    erase_star(bulletX[i], bulletY[i] - 1); 
                    erase_bullet(bulletX[i], bulletY[i]);
                    Beep(800, 50);
                    bullets[i] = 0;
                    score += 10;
                    starN -= 1;
                }
                
            }
     
            Sleep(50);

        } while (ch != 'x');

        return 0;
    }

    
