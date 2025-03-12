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


struct Vertex
{
	float4 position : POSITION0;
    float3 normal : NORMAL0;
};


struct VSToPS
{
	float4 position : SV_POSITION;
    float3 normal : NORMAL0;
};


VSToPS VSMain(Vertex input)
{
	VSToPS output;
    
    output.position = mul(g_InstanceData.WorldMatrix, input.position);
    output.position = mul(g_PassCB.ViewProjectionMatrix, output.position);
    
    output.normal = input.normal;
    
	return output;
}


float4 PSMain(VSToPS input, uint primitiveID : SV_PrimitiveID) : SV_TARGET
{
    return float4(input.normal * 0.5f + 0.5f, 1.0f);
}

#endif