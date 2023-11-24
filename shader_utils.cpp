#include "shader_utils.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <format>
#include <map>

template<typename getIvFn, typename getInfoLogFn, typename deleteEntityFn>
[[noreturn]] static void printInfoLogAndExit(
	const std::string& actionDesc,
	GLuint id,
	getIvFn getIv,
	getInfoLogFn getInfoLog,
	deleteEntityFn deleteEntity)
{
	GLint maxLength = 0;
	getIv(id, GL_INFO_LOG_LENGTH, &maxLength);

	// The maxLength includes the NULL character
	GLchar* pInfoLog = new GLchar[maxLength];
	getInfoLog(id, maxLength, &maxLength, pInfoLog);

	std::string strLog(pInfoLog, maxLength);
	std::cerr << actionDesc << std::endl;
	std::cerr << strLog << std::endl;

	delete[] pInfoLog;

	deleteEntity(id);

	exit(EXIT_FAILURE);
}

static GLuint loadShader(const char* programDebugName, const char* shaderDebugType, GLenum shaderType, const char* shaderContent)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderContent, NULL);
	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		std::string actionDesc = std::format("Compiling {} ({})", programDebugName, shaderDebugType);

		printInfoLogAndExit(actionDesc, shader, glGetShaderiv, glGetShaderInfoLog, glDeleteShader);
	}

	return shader;
}

GLuint loadShaderProgram(const char* programDebugName, const char* vertShaderContent, const char* fragShaderContent)
{
	GLuint program = glCreateProgram();

	GLuint vertShader = loadShader(programDebugName, "vertex", GL_VERTEX_SHADER, vertShaderContent);
	GLuint fragShader = loadShader(programDebugName, "fragment", GL_FRAGMENT_SHADER, fragShaderContent);

	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);

	GLint linkingSuccess;
	glGetProgramiv(program, GL_LINK_STATUS, &linkingSuccess);
	if (linkingSuccess == GL_FALSE)
	{
		std::string actionDesc = std::format("Linking {}", programDebugName);

		printInfoLogAndExit(actionDesc, program, glGetProgramiv, glGetProgramInfoLog, glDeleteProgram);
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}