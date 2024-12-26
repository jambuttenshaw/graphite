#pragma once

#include "Graphite/Core/Core.h"

#include "dxcapi.h"

using Microsoft::WRL::ComPtr;


namespace Graphite
{

	enum class ShaderType : uint8_t
	{
		Vertex = 0,
		Hull,
		Domain,
		Geometry,
		Pixel,
		Compute,
		Library
	};

	struct ShaderDescription
	{
		// TODO: This could also allow for directly using pre-compiled shaders with the same structure
		const wchar_t* FilePath = nullptr;
		const wchar_t* EntryPoint = nullptr;

		operator bool() const
		{
			return (FilePath != nullptr && EntryPoint != nullptr);
		}
	};

	class ShaderBytecode
	{
	public:
		// Default constructor creates a null bytecode
		ShaderBytecode();
		ShaderBytecode(IDxcBlob* shaderBlob);

		~ShaderBytecode() = default;

		DEFAULT_COPY(ShaderBytecode);
		DEFAULT_MOVE(ShaderBytecode);

		inline bool IsValid() const { return m_Bytecode.pShaderBytecode != nullptr; }
		inline D3D12_SHADER_BYTECODE GetBytecode() const { return m_Bytecode; }

	public:
		// Null bytecode object when a shader type is not desired in a pipeline
		static ShaderBytecode NullShaderBytecode;

	private:
		ComPtr<IDxcBlob> m_Blob;
		D3D12_SHADER_BYTECODE m_Bytecode;
	};


	class ShaderCompiler
	{
		// Lazy initialized singleton
		ShaderCompiler();
	public:
		static ShaderCompiler& Get();

		// Shader compiler interface
		inline ShaderBytecode CompileFromFile(
			const ShaderDescription& shader,
			ShaderType target) const
		{
			return CompileFromFile(shader.FilePath, shader.EntryPoint, target);
		}
		ShaderBytecode CompileFromFile(
			const wchar_t* file,
			const wchar_t* entryPoint,
			ShaderType target) const;

	private:
		ComPtr<IDxcUtils> m_Utils;
		ComPtr<IDxcCompiler3> m_Compiler;
		ComPtr<IDxcIncludeHandler> m_IncludeHandler;

		uint32_t m_ShaderModelMajor = 6;
		uint32_t m_ShaderModelMinor = 5;
	};

}
