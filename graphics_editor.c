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

typedef enum { CIRCLE, RECTANGLE, LINE, TRIANGLE } ShapeType;

typedef struct {
    ShapeType type;
    int x1, y1, x2, y2;
    int radius;
    int active;
} Object;

Object objects[MAX_OBJECTS];
int obj_count = 0;

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

void add_object(void) {
    if (obj_count >= MAX_OBJECTS) {
        printf("Canvas full!\n");
        return;
    }
    Object *o = &objects[obj_count];
    o->active = 1;
    int choice;
    printf("\nShape: 1=Circle 2=Rectangle 3=Line 4=Triangle: ");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            o->type = CIRCLE;
            printf("Enter centre row, col and radius: ");
            scanf("%d %d %d", &o->x1, &o->y1, &o->radius);
            break;
        case 2:
            o->type = RECTANGLE;
            printf("Enter top-left row col, bottom-right row col: ");
            scanf("%d %d %d %d", &o->x1, &o->y1, &o->x2, &o->y2);
            break;
        case 3:
            o->type = LINE;
            printf("Enter start row col, end row col: ");
            scanf("%d %d %d %d", &o->x1, &o->y1, &o->x2, &o->y2);
            break;
        case 4:
            o->type = TRIANGLE;
            printf("Enter vertex1 row col: ");
            scanf("%d %d", &o->x1, &o->y1);
            printf("Enter vertex2 row col: ");
            scanf("%d %d", &o->x2, &o->y2);
            printf("Enter vertex3 row col: ");
            scanf("%d %d", &o->radius, &o->x2);
            break;
        default:
            printf("Invalid choice!\n");
            return;
    }
    obj_count++;
    printf("Object %d added successfully!\n", obj_count);
}

void delete_object(void) {
    if (obj_count == 0) {
        printf("No objects to delete!\n");
        return;
    }
    int id;
    printf("Enter object ID to delete (1-%d): ", obj_count);
    scanf("%d", &id);
    if (id < 1 || id > obj_count) {
        printf("Invalid ID!\n");
        return;
    }
    objects[id - 1].active = 0;
    printf("Object %d deleted!\n", id);
}

/* NEW Day 5: Redraw all active objects */
void redraw_all(void) {
    clear_canvas();
    for (int i = 0; i < obj_count; i++) {
        if (!objects[i].active) continue;
        Object *o = &objects[i];
        switch (o->type) {
            case CIRCLE:
                draw_circle(o->x1, o->y1, o->radius);
                break;
            case RECTANGLE:
                draw_rectangle(o->x1, o->y1, o->x2, o->y2);
                break;
            case LINE:
                draw_line(o->x1, o->y1, o->x2, o->y2);
                break;
            case TRIANGLE:
                draw_triangle(o->x1, o->y1, o->x2, o->y2,
                              o->radius, o->x2 + 5);
                break;
        }
    }
}

/* NEW Day 5: List all objects */
void list_objects(void) {
    if (obj_count == 0) {
        printf("No objects yet!\n");
        return;
    }
    const char *names[] = {"Circle","Rectangle","Line","Triangle"};
    printf("\nID  Shape      Active\n");
    printf("--- ---------- ------\n");
    for (int i = 0; i < obj_count; i++)
        printf("%-3d %-10s %s\n", i + 1,
               names[objects[i].type],
               objects[i].active ? "Yes" : "No");
}

int main(void) {
    clear_canvas();

    /* Test: add a rectangle */
    printf("=== Add Object ===\n");
    add_object();

    /* Test: add a circle */
    printf("=== Add Another Object ===\n");
    add_object();

    /* Test: list objects */
    printf("\n=== List Objects ===\n");
    list_objects();

    /* Test: redraw all */
    printf("\n=== Redraw All ===\n");
    redraw_all();
    display_canvas();

    return 0;
}