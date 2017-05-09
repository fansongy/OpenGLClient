#pragma once
#include "glew.h"

class FullScreenQuad
{
public:
	GLuint mVBO;
public:
	void Init();
	void Draw(GLint posLoc,GLint texcoordLoc);
	void DrawToLeftTop(GLint posLoc, GLint texcoordLoc);
	void DrawToRightTop(GLint posLoc, GLint texcoordLoc);
	void DrawToLeftBottom(GLint posLoc, GLint texcoordLoc);
	void DrawToRightBottom(GLint posLoc, GLint texcoordLoc);
};