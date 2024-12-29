#include "graphite_pch.h"
#include "ShaderCompiler.h"

#include "Graphite/Core/Assert.h"


namespace Graphite::D3D12
{
	extern ShaderCompiler* CreateD3D12ShaderCompiler();
}


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


	std::unique_ptr<ShaderCompiler> s_ShaderCompiler;

	void ShaderCompiler::CreateShaderCompiler()
	{
		GRAPHITE_ASSERT(!s_ShaderCompiler, "Only one shader compiler can exist. Call ShaderCompiler::Get() to retrieve the shader compiler.");

		s_ShaderCompiler = std::unique_ptr<ShaderCompiler>(D3D12::CreateD3D12ShaderCompiler());
	}

	void ShaderCompiler::DestroyShaderCompiler()
	{
		s_ShaderCompiler.reset();
	}

	ShaderCompiler& ShaderCompiler::Get()
	{
		GRAPHITE_ASSERT(s_ShaderCompiler, "Shader compiler has not been created. Call ShaderCompiler::CreateShaderCompiler() first.")
		return *s_ShaderCompiler;
	}

}
