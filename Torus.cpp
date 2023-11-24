#define _USE_MATH_DEFINES

#include "Torus.h"
#include <cmath>
#include "shader_utils.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/geometric.hpp"
#include "matrix_utils.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

inline void setVertexToArray(float* array, float x, float y, float z)
{
	array[0] = x;
	array[1] = y;
	array[2] = z;
}

inline void addVertexAndNormal(
	float* vertices, float* normals, 
	float vertexComp, float z,
	float uSin, float uCos,
	float vSin, float vCos)
{
	// compute vertex
	float x = vertexComp * uCos;
	float y = vertexComp * uSin;

	// compute normal 
	float nx = vCos * uCos;
	float ny = vCos * uSin;
	float nz = vSin;

	setVertexToArray(vertices, x, y, z);
	setVertexToArray(normals, nx, ny, nz);
}

void Torus::compute()
{
	if (_isComputed)
	{
		return;
	}

	_isComputed = true;

	_vertexArraySize = (size_t)_nR * ((size_t)_nr + 1) * 3 * 2;
	_texCoordsSize = (size_t)_nR * ((size_t)_nr + 1) * 2 * 2;

	if (_vertices == nullptr)
	{
		_vertices = new float[_vertexArraySize];
		_normals = new float[_vertexArraySize];
		_texCoords = new float[_texCoordsSize];
	}

	// A torus is given by the paramteric equations:
	// x(u, v) = (R + r * cos(v)) * cos(u)
	// y(u, v) = (R + r * cos(v)) * sin(u)
	// z(v) = r * sin(v)
	// u, v => [0, 2pi]

	size_t vertexArrayOffset = 0;
	size_t texArrayOffset = 0;

	float du = (float)(2 * M_PI) / _nR;
	float dv = (float)(2 * M_PI) / _nr;

	float u = 0.0f;

	for (uint32_t i = 0; i < _nR; i++)
	{
		float uu = u + du;
		float uSin = sin(u);
		float uCos = cos(u);

		float uuSin = sin(uu);
		float uuCos = cos(uu);

		// Texture x-coordinates
		float tx1 = u / (float)(2 * M_PI);
		float tx2 = uu / (float)(2 * M_PI);

		float v = 0.0f;
		size_t startVertexArrayOffset = vertexArrayOffset;

		for (uint32_t j = 0; j < _nr; j++)
		{
			float ty = v / (float)(2 * M_PI);

			float vSin = sin(v);
			float vCos = cos(v);

			float vertexComp = _R + _r * vCos;
			float z = _r * vSin;

			addVertexAndNormal(
				_vertices + vertexArrayOffset, _normals + vertexArrayOffset,
				vertexComp, z,
				uSin, uCos,
				vSin, vCos);

			addVertexAndNormal(
				_vertices + vertexArrayOffset + 3, _normals + vertexArrayOffset + 3,
				vertexComp, z,
				uuSin, uuCos,
				vSin, vCos);

			_texCoords[texArrayOffset] = tx1;
			_texCoords[texArrayOffset + 1] = ty;
			_texCoords[texArrayOffset + 2] = tx2;
			_texCoords[texArrayOffset + 3] = ty;

			vertexArrayOffset += 6;
			texArrayOffset += 4;

			v += dv;
		}

		// Copy first two vertices and normals to the end.
		std::memcpy(_vertices + vertexArrayOffset, _vertices + startVertexArrayOffset, 6 * sizeof(float));
		std::memcpy(_normals + vertexArrayOffset, _normals + startVertexArrayOffset, 6 * sizeof(float));

		// Y-coordinate for the last iteration is 1
		_texCoords[texArrayOffset] = tx1;
		_texCoords[texArrayOffset + 1] = 1.0;
		_texCoords[texArrayOffset + 2] = tx2;
		_texCoords[texArrayOffset + 3] = 1.0;

		vertexArrayOffset += 6;
		texArrayOffset += 4;

		u = uu;
	}
}

void Torus::setMode(TorusMode mode)
{
	_mode = mode;

	switch (mode)
	{
	case TORUS_SOLID_COLOR:
		_shader = _solidShader;
		break;
	case TORUS_TEXTURE:
		if (!_isTextureLoaded)
		{
			loadTexture();
			_textureShader = new Shader(LOAD_FRAG_VERT_SHADER(torus_tex));
		}

		_shader = _textureShader;
	}
}

void Torus::initGeometry()
{
	if (_isBuffersInitialized)
	{
		return;
	}

	_isBuffersInitialized = true;

	compute();

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	GLuint buffer[3];

	// create buffers
	glCreateBuffers(3, buffer);

	// vertices 
	glNamedBufferStorage(buffer[0], sizeof(float) * _vertexArraySize, _vertices, 0);
	glVertexArrayVertexBuffer(_vao, 0, buffer[0], 0, 3 * sizeof(float));
	glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(_vao, 0, 0);
	glEnableVertexArrayAttrib(_vao, 0);

	// normals 
	glNamedBufferStorage(buffer[1], sizeof(float) * _vertexArraySize, _normals, 0);
	glVertexArrayVertexBuffer(_vao, 1, buffer[1], 0, 3 * sizeof(float));
	glVertexArrayAttribFormat(_vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(_vao, 1, 1);
	glEnableVertexArrayAttrib(_vao, 1);

	// texture coordinates
	glNamedBufferStorage(buffer[2], sizeof(float) * _texCoordsSize, _texCoords, 0);
	glVertexArrayVertexBuffer(_vao, 2, buffer[2], 0, 2 * sizeof(float));
	glVertexArrayAttribFormat(_vao, 2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(_vao, 2, 2);
	glEnableVertexArrayAttrib(_vao, 2);

	glBindVertexArray(0);
}

void Torus::loadTexture()
{
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	

	int width, height, comp;
	
	stbi_uc* data = stbi_load("./iron.jpg", &width, &height, &comp, 0);
	if (!data)
	{
		std::cerr << "Cannot load iron.png texture";
		exit(EXIT_FAILURE);
	}

	GLenum format;

	switch (comp)
	{
	case 1:
		format = GL_RED;
		break;
	case 4:
		format = GL_RGBA;
		break;
	default:
		format = GL_RGB;
	}

	if (comp == 1)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	_isTextureLoaded = true;

	stbi_image_free(data);
}

void Torus::render(const glm::mat4& projMat, const glm::mat4& viewMat)
{
	// compute normal matrix 
	// It's the transpose of the inverse of the model matrix 
	glm::mat4 normalMat = glm::transpose(glm::inverse(_modelMat));

	_shader->use();
	_shader->setUniformMatrix("normalMat", normalMat);
	_shader->setUniformMatrix("modelMat", normalMat);
	_shader->setUniformMatrix("viewMat", viewMat);
	_shader->setUniformMatrix("projMat", projMat);
	_shader->setUniformVector("materialColor", _materialColor);
	_shader->setUniformVector("lightPos", _lightPos);
	_shader->setUniformVector("eyePos", _eyePos);

	if (_mode == TORUS_TEXTURE)
	{
		assert(_isTextureLoaded);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture);

		_shader->setUniformValue("sampler", 0);
	}

	glBindVertexArray(_vao);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)(_vertexArraySize / 3));
}

void Torus::setRotation(rotation value)
{
	_rotation = value;
	_modelMat = createRotationMatrix(value);
}