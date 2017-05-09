#include "FullScreenQuad.h"


void FullScreenQuad::Init()
{
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FullScreenQuad::Draw(GLint posLoc, GLint texcoordLoc)
{
	//draw fullscreen
	float vertices[] = {
		-0.5f,-0.5f,-1.0f,0.0f,0.0f,
		0.5f,-0.5f,-1.0f,1.0f,0.0f,
		0.5f,0.5f,-1.0f,1.0f,1.0f,
		-0.5f,0.5f,-1.0f,0.0f,1.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(texcoordLoc);
	glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float)*3));
	glDrawArrays(GL_QUADS, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FullScreenQuad::DrawToLeftTop(GLint posLoc, GLint texcoordLoc)
{
	//draw to left top qualter of screen
	float vertices[] = {
		-0.5f,0.0f,-1.0f,0.0f,0.0f,
		0.0f,0.0f,-1.0f,1.0f,0.0f,
		0.0f,0.5f,-1.0f,1.0f,1.0f,
		-0.5f,0.5f,-1.0f,0.0f,1.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(texcoordLoc);
	glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glDrawArrays(GL_QUADS, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FullScreenQuad::DrawToLeftBottom(GLint posLoc, GLint texcoordLoc)
{
	float vertices[] = {
		-0.5f,-0.5f,-1.0f,0.0f,0.0f,
		0.0f,-0.5f,-1.0f,1.0f,0.0f,
		0.0f,0.0f,-1.0f,1.0f,1.0f,
		-0.5f,0.0f,-1.0f,0.0f,1.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(texcoordLoc);
	glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glDrawArrays(GL_QUADS, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FullScreenQuad::DrawToRightTop(GLint posLoc, GLint texcoordLoc)
{
	float vertices[] = {
		0.0f,-0.0f,-1.0f,0.0f,0.0f,
		0.5f,-0.0f,-1.0f,1.0f,0.0f,
		0.5f,0.5f,-1.0f,1.0f,1.0f,
		0.0f,0.5f,-1.0f,0.0f,1.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(texcoordLoc);
	glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glDrawArrays(GL_QUADS, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FullScreenQuad::DrawToRightBottom(GLint posLoc, GLint texcoordLoc)
{
	float vertices[] = {
		0.0f,-0.5f,-1.0f,0.0f,0.0f,
		0.5f,-0.5f,-1.0f,1.0f,0.0f,
		0.5f,0.0f,-1.0f,1.0f,1.0f,
		0.0f,0.0f,-1.0f,0.0f,1.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(texcoordLoc);
	glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glDrawArrays(GL_QUADS, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}