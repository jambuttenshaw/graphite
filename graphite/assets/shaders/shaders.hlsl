#ifndef SHADERS_HLSL
#define SHADERS_HLSL

struct TriangleConstantBufferType
{
    float4 Color;
};
ConstantBuffer<TriangleConstantBufferType> g_TriangleCB : register(b0);


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
    return g_TriangleCB.Color;
}

#endif