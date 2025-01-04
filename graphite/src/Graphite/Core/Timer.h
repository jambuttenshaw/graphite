#pragma once


namespace Graphite
{
	class Timer
	{
	public:
		Timer();

		inline float GetTimeSinceReset() const { return static_cast<float>(m_TimeSinceReset); }	// In seconds
		inline float GetDeltaTime() const { return static_cast<float>(m_DeltaTime); }			// In seconds
		inline float GetFPS() const { return static_cast<float>(m_FPS); }

		void Reset();

		// Returns time since last tick (delta time)
		float Tick();
	private:
		double m_SecondsPerCount = 0.0;

		double m_DeltaTime = 0.0;
		double m_TimeSinceReset = 0.0;

		int64_t m_CountsOnReset = 0;
		int64_t m_LastTickCounts = 0;
		int64_t m_CurrentCounts = 0;

		int64_t m_CountsOnLastFPS = 0;
		int32_t m_FPSFrameCounter = 0;
		double m_FPS = 0.0;
		double m_FPSUpdateRate = 1.0; // Recalculate fps every second
	};

}
