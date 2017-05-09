#pragma once
#include "glew.h"
#include <map>
#include <vector>
#include <string>

class FBO
{
public:
	GLuint mFBO;
	std::map<std::string, GLuint> mBuffers;
	std::vector<GLenum> mDrawBuffers;
public:
	FBO();
	//hdr/normal color
	void AttachColorBuffer(const char*bufferName,GLenum attachment,GLenum dataType,int width,int height);
	void AttachDepthBuffer(const char*bufferName, int width, int height);
	//complete fbo settings
	void Finish();
	void Bind();
	void Unbind();

	GLuint GetBuffer(const char*bufferName);
};