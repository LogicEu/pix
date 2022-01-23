#include <pix.h>

static unsigned int shader;

void renderer_init()
{
    shader = glee_shader_load("shaders/vert.glsl", "shaders/polygon.glsl");
    glee_screen_color(0.3, 0.3, 1.0, 1.0);
}

void render_screen(const unsigned int id, const unsigned int count)
{
    glUseProgram(shader);
    glBindVertexArray(id);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}