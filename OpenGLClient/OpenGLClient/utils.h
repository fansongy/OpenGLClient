#pragma once
#include "glew.h"
#include "SOIL.h"//simple opengl image library
char* LoadFileContent(const char*path);
unsigned char* LoadBMP(const char*path, int &width, int &height);
GLuint CreateTextureFromFile(const char*filePath);

void CheckGLError(const char*file, int line);
#define GL_CALL(x) do{ x;CheckGLError(__FILE__,__LINE__);}while (0)