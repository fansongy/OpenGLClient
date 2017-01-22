#include "frustum.h"
#include "misc.h"

Frustum:: Frustum()
{

}

void Frustum::InitProgram()
{
	mProgram= CreateGPUProgram("res/shader/WhiteColor.vs", "res/shader/WhiteColor.fs");
	mPLocation= glGetAttribLocation(mProgram, "pos");

	mMLocation = glGetUniformLocation(mProgram, "M");
	mVLocation = glGetUniformLocation(mProgram, "V");
	mPLocation = glGetUniformLocation(mProgram, "P");
}

void Frustum::InitPerspective(float fov,float aspect ,float zNear,float zFar)
{
	//near
	float halfFov = 22.5f;
	float randianHalfFov = 3.14f * halfFov/ 180.0f;
	float tanHalFov = sinf(randianHalfFov)/cosf(randianHalfFov);
	float yNear  = tanHalFov *zNear;
	float xNear = yNear*aspect;

	//far
	float yFar = tanHalFov *zFar;
	float xFar = yFar*aspect;	

	float vertexs[24] = {
		-xNear,-yNear,-zNear,
		xNear,-yNear,-zNear,
		xNear,yNear,-zNear,
		-xNear,yNear,-zNear,

		-xNear,-yNear,-zNear,
		xNear,-yNear,-zNear,
		xNear,yNear,-zNear,
		-xNear,yNear,-zNear,
	};

	mVBO = CreateBufferObject(GL_ARRAY_BUFFER,sizeof(float)*24,GL_STATIC_DRAW,vertexs);
	unsigned int indexes[] = {
		0,1,1,2,2,3,3,0, //near plane
		4,5,5,6,6,7,7,4, //far plane
		0,4,3,7,2,6,1,5, //connect
	};
	mIBO = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*24,GL_STATIC_DRAW,indexes);
}

void Frustum::Draw(float *M,float* V,float *P)
{
	glUseProgram(mProgram); 
	glUniformMatrix4fv(mMLocation, 1, GL_FALSE, M);
	glUniformMatrix4fv(mVLocation, 1, GL_FALSE, V);
	glUniformMatrix4fv(mPLocation, 1, GL_FALSE, P);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(mPLocation);
	glVertexAttribPointer(mPLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

