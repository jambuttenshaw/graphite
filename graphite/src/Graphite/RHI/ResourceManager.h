#pragma once


namespace D3D12MA
{
	class Allocator;
}


namespace Graphite
{
	class ResourceManager
	{
		// Private constructor - must be accessed through Get()
		ResourceManager() = default;
	public:
		static ResourceManager& Get();

		~ResourceManager() = default;

		// Initialization
		void CreateAllocator(IDXGIAdapter* adapter, ID3D12Device* device);
		void DestroyAllocator();

	private:
		D3D12MA::Allocator* m_Allocator = nullptr;
	};
}
