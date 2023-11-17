#version 330 core

in vec3 pos;

uniform vec2 movement;

void main()
{
  gl_Position = vec4(pos.x + movement.x, pos.y + movement.y, pos.z, 1.0);
}
