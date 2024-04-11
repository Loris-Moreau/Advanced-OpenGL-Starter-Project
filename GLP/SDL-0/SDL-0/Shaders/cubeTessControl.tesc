#version 450 core

layout (vertices = 3) out;

uniform float outerRatio;
uniform float innerRatio;

void main(void)
{
    if(gl_InvocationID == 0) 
    {
        gl_TessLevelInner[0] = innerRatio;
        
        gl_TessLevelOuter[0] = outerRatio;
        gl_TessLevelOuter[1] = outerRatio;
        gl_TessLevelOuter[2] = outerRatio;
    }
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
