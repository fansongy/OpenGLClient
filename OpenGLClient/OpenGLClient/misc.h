#pragma once

#include "glew.h"
#include <functional>

GLuint CreateBufferObject(GLenum bufferType,GLsizeiptr size,GLenum usage,void* data);
GLuint CreateVAOWithVBOSettings(std::function<void()> settings); 
GLuint CreateFrameBufferObject(GLuint &colorBuffer,GLuint &depthBuffer,int width,int height,GLuint *colorBuffer2 = nullptr);

char *LoadFileContent(const char* path);
GLuint CompileShader(GLenum shaderType,const char* shaderPath);
GLuint CreateGPUProgram(const char* vsShaderPath,const char* fsShaderPath);
GLuint CreateComputeProgram(const char* ComputeShaderPath);
GLuint CreateTextureFromFile(const char* imagePath);
GLuint SaveImage(const char* imagePath,unsigned char* imgData,int& width,int& height);
unsigned char* DecodeBMPData(unsigned char* imageData,int&width,int& heigh);

void CheckGLError(const char* file,int line);
#define  GL_CALL(x) do{x;CheckGLError(__FILE__,__LINE__);}while(0)