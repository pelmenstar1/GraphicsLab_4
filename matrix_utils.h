#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "rotation.h"

glm::mat4 createRotationMatrix(rotation r)
{
	glm::mat4 rMat;

	float vSin = sin(r.value);
	float vCos = cos(r.value);

	switch (r.type)
	{
	case ROTATION_X:
		rMat = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f, 
			0.0f, vCos, -vSin, 0.0f,
			0.0f, vSin, vCos, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		break;
	case ROTATION_Y:
		rMat = glm::mat4(
			vCos, 0.0f, vSin, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-vSin, 0.0f, vCos, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		break;
	case ROTATION_Z:
		rMat = glm::mat4(
			vCos, -vSin, 0.0f, 0.0f,
			vSin, vCos, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	return rMat;
}