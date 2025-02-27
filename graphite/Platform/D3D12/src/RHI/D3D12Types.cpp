#include "graphite_d3d12_pch.h"
#include "D3D12Types.h"

#include "Graphite/Core/Log.h"

#include "Graphite/RHI/Resources/InputLayout.h"
#include "Graphite/RHI/Resources/ResourceViews.h"


namespace Graphite::D3D12
{
    D3D12_GPU_VIRTUAL_ADDRESS GraphiteGPUAddressToD3D12GPUAddress(GPUVirtualAddress address)
    {
	    return static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(address);
    }


    DXGI_FORMAT GraphiteFormatToD3D12Format(GraphiteFormat format)
    {
        switch (format)
        {
        case GraphiteFormat_Unknown: return DXGI_FORMAT_UNKNOWN;
        case GraphiteFormat_R32G32B32A32_TYPELESS: return DXGI_FORMAT_R32G32B32A32_TYPELESS;
        case GraphiteFormat_R32G32B32A32_FLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case GraphiteFormat_R32G32B32A32_UINT: return DXGI_FORMAT_R32G32B32A32_UINT;
        case GraphiteFormat_R32G32B32A32_SINT: return DXGI_FORMAT_R32G32B32A32_SINT;
        case GraphiteFormat_R32G32B32_TYPELESS: return DXGI_FORMAT_R32G32B32_TYPELESS;
        case GraphiteFormat_R32G32B32_FLOAT: return DXGI_FORMAT_R32G32B32_FLOAT;
        case GraphiteFormat_R32G32B32_UINT: return DXGI_FORMAT_R32G32B32_UINT;
        case GraphiteFormat_R32G32B32_SINT: return DXGI_FORMAT_R32G32B32_SINT;
        case GraphiteFormat_R16G16B16A16_TYPELESS: return DXGI_FORMAT_R16G16B16A16_TYPELESS;
        case GraphiteFormat_R16G16B16A16_FLOAT: return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case GraphiteFormat_R16G16B16A16_UNORM: return DXGI_FORMAT_R16G16B16A16_UNORM;
        case GraphiteFormat_R16G16B16A16_UINT: return DXGI_FORMAT_R16G16B16A16_UINT;
        case GraphiteFormat_R16G16B16A16_SNORM: return DXGI_FORMAT_R16G16B16A16_SNORM;
        case GraphiteFormat_R16G16B16A16_SINT: return DXGI_FORMAT_R16G16B16A16_SINT;
        case GraphiteFormat_R32G32_TYPELESS: return DXGI_FORMAT_R32G32_TYPELESS;
        case GraphiteFormat_R32G32_FLOAT: return DXGI_FORMAT_R32G32_FLOAT;
        case GraphiteFormat_R32G32_UINT: return DXGI_FORMAT_R32G32_UINT;
        case GraphiteFormat_R32G32_SINT: return DXGI_FORMAT_R32G32_SINT;
        case GraphiteFormat_R32G8X24_TYPELESS: return DXGI_FORMAT_R32G8X24_TYPELESS;
        case GraphiteFormat_D32_FLOAT_S8X24_UINT: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        case GraphiteFormat_R32_FLOAT_X8X24_TYPELESS: return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
        case GraphiteFormat_X32_TYPELESS_G8X24_UINT: return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
        case GraphiteFormat_R10G10B10A2_TYPELESS: return DXGI_FORMAT_R10G10B10A2_TYPELESS;
        case GraphiteFormat_R10G10B10A2_UNORM: return DXGI_FORMAT_R10G10B10A2_UNORM;
        case GraphiteFormat_R10G10B10A2_UINT: return DXGI_FORMAT_R10G10B10A2_UINT;
        case GraphiteFormat_R11G11B10_FLOAT: return DXGI_FORMAT_R11G11B10_FLOAT;
        case GraphiteFormat_R8G8B8A8_TYPELESS: return DXGI_FORMAT_R8G8B8A8_TYPELESS;
        case GraphiteFormat_R8G8B8A8_UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
        case GraphiteFormat_R8G8B8A8_UNORM_SRGB: return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        case GraphiteFormat_R8G8B8A8_UINT: return DXGI_FORMAT_R8G8B8A8_UINT;
        case GraphiteFormat_R8G8B8A8_SNORM: return DXGI_FORMAT_R8G8B8A8_SNORM;
        case GraphiteFormat_R8G8B8A8_SINT: return DXGI_FORMAT_R8G8B8A8_SINT;
        case GraphiteFormat_R16G16_TYPELESS: return DXGI_FORMAT_R16G16_TYPELESS;
        case GraphiteFormat_R16G16_FLOAT: return DXGI_FORMAT_R16G16_FLOAT;
        case GraphiteFormat_R16G16_UNORM: return DXGI_FORMAT_R16G16_UNORM;
        case GraphiteFormat_R16G16_UINT: return DXGI_FORMAT_R16G16_UINT;
        case GraphiteFormat_R16G16_SNORM: return DXGI_FORMAT_R16G16_SNORM;
        case GraphiteFormat_R16G16_SINT: return DXGI_FORMAT_R16G16_SINT;
        case GraphiteFormat_R32_TYPELESS: return DXGI_FORMAT_R32_TYPELESS;
        case GraphiteFormat_D32_FLOAT: return DXGI_FORMAT_D32_FLOAT;
        case GraphiteFormat_R32_FLOAT: return DXGI_FORMAT_R32_FLOAT;
        case GraphiteFormat_R32_UINT: return DXGI_FORMAT_R32_UINT;
        case GraphiteFormat_R32_SINT: return DXGI_FORMAT_R32_SINT;
        case GraphiteFormat_R24G8_TYPELESS: return DXGI_FORMAT_R24G8_TYPELESS;
        case GraphiteFormat_D24_UNORM_S8_UINT: return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case GraphiteFormat_R24_UNORM_X8_TYPELESS: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        case GraphiteFormat_X24_TYPELESS_G8_UINT: return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
        case GraphiteFormat_R8G8_TYPELESS: return DXGI_FORMAT_R8G8_TYPELESS;
        case GraphiteFormat_R8G8_UNORM: return DXGI_FORMAT_R8G8_UNORM;
        case GraphiteFormat_R8G8_UINT: return DXGI_FORMAT_R8G8_UINT;
        case GraphiteFormat_R8G8_SNORM: return DXGI_FORMAT_R8G8_SNORM;
        case GraphiteFormat_R8G8_SINT: return DXGI_FORMAT_R8G8_SINT;
        case GraphiteFormat_R16_TYPELESS: return DXGI_FORMAT_R16_TYPELESS;
        case GraphiteFormat_R16_FLOAT: return DXGI_FORMAT_R16_FLOAT;
        case GraphiteFormat_D16_UNORM: return DXGI_FORMAT_D16_UNORM;
        case GraphiteFormat_R16_UNORM: return DXGI_FORMAT_R16_UNORM;
        case GraphiteFormat_R16_UINT: return DXGI_FORMAT_R16_UINT;
        case GraphiteFormat_R16_SNORM: return DXGI_FORMAT_R16_SNORM;
        case GraphiteFormat_R16_SINT: return DXGI_FORMAT_R16_SINT;
        case GraphiteFormat_R8_TYPELESS: return DXGI_FORMAT_R8_TYPELESS;
        case GraphiteFormat_R8_UNORM: return DXGI_FORMAT_R8_UNORM;
        case GraphiteFormat_R8_UINT: return DXGI_FORMAT_R8_UINT;
        case GraphiteFormat_R8_SNORM: return DXGI_FORMAT_R8_SNORM;
        case GraphiteFormat_R8_SINT: return DXGI_FORMAT_R8_SINT;
        case GraphiteFormat_A8_UNORM: return DXGI_FORMAT_A8_UNORM;
        case GraphiteFormat_R1_UNORM: return DXGI_FORMAT_R1_UNORM;
        case GraphiteFormat_R9G9B9E5_SHAREDEXP: return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
        case GraphiteFormat_R8G8_B8G8_UNORM: return DXGI_FORMAT_R8G8_B8G8_UNORM;
        case GraphiteFormat_G8R8_G8B8_UNORM: return DXGI_FORMAT_G8R8_G8B8_UNORM;
        case GraphiteFormat_BC1_TYPELESS: return DXGI_FORMAT_BC1_TYPELESS;
        case GraphiteFormat_BC1_UNORM: return DXGI_FORMAT_BC1_UNORM;
        case GraphiteFormat_BC1_UNORM_SRGB: return DXGI_FORMAT_BC1_UNORM_SRGB;
        case GraphiteFormat_BC2_TYPELESS: return DXGI_FORMAT_BC2_TYPELESS;
        case GraphiteFormat_BC2_UNORM: return DXGI_FORMAT_BC2_UNORM;
        case GraphiteFormat_BC2_UNORM_SRGB: return DXGI_FORMAT_BC2_UNORM_SRGB;
        case GraphiteFormat_BC3_TYPELESS: return DXGI_FORMAT_BC3_TYPELESS;
        case GraphiteFormat_BC3_UNORM: return DXGI_FORMAT_BC3_UNORM;
        case GraphiteFormat_BC3_UNORM_SRGB: return DXGI_FORMAT_BC3_UNORM_SRGB;
        case GraphiteFormat_BC4_TYPELESS: return DXGI_FORMAT_BC4_TYPELESS;
        case GraphiteFormat_BC4_UNORM: return DXGI_FORMAT_BC4_UNORM;
        case GraphiteFormat_BC4_SNORM: return DXGI_FORMAT_BC4_SNORM;
        case GraphiteFormat_BC5_TYPELESS: return DXGI_FORMAT_BC5_TYPELESS;
        case GraphiteFormat_BC5_UNORM: return DXGI_FORMAT_BC5_UNORM;
        case GraphiteFormat_BC5_SNORM: return DXGI_FORMAT_BC5_SNORM;
        case GraphiteFormat_B5G6R5_UNORM: return DXGI_FORMAT_B5G6R5_UNORM;
        case GraphiteFormat_B5G5R5A1_UNORM: return DXGI_FORMAT_B5G5R5A1_UNORM;
        case GraphiteFormat_B8G8R8A8_UNORM: return DXGI_FORMAT_B8G8R8A8_UNORM;
        case GraphiteFormat_B8G8R8X8_UNORM: return DXGI_FORMAT_B8G8R8X8_UNORM;
        case GraphiteFormat_R10G10B10_XR_BIAS_A2_UNORM: return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
        case GraphiteFormat_B8G8R8A8_TYPELESS: return DXGI_FORMAT_B8G8R8A8_TYPELESS;
        case GraphiteFormat_B8G8R8A8_UNORM_SRGB: return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
        case GraphiteFormat_B8G8R8X8_TYPELESS: return DXGI_FORMAT_B8G8R8X8_TYPELESS;
        case GraphiteFormat_B8G8R8X8_UNORM_SRGB: return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
        case GraphiteFormat_BC6H_TYPELESS: return DXGI_FORMAT_BC6H_TYPELESS;
        case GraphiteFormat_BC6H_UF16: return DXGI_FORMAT_BC6H_UF16;
        case GraphiteFormat_BC6H_SF16: return DXGI_FORMAT_BC6H_SF16;
        case GraphiteFormat_BC7_TYPELESS: return DXGI_FORMAT_BC7_TYPELESS;
        case GraphiteFormat_BC7_UNORM: return DXGI_FORMAT_BC7_UNORM;
        case GraphiteFormat_BC7_UNORM_SRGB: return DXGI_FORMAT_BC7_UNORM_SRGB;
        case GraphiteFormat_AYUV: return DXGI_FORMAT_AYUV;
        case GraphiteFormat_Y410: return DXGI_FORMAT_Y410;
        case GraphiteFormat_Y416: return DXGI_FORMAT_Y416;
        case GraphiteFormat_NV12: return DXGI_FORMAT_NV12;
        case GraphiteFormat_P010: return DXGI_FORMAT_P010;
        case GraphiteFormat_P016: return DXGI_FORMAT_P016;
        case GraphiteFormat_420_OPAQUE: return DXGI_FORMAT_420_OPAQUE;
        case GraphiteFormat_YUY2: return DXGI_FORMAT_YUY2;
        case GraphiteFormat_Y210: return DXGI_FORMAT_Y210;
        case GraphiteFormat_Y216: return DXGI_FORMAT_Y216;
        case GraphiteFormat_NV11: return DXGI_FORMAT_NV11;
        case GraphiteFormat_AI44: return DXGI_FORMAT_AI44;
        case GraphiteFormat_IA44: return DXGI_FORMAT_IA44;
        case GraphiteFormat_P8: return DXGI_FORMAT_P8;
        case GraphiteFormat_A8P8: return DXGI_FORMAT_A8P8;
        case GraphiteFormat_B4G4R4A4_UNORM: return DXGI_FORMAT_B4G4R4A4_UNORM;
        default: break;
        }

        GRAPHITE_LOG_ERROR("Unknown/unsupported Graphite format!");
        return DXGI_FORMAT_UNKNOWN;
    }

    D3D_PRIMITIVE_TOPOLOGY GraphiteTopologyToD3D12Topology(GraphiteTopology topology)
    {
        switch (topology)
        {
        case GraphiteTopology_UNDEFINED: return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
        case GraphiteTopology_POINTLIST: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
        case GraphiteTopology_LINELIST: return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
        case GraphiteTopology_LINESTRIP: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
        case GraphiteTopology_TRIANGLELIST: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        case GraphiteTopology_TRIANGLESTRIP: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        case GraphiteTopology_TRIANGLEFAN: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLEFAN;
        case GraphiteTopology_LINELIST_ADJ: return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
        case GraphiteTopology_LINESTRIP_ADJ: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
        case GraphiteTopology_TRIANGLELIST_ADJ: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
        case GraphiteTopology_TRIANGLESTRIP_ADJ: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
        case GraphiteTopology_1_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_2_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_3_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_4_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_5_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_6_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_7_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_8_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_9_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_10_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_11_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_12_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_13_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_14_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_15_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_16_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_17_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_18_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_19_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_20_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_21_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_22_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_23_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_24_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_25_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_26_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_27_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_28_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_29_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_30_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_31_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;
        case GraphiteTopology_32_CONTROL_POINT_PATCHLIST: return D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;
        default: break;
        }

        GRAPHITE_LOG_ERROR("Unknown topology!");
        return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
    }


    D3D12_DESCRIPTOR_HEAP_TYPE GraphiteDescriptorHeapTypeToD3D12DescriptorHeapType(DescriptorHeapType heapType)
    {
        switch (heapType)
        {
        case GraphiteDescriptorHeap_RTV:		return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        case GraphiteDescriptorHeap_DSV:		return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        case GraphiteDescriptorHeap_Resource:	return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        case GraphiteDescriptorHeap_Sampler:	return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
        default: break;
        }

        GRAPHITE_LOG_ERROR("Unknown/unsupported Graphite format!");
        return D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE GraphiteCPUDescriptorToD3D12Descriptor(CPUDescriptorHandle descriptor)
    {
        return D3D12_CPU_DESCRIPTOR_HANDLE{ descriptor };
    }

    CPUDescriptorHandle D3D12CPUDescriptorToGraphiteDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE descriptor)
    {
        return descriptor.ptr;
    }

    D3D12_GPU_DESCRIPTOR_HANDLE GraphiteGPUDescriptorToD3D12Descriptor(GPUDescriptorHandle descriptor)
    {
        return D3D12_GPU_DESCRIPTOR_HANDLE{ descriptor };
    }

    GPUDescriptorHandle D3D12GPUDescriptorToGraphiteDescriptor(D3D12_GPU_DESCRIPTOR_HANDLE descriptor)
    {
        return descriptor.ptr;
    }


    D3D12_VIEWPORT GraphiteViewportToD3D12Viewport(const Viewport& viewport)
    {
        return {
            .TopLeftX = viewport.Left,
            .TopLeftY = viewport.Top,
            .Width = viewport.Width,
            .Height = viewport.Height,
            .MinDepth = viewport.MinDepth,
            .MaxDepth = viewport.MaxDepth
        };
    }


    void GraphiteInputLayoutToD3D12InputLayout(const InputLayout& inputLayout, std::vector<D3D12_INPUT_ELEMENT_DESC>& outLayout)
    {
        outLayout.clear();
        outLayout.reserve(inputLayout.GetElementCount());

        // Build a D3D12 layout description out of the input elements
        uint32_t layoutSizeInBytes = 0;
        for (const auto& element : inputLayout)
        {
            // Build D3D12 element desc
            D3D12_INPUT_ELEMENT_DESC desc = {
                .SemanticName = element.SemanticName.c_str(),
                .SemanticIndex = element.SemanticIndex,
                .Format = GraphiteFormatToD3D12Format(element.Format),
                .InputSlot = 0,
                .AlignedByteOffset = layoutSizeInBytes,
                // Unused
                .InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
                .InstanceDataStepRate = 0
            };
            outLayout.push_back(desc);
            layoutSizeInBytes += GraphiteFormatSizeInBytes(element.Format);
        }
    }


    D3D12_VERTEX_BUFFER_VIEW GraphiteVBVToD3D12VBV(const VertexBufferView& vbv)
    {
        return {
            .BufferLocation = vbv.BufferAddress,
            .SizeInBytes = static_cast<UINT>(vbv.BufferSize),
            .StrideInBytes = static_cast<UINT>(vbv.VertexStride)
        };
    }

    D3D12_INDEX_BUFFER_VIEW GraphiteIBVToD3D12IBV(const IndexBufferView& ibv)
    {
        return {
            .BufferLocation = ibv.BufferAddress,
            .SizeInBytes = static_cast<UINT>(ibv.BufferSize),
            .Format = GraphiteFormatToD3D12Format(ibv.IndexFormat)
        };
    }


    D3D12_DESCRIPTOR_RANGE_TYPE GraphiteResourceTypeToD3D12DescriptorRangeType(PipelineResourceType type)
    {
        switch (type)
        {
        case PipelineResourceType::ConstantBufferView: return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        case PipelineResourceType::ShaderResourceView: return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        case PipelineResourceType::UnorderedAccessView: return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
        default:
            GRAPHITE_LOG_ERROR("Invalid resource type!");
            return static_cast<D3D12_DESCRIPTOR_RANGE_TYPE>(-1);
        }
    }

    D3D12_ROOT_PARAMETER_TYPE GraphiteResourceTypeToD3D12RootParameterType(PipelineResourceType type)
    {
        switch (type)
        {
        case PipelineResourceType::ConstantBufferView: return D3D12_ROOT_PARAMETER_TYPE_CBV;
        case PipelineResourceType::ShaderResourceView: return D3D12_ROOT_PARAMETER_TYPE_SRV;
        case PipelineResourceType::UnorderedAccessView: return D3D12_ROOT_PARAMETER_TYPE_UAV;
        default:
            GRAPHITE_LOG_ERROR("Invalid resource type!");
            return static_cast<D3D12_ROOT_PARAMETER_TYPE>(-1);
        }
    }

    D3D12_SHADER_VISIBILITY GraphiteShaderVisibilityToD3D12ShaderVisibility(PipelineResourceShaderVisibility visibility)
    {
        switch (visibility)
        {
        case ShaderVisibility_Vertex: return D3D12_SHADER_VISIBILITY_VERTEX;
        case ShaderVisibility_Hull: return D3D12_SHADER_VISIBILITY_HULL;
        case ShaderVisibility_Domain: return D3D12_SHADER_VISIBILITY_DOMAIN;
        case ShaderVisibility_Geometry: return D3D12_SHADER_VISIBILITY_GEOMETRY;
        case ShaderVisibility_Pixel: return D3D12_SHADER_VISIBILITY_PIXEL;
        case ShaderVisibility_Amplification: return D3D12_SHADER_VISIBILITY_AMPLIFICATION;
        case ShaderVisibility_Mesh: return D3D12_SHADER_VISIBILITY_MESH;
        case ShaderVisibility_All: return D3D12_SHADER_VISIBILITY_ALL;
        default:
        case ShaderVisibility_None:
            GRAPHITE_LOG_FATAL("Invalid shader visibility!") return static_cast<D3D12_SHADER_VISIBILITY>(-1);
        }
    }
}
