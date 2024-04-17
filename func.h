#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#endif
#include <stdio.h>	//стандартный ввод-вывод
#include <locale.h> //локализация
#include <conio.h>	//usleep
#include <stdlib.h> //rand
#include <unistd.h>
#include <time.h> //srand(time(NULL))
#include <math.h> //pow
#include <pdcurses.h>

void header()
{
	setlocale(LC_ALL, "Rus");
	system("chcp 65001"); // переключаем шрифт для отображения кириллицы
}

void create_row(int *row, int y, int x)
{
	while (*row <= y)
	{
		mvaddch(*row, ((x - 1) / 2), '|');
		mvaddch(*row, ((x + 1) / 2), '|');
		*row++;
	}
	*row = 0;
}