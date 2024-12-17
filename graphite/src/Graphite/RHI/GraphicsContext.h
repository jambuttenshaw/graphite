#pragma once
#include "Graphite/Core/Core.h"


namespace Graphite
{
	using Microsoft::WRL::ComPtr;

	class GraphicsContext
	{
	public:
		GraphicsContext();
		~GraphicsContext();

		DELETE_COPY(GraphicsContext);
		DEFAULT_MOVE(GraphicsContext);

	private:
		// Initialization
		void CreateAdapter();
		void CreateDevice();

	private:
		ComPtr<IDXGIFactory6> m_Factory;
		ComPtr<IDXGIAdapter1> m_Adapter;

		ComPtr<ID3D12Device> m_Device;
		ComPtr<ID3D12InfoQueue1> m_InfoQueue;

		// Feature support
		bool m_TearingSupport = false;
	};

}
