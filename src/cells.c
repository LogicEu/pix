#include <pix.h>
#include <stdlib.h>
#include <stdio.h>

static int** cellptr;
static int w, h;

typedef enum {
    CELL_NULL,
    CELL_BLOCK,
} cellEnum;

void cells_size(int* width, int* height)
{
    *width = w;
    *height = h;
}

int** cells_get()
{
    return cellptr;
}

void cells_init(const int width, const int height)
{
    w = width;
    h = height;

    cellptr = malloc(sizeof(int*) * height);
    for (int y = 0; y < height; y++) {
        cellptr[y] = malloc(sizeof(int) * width);
        for (int x = 0; x < width; x++) {
            if (rand_next() % 1000 > 900) cellptr[y][x] = CELL_BLOCK;
            else cellptr[y][x] = CELL_NULL;
        }
    }
}

static inline void cells_sand_chaos()
{
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (cellptr[y][x] == CELL_BLOCK && y > 0) {
                if (cellptr[y - 1][x] == CELL_NULL) {
                    cellptr[y][x] = CELL_NULL;
                    cellptr[y - 1][x] = CELL_BLOCK;
                } 
                else if (x + 1 < w && cellptr[y - 1][x + 1] == CELL_NULL) {
                    cellptr[y][x] = CELL_NULL;
                    cellptr[y - 1][x + 1] = CELL_BLOCK;
                }
                else if (x > 0 && cellptr[y - 1][x - 1] == CELL_NULL) {
                    cellptr[y][x] = CELL_NULL;
                    cellptr[y - 1][x - 1] = CELL_BLOCK;
                }
            }
        }
    }
}

static inline void cells_sand()
{
    int buffer[h][w];

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            buffer[y][x] = cellptr[y][x];
            
            if (cellptr[y][x] == CELL_BLOCK && y > 0) {
                if (cellptr[y - 1][x] == CELL_NULL) {
                    buffer[y][x] = CELL_NULL;
                    buffer[y - 1][x] = CELL_BLOCK;
                } 
                else if (x + 1 < w && cellptr[y - 1][x + 1] == CELL_NULL) {
                    buffer[y][x] = CELL_NULL;
                    buffer[y - 1][x + 1] = CELL_BLOCK;
                }
                else if (x > 0 && cellptr[y - 1][x - 1] == CELL_NULL) {
                    buffer[y][x] = CELL_NULL;
                    buffer[y - 1][x - 1] = CELL_BLOCK;
                }
            }
        }
    }

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            cellptr[y][x] = buffer[y][x];
        }
    }
}

static inline void cells_automata()
{
    int buffer[h][w];

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            int count = 0;
            if (x + 1 < w) count += cellptr[y][x + 1];
            if (x > 0) count += cellptr[y][x - 1];
            if (y + 1 < h) count += cellptr[y + 1][x];
            if (y > 0) count += cellptr[y - 1][x];
            if (x + 1 < w && y + 1 < h) count += cellptr[y + 1][x + 1];
            if (x > 0 && y > 0) count += cellptr[y - 1][x - 1];
            if (x + 1 < w && y > 0) count += cellptr[y - 1][x + 1];
            if (x > 0 && y + 1 < h) count += cellptr[y + 1][x - 1];

            int val;
            if (cellptr[y][x]) {
                if (count < 2 || count > 3) {
                    val = CELL_NULL;
                } else val = CELL_BLOCK;
            } else {
                if (count == 3) {
                    val = CELL_BLOCK;
                } else val = CELL_NULL;
            }

            buffer[y][x] = val;
        }
    }

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            cellptr[y][x] = buffer[y][x];
        }
    }
}

static inline void cells_automata_chaos()
{
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int count = 0;
            if (x + 1 < w) count += cellptr[y][x + 1];
            if (x > 0) count += cellptr[y][x - 1];
            if (y + 1 < h) count += cellptr[y + 1][x];
            if (y > 0) count += cellptr[y - 1][x];
            if (x + 1 < w && y + 1 < h) count += cellptr[y + 1][x + 1];
            if (x > 0 && y > 0) count += cellptr[y - 1][x - 1];
            if (x + 1 < w && y > 0) count += cellptr[y - 1][x + 1];
            if (x > 0 && y + 1 < h) count += cellptr[y + 1][x - 1];

            if (cellptr[y][x]) {
                if (count < 2 || count > 3) {
                    cellptr[y][x] = CELL_NULL;
                }
            } else if (count == 3) {
                cellptr[y][x] = CELL_BLOCK;
            }
        }
    }
}

void cells_nothing()
{

}

void cells_update()
{
    static vec2 mouse;
    glee_mouse_pos(&mouse.x, &mouse.y);
    mouse = vec2_mult(mouse, iScale);

    if (glee_mouse_down(GLFW_MOUSE_BUTTON_LEFT)) {
        cellptr[(int)mouse.y % h][(int)mouse.x % w] = CELL_BLOCK;
    }

    static void (*func_ptr)() = &cells_sand;

    if (glee_key_pressed(GLFW_KEY_0)) {
        printf("Func nothing\n");
        func_ptr = &cells_nothing;
    }
    if (glee_key_pressed(GLFW_KEY_1)) {
        printf("Func sand\n");
        func_ptr = &cells_sand;
    }
    if (glee_key_pressed(GLFW_KEY_2)) {
        printf("Func sand chaos\n");
        func_ptr = &cells_sand_chaos;
    }
    if (glee_key_pressed(GLFW_KEY_3)) {
        printf("Func automata\n");
        func_ptr = &cells_automata;
    }
    if (glee_key_pressed(GLFW_KEY_4)) {
        printf("Func automata chaos\n");
        func_ptr = &cells_automata_chaos;
    }

    func_ptr();
}

void cells_deinit()
{
    for (int y = 0; y < h; y++) {
        free(cellptr[y]);
    }
    free(cellptr);
}