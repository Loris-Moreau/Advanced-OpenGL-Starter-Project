#version 330 core

in vec3 position;

uniform float offset;

void main()
{
    //Move the paddles along the Y-axis based on the 'offset' value
    vec3 newPosition = position + vec3(0.0, offset, 0.0);
    gl_Position = vec4(newPosition, 1.0);
}
