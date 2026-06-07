#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define ROWS 22
#define COLS 60
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

void draw_circle(int cx, int cy, int r) {
    for (int deg = 0; deg < 360; deg++) {
        double rad = deg * 3.14159265 / 180.0;
        int row = cx + (int)(r * sin(rad));
        int col = cy + (int)(r * cos(rad) * 2);
        plot(row, col, BORDER);
    }
}

void draw_rectangle(int r1, int c1, int r2, int c2) {
    for (int c = c1; c <= c2; c++) {
        plot(r1, c, BORDER);
        plot(r2, c, BORDER);
    }
    for (int r = r1; r <= r2; r++) {
        plot(r, c1, BORDER);
        plot(r, c2, BORDER);
    }
}

void draw_line(int r1, int c1, int r2, int c2) {
    int dr = abs(r2 - r1);
    int dc = abs(c2 - c1);
    int sr = (r1 < r2) ? 1 : -1;
    int sc = (c1 < c2) ? 1 : -1;
    int err = dr - dc;
    while (1) {
        plot(r1, c1, FILL);
        if (r1 == r2 && c1 == c2) break;
        int e2 = 2 * err;
        if (e2 > -dc) { err -= dc; r1 += sr; }
        if (e2 <  dr) { err += dr; c1 += sc; }
    }
}

void draw_triangle(int r1, int c1, int r2, int c2, int r3, int c3) {
    draw_line(r1, c1, r2, c2);
    draw_line(r2, c2, r3, c3);
    draw_line(r3, c3, r1, c1);
}

int main(void) {
    clear_canvas();

    /* Rectangle — top left, no overlap */
    draw_rectangle(1, 1, 8, 15);

    /* Triangle — top middle, no overlap */
    draw_triangle(1, 20, 8, 17, 8, 28);

    /* Line — diagonal, right side only */
    draw_line(1, 35, 10, 55);

    /* Circle — bottom middle */
    draw_circle(17, 30, 4);

    display_canvas();
    return 0;
}