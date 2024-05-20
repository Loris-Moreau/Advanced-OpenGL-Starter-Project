#version 450 core

out vec4 color;

layout(binding = 3) uniform sampler2D tex_color;

uniform bool enable_fog = true;
uniform vec4 fog_color = vec4(0.7, 0.8, 0.9, 0.0);

uniform float minHeight;
uniform float maxHeight;

in TES_OUT
{
    vec2 tc;
    float height;
} fs_in;

void main(void)
{
    float normalizedHeight = (fs_in.height - minHeight) / (maxHeight - minHeight);
    
    vec4 height_color = vec4(normalizedHeight, normalizedHeight, normalizedHeight + 200, 0.5);

    color = mix(texture(tex_color, fs_in.tc), height_color, 0.5);
}