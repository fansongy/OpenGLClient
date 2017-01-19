#include "misc.h"
#include <stdio.h>

GLuint CreateBufferObject(GLenum bufferType,GLsizeiptr size,GLenum usage,void* data)
{
	GLuint object;
	glGenBuffers(1,&object);
	glBindBuffer(bufferType,object);
	glBufferData(bufferType,size,data,usage);
	glBindBuffer(bufferType,0);
	return object;
}

char *LoadFileContent(const char* path){
	FILE *pFile =fopen(path,"rb");
	if(pFile)
	{
		fseek(pFile,0,SEEK_END);
		int nLen = ftell(pFile);
		char *buffer = nullptr;
		if(nLen !=0){
			buffer = new char[nLen+1];
			rewind(pFile);
			fread(buffer,nLen,1,pFile);
			buffer[nLen] = '\0';
		}
		else
		{
			printf("load file fail %s content len is 0\n",path);
		}
		fclose(pFile);
		return buffer;
	}
	else
	{
		printf("open file %s fail\n",path);
	}
	fclose(pFile);
	return nullptr;
}

GLuint CompileShader(GLenum shaderType,const char* shaderPath)
{
	GLuint shader = glCreateShader(shaderType);
	if(shader == 0)
	{
		printf("create shader fail: %s\n",shaderPath);
		glDeleteShader(shader);
		return 0;
	}
	const char* shaderCode = LoadFileContent(shaderPath);
	if(shaderCode == nullptr)
	{
		printf("load shader code from %s fail\n",shaderPath);
		glDeleteShader(shader);
		return 0;
	}

	glShaderSource(shader,1,&shaderCode,nullptr);
	glCompileShader(shader);

	GLint compileResult = GL_TRUE;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&compileResult);
	if(compileResult == GL_FALSE){
		char szLog[1024] = {0};
		GLsizei logLen = 0;
		glGetShaderInfoLog(shader,1024,&logLen,szLog);
		printf("Compile Shader fail error log: %s \nshader code:\n%s\n",szLog,shaderCode);
		glDeleteShader(shader);
		shader = 0;
	}
	delete shaderCode;
	return shader;
}

GLuint CreateGPUProgram(const char* vsShaderPath,const char* fsShaderPath)
{
	GLuint vsShader = CompileShader(GL_VERTEX_SHADER,vsShaderPath);
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER,fsShaderPath);

	//Attach
	GLuint program = glCreateProgram();
	glAttachShader(program,vsShader);
	glAttachShader(program,fsShader);

	//Link
	glLinkProgram(program);

	//Clear
	glDetachShader(program,vsShader);
	glDetachShader(program,fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);

	//check error
	GLint linkResult = GL_TRUE;
	glGetProgramiv(program,GL_LINK_STATUS,&linkResult);
	if(linkResult == GL_FALSE){
		char szLog[1024] = {0};
		GLsizei logLen = 0;
		glGetProgramInfoLog(program,1024,&logLen,szLog);
		printf("Link program fail error log: %s \nvs shader code:\n%s\nfs shader code:\n%s\n",szLog,vsShaderPath,fsShaderPath);
		glDeleteShader(program);
		program = 0;
	}

	return program;
}
