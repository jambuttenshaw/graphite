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
struct TestCBType
{
	float4 A;
	float4 B;
};

ConstantBuffer<TriangleOffsetConstantBufferType> g_TriangleOffsetCB : register(b0); // Used in VS
ConstantBuffer<TriangleColorConstantBufferType> g_TriangleColorCB : register(b0); // Used in PS

ConstantBuffer<TestCBType> g_TestCB : register(b1);


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
	output.position = input.position + float4(g_TriangleOffsetCB.Offset, 0, 1) + g_TestCB.A.wwxx;

	return output;
}

float4 PSMain(VSToPS input) : SV_TARGET
{
	return g_TriangleColorCB.Color + g_TestCB.A.wwxx;
}

#endif