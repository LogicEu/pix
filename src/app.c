#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <pix.h>

vec2 iResolution = {800.0, 600.0};
float iTime, iDeltaTime, iScale = 0.25;

static inline void app_setup()
{
    unsigned int width, height;
    width = (int)iResolution.x * iScale;
    height = (int)iResolution.y * iScale;

    cells_init(width, height);
    fps_init();
}

static inline void app_init(int argc, char** argv)
{
    int fullscreen = 0;

    rand_seed(time(NULL));
    
    if (argc > 1) {
        if (!strcmp(argv[1], "-f") || !strcmp(argv[1], "-fullscreen")) {
            fullscreen++;
        } else {
            iScale = atof(argv[1]);
        }
    }

    glee_init();
    glee_window_create("pix", iResolution.x, iResolution.y, fullscreen, 0);

    renderer_init();
    app_setup();
    screen_init();

    iTime = glee_time_get();
}

static inline void app_unsetup()
{
    cells_deinit();
    screen_deinit();
}

static inline void app_deinit()
{
    app_unsetup();
    glee_deinit();
}

static inline int app_exit()
{
    app_deinit();
    return EXIT_SUCCESS;
}

void app_quit()
{
    app_deinit();
    exit(EXIT_SUCCESS);
}

void app_restart()
{
    cells_deinit();
    app_setup();
}

static inline void app_run()
{
    static int mode = 0;
    while(glee_window_is_open()) {
        glee_screen_clear();
        iDeltaTime = glee_time_delta(&iTime);
        printf("FPS: %f\r", 60.0 / iDeltaTime);
        
        if (glee_key_pressed(GLFW_KEY_ESCAPE)) break;
        if (glee_key_pressed(GLFW_KEY_R)) app_restart();
        if (glee_key_pressed(GLFW_KEY_M)) {
            if (mode) mode--;
            else mode++;
        }

        if (!mode) {
            cells_update();
            screen_update();
        } else {
            fps_update();
        }

        glee_screen_refresh();
    }
}

int main(int argc, char** argv)
{
    app_init(argc, argv);
    app_run();
    return app_exit();
}
