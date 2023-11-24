#pragma once

#include "shader_utils.h"
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#define LOAD_FRAG_VERT_SHADER(name) \
  Shader::load(#name, GET_VERT_SHADER_CONTENT(name), GET_FRAG_SHADER_CONTENT(name))

class Shader
{
public:
	Shader() : _program(0) {}
	Shader(GLuint program) : _program(program) {}

	void setUniformValue(const char* name, GLint value)
	{
		glUniform1i(getLocation(name), value);
	}

	void setUniformMatrix(const char* name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void setUniformVector(const char* name, const glm::vec3& vector)
	{
		glUniform3fv(getLocation(name), 1, glm::value_ptr(vector));
	}

	void setUniformVector(const char* name, float x, float y, float z)
	{
		glUniform3f(getLocation(name), x, y, z);
	}

	void use() { glUseProgram(_program); }

	static Shader load(const char* programDebugName,
		const char* vertShaderContent,
		const char* fragShaderContent)
	{
		return Shader(loadShaderProgram(programDebugName, vertShaderContent, fragShaderContent));
	}

private:
	GLuint _program;

	GLint getLocation(const char* name)
	{
		return glGetUniformLocation(_program, name);
	}
};