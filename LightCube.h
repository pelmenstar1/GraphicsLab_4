#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include "Entity3D.h"

class LightCube : public Entity3D
{
public:
	virtual void initGeometry();
	virtual void render(const glm::mat4& projMat, const glm::mat4& viewMat);

	void setPosition(glm::vec3 pos);
protected:
	virtual Shader loadShader() { return LOAD_FRAG_VERT_SHADER(light_cube); }
};