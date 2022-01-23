#include <pix.h>

#define STROKE 1.0

typedef struct Vertex {
    vec2 position;
    vec4 color;
} Vertex;

typedef struct Q2D {
    Vertex verts[4];
} Q2D;

static vec2 resolution;
static int width, height;
static unsigned int cellsID, cellsVAO, cellsEBO;
static array_t *vertices, *indices;

static void cell_push(const float x, const float y)
{
    static int marker = 0;
    
    unsigned int ind[6] = {
        0 + marker,  1 + marker,  3 + marker,
        1 + marker,  2 + marker,  3 + marker
    };

    vec4 c = {0.0, 0.0, 0.0, 0.0};
    vec2 p = screen_to_world(vec2_new(x, y));

    Vertex v[4] = {
        {{p.x + STROKE / resolution.x, p.y + STROKE / resolution.y}, c},
        {{p.x + STROKE / resolution.x, p.y - STROKE / resolution.y}, c},
        {{p.x - STROKE / resolution.x, p.y - STROKE / resolution.y}, c},
        {{p.x - STROKE / resolution.x, p.y + STROKE / resolution.y}, c}
    };

    for (int i = 0; i < 4; i++) array_push(vertices, &v[i]);
    for (int i = 0; i < 6; i++) array_push(indices, &ind[i]);
    
    marker += 4;
}

static inline unsigned int screen_bind()
{
    cellsID = glee_buffer_id();
    glBindVertexArray(cellsID);
    glGenBuffers(1, &cellsVAO);
    glGenBuffers(1, &cellsEBO);
    glBindBuffer(GL_ARRAY_BUFFER, cellsVAO);
    glBufferData(GL_ARRAY_BUFFER, vertices->used * vertices->bytes, vertices->data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cellsEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->used * indices->bytes, indices->data, GL_STATIC_DRAW);
    glee_buffer_attribute_set(0, 2, sizeof(Vertex), 0);
    glee_buffer_attribute_set(1, 4, sizeof(Vertex), offsetof(Vertex, color));
    return cellsID;
}

static inline void screen_rebind()
{
    glBindVertexArray(cellsID);
    glBindBuffer(GL_ARRAY_BUFFER, cellsVAO);
    glBufferData(GL_ARRAY_BUFFER, vertices->used * vertices->bytes, vertices->data, GL_STATIC_DRAW);
}

unsigned int screen_init()
{
    cells_size(&width, &height);
    resolution = vec2_new((float)width, (float)height);
    const int size = width * height;
    
    vertices = array_new(size * 4, sizeof(Vertex));
    indices = array_new(size * 6, sizeof(unsigned int));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            cell_push((float)x, (float)y);
        }
    }

    return screen_bind();
}

void screen_pixel_color_set(const int x, const int y, const vec4 color)
{
    Q2D *q = vertices->data;
    q += y * width + x;
    for (int i = 0; i < 4; i++) q->verts[i].color = color;
}

vec4 screen_pixel_color_get(const int x, const int y)
{
    Q2D *q = vertices->data;
    q += y * width + x;
    return q->verts[0].color;
}

void* screen_ger()
{
    return vertices->data;
}

void screen_draw()
{
    screen_rebind();
    render_screen(cellsID, indices->used);
}

void screen_update()
{
    static vec4 null = {0.0, 0.0, 0.0, 0.0};
    int** cells = cells_get();

    Q2D *q = vertices->data;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            
            vec4 c = null;
            if (cells[y][x]) c = vec4_new(1.0, (float)x / (float)width, (float)y / (float)height, 1.0);
                
            for (int i = 0; i < 4; i++) { q->verts[i].color = c; }
            q++;
        }
    }

    screen_draw();
}

void screen_deinit()
{
    array_destroy(vertices);
    array_destroy(indices);
}