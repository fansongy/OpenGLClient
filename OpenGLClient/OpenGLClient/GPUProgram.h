#pragma once
#include "glew.h"
#include <stack>
#include <map>
#include <string>

class GPUProgram
{
public:
	//member
	GLuint mProgram;//gpu resource id
	std::stack<GLuint> mAttachedShaders;
	std::map<std::string, GLint> mLocations;
public:
	GPUProgram();
	~GPUProgram();
	void DetectAttribute(const char*attributeName);
	void DetectUniform(const char*uniformName);
	GLint GetLocation(const char*name);
	void AttachShader(GLenum shaderType, const char*shaderPath);
	void Link();
};