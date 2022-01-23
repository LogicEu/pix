#include <pix.h>

static float rot_x, rot_y;
static int width, height;
static vec3 player;

static inline vec4 vec4_lerp(const vec4 a, const vec4 b, const float t)
{
    vec4 v = {lerpf(a.x, b.x, t), lerpf(a.y, b.y, t), lerpf(a.z, b.z, t), lerpf(a.w, b.w, t)};
    return v;
}

static inline vec3 vec3_spawn()
{
    return vec3_new(rand_next() % width, 0.0, rand_next() % height);
}

void fps_init()
{
    cells_size(&width, &height);
    int** cells = cells_get();
    player = vec3_spawn();
    while (cells[(int)player.y][(int)player.x]) {
        player = vec3_spawn();
    }
}

void fps_update()
{
    static const float ffov = 3.14159 / 4.0;
    static const float depth = 16.0;
    static const float gravity = 0.1;

    static float yvelocity = 0.0;

    static vec4 sky_color = {0.3, 0.3, 1.0, 1.0};
    static vec4 wall_color = {0.3, 0.3, 0.3, 1.0};
    static vec4 floor_color = {0.2, 0.5, 0.4, 1.0};
    static vec4 black = {0.0, 0.0, 0.0, 0.0};

    int** cells = cells_get();

    for (int y = 0; y < height; y++) {

        float ray_angle_y = (rot_y - ffov * 0.5) + ((float)y / (float)height);
        float eye_y = sinf(ray_angle_y);
        
        for (int x = 0; x < width; x++) {
            
            float ray_angle_x = (rot_x - ffov * 0.5) + ((float)x / (float)width);
            float eye_x = sinf(ray_angle_x);
            float eye_z = cosf(ray_angle_x) * cosf(ray_angle_y);

            float distance_to_wall = 0.0;
            int bool_hit_wall = 0;
            
            while(!bool_hit_wall && distance_to_wall < depth) {
                distance_to_wall += 0.08;
                
                float test_y = (player.y + eye_y * distance_to_wall);
                float test_x = (player.x + eye_x * distance_to_wall);
                float test_z = (player.z + eye_z * distance_to_wall);
                
                if (test_y > 1.0) {
                    screen_pixel_color_set(x, y, vec4_mult(floor_color, (1.0 - distance_to_wall / depth) * 0.5)); 
                    //screen_pixel_color_set(x, y, vec4_lerp(c, black, distance_to_wall / depth));
                    break;
                }
                else if (test_x < 0 || test_x >= width || test_z < 0 || test_z >= height){
                    bool_hit_wall = 1;
                    distance_to_wall = depth;
                    screen_pixel_color_set(x, y, vec4_mult(sky_color, (float)y / (float)height));
                }
                else if (cells[(int)test_z][(int)test_x] && test_y > -1.0 && test_y < 2.0) {
                    bool_hit_wall = 1;
                    screen_pixel_color_set(x, y, vec4_lerp(wall_color, black, 1.0 - distance_to_wall / depth));
                }
                else {
                    screen_pixel_color_set(x, y, vec4_mult(sky_color, (float)y / (float)height));
                }
            }
        }
    }

    float delta_speed = iDeltaTime * 4.0;

    glee_mouse_pos(&rot_x, &rot_y);
    rot_x *= 0.01;
    rot_y *= 0.01;

    if (glee_key_down(GLFW_KEY_A)) {
        vec2 off = {cosf(rot_x) * delta_speed, sinf(rot_x) * delta_speed};
        player.x -= off.x;
        player.z -= off.y;
        if (cells[(int)player.z][(int)player.x] > 0 && player.y > -1.5) { 
            player.x += off.x;
            player.z += off.y;
        }
    }
    if (glee_key_down(GLFW_KEY_D)) {
        vec2 off = {cosf(rot_x) * delta_speed, sinf(rot_x) * delta_speed};
        player.x += off.x;
        player.z += off.y;
        if (cells[(int)player.z][(int)player.x] > 0 && player.y > -1.5) { 
            player.x -= off.x;
            player.z -= off.y;
        }
    }
    if (glee_key_down(GLFW_KEY_W)) {
        vec2 off = {sinf(rot_x) * delta_speed, cosf(rot_x) * delta_speed};
        player.x += off.x;
        player.z -= off.y;
        if (cells[(int)player.z][(int)player.x] > 0 && player.y > -1.5) { 
            player.x -= off.x;
            player.z += off.y;
        }
    }
    if (glee_key_down(GLFW_KEY_S)) {
        vec2 off = {sinf(rot_x) * delta_speed, cosf(rot_x) * delta_speed};
        player.x -= off.x;
        player.z += off.y;
        if (cells[(int)player.z][(int)player.x] > 0 && player.y > -1.5) { 
            player.x += off.x;
            player.z -= off.y;
        }
    }
    if (glee_key_pressed(GLFW_KEY_SPACE)) {
        yvelocity = -2.0;
    }

    player.y += yvelocity * delta_speed;
    if (player.y < 0.1) {
        if (cells[(int)player.z][(int)player.x]) {
            if (player.y < -1.5) {
                yvelocity += gravity;
            }
            else {
                yvelocity = 0.0;
                player.y = -1.5;
            }
        }
        else if (player.y < 0.0) {
            yvelocity += gravity;
        }
        else {
            yvelocity = 0.0;
            player.y = 0.0;
        }
    } else {
        player.y = 0.0;
        yvelocity = 0.0;
    }

    screen_draw();
}