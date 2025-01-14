#ifndef SHADERS_HLSL
#define SHADERS_HLSL

struct TriangleOffsetConstantBufferType
{
    float2 Offset;
};
struct TriangleColorConstantBufferType
{
    float4 Color;
};

ConstantBuffer<TriangleOffsetConstantBufferType> g_TriangleOffsetCB : register(b0); // Used in VS
//ConstantBuffer<TriangleOffsetConstantBufferType> g_TriangleOffsetCB2 : register(b1); // Used in VS
ConstantBuffer<TriangleColorConstantBufferType> g_TriangleColorCB : register(b0);   // Used in PS


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
    output.position = input.position + float4(g_TriangleOffsetCB.Offset, 0, 1);

    return output;
}

float4 PSMain(VSToPS input) : SV_TARGET
{
    return g_TriangleColorCB.Color;
}

#endif