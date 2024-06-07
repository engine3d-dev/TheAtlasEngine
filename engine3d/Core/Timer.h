#pragma once
#include <chrono>

namespace Engine3D{
	class Timer{
	public:
		Timer(){
			reset();
		}

		void reset(){
			_start = std::chrono::high_resolution_clock::now();
		}

		float elapsed(){
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - _start).count() * 0.001f * 0.001f * 0.001f;
		}

		float elapsedMilliseconds(){
			return elapsed() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> _start;
	};
};