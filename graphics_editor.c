#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 80
#define HEIGHT 24
#define EMPTY '_'
#define PIXEL '*'
#define MAX_OBJECTS 100

char picture[HEIGHT][WIDTH];

/* Shape types */
#define SHAPE_LINE      1
#define SHAPE_RECTANGLE 2
#define SHAPE_CIRCLE    3
#define SHAPE_TRIANGLE  4

typedef struct {
    int type;
    int params[6]; /* x1 y1 x2 y2 x3 y3 — used depending on type */
} Object;

Object objects[MAX_OBJECTS];
int objectCount = 0;

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
        if (e2 < dx)  { err += dx; y1 += sy; }
    }
}

/* Draw rectangle using 4 lines */
void drawRectangle(int x1, int y1, int x2, int y2) {
    drawLine(x1, y1, x2, y1);
    drawLine(x1, y2, x2, y2);
    drawLine(x1, y1, x1, y2);
    drawLine(x2, y1, x2, y2);
}

/* Draw circle outline */
void drawCircle(int cx, int cy, int radius) {
    for (int px = -radius; px <= radius; px++) {
        int py_f = (int)round(sqrt((double)(radius*radius - px*px)));
        setPixel(cx + px, cy + py_f);
        setPixel(cx + px, cy - py_f);
    }
    for (int py = -radius; py <= radius; py++) {
        int px_f = (int)round(sqrt((double)(radius*radius - py*py)));
        setPixel(cx + px_f, cy + py);
        setPixel(cx - px_f, cy + py);
    }
}

/* Draw triangle using 3 lines */
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

/* Redraw canvas from all stored objects */
void redrawAll() {
    clearPicture();
    for (int i = 0; i < objectCount; i++) {
        Object *o = &objects[i];
        if (o->type == SHAPE_LINE)
            drawLine(o->params[0], o->params[1], o->params[2], o->params[3]);
        else if (o->type == SHAPE_RECTANGLE)
            drawRectangle(o->params[0], o->params[1], o->params[2], o->params[3]);
        else if (o->type == SHAPE_CIRCLE)
            drawCircle(o->params[0], o->params[1], o->params[2]);
        else if (o->type == SHAPE_TRIANGLE)
            drawTriangle(o->params[0], o->params[1], o->params[2], o->params[3], o->params[4], o->params[5]);
    }
}

void printMenu() {
    printf("\n2D Graphics Editor\n");
    printf("Canvas size: %d x %d\n", WIDTH, HEIGHT);
    printf("1. Add object\n");
    printf("2. Delete object\n");
    printf("3. Modify object\n");
    printf("4. Display picture\n");
    printf("5. List objects\n");
    printf("0. Exit\n");
    printf("Enter choice: ");
}

int main() {
    clearPicture();

    int choice;

    while (1) {
        printMenu();
        scanf("%d", &choice);

        if (choice == 1) {
            /* Add object */
            int shapeType;
            printf("\nChoose shape type:\n");
            printf("1. Line\n");
            printf("2. Rectangle\n");
            printf("3. Circle\n");
            printf("4. Triangle\n");
            printf("Enter shape type: ");
            scanf("%d", &shapeType);

            Object o;
            o.type = shapeType;

            if (shapeType == SHAPE_LINE) {
                printf("Enter x1 y1 x2 y2: ");
                scanf("%d %d %d %d", &o.params[0], &o.params[1], &o.params[2], &o.params[3]);
            } else if (shapeType == SHAPE_RECTANGLE) {
                printf("Enter top-left x y and bottom-right x y: ");
                scanf("%d %d %d %d", &o.params[0], &o.params[1], &o.params[2], &o.params[3]);
            } else if (shapeType == SHAPE_CIRCLE) {
                printf("Enter center x y and radius: ");
                scanf("%d %d %d", &o.params[0], &o.params[1], &o.params[2]);
            } else if (shapeType == SHAPE_TRIANGLE) {
                printf("Enter x1 y1 x2 y2 x3 y3: ");
                scanf("%d %d %d %d %d %d",
                      &o.params[0], &o.params[1], &o.params[2],
                      &o.params[3], &o.params[4], &o.params[5]);
            } else {
                printf("Invalid shape type.\n");
                continue;
            }

            objects[objectCount] = o;
            printf("Object added with index %d.\n", objectCount);
            objectCount++;
            redrawAll();
        }

        else if (choice == 2) {
            /* Delete object */
            int idx;
            printf("Enter index to delete: ");
            scanf("%d", &idx);
            if (idx < 0 || idx >= objectCount) {
                printf("Invalid index.\n");
            } else {
                for (int i = idx; i < objectCount - 1; i++)
                    objects[i] = objects[i + 1];
                objectCount--;
                redrawAll();
                printf("Object deleted.\n");
            }
        }

        else if (choice == 3) {
            /* Modify object */
            int idx;
            printf("Enter index to modify: ");
            scanf("%d", &idx);
            if (idx < 0 || idx >= objectCount) {
                printf("Invalid index.\n");
            } else {
                int shapeType;
                printf("Choose shape type:\n");
                printf("1. Line\n");
                printf("2. Rectangle\n");
                printf("3. Circle\n");
                printf("4. Triangle\n");
                printf("Enter shape type: ");
                scanf("%d", &shapeType);

                Object o;
                o.type = shapeType;

                if (shapeType == SHAPE_LINE) {
                    printf("Enter x1 y1 x2 y2: ");
                    scanf("%d %d %d %d", &o.params[0], &o.params[1], &o.params[2], &o.params[3]);
                } else if (shapeType == SHAPE_RECTANGLE) {
                    printf("Enter top-left x y and bottom-right x y: ");
                    scanf("%d %d %d %d", &o.params[0], &o.params[1], &o.params[2], &o.params[3]);
                } else if (shapeType == SHAPE_CIRCLE) {
                    printf("Enter center x y and radius: ");
                    scanf("%d %d %d", &o.params[0], &o.params[1], &o.params[2]);
                } else if (shapeType == SHAPE_TRIANGLE) {
                    printf("Enter x1 y1 x2 y2 x3 y3: ");
                    scanf("%d %d %d %d %d %d",
                          &o.params[0], &o.params[1], &o.params[2],
                          &o.params[3], &o.params[4], &o.params[5]);
                } else {
                    printf("Invalid shape type.\n");
                    continue;
                }

                objects[idx] = o;
                redrawAll();
                printf("Object modified.\n");
            }
        }

        else if (choice == 4) {
            /* Display picture */
            printf("\n");
            displayPicture();
        }

        else if (choice == 5) {
            /* List objects */
            if (objectCount == 0) {
                printf("No objects.\n");
            } else {
                for (int i = 0; i < objectCount; i++) {
                    Object *o = &objects[i];
                    printf("Index %d: ", i);
                    if (o->type == SHAPE_LINE)
                        printf("Line (%d,%d) to (%d,%d)\n", o->params[0], o->params[1], o->params[2], o->params[3]);
                    else if (o->type == SHAPE_RECTANGLE)
                        printf("Rectangle (%d,%d) to (%d,%d)\n", o->params[0], o->params[1], o->params[2], o->params[3]);
                    else if (o->type == SHAPE_CIRCLE)
                        printf("Circle center (%d,%d) radius %d\n", o->params[0], o->params[1], o->params[2]);
                    else if (o->type == SHAPE_TRIANGLE)
                        printf("Triangle (%d,%d) (%d,%d) (%d,%d)\n",
                               o->params[0], o->params[1], o->params[2],
                               o->params[3], o->params[4], o->params[5]);
                }
            }
        }

        else if (choice == 0) {
            printf("Goodbye.\n");
            break;
        }

        else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}