#version 330 core
in vec3 pos;
in vec2 texCoord;

out vec2 TexCoord;

uniform float scale;
void main()
{
    gl_Position = vec4(pos*scale, 1.0);
    TexCoord = texCoord;
}