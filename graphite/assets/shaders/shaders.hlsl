#ifndef SHADERS_HLSL
#define SHADERS_HLSL

struct PassConstantBufferType
{
    float4x4 ViewProjectionMatrix;
};
struct InstanceDataConstantBufferType
{
    float4x4 WorldMatrix;
};

ConstantBuffer<PassConstantBufferType> g_PassCB : register(b0); // Used in VS
ConstantBuffer<InstanceDataConstantBufferType> g_InstanceData : register(b1); // Used in VS


struct Vertex_Position
{
	float4 position : POSITION;
	float3 normal : NORMAL;
};


struct VSToPS
{
	float4 position : SV_POSITION;
};


VSToPS VSMain(Vertex_Position input)
{
	VSToPS output;
    
    output.position = mul(g_InstanceData.WorldMatrix, input.position);
    output.position = mul(g_PassCB.ViewProjectionMatrix, output.position);

	return output;
}


float4 PSMain(VSToPS input, uint primitiveID : SV_PrimitiveID) : SV_TARGET
{
    //return float4(0.8f, 0.2f, 0.2f, 1.0f);
    return float4((float)primitiveID / 8.0f, 0.0f, 0.0f, 1.0f);
}

#endif