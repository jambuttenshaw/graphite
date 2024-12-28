#include "graphite_pch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>


namespace Graphite
{
	namespace Log
	{
		std::shared_ptr<spdlog::logger> s_EngineLogger = nullptr;
		std::shared_ptr<spdlog::logger> s_AppLogger = nullptr;

		void InitLogging()
		{
			spdlog::set_pattern("%^[%T] %n: %v%$");

			// Create D3D logger
			{
				s_EngineLogger = spdlog::stdout_color_mt("Engine");
				s_EngineLogger->set_level(spdlog::level::trace);

				spdlog::sinks::stderr_color_sink_mt* color = static_cast<spdlog::sinks::stderr_color_sink_mt*>(s_EngineLogger->sinks().back().get());
				color->set_color(spdlog::level::level_enum::info, 0x0003);	// first 8 bits not relevant for us - background RGBA - RGBA -> 1 blue and 1 alpha
			}

			// Create app logger
			{
				s_AppLogger = spdlog::stdout_color_mt("App");
				s_AppLogger->set_level(spdlog::level::info);

				spdlog::sinks::stderr_color_sink_mt* color = static_cast<spdlog::sinks::stderr_color_sink_mt*>(s_AppLogger->sinks().back().get());
				color->set_color(spdlog::level::level_enum::info, 0x0003);	// first 8 bits not relevant for us - background RGBA - RGBA -> 1 blue and 1 alpha
			}
		}

		std::shared_ptr<spdlog::logger>& GetEngineLogger()
		{
			return s_EngineLogger;
		}

		std::shared_ptr<spdlog::logger>& GetAppLogger()
		{
			return s_AppLogger;
		}
	}
}