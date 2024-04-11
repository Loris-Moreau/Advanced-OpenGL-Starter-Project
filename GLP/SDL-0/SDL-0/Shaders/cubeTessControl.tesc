#version 450 core

layout (vertices = 3) out;

uniform float outerRatio;
uniform float innerRatio;
//in VS_OUT {vec4 color;} tesc_in[];
//out TESC_OUT {vec4 color;} tesc_out[];

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
    //tesc_out[gl_InvocationID].color = tesc_in[gl_InvocationID].color;
}
