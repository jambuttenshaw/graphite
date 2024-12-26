#ifndef SHADERS_HLSL
#define SHADERS_HLSL

struct Vertex_Position
{
    float4 position : POSITION;
};


struct VSToPS
{
    float4 position : SV_POSITION;
};


VSToPS VSMain(Vertex_Position input)
{
    VSToPS output;
    output.position = input.position;

    return output;
}

float4 PSMain(VSToPS input) : SV_TARGET
{
    return float4(1.0f, 1.0f, 0.0f, 1.0f);
}

#endif