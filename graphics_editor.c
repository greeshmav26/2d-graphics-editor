#include <stdio.h>
#include <string.h>
#include <math.h>

#define ROWS 25
#define COLS 80
#define EMPTY ' '
#define BORDER '*'
#define FILL  '_'
#define MAX_OBJECTS 20

char canvas[ROWS][COLS];

void clear_canvas(void) {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            canvas[r][c] = EMPTY;
}

void plot(int r, int c, char ch) {
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
        canvas[r][c] = ch;
}

void display_canvas(void) {
    printf("\n");
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++)
            putchar(canvas[r][c]);
        putchar('\n');
    }
}

int main(void) {
    clear_canvas();
    display_canvas();
    return 0;
}