#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "_conio.h"

void window(int left, int top, int right, int bottom) {
    SMALL_RECT rect;
    rect.Left = left;
    rect.Top = top;
    rect.Right = right;
    rect.Bottom = bottom + 1;
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rect);
}
