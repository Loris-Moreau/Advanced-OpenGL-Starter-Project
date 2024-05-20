#version 330 core
in vec3 pos;
uniform vec3 offset;
void main()
{
    gl_Position = vec4(pos + offset, 1.0f);
}