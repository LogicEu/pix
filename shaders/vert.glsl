layout (location = 0) in vec2 vertCoord;
layout (location = 1) in vec4 vertColor;

out vec4 VertColor;

void main()
{
    VertColor = vertColor;
    gl_Position = vec4(vertCoord, 0.0, 1.0);
}
