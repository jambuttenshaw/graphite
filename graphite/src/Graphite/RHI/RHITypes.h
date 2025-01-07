#pragma once


namespace Graphite
{
	// Common RHI types
	// To translate from API-specific types to Graphite-specific types

	using GPUVirtualAddress = uint64_t;

    using CPUDescriptorHandle = uint64_t;
    using GPUDescriptorHandle = uint64_t;


	enum GraphiteFormat : uint8_t
	{
        GraphiteFormat_Unknown,
        GraphiteFormat_R32G32B32A32_TYPELESS,
        GraphiteFormat_R32G32B32A32_FLOAT,
        GraphiteFormat_R32G32B32A32_UINT,
        GraphiteFormat_R32G32B32A32_SINT,
        GraphiteFormat_R32G32B32_TYPELESS,
        GraphiteFormat_R32G32B32_FLOAT,
        GraphiteFormat_R32G32B32_UINT,
        GraphiteFormat_R32G32B32_SINT,
        GraphiteFormat_R16G16B16A16_TYPELESS,
        GraphiteFormat_R16G16B16A16_FLOAT,
        GraphiteFormat_R16G16B16A16_UNORM,
        GraphiteFormat_R16G16B16A16_UINT,
        GraphiteFormat_R16G16B16A16_SNORM,
        GraphiteFormat_R16G16B16A16_SINT,
        GraphiteFormat_R32G32_TYPELESS,
        GraphiteFormat_R32G32_FLOAT,
        GraphiteFormat_R32G32_UINT,
        GraphiteFormat_R32G32_SINT,
        GraphiteFormat_R32G8X24_TYPELESS,
        GraphiteFormat_D32_FLOAT_S8X24_UINT,
        GraphiteFormat_R32_FLOAT_X8X24_TYPELESS,
        GraphiteFormat_X32_TYPELESS_G8X24_UINT,
        GraphiteFormat_R10G10B10A2_TYPELESS,
        GraphiteFormat_R10G10B10A2_UNORM,
        GraphiteFormat_R10G10B10A2_UINT,
        GraphiteFormat_R11G11B10_FLOAT,
        GraphiteFormat_R8G8B8A8_TYPELESS,
        GraphiteFormat_R8G8B8A8_UNORM,
        GraphiteFormat_R8G8B8A8_UNORM_SRGB,
        GraphiteFormat_R8G8B8A8_UINT,
        GraphiteFormat_R8G8B8A8_SNORM,
        GraphiteFormat_R8G8B8A8_SINT,
        GraphiteFormat_R16G16_TYPELESS,
        GraphiteFormat_R16G16_FLOAT,
        GraphiteFormat_R16G16_UNORM,
        GraphiteFormat_R16G16_UINT,
        GraphiteFormat_R16G16_SNORM,
        GraphiteFormat_R16G16_SINT,
        GraphiteFormat_R32_TYPELESS,
        GraphiteFormat_D32_FLOAT,
        GraphiteFormat_R32_FLOAT,
        GraphiteFormat_R32_UINT,
        GraphiteFormat_R32_SINT,
        GraphiteFormat_R24G8_TYPELESS,
        GraphiteFormat_D24_UNORM_S8_UINT,
        GraphiteFormat_R24_UNORM_X8_TYPELESS,
        GraphiteFormat_X24_TYPELESS_G8_UINT,
        GraphiteFormat_R8G8_TYPELESS,
        GraphiteFormat_R8G8_UNORM,
        GraphiteFormat_R8G8_UINT,
        GraphiteFormat_R8G8_SNORM,
        GraphiteFormat_R8G8_SINT,
        GraphiteFormat_R16_TYPELESS,
        GraphiteFormat_R16_FLOAT,
        GraphiteFormat_D16_UNORM,
        GraphiteFormat_R16_UNORM,
        GraphiteFormat_R16_UINT,
        GraphiteFormat_R16_SNORM,
        GraphiteFormat_R16_SINT,
        GraphiteFormat_R8_TYPELESS,
        GraphiteFormat_R8_UNORM,
        GraphiteFormat_R8_UINT,
        GraphiteFormat_R8_SNORM,
        GraphiteFormat_R8_SINT,
        GraphiteFormat_A8_UNORM,
        GraphiteFormat_R1_UNORM,
        GraphiteFormat_R9G9B9E5_SHAREDEXP,
        GraphiteFormat_R8G8_B8G8_UNORM,
        GraphiteFormat_G8R8_G8B8_UNORM,
        GraphiteFormat_BC1_TYPELESS,
        GraphiteFormat_BC1_UNORM,
        GraphiteFormat_BC1_UNORM_SRGB,
        GraphiteFormat_BC2_TYPELESS,
        GraphiteFormat_BC2_UNORM,
        GraphiteFormat_BC2_UNORM_SRGB,
        GraphiteFormat_BC3_TYPELESS,
        GraphiteFormat_BC3_UNORM,
        GraphiteFormat_BC3_UNORM_SRGB,
        GraphiteFormat_BC4_TYPELESS,
        GraphiteFormat_BC4_UNORM,
        GraphiteFormat_BC4_SNORM,
        GraphiteFormat_BC5_TYPELESS,
        GraphiteFormat_BC5_UNORM,
        GraphiteFormat_BC5_SNORM,
        GraphiteFormat_B5G6R5_UNORM,
        GraphiteFormat_B5G5R5A1_UNORM,
        GraphiteFormat_B8G8R8A8_UNORM,
        GraphiteFormat_B8G8R8X8_UNORM,
        GraphiteFormat_R10G10B10_XR_BIAS_A2_UNORM,
        GraphiteFormat_B8G8R8A8_TYPELESS,
        GraphiteFormat_B8G8R8A8_UNORM_SRGB,
        GraphiteFormat_B8G8R8X8_TYPELESS,
        GraphiteFormat_B8G8R8X8_UNORM_SRGB,
        GraphiteFormat_BC6H_TYPELESS,
        GraphiteFormat_BC6H_UF16,
        GraphiteFormat_BC6H_SF16,
        GraphiteFormat_BC7_TYPELESS,
        GraphiteFormat_BC7_UNORM,
        GraphiteFormat_BC7_UNORM_SRGB,
        GraphiteFormat_AYUV,
        GraphiteFormat_Y410,
        GraphiteFormat_Y416,
        GraphiteFormat_NV12,
        GraphiteFormat_P010,
        GraphiteFormat_P016,
        GraphiteFormat_420_OPAQUE,
        GraphiteFormat_YUY2,
        GraphiteFormat_Y210,
        GraphiteFormat_Y216,
        GraphiteFormat_NV11,
        GraphiteFormat_AI44,
        GraphiteFormat_IA44,
        GraphiteFormat_P8,
        GraphiteFormat_A8P8,
        GraphiteFormat_B4G4R4A4_UNORM
	};

    enum GraphiteTopology : uint8_t
    {
        GraphiteTopology_UNDEFINED,
        GraphiteTopology_POINTLIST,
        GraphiteTopology_LINELIST,
        GraphiteTopology_LINESTRIP,
        GraphiteTopology_TRIANGLELIST,
        GraphiteTopology_TRIANGLESTRIP,
        GraphiteTopology_TRIANGLEFAN,
        GraphiteTopology_LINELIST_ADJ,
        GraphiteTopology_LINESTRIP_ADJ,
        GraphiteTopology_TRIANGLELIST_ADJ,
        GraphiteTopology_TRIANGLESTRIP_ADJ,
        GraphiteTopology_1_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_2_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_3_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_4_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_5_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_6_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_7_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_8_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_9_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_10_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_11_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_12_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_13_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_14_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_15_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_16_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_17_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_18_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_19_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_20_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_21_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_22_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_23_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_24_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_25_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_26_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_27_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_28_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_29_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_30_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_31_CONTROL_POINT_PATCHLIST,
        GraphiteTopology_32_CONTROL_POINT_PATCHLIST,
    };

	enum DescriptorHeapType : uint8_t
	{
		GraphiteDescriptorHeap_RTV,
		GraphiteDescriptorHeap_DSV,
		GraphiteDescriptorHeap_RESOURCE,
		GraphiteDescriptorHeap_SAMPLER
	};


	struct Viewport
	{
		float Left;
		float Top;

		float Width;
		float Height;

		float MinDepth;
		float MaxDepth;
	};

	// General purpose rectangle definition
	struct Rectangle
	{
		int32_t Left;
		int32_t Top;

		int32_t Width;
		int32_t Height;
	};


    enum class PipelineResourceType : uint8_t
    {
        Invalid = 0,
        ConstantBufferView,
        ShaderResourceView,
        UnorderedAccessView
    };

    // Describes how (and the frequency) a resource should be bound to the pipeline
    enum class PipelineResourceBindingFrequency : uint8_t
    {
        Invalid = 0,
        Static,			// Doesn't change throughout a pass
        Mutable,		// Changes infrequently throughout a pass
        Dynamic			// Changes constantly throughout a pass
    };

    // Describes how resources should be bound to a pipeline
    enum class PipelineResourceBindingMethod : uint8_t
    {
	    Invalid = 0,
        Default,    // Descriptor tables are the default method of binding resources

        Inline      // Inlined bindings will store their address directly in the root signature, resulting in one less indirection to access
					// Only works for buffers and consumes a greater amount of the root signature than default (descriptor tables)
    };

    // A bitfield that describes which shaders a resource should be bound to
    enum PipelineResourceShaderVisibility : uint8_t
    {
        ShaderVisibility_None = 0x00,
        ShaderVisibility_Vertex = 0x01,
        ShaderVisibility_Hull = 0x02,
        ShaderVisibility_Domain = 0x04,
        ShaderVisibility_Geometry = 0x08,
        ShaderVisibility_Pixel = 0x10,
        ShaderVisibility_Amplification = 0x20,
        ShaderVisibility_Mesh = 0x40,
        ShaderVisibility_All = 0x7F
    };

}
