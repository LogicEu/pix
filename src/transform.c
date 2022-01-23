
#include <pix.h>

vec2 world_to_screen(const vec2 p)
{
    vec2 v;
    v.x = (p.x + 1.0) * iResolution.x * iScale * 0.5;
    v.y = (p.y + 1.0) * iResolution.y * iScale * 0.5;
    return v;
}

vec2 screen_to_world(const vec2 p)
{
    vec2 v;
    v.x = 2.0 * (p.x / (iResolution.x * iScale) - 0.5);
    v.y = 2.0 * (p.y / (iResolution.y * iScale) - 0.5);
    return v;
}