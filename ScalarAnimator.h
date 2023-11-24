#include <cstdint>

class ScalarAnimator
{
public:
	// Starts the animation from the start value
	void start();

	// Starts the animation from the value when the animator was paused.
	void resume();

	// Pauses the animation.
	void pause();

	bool isPaused() { return _isPaused; }

	uint64_t getLoopDuration() const { return _loopDuration; }

	// Sets duration of a loop. The changes will be observed from the next loop.
	void setLoopDuration(uint64_t ms) { _loopDuration = ms; }

	void setStartValue(float value) { 
		_startValue = value; 
	}

	void setEndValue(float value) { _endValue = value; }

	// Returns current animated value. If the animation is ended and isLooped is false, the method returns the end value.
	// If the animation is paused, the method returns the value when the animation was paused.
	float getCurrentValue();
private:
	float _startValue = 0.0f;
	float _endValue = 0.0f;

	uint64_t _elapsedOffsetOnPause = 0;

	// A general duration of a loop.
	uint64_t _loopDuration = 0;

	// Start time of the current loop.
	uint64_t _currentLoopStartTime = 0;

	// Duration of the current loop. It can be different from _loopDurationMs if the latter is changed **during** the loop.
	// The _currentLoopDurationMs should not be changed during the loop.
	uint64_t _currentLoopDuration = 0;

	bool _isPaused = false;
};