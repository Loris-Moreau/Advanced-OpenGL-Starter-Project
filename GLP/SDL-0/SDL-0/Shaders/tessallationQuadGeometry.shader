#version 450 core

layout (quads) in;
layout (points, max_vertices = 4) out;

void main(void)
{
    int i;
    for (i=0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
}