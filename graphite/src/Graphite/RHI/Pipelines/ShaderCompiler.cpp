#include "graphite_pch.h"
#include "ShaderCompiler.h"

#include "Core/Application.h"
#include "Core/Platform.h"
#include "Graphite/Core/Assert.h"


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
		auto shaderCompiler = Application::Get()->GetPlatform()->GetShaderCompiler();
		GRAPHITE_ASSERT(shaderCompiler, "Shader compiler has not been created.")
		return *shaderCompiler;
	}

}
