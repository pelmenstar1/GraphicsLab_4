#pragma once

#include <vector>
#include <cstdint>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Entity3D.h"
#include "rotation.h"

constexpr uint32_t TORUS_MODE_COUNT = 2;

enum TorusMode
{
	TORUS_SOLID_COLOR = 0,
	TORUS_TEXTURE = 1
};

class Torus : public Entity3D
{
public:
	Torus(float r, float R, uint32_t nr, uint32_t nR) : _r(r), _R(R), _nr(nr), _nR(nR)
	{
	}

	~Torus()
	{
		delete[] _vertices;
		delete[] _normals;
	}

	void compute();

	virtual void initGeometry();

	virtual void render(const glm::mat4& projMat, const glm::mat4& viewMat);

	rotation getRotation() { return _rotation; }
	void setRotation(rotation value);

	glm::vec3 getMaterialColor() const { return _materialColor; }
	void setMaterialColor(glm::vec3 color) { _materialColor = color; }

	glm::vec3 getLightPos() const { return _lightPos; }
	void setLightPos(glm::vec3 pos) { _lightPos = pos; }

	glm::vec3 getEyePos() const { return _eyePos; }
	void setEyePos(glm::vec3 pos) { _eyePos = pos; }

	TorusMode getMode() const { return _mode; }
	void setMode(TorusMode value);

	void clearComptued()
	{
		_isComputed = false;
		_isBuffersInitialized = false;
	}

	virtual void initShader()
	{
		if (_solidShader == nullptr)
		{
			_solidShader = new Shader(LOAD_FRAG_VERT_SHADER(torus));
		}

		_shader = _solidShader;
	}
private:
	const float _r;
	const float _R;
	const uint32_t _nr;
	const uint32_t _nR;

	TorusMode _mode = TORUS_SOLID_COLOR;

	size_t _vertexArraySize = 0;
	size_t _texCoordsSize = 0;

	float* _vertices = nullptr;
	float* _normals = nullptr;
	float* _texCoords = nullptr;

	GLuint _texture = 0;
	bool _isTextureLoaded = false;

	rotation _rotation = { };

	glm::vec3 _materialColor = glm::vec3(0);
	glm::vec3 _lightPos = glm::vec3(0);
	glm::vec3 _eyePos = glm::vec3(0);

	bool _isComputed = false;
	bool _isBuffersInitialized = false;

	Shader* _solidShader = nullptr;
	Shader* _textureShader = nullptr;

	void loadTexture();
};