#version 330 core

in vec3 pos;
in vec3 color;

out vec4 ourColor;

uniform vec3 offset;

void main()
{
    gl_Position = vec4(pos + offset, 1.0);
    ourColor = vec4(color, 1.0);
}
