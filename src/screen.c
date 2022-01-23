#include <pix.h>

#define STROKE 1.0

typedef struct Vertex {
    vec2 position;
    vec4 color;
} Vertex;

static vec2 resolution;
static unsigned int cellsID, cellsVAO, cellsEBO;
static array_t *vertices, *indices;

static void cell_push(const float x, const float y, vec4 c)
{
    static int marker = 0;
    
    unsigned int ind[6] = {
        0 + marker,  1 + marker,  3 + marker,
        1 + marker,  2 + marker,  3 + marker
    };

    vec2 p = screen_to_world(vec2_new(x, y));
    //vec4 c = vec4_uni(1.0);
    //vec4 c = {x / resolution.x, y / resolution.y, 0.0, 1.0};

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
    int width, height;
    cells_size(&width, &height);
    resolution = vec2_new((float)width, (float)height);
    const int size = width * height;
    
    vertices = array_new(size * 4, sizeof(Vertex));
    indices = array_new(size * 6, sizeof(unsigned int));

    vec4 red = {1.0, 0.0, 0.0, 1.0};
    vec4 null = {0.0, 0.0, 0.0, 0.0};

    int** cells = cells_get();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            
            vec4 c = null;          
            if (cells[y][x] > 0) c = red;
            
            cell_push((float)x, (float)y, c);
        }
    }

    return screen_bind();
}


void screen_update()
{
    typedef struct Q2D {
        Vertex verts[4];
    } Q2D;

    int width, height;
    cells_size(&width, &height);
    int** cells = cells_get();

    vec4 red = {1.0, 0.0, 0.0, 1.0};
    vec4 null = {0.0, 0.0, 0.0, 0.0};

    Q2D *q = vertices->data;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            
            vec4 c = null;
            if (cells[y][x]) c = red;
                
            for (int i = 0; i < 4; i++) { q->verts[i].color = c; }
            q++;
        }
    }

    screen_rebind();
    render_screen(cellsID, indices->used);
}