#include "graphite_pch.h"
#include "RHITypes.h"

#include "Core/Log.h"


namespace Graphite
{

	uint32_t GraphiteFormatSizeInBytes(GraphiteFormat format)
	{
		switch(format)
		{
            case GraphiteFormat_R32G32B32A32_TYPELESS:
            case GraphiteFormat_R32G32B32A32_FLOAT:
            case GraphiteFormat_R32G32B32A32_UINT:
            case GraphiteFormat_R32G32B32A32_SINT:
                return 16;
            case GraphiteFormat_R32G32B32_TYPELESS:
            case GraphiteFormat_R32G32B32_FLOAT:
            case GraphiteFormat_R32G32B32_UINT:
            case GraphiteFormat_R32G32B32_SINT:
                return 12;
            case GraphiteFormat_R16G16B16A16_TYPELESS:
            case GraphiteFormat_R16G16B16A16_FLOAT:
            case GraphiteFormat_R16G16B16A16_UNORM:
            case GraphiteFormat_R16G16B16A16_UINT:
            case GraphiteFormat_R16G16B16A16_SNORM:
            case GraphiteFormat_R16G16B16A16_SINT:
            case GraphiteFormat_R32G32_TYPELESS:
            case GraphiteFormat_R32G32_FLOAT:
            case GraphiteFormat_R32G32_UINT:
            case GraphiteFormat_R32G32_SINT:
            case GraphiteFormat_R32G8X24_TYPELESS:
            case GraphiteFormat_D32_FLOAT_S8X24_UINT:
            case GraphiteFormat_R32_FLOAT_X8X24_TYPELESS:
            case GraphiteFormat_X32_TYPELESS_G8X24_UINT:
                return 8;
            case GraphiteFormat_R10G10B10A2_TYPELESS:
            case GraphiteFormat_R10G10B10A2_UNORM:
            case GraphiteFormat_R10G10B10A2_UINT:
            case GraphiteFormat_R11G11B10_FLOAT:
            case GraphiteFormat_R8G8B8A8_TYPELESS:
            case GraphiteFormat_R8G8B8A8_UNORM:
            case GraphiteFormat_R8G8B8A8_UNORM_SRGB:
            case GraphiteFormat_R8G8B8A8_UINT:
            case GraphiteFormat_R8G8B8A8_SNORM:
            case GraphiteFormat_R8G8B8A8_SINT:
            case GraphiteFormat_R16G16_TYPELESS:
            case GraphiteFormat_R16G16_FLOAT:
            case GraphiteFormat_R16G16_UNORM:
            case GraphiteFormat_R16G16_UINT:
            case GraphiteFormat_R16G16_SNORM:
            case GraphiteFormat_R16G16_SINT:
            case GraphiteFormat_R32_TYPELESS:
            case GraphiteFormat_D32_FLOAT:
            case GraphiteFormat_R32_FLOAT:
            case GraphiteFormat_R32_UINT:
            case GraphiteFormat_R32_SINT:
            case GraphiteFormat_R24G8_TYPELESS:
            case GraphiteFormat_D24_UNORM_S8_UINT:
            case GraphiteFormat_R24_UNORM_X8_TYPELESS:
            case GraphiteFormat_X24_TYPELESS_G8_UINT:
                return 4;
            case GraphiteFormat_R8G8_TYPELESS:
            case GraphiteFormat_R8G8_UNORM:
            case GraphiteFormat_R8G8_UINT:
            case GraphiteFormat_R8G8_SNORM:
            case GraphiteFormat_R8G8_SINT:
            case GraphiteFormat_R16_TYPELESS:
            case GraphiteFormat_R16_FLOAT:
            case GraphiteFormat_D16_UNORM:
            case GraphiteFormat_R16_UNORM:
            case GraphiteFormat_R16_UINT:
            case GraphiteFormat_R16_SNORM:
            case GraphiteFormat_R16_SINT:
                return 2;
            case GraphiteFormat_R8_TYPELESS:
            case GraphiteFormat_R8_UNORM:
            case GraphiteFormat_R8_UINT:
            case GraphiteFormat_R8_SNORM:
            case GraphiteFormat_R8_SINT:
            case GraphiteFormat_A8_UNORM:
            case GraphiteFormat_R1_UNORM: // TODO: Would this also be 1?
                return 1;
            case GraphiteFormat_R9G9B9E5_SHAREDEXP:
            case GraphiteFormat_R8G8_B8G8_UNORM:
            case GraphiteFormat_G8R8_G8B8_UNORM:
            case GraphiteFormat_BC1_TYPELESS:
            case GraphiteFormat_BC1_UNORM:
            case GraphiteFormat_BC1_UNORM_SRGB:
            case GraphiteFormat_BC2_TYPELESS:
            case GraphiteFormat_BC2_UNORM:
            case GraphiteFormat_BC2_UNORM_SRGB:
            case GraphiteFormat_BC3_TYPELESS:
            case GraphiteFormat_BC3_UNORM:
            case GraphiteFormat_BC3_UNORM_SRGB:
            case GraphiteFormat_BC4_TYPELESS:
            case GraphiteFormat_BC4_UNORM:
            case GraphiteFormat_BC4_SNORM:
            case GraphiteFormat_BC5_TYPELESS:
            case GraphiteFormat_BC5_UNORM:
            case GraphiteFormat_BC5_SNORM:
            case GraphiteFormat_B5G6R5_UNORM:
            case GraphiteFormat_B5G5R5A1_UNORM:
            case GraphiteFormat_B8G8R8A8_UNORM:
            case GraphiteFormat_B8G8R8X8_UNORM:
            case GraphiteFormat_R10G10B10_XR_BIAS_A2_UNORM:
            case GraphiteFormat_B8G8R8A8_TYPELESS:
            case GraphiteFormat_B8G8R8A8_UNORM_SRGB:
            case GraphiteFormat_B8G8R8X8_TYPELESS:
            case GraphiteFormat_B8G8R8X8_UNORM_SRGB:
            case GraphiteFormat_BC6H_TYPELESS:
            case GraphiteFormat_BC6H_UF16:
            case GraphiteFormat_BC6H_SF16:
            case GraphiteFormat_BC7_TYPELESS:
            case GraphiteFormat_BC7_UNORM:
            case GraphiteFormat_BC7_UNORM_SRGB:
            case GraphiteFormat_AYUV:
            case GraphiteFormat_Y410:
            case GraphiteFormat_Y416:
            case GraphiteFormat_NV12:
            case GraphiteFormat_P010:
            case GraphiteFormat_P016:
            case GraphiteFormat_420_OPAQUE:
            case GraphiteFormat_YUY2:
            case GraphiteFormat_Y210:
            case GraphiteFormat_Y216:
            case GraphiteFormat_NV11:
            case GraphiteFormat_AI44:
            case GraphiteFormat_IA44:
            case GraphiteFormat_P8:
            case GraphiteFormat_A8P8:
            case GraphiteFormat_B4G4R4A4_UNORM:
            case GraphiteFormat_Unknown:
			break;
		}

		GRAPHITE_LOG_ERROR("Missing/invalid format!");
        return static_cast<uint32_t>(-1);
	}


}
