#pragma once

#include "Core.h"
#include <memory>


namespace Graphite
{

	class GRAPHITE_API Application
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		virtual void Run();

	private:
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

	};

	extern std::unique_ptr<Application> CreateApplication();

}