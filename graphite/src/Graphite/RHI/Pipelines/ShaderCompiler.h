#pragma once

#include "Graphite/Core/Core.h"


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
		GRAPHITE_API ShaderBytecode();
		GRAPHITE_API ShaderBytecode(uint64_t bufferSize, void* bufferPointer);

		GRAPHITE_API virtual ~ShaderBytecode() = default;

		GRAPHITE_API_DEFAULT_COPY(ShaderBytecode);
		GRAPHITE_API_DEFAULT_MOVE(ShaderBytecode);

		GRAPHITE_API bool IsValid() const { return m_BufferPointer != nullptr; };

		GRAPHITE_API uint64_t GetBufferSize() const { return m_BufferSize; }
		GRAPHITE_API void* GetBufferPointer() const { return m_BufferPointer; }

	protected:
		uint64_t m_BufferSize;
		void* m_BufferPointer;
	};


	class ShaderCompiler
	{
	protected:
		// Lazy initialized singleton
		GRAPHITE_API ShaderCompiler() = default;
	public:
		GRAPHITE_API virtual ~ShaderCompiler() = default;

		GRAPHITE_API_DELETE_COPY(ShaderCompiler);
		GRAPHITE_API_DEFAULT_MOVE(ShaderCompiler);

	public:
		GRAPHITE_API static ShaderCompiler& Get();

	public:
		// Shader compiler interface
		GRAPHITE_API inline bool CompileFromFile(
			const ShaderDescription& shader,
			ShaderType target,
			std::unique_ptr<ShaderBytecode>& outBytecode) const
		{
			return CompileFromFile(shader.FilePath, shader.EntryPoint, target, outBytecode);
		}
		GRAPHITE_API virtual bool CompileFromFile(
			const wchar_t* file,
			const wchar_t* entryPoint,
			ShaderType target,
			std::unique_ptr<ShaderBytecode>& outBytecode) const = 0;

	protected:
		uint32_t m_ShaderModelMajor = 6;
		uint32_t m_ShaderModelMinor = 5;
	};

}
