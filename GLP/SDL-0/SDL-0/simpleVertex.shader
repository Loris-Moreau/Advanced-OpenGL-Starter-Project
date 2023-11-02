#version 330 core

in vec3 pos;
in vec3 color;

out vec4 ourColor;

uniform float HOffset;
uniform float VOffset;

void main()
{
    gl_Position = vec4(pos.x + HOffset, pos.y + VOffset, pos.z,  1.0);
    //ourColor = vec4((pos.x + HOffset)/2 + 0.5, (pos.y + VOffset)/2 + 0.5, pos.z/2 + 0.5, 1.0);
    ourColor = vec4(color, 1.0);;
}
