#pragma once
#include "glew.h"
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"

class  Frustum
{
public:
	Frustum();
	GLuint mVBO,mIBO,mProgram;
	GLint mPLocation,mVLocation,mMLocation,mPosLocation;
public:
	void InitPerspective(float fov,float aspect ,float zNear,float zFar);
	void InitOrtho(float left,float right,float top,float bottom,float zNear,float zFar);
	void Draw(float *M,float* V,float *P);
	void InitProgram();
private:

};

