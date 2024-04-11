#version 450 core

out vec4 fragColor;
//in TESE_OUT {vec4 color;}frag_in;

void main()
{
    fragColor = vec4(1.0,0.0,0.0,1.0);
    //fragColor = frag_in.color;
}
