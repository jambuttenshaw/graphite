#pragma once

#include "Graphite/RHI/Pipelines/ShaderCompiler.h"


namespace Graphite::D3D12
{
	class D3D12ShaderBytecode : public Graphite::ShaderBytecode
	{
	public:
		// Default constructor creates a null bytecode
		D3D12ShaderBytecode() = default;
		D3D12ShaderBytecode(IDxcBlob* shaderBlob);

		virtual ~D3D12ShaderBytecode() = default;

		DEFAULT_COPY(D3D12ShaderBytecode);
		DEFAULT_MOVE(D3D12ShaderBytecode);

		inline D3D12_SHADER_BYTECODE GetNativeBytecode() const { return m_Bytecode; }

	protected:
		ComPtr<IDxcBlob> m_Blob;
		D3D12_SHADER_BYTECODE m_Bytecode;
	};


	class D3D12ShaderCompiler : public ShaderCompiler
	{
	protected:
		friend class ShaderCompiler;
		D3D12ShaderCompiler();
	public:
		virtual ~D3D12ShaderCompiler() = default;

		DELETE_COPY(D3D12ShaderCompiler);
		DEFAULT_MOVE(D3D12ShaderCompiler);


		bool CompileFromFile(
			const wchar_t* file,
			const wchar_t* entryPoint,
			ShaderType target,
			std::unique_ptr<ShaderBytecode>& outBytecode) const override;

	private:
		ComPtr<IDxcUtils> m_Utils;
		ComPtr<IDxcCompiler3> m_Compiler;
		ComPtr<IDxcIncludeHandler> m_IncludeHandler;
	};
}
