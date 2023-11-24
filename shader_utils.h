#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include "shaders.h"

#define SHADER_VAR_NAME(shader_name, ext) _shader_ ## shader_name ## _ ## ext ## _data
#define GET_VERT_SHADER_CONTENT(shader_name) SHADER_VAR_NAME(shader_name, vert)
#define GET_FRAG_SHADER_CONTENT(shader_name) SHADER_VAR_NAME(shader_name, frag)

GLuint loadShaderProgram(const char* programDebugName, const char* vertShaderContent, const char* fragShaderContent);