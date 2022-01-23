#ifndef PIX_H
#define PIX_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************
        PIX
******************/

#include <glee.h>
#include <photon.h>
#include <utopia.h>

extern vec2 iResolution;
extern float iTime, iScale, iDeltaTime;

vec2 world_to_screen(const vec2 p);
vec2 screen_to_world(const vec2 p);

unsigned int screen_init();
void screen_update();
void screen_deinit();
void screen_draw();
void screen_pixel_color_set(const int x, const int y, const vec4 color);
vec4 screen_pixel_color_get(const int x, const int y);

void cells_init(const int width, const int height);
void cells_size(int* width, int* height);
int** cells_get();
void cells_update();
void cells_deinit();

void fps_init();
void fps_update();

void renderer_init();
void render_screen(const unsigned int id, const unsigned int count);

void app_quit();
void app_restart();

#ifdef __cplusplus
}
#endif
#endif