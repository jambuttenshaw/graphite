#pragma once

#include "Graphite/Core/Core.h"

using Microsoft::WRL::ComPtr;


namespace Graphite
{

	class CommandRecordingContext
	{
		// Recording contexts should only be created by the graphics context
		friend class GraphicsContext;
		CommandRecordingContext(ID3D12Device* device, ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type, const wchar_t* name = nullptr);
	public:
		~CommandRecordingContext() = default;

		DELETE_COPY(CommandRecordingContext);
		DEFAULT_MOVE(CommandRecordingContext);

		// Command recording API


	public:
		// Getters

		ID3D12CommandList* GetCommandList() const { return m_CommandList.Get(); }

		bool IsClosed() const { return m_IsClosed; }

	private:
		// Called by the graphics context
		void Reset(ID3D12CommandAllocator* allocator);
		void Close();

	private:
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		bool m_IsClosed = false;
	};
}
