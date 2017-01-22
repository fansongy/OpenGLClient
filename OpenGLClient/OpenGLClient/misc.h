#pragma once

#include "glew.h"

GLuint CreateBufferObject(GLenum bufferType,GLsizeiptr size,GLenum usage,void* data);

char *LoadFileContent(const char* path);
GLuint CompileShader(GLenum shaderType,const char* shaderPath);
GLuint CreateGPUProgram(const char* vsShaderPath,const char* fsShaderPath);
GLuint CreateTextureFromFile(const char* imagePath);
GLuint SaveImage(const char* imagePath,unsigned char* imgData,int width,int height);

void CheckGLError(const char* file,int line);
#define  GL_CALL(x) do{x;CheckGLError(__FILE__,__LINE__);}while(0)