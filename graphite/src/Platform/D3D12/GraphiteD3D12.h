#pragma once


// This header is safe to include into Core Graphite,
// and exposes the required functionality to interact with the D3D12 RHI for Graphite

namespace Graphite::D3D12
{
	extern class GraphicsContext*	CreateD3D12GraphicsContext	(const struct GraphiteGraphicsContextDesc& contextDesc);
	extern class ResourceFactory*	CreateD3D12ResourceFactory	(const class GraphicsContext&);
	extern class GraphicsPipeline*	CreateD3D12GraphicsPipeline	(const GraphicsContext&, const struct GraphicsPipelineDescription&);
	extern class ShaderCompiler*	CreateD3D12ShaderCompiler	();

	extern class ImGuiBackend*		CreateD3D12ImGuiBackend		(const GraphicsContext&, const class DescriptorAllocation&);
}
