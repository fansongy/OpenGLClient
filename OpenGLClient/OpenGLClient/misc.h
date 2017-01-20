#pragma once

#include "glew.h"

GLuint CreateBufferObject(GLenum bufferType,GLsizeiptr size,GLenum usage,void* data);

char *LoadFileContent(const char* path);
GLuint CompileShader(GLenum shaderType,const char* shaderPath);
GLuint CreateGPUProgram(const char* vsShaderPath,const char* fsShaderPath);
GLuint CreateTextureFromFile(const char* imagePath);