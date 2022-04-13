#pragma once
#ifndef __TIME_H__
#define __TIME_H__

#pragma region INCLUDE
#include <chrono>
#pragma endregion

class CTime {
	/* Chronos */
private:
	std::chrono::steady_clock _clock;
	std::chrono::steady_clock::time_point _startTimeStamp;
	std::chrono::steady_clock::time_point _previousTickTimeStamp;
	std::chrono::steady_clock::time_point _currentTickTimeStamp;

	float _elapsedMs = 0.0f;
	float _totalElapsedMs = 0.0f;

public:
	float GetElapsedMs() { return _elapsedMs; }
	float GetTotalElapsedMs() { return _totalElapsedMs; }

public:
	void StartClock();
	void TickClock();
};
typedef CTime* pTime;

#endif // !__TIME_H__