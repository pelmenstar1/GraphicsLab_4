#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"

class Entity3D
{
public:
	Entity3D() : _vao(0), _shader(nullptr), _modelMat(1.0)
	{
		_modelMat = glm::mat4(1.0);
	}

	~Entity3D()
	{
		delete _shader;
	}

	virtual void initShader()
	{
		_shader = new Shader(loadShader());
	}

	virtual void initGeometry() = 0;
	virtual void render(const glm::mat4& projMat, const glm::mat4& viewMat) = 0;
protected:
	glm::mat4 _modelMat;

	Shader* _shader;
	GLuint _vao;

	virtual Shader loadShader() { return Shader(); }
};