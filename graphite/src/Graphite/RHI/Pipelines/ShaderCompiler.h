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
		ShaderBytecode();
		ShaderBytecode(uint64_t bufferSize, void* bufferPointer);

		virtual ~ShaderBytecode() = default;

		DEFAULT_COPY(ShaderBytecode);
		DEFAULT_MOVE(ShaderBytecode);

		bool IsValid() const { return m_BufferPointer != nullptr; };

		uint64_t GetBufferSize() const { return m_BufferSize; }
		void* GetBufferPointer() const { return m_BufferPointer; }

	protected:
		uint64_t m_BufferSize;
		void* m_BufferPointer;
	};


	class ShaderCompiler
	{
	protected:
		// Lazy initialized singleton
		ShaderCompiler() = default;
	public:
		virtual ~ShaderCompiler() = default;

		DELETE_COPY(ShaderCompiler);
		DEFAULT_MOVE(ShaderCompiler);

	public:

		// Static access to compiler
		static ShaderCompiler& Get();

	public:
		// Shader compiler interface
		inline bool CompileFromFile(
			const ShaderDescription& shader,
			ShaderType target,
			std::unique_ptr<ShaderBytecode>& outBytecode) const
		{
			return CompileFromFile(shader.FilePath, shader.EntryPoint, target, outBytecode);
		}
		virtual bool CompileFromFile(
			const wchar_t* file,
			const wchar_t* entryPoint,
			ShaderType target,
			std::unique_ptr<ShaderBytecode>& outBytecode) const = 0;

	protected:
		uint32_t m_ShaderModelMajor = 6;
		uint32_t m_ShaderModelMinor = 5;
	};

}
