#include "graphite_pch.h"
#include "ShaderCompiler.h"

#include "Platform/D3D12/Pipelines/D3D12ShaderCompiler.h"


namespace Graphite
{
	ShaderBytecode::ShaderBytecode()
		: m_BufferSize(0)
		, m_BufferPointer(nullptr)
	{
	}

	ShaderBytecode::ShaderBytecode(uint64_t bufferSize, void* bufferPointer)
		: m_BufferSize(bufferSize)
		, m_BufferPointer(bufferPointer)
	{
	}

	ShaderCompiler& ShaderCompiler::Get()
	{
		static D3D12::D3D12ShaderCompiler Instance;
		return Instance;
	}

}
