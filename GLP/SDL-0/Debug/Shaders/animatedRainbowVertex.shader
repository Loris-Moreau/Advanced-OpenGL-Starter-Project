#version 330 core
in vec3 pos;
in vec3 color;
uniform vec3 offset;
out vec4 ourColor;
void main()
{
    gl_Position = vec4(pos+offset, 1.0f);
    ourColor = vec4(pos+offset+0.5f, 1.0f);
}