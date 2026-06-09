#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH  80
#define HEIGHT 24
#define EMPTY  '_'
#define PIXEL  '*'

char picture[HEIGHT][WIDTH];

/* Fill entire canvas with '_' */
void clearPicture() {
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            picture[y][x] = EMPTY;
}

/* Print canvas row by row */
void displayPicture() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++)
            putchar(picture[y][x]);
        putchar('\n');
    }
}

/* Set one pixel if inside canvas */
void setPixel(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        picture[y][x] = PIXEL;
}

/* Draw line using Bresenham's algorithm */
void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        setPixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
}

/* Draw rectangle using 4 lines */
void drawRectangle(int x1, int y1, int x2, int y2) {
    drawLine(x1, y1, x2, y1); /* top edge    */
    drawLine(x1, y2, x2, y2); /* bottom edge */
    drawLine(x1, y1, x1, y2); /* left edge   */
    drawLine(x2, y1, x2, y2); /* right edge  */
}

/* Draw circle outline */
void drawCircle(int cx, int cy, int radius) {
    for (int deg = 0; deg < 360; deg++) {
        double rad = deg * 3.14159265 / 180.0;
        int x = cx + (int)(radius * cos(rad));
        int y = cy + (int)(radius * sin(rad) * 0.5);
        setPixel(x, y);
    }
}

/* Draw triangle using 3 lines */
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

int main() {
    int choice;
    clearPicture();

    printf("2D Graphics Editor\n");
    printf("Canvas size: %d x %d\n", WIDTH, HEIGHT);
    printf("Use coordinates x y.\n");
    printf("x range: 0 to %d\n", WIDTH - 1);
    printf("y range: 0 to %d\n", HEIGHT - 1);

    while (1) {
        printf("\nMenu\n");
        printf("1. Draw Line\n");
        printf("2. Draw Rectangle\n");
        printf("3. Draw Circle\n");
        printf("4. Draw Triangle\n");
        printf("5. Display Picture\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int x1, y1, x2, y2;
            printf("Enter x1 y1 x2 y2: ");
            scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
            drawLine(x1, y1, x2, y2);
        }
        else if (choice == 2) {
            int x1, y1, x2, y2;
            printf("Enter top-left x y and bottom-right x y: ");
            scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
            drawRectangle(x1, y1, x2, y2);
        }
        else if (choice == 3) {
            int cx, cy, radius;
            printf("Enter center x y and radius: ");
            scanf("%d %d %d", &cx, &cy, &radius);
            drawCircle(cx, cy, radius);
        }
        else if (choice == 4) {
            int x1, y1, x2, y2, x3, y3;
            printf("Enter x1 y1 x2 y2 x3 y3: ");
            scanf("%d %d %d %d %d %d",
                  &x1, &y1, &x2, &y2, &x3, &y3);
            drawTriangle(x1, y1, x2, y2, x3, y3);
        }
        else if (choice == 5) {
            printf("The picture is:\n");
            displayPicture();
        }
        else if (choice == 0) {
            printf("Exiting program.\n");
            break;
        }
        else {
            printf("Invalid choice.\n");
        }
    }
    return 0;
}