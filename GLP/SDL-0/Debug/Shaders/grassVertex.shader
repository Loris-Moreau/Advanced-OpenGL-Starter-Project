#version 330 core
in vec3 pos;

uniform mat4 uViewProj;

void main()
{
    gl_Position = uViewProj * vec4(pos, 1.0);
}