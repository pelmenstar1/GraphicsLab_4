#include "Axis3D.h"

#include "shader_utils.h"
#include "glm/gtc/type_ptr.hpp"

void Axis3D::initGeometry() {
	glCreateVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	static const float pos[] = {
		0.0f, 0.0f, 0.0f,
		_length, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, _length, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, _length
	};

	GLuint buffer;

	glCreateBuffers(1, &buffer);
	glNamedBufferStorage(buffer, sizeof(pos), pos, 0);
	glVertexArrayVertexBuffer(_vao, 0, buffer, 0, 3 * sizeof(float));
	glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(_vao, 0, 0);
	glEnableVertexArrayAttrib(_vao, 0);

	glBindVertexArray(0);
}

void Axis3D::render(const glm::mat4& projMat, const glm::mat4& viewMat) {
	_shader->use();
	_shader->setUniformMatrix("pMat", projMat);
	_shader->setUniformMatrix("vMat", viewMat);

	glBindVertexArray(_vao);

	glPointSize(10.0);

	// draw X axis
	_shader->setUniformVector("uColor", 1.0, 0.0, 0.0);
	glDrawArrays(GL_LINES, 0, 2);

	// draw Y axis
	_shader->setUniformVector("uColor", 0.0, 1.0, 0.0);
	glDrawArrays(GL_LINES, 2, 2);

	// draw Z axis
	_shader->setUniformVector("uColor", 0.0, 0.0, 1.0);
	glDrawArrays(GL_LINES, 4, 2);
}