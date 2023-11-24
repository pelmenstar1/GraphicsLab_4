#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Entity3D.h"

class Axis3D : public Entity3D {
public:
	Axis3D(float length) : _length(length) {}

	virtual void initGeometry();
	virtual void render(const glm::mat4& projMat, const glm::mat4& viewMat);
private:
	float _length;
protected:
	virtual Shader loadShader() { return LOAD_FRAG_VERT_SHADER(axis); }
};