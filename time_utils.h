#pragma once

#include <chrono>
#include <cstdint>

static uint64_t getCurrentTimeMillis()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();
}