#include "pch.h"
#include "GraphicsContext.h"

#include "Exceptions.h"
#include "Graphite/Core/Assert.h"


namespace Graphite
{
	GraphicsContext::GraphicsContext()
	{
		static bool s_GraphicsContextExists = false;
		GRAPHITE_ASSERT(!s_GraphicsContextExists, "Only one graphics context may exist!");
		s_GraphicsContextExists = true;

		GRAPHITE_LOG_INFO("Creating D3D12 Graphics Context...");

		// Initialization
		CreateAdapter();
		CreateDevice();

		GRAPHITE_LOG_INFO("D3D12 Graphics Context created successfully.");
	}

	GraphicsContext::~GraphicsContext()
	{
		
	}

	void GraphicsContext::CreateAdapter()
	{
		UINT dxgiFactoryFlags = 0;

#ifdef GRAPHITE_DEBUG
		{
			// Enable debug layer
			// TODO: Acquire latest debug layer available for improved debugging
			ComPtr<ID3D12Debug> debugController;
			HRESULT result = D3D12GetDebugInterface(IID_PPV_ARGS(debugController.ReleaseAndGetAddressOf()));
			if (SUCCEEDED(result))
			{
				debugController->EnableDebugLayer();
				dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

				GRAPHITE_LOG_INFO("Enabled D3D12 debug layer");
			}
			else
			{
				GRAPHITE_LOG_WARN("Failed to D3D12 enable debug layer!");
				DX_LOG_HR_ERROR(result);
			}
		}
#endif

		// Create factory
		DX_THROW_IF_FAIL(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(m_Factory.ReleaseAndGetAddressOf())));

		// Check tearing support
		{
			BOOL allowTearing = FALSE;
			const HRESULT result = m_Factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
			m_TearingSupport = SUCCEEDED(result) && allowTearing;
		}

		// Select adapter
		for (UINT adapterIndex = 0;
			SUCCEEDED(m_Factory->EnumAdapterByGpuPreference(
				adapterIndex,
				DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
				IID_PPV_ARGS(m_Adapter.ReleaseAndGetAddressOf())));
			++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			m_Adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			// Test to make sure this adapter supports D3D12 - but don't actually create the device yet
			if (SUCCEEDED(
				D3D12CreateDevice(m_Adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)
			))
			{
#ifdef GRAPHITE_DEBUG
				std::wstring deviceInfo;
				deviceInfo += L"Selected Adapter:\n\t";
				deviceInfo += desc.Description;
				deviceInfo += L"\n\tAvailable Dedicated Video Memory: ";
				deviceInfo += std::to_wstring(desc.DedicatedVideoMemory / 1000000000.f);
				deviceInfo += L" GB";
				GRAPHITE_LOG_INFO(deviceInfo.c_str());
#endif

				// Satisfactory adapter has been found
				break;
			}
		}

		// TODO: This shouldn't be a debug only assert - this should terminate the application in release
		GRAPHITE_ASSERT(m_Adapter, "Failed to find any adapter!");
	}

	void GraphicsContext::CreateDevice()
	{
		DX_THROW_IF_FAIL(D3D12CreateDevice(m_Adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(m_Device.ReleaseAndGetAddressOf())));

#ifdef GRAPHITE_DEBUG
		// Create info queue to receive debug callback
		// Debug layer must be enabled for this to succeed
		HRESULT result = m_Device->QueryInterface(IID_PPV_ARGS(m_InfoQueue.ReleaseAndGetAddressOf()));
		if (SUCCEEDED(result))
		{
			// TODO: Setup message callback

			DX_THROW_IF_FAIL(m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true));
			DX_THROW_IF_FAIL(m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true));
			DX_THROW_IF_FAIL(m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true));
		}
		else
		{
			DX_LOG_HR_ERROR(result);
			GRAPHITE_LOG_WARN("Failed to create Info Queue. D3D12 messages and TDR debug information will not be received.");
		}
#endif
	}
}
