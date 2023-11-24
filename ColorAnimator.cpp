#include "ColorAnimator.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/compatibility.hpp"
#include "time_utils.h"

void ColorAnimator::start() {
	_startTime = getCurrentTimeMillis();
}

glm::vec3 ColorAnimator::getColor(bool* isRunning) {
	uint64_t nowMs = getCurrentTimeMillis();
	uint64_t elapsed = nowMs - _startTime;

	if (elapsed >= _duration) {
		*isRunning = false;
		return _endColor;
	}

	*isRunning = true;

	float fraction = (float)elapsed / (float)_duration;

	// RGB lerp
	return glm::mix(_startColor, _endColor, fraction);
}