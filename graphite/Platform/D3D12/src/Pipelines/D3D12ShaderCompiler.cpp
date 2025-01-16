#include "graphite_d3d12_pch.h"
#include "D3D12ShaderCompiler.h"

#include "D3D12Exceptions.h"


namespace Graphite::D3D12
{

	static std::wstring CreateTargetStr(ShaderType type, uint32_t shaderModelMajor, uint32_t shaderModelMinor)
	{
		std::wstring target;
		switch (type)
		{
		case ShaderType::Vertex:	target = L"vs";  break;
		case ShaderType::Hull:		target = L"hs";	 break;
		case ShaderType::Domain:	target = L"ds";	 break;
		case ShaderType::Geometry:	target = L"gs";	 break;
		case ShaderType::Pixel:		target = L"ps";	 break;
		case ShaderType::Compute:	target = L"cs";	 break;
		case ShaderType::Library:	target = L"lib"; break;
		default:
			GRAPHITE_LOG_ERROR("Invalid shader type: {}", static_cast<int>(type));
			break;
		}

		// TODO: Ensure shader model is a model supported by this device
		// Create extension for shader model
		target += L"_" + std::to_wstring(shaderModelMajor) + L"_" + std::to_wstring(shaderModelMinor);

		return target;
	}


	D3D12ShaderBytecode::D3D12ShaderBytecode(IDxcBlob* shaderBlob)
		: ShaderBytecode(
			shaderBlob->GetBufferSize(),
			shaderBlob->GetBufferPointer()
		)
	{
		m_Blob.Attach(shaderBlob);
		m_Bytecode = { m_BufferPointer, m_BufferSize };
	}


	D3D12ShaderCompiler::D3D12ShaderCompiler()
	{
		// Create shader compiler object
		DX_THROW_IF_FAIL(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&m_Utils)));
		DX_THROW_IF_FAIL(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&m_Compiler)));

		DX_THROW_IF_FAIL(m_Utils->CreateDefaultIncludeHandler(&m_IncludeHandler));
	}

	bool D3D12ShaderCompiler::CompileFromFile(const wchar_t* file, const wchar_t* entryPoint, ShaderType target, std::unique_ptr<ShaderBytecode>& outBytecode) const
	{
		if (!file || !entryPoint)
		{
			GRAPHITE_LOG_WARN("Null shader description passed to CompileFromFile. Consider checking for unnecessary shader compilation at an earlier stage.");
			return {};
		}

		// format target
		std::wstring targetStr = CreateTargetStr(target, m_ShaderModelMajor, m_ShaderModelMinor);

		std::vector<LPCWSTR> args;
		args.push_back(file);

#ifdef _DEBUG
		args.push_back(L"-Zi");
		args.push_back(L"-Od");
		args.push_back(L"-Qembed_debug");
#else
		args.push_back(L"-Qstrip_debug");
		args.push_back(L"-O3");
#endif

		args.push_back(L"-E");
		args.push_back(entryPoint);

		args.push_back(L"-T");
		args.push_back(targetStr.c_str());

		args.push_back(L"-Qstrip_reflect");
		args.push_back(L"-enable-16bit-types");

		// Open source file.  
		ComPtr<IDxcBlobEncoding> pSource = nullptr;
		HRESULT result = m_Utils->LoadFile(file, nullptr, &pSource);

		// Check for success
		if (FAILED(result))
		{
			GRAPHITE_LOG_ERROR(L"Failed to find shader file: {0}", file);
			return false;
		}

		DxcBuffer Source;
		Source.Ptr = pSource->GetBufferPointer();
		Source.Size = pSource->GetBufferSize();
		Source.Encoding = DXC_CP_ACP;

		ComPtr<IDxcResult> pResults;
		m_Compiler->Compile(
			&Source,								// Source buffer.
			args.data(),							// Array of pointers to arguments.
			static_cast<UINT32>(args.size()),		// Number of arguments.
			m_IncludeHandler.Get(),					// User-provided interface to handle #include directives (optional).
			IID_PPV_ARGS(&pResults)					// Compiler output status, buffer, and errors.
		);

		ComPtr<IDxcBlobUtf8> pErrors = nullptr;
		pResults->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&pErrors), nullptr);
		// Note that d3dcompiler would return null if no errors or warnings are present.
		// IDxcCompiler3::Compile will always return an error buffer, but its length
		// will be zero if there are no warnings or errors.
		if (pErrors != nullptr && pErrors->GetStringLength() != 0)
		{
			GRAPHITE_LOG_WARN("Warnings and Errors:\n{0}", pErrors->GetStringPointer());
		}

		//
		// Quit if the compilation failed.
		//
		pResults->GetStatus(&result);
		if (FAILED(result))
		{
			GRAPHITE_LOG_ERROR("Shader compilation failed.");
			return false;
		}


		ComPtr<IDxcBlob> blob;
		ComPtr<IDxcBlobUtf16> pShaderName = nullptr;
		result = pResults->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(blob.GetAddressOf()), &pShaderName);
		if (FAILED(result))
		{
			GRAPHITE_LOG_ERROR("Failed to get compiled shader object.");
			return false;
		}

		outBytecode = std::make_unique<D3D12ShaderBytecode>(blob.Detach());
		return true;
	}
}
