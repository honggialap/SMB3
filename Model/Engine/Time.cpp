#pragma region INCLUDE
#include "Time.h"
#pragma endregion

void CTime::StartClock() {
	_elapsedMs = 0.0f;
	_totalElapsedMs = 0.0f;

	_startTimeStamp = _clock.now();
	_previousTickTimeStamp = _startTimeStamp;
	_currentTickTimeStamp = _startTimeStamp;
}

void CTime::TickClock() {
	_previousTickTimeStamp = _currentTickTimeStamp;
	_currentTickTimeStamp = _clock.now();

	_elapsedMs = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>
		(_currentTickTimeStamp - _previousTickTimeStamp).count();

	_totalElapsedMs = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>
		(_currentTickTimeStamp - _startTimeStamp).count();
}
