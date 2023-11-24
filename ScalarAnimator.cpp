#include "ScalarAnimator.h"
#include <chrono>
#include <glm/glm.hpp>
#include "time_utils.h"

void ScalarAnimator::start()
{
	_currentLoopDuration = _loopDuration;
	_currentLoopStartTime = getCurrentTimeMillis();
	_elapsedOffsetOnPause = 0;
	
	_isPaused = false;
}

void ScalarAnimator::pause()
{
	if (_isPaused) return;

	uint64_t now = getCurrentTimeMillis();
	uint64_t elapsed = now - _currentLoopStartTime;

	_elapsedOffsetOnPause = std::min(_elapsedOffsetOnPause + elapsed, _currentLoopDuration);
	_isPaused = true;
}

void ScalarAnimator::resume()
{
	_currentLoopDuration = _loopDuration;
	_currentLoopStartTime = getCurrentTimeMillis();

	_isPaused = false;
}

float ScalarAnimator::getCurrentValue()
{
	if (_isPaused)
	{
		return glm::mix(_startValue, _endValue, (float)_elapsedOffsetOnPause / (float)_currentLoopDuration);
	}

	uint64_t now  = getCurrentTimeMillis();
	uint64_t elapsed = now - _currentLoopStartTime + _elapsedOffsetOnPause;

	if (elapsed >= _currentLoopDuration)
	{
		// Restart the loop.
		_currentLoopStartTime = now;
		_currentLoopDuration = _loopDuration;
		_elapsedOffsetOnPause = 0;

		return _startValue;
	}
	else
	{
		float fraction = (float)elapsed / (float)_currentLoopDuration;

		return glm::mix(_startValue, _endValue, fraction);
	}
}