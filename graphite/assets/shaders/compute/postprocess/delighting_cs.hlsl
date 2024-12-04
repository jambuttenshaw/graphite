#ifndef DELIGHTING_HLSL
#define DELIGHTING_HLSL

#define HLSL
#include "../../HlslCompat/StructureHlslCompat.h"

ConstantBuffer<PassConstantBuffer> g_PassCB : register(b0, space0);

ConstantBuffer<DelightingParametersConstantBuffer> g_DelightingCB : register(b1, space0);

// G-buffer resources
Texture2D<float3> g_Albedo : register(t0, space0);
Texture2D<float3> g_Normal : register(t1, space0);
Texture2D<float2> g_RoughnessMetallic : register(t2, space0);
Texture2D<float> g_Depth : register(t3, space0);

// Scene lighting
ConstantBuffer<LightingConstantBuffer> g_LightCB : register(b2, space0);

// Environmental lighting resources
TextureCube g_IrradianceMap : register(t0, space1);

// Samplers
SamplerState g_EnvironmentSampler : register(s0, space0);


RWTexture2D<float4> g_OutputResource : register(u0, space0);


[numthreads(8, 8, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	if (any(DTid.xy >= g_DelightingCB.OutputDimensions))
	{
		return;
	}

	if (g_DelightingCB.ShowAlbedo)
	{
		const float3 albedo = g_Albedo[DTid.xy];
		g_OutputResource[DTid.xy] = float4(albedo, 1.0f);
		return;
	}

	// Recover NDC coordinates from thread ID
	//float2 uv = (DTid.xy + float2(0.5f, 0.5f)) / float2(g_DelightingCB.OutputDimensions);
	//uv.y = 1.0f - uv.y;
	//float2 ndc = 2.0f * uv - 1.0f;

	// Get depth from buffer
	//float depth = g_Depth[DTid.xy];
	//if (depth == 1.0f)
	//{
	//	return;
	//}

	// Un-project to world-space
	//float4 worldPos = mul(float4(ndc, depth, 1.0f), g_PassCB.InvViewProj);
	//worldPos = worldPos / worldPos.w;
	//const float3 w_o = normalize(g_PassCB.WorldEyePos - worldPos.xyz);

	// Perform delighting
	const float3 n = g_Normal[DTid.xy] * 2.0f - 1.0f;
	
	// Calculate light contribution from directional light
	const float3 l_o = g_OutputResource[DTid.xy].rgb;

	//const float3 l = -normalize(g_LightCB.DirectionalLight.Direction);
	//const float3 w_i = g_LightCB.DirectionalLight.Color * g_LightCB.DirectionalLight.Intensity;

	const float3 l = -normalize(g_DelightingCB.SunDirection);
	const float3 w_i = g_DelightingCB.SunIntensity;

	// Sample ambient lighting from irradiance map
	const float3 ambient = (g_LightCB.IndirectIllumination != IndirectIlluminationMethod_None)
							? g_IrradianceMap.SampleLevel(g_EnvironmentSampler, n, 0).rgb : float3(0.0f, 0.0f, 0.0f);

	const float3 albedo = (PI * l_o) / (w_i * saturate(dot(n, l)) + PI * ambient);

	g_OutputResource[DTid.xy] = float4(albedo, 1.0f);
}

#endif