#pragma once

#include <cstdint>
#include "glm/glm.hpp"

class ColorAnimator
{
public:
	ColorAnimator() : _duration(0) {}
	ColorAnimator(uint64_t duration) : _duration(duration) {}

	void setStartColor(glm::vec3 color) { _startColor = color; }
	void setEndColor(glm::vec3 color) { _endColor = color; }
	void setDuration(uint64_t duration) { _duration = duration; }

	void start();

	glm::vec3 getColor(bool* isRunning);
private:
	uint64_t _startTime = 0;
	uint64_t _duration;

	glm::vec3 _startColor = glm::vec3(0);
	glm::vec3 _endColor = glm::vec3(0);
};