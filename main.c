#include <stdio.h>  //стандартный ввод-вывод
#include <locale.h> //локализация
#include <conio.h>  //kbhit
#include <stdlib.h> //rand
#include <time.h>   //srand(time(NULL))
#include <math.h>   //pow
#include <pdcurses.h>

int main()
{

    struct timespec tw = {0, 125000000};
    struct timespec tr;
    setlocale(LC_ALL, "Rus"); // поддержка кириллицы в терминале
    system("chcp 65001");     // переключаем шрифт для отображения кириллицы

    int x = 49, y = 24;                     // scr size
                                            // printf("Введите размеры игрового поля\nДлина = ");
                                            // scanf("%d", &x);
                                            // printf("\nШирина = ");
                                            // scanf("%d", &y);
    int y_l = y / 2;                        // начальные коррдинаты левой платформы по y
    int y_r = y / 2;                        // начальные координаты правой платформы по y
    int y_b = y / 2;                        // начальные координаты шара по y
    int x_lpb = (x - 1) / 2;                // начальные координаты правой части шара по x [lpb = left piece ball]
    int x_rpb = (x + 1) / 2;                // начальные координаты правой части шара по x [rpb = right piece ball]
    int row = 0;                            // counter for building rows
    int c = 0;                              // input from keyboard
    srand(time(NULL));                      //
    int mark = 1 + rand() % (4);            // marker for way-moving ball
    int A = 0;                              // player A counter
    int B = 0;                              // player B counter
    bool rest = false;                      // restart the game
    bool cont = true;                       // cont trigger попал на стену или платформу
    initscr();                              // initialize display curses
    cbreak();                               // Включаем режим "cbreak" для немедленного получения символов
    nodelay(stdscr, TRUE);                  //
    keypad(stdscr, 1);                      // разрешаем стрелки с клавиатуры
    curs_set(0);                            // hide cursor
    noecho();                               // no input effect
    start_color();                          // запускаем цветной режим
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // режим цветов
    bkgd(COLOR_PAIR(1));                    // цвет фона, режим 1

    /*GAME START*/
    while (1)
    {

        clear(); // clear the window

        /*NEW GAME*/
        if (rest == true)
        {
            /* random generation for y*/
            int d1 = 1 + rand() % (2); // rand for y-rand
            int d2 = rand() % (2);     // rand for left or right spawn ball
            y_b = (y / 2) + pow(-1, d1) * (rand() % (3));

            // random generation for x with honest for first or second pieces
            if (d2 == 0)
            {
                x_lpb = ((x - 1) / 2) + pow(-1, d1) * (rand() % (5));
                x_rpb = x_lpb + 1;
            }
            else if (d2 == 1)
            {
                x_rpb = ((x + 1) / 2) + pow(-1, d1) * (rand() % (5));
                x_lpb = x_rpb - 1;
            }

            if (x_lpb >= ((x + 1) / 2))
            {
                mark = 1 + rand() % (2);
            }
            else if (x_rpb <= ((x - 1) / 2))
            {
                mark = 3 + rand() % (2);
            }
            rest = false;
        }

        /*GRAPHICS*/
        // create row
        while (row <= y)
        {
            mvaddch(row, ((x - 1) / 2), '|');
            mvaddch(row, ((x + 1) / 2), '|');
            row++;
        }
        row = 0;
        // create ball
        mvaddch(y_b, x_lpb, '('); // left piece
        mvaddch(y_b, x_rpb, ')'); // right piece
        // create left platform
        mvaddch(y_l - 1, 0, '|'); // top piece
        mvaddch(y_l, 0, '|');     // center piece
        mvaddch(y_l + 1, 0, '|'); // bottom piece
        // create right platform
        mvaddch(y_r - 1, x, '|'); // top piece
        mvaddch(y_r, x, '|');     // center piece
        mvaddch(y_r + 1, x, '|'); // down piece
        // players counters
        mvprintw(y / 2, x + 15, "%d", A);
        mvprintw(y / 2, x + 25, "%d", B);

        /*УСЛОВИЯ ДЛЯ ДВИЖЕНИЯ*/
        // y-change the direction of the ball's movement
        if (y_b == 0)
        { /*up right -> down right*/
            if (mark == 1)
            {
                mark = 2;
            }
            /*up left -> down left*/
            else if (mark == 3)
            {
                mark = 4;
            }
        }
        else if (y_b == y)
        { /*down right -> up right*/
            if (mark == 2)
            {
                mark = 1;
            }
            /*down left -> up left*/
            else if (mark == 4)
            {
                mark = 3;
            }
        }
        // x-change the direction of the ball's movement
        if (x_lpb == 1 && (y_b == y_l || y_b == (y_l - 1) || y_b == (y_l + 1)))
        {
            if (mark == 3)
            {
                mark = 1;
                cont = true;
            }
            else if (mark == 4)
            {
                mark = 2;
                cont = true;
            }
        }
        else if ((x - x_rpb) == 1 && (y_b == y_r || y_b == (y_r - 1) || y_b == (y_r + 1)))
        {
            if (mark == 1)
            {
                mark = 3;
                cont = true;
            }
            else if (mark == 2)
            {
                mark = 4;
                cont = true;
            }
        }
        else
        {
            cont = false;
        }

        mvprintw(y / 4, x + 15, "mark = %d", mark);
        mvprintw((y / 4) + 2, x + 15, "cont = %d", cont);
        mvprintw(y - (y / 4), x + 15, "x_rpb = %d", x_rpb);
        mvprintw((y - (y / 4)) - 2, x + 15, "x_lpb = %d", x_lpb);
        mvprintw((y - (y / 4)) + 2, x + 15, "y_b = %d", y_b);
        mvprintw((y - (y / 4)) + 4, x + 15, "y_r = %d", y_r);
        mvprintw((y - (y / 4)) + 6, x + 15, "y_r+1 = %d", y_r + 1);

        // player A counter increase
        if (cont == false)
        {
            if ((x - x_rpb) == 1 && (y_b != y_r || y_b != y_r - 1 || y_b != y_r + 1))
            {
                A++;
                rest = true;
                continue;
            }
            // player B counter increase
            else if (x_lpb == 1 && (y_b != y_l || y_b != y_l - 1 || y_b != y_l + 1))
            {
                B++;
                rest = true;
                continue;
            }
        }

        /*ЦИКЛЫ САМОГО ДВИЖЕНИЯ*/
        if (mark == 1) // up right
        {
            while (y_b > 0)
            {
                y_b--;
                x_lpb++;
                x_rpb++;
                nanosleep(&tw, &tr);
                break;
            }
        }
        else if (mark == 2) // down right
        {
            while (y_b < y)
            {
                y_b++;
                x_lpb++;
                x_rpb++;
                nanosleep(&tw, &tr);
                break;
            }
        }
        else if (mark == 3) // up left
        {
            while (y_b > 0)
            {
                y_b--;
                x_lpb--;
                x_rpb--;
                nanosleep(&tw, &tr);
                break;
            }
        }
        else if (mark == 4) // down left
        {
            while (y_b < y)
            {
                y_b++;
                x_lpb--;
                x_rpb--;
                nanosleep(&tw, &tr);
                break;
            }
        }

        c = getch();

        if (c == KEY_UP)
        {
            y_l--;
        }
        else if (c == KEY_DOWN)
        {
            y_l++;
        }

        if (c == KEY_LEFT)
        {
            y_r--;
        }
        else if (c == KEY_RIGHT)
        {
            y_r++;
        }

        if (c == 27)
        {
            break;
        }
    }
    //(c = getch()) != 27
    endwin(); // close the screen

    return 0;
}
