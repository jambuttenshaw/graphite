#pragma once

#include "Core.h"


namespace Graphite
{
	namespace Log
	{
		extern void InitLogging();
		
		GRAPHITE_API extern std::shared_ptr<spdlog::logger>& GetEngineLogger();
		GRAPHITE_API extern std::shared_ptr<spdlog::logger>& GetAppLogger();
	}
}


// Convenience logging macros

#ifdef GRAPHITE_DEBUG

#define GRAPHITE_LOG_FATAL(...)		{::Graphite::Log::GetEngineLogger()->critical(__VA_ARGS__); throw;}
#define GRAPHITE_LOG_ERROR(...)		 ::Graphite::Log::GetEngineLogger()->error(__VA_ARGS__);
#define GRAPHITE_LOG_WARN(...)		 ::Graphite::Log::GetEngineLogger()->warn(__VA_ARGS__);
#define GRAPHITE_LOG_INFO(...)		 ::Graphite::Log::GetEngineLogger()->info(__VA_ARGS__);
#define GRAPHITE_LOG_TRACE(...)		 ::Graphite::Log::GetEngineLogger()->trace(__VA_ARGS__);

#define APP_LOG_FATAL(...)			{::Graphite::Log::GetAppLogger()->critical(__VA_ARGS__); throw;}
#define APP_LOG_ERROR(...)			 ::Graphite::Log::GetAppLogger()->error(__VA_ARGS__);
#define APP_LOG_WARN(...)			 ::Graphite::Log::GetAppLogger()->warn(__VA_ARGS__);
#define APP_LOG_INFO(...)			 ::Graphite::Log::GetAppLogger()->info(__VA_ARGS__);
#define APP_LOG_TRACE(...)			 ::Graphite::Log::GetAppLogger()->trace(__VA_ARGS__);

#else

#define GRAPHITE_LOG_FATAL(...)		
#define GRAPHITE_LOG_ERROR(...)		
#define GRAPHITE_LOG_WARN(...)		
#define GRAPHITE_LOG_INFO(...)		
#define GRAPHITE_LOG_TRACE(...)		

#define APP_LOG_FATAL(...)			
#define APP_LOG_ERROR(...)			
#define APP_LOG_WARN(...)			
#define APP_LOG_INFO(...)			
#define APP_LOG_TRACE(...)			

#endif
