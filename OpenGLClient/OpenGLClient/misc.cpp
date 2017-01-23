#include "misc.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

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

static unsigned char* DecodeBMPData(unsigned char* imageData,int&width,int& heigh)
{
	//decode bmp
	int pixelDataOffset =*((int*)(imageData+10));
	width = *((int*)(imageData +18));
	heigh = *((int*)(imageData +22));
	unsigned char* pixelData = (imageData+pixelDataOffset);
	for(int i = 0;i<width*heigh*3;i+=3)
	{
		//bgr->rgb
		unsigned char temp = pixelData[i+2];
		pixelData[i+2] = pixelData[i];
		pixelData[i] = temp;
	}
	return pixelData;
}

GLuint SaveImage(const char* imagePath,unsigned char* imgData,int width,int height)
{
	FILE* pFile = fopen(imagePath,"wb");
	if (pFile)
	{
		BITMAPFILEHEADER bfh;
		memset(&bfh,0,sizeof(BITMAPFILEHEADER));
		bfh.bfType = 0x4D42;
		bfh.bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+width*height*3;
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
		fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,pFile);
		BITMAPINFOHEADER bih;
		memset(&bih,0,sizeof(BITMAPINFOHEADER));
		bih.biWidth = width;
		bih.biHeight = height;
		bih.biBitCount = 24;
		bih.biSize = sizeof(BITMAPINFOHEADER);
		fwrite(&bih,sizeof(BITMAPINFOHEADER),1,pFile);
		unsigned char temp = 0;
		for(int i = 0;i<width*height*3;i+=3)
		{
			//bgr->rgb
			temp = imgData[i+2];
			imgData[i+2] = imgData[i];
			imgData[i] = temp;
		}
		fwrite(imgData,1,width*height*3,pFile);
		fclose(pFile);
	}
	return 0;
}

const unsigned long	 FORMATE_DXT1 = 0x31545844l; //DXT1-> 1 T X D

static unsigned char* DecodeDXT1Data(unsigned char* imageData,int&width,int& height,int& pixelSize)
{
	height = *(unsigned long*)(imageData+sizeof(unsigned long)*3);
	width = *(unsigned long*)(imageData+sizeof(unsigned long)*4);
	pixelSize = *(unsigned long*)(imageData+sizeof(unsigned long)*5);
	unsigned long compressFormate;
	compressFormate = *(unsigned long*)(imageData+sizeof(unsigned long)*21);

	switch (compressFormate)
	{
	case FORMATE_DXT1:
		printf("DXT1\n");
		break;
	default:
		break;
	}
	unsigned char* pixelData = new unsigned char[pixelSize];
	memcpy(pixelData,(imageData+sizeof(unsigned long)*32),pixelSize);

	return pixelData;
}

GLuint CreateTextureFromFile(const char* imagePath)
{
	unsigned char* imageData =(unsigned char*) LoadFileContent(imagePath);

	int width = 0;
	int heigh = 0;
	//decode bmp
	unsigned char* pixelData =nullptr;
	int pixelDataSize = 0;
	GLenum srcForamte = GL_RGB;
	if ((*(unsigned short*)imageData) == 0x4D42)
	{
		pixelData = DecodeBMPData(imageData,width,heigh);
	} 
	else if (memcmp(imageData,"DDS ",4)==0)
	{
		pixelData = DecodeDXT1Data(imageData,width,heigh,pixelDataSize);
		srcForamte = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	}
	
	if (pixelData == nullptr)
	{
		printf("cannot decode %s \n",imagePath);
		delete imageData;
		return 0;
	}

	GLuint texture;
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	if(srcForamte == GL_RGB){
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,heigh,0,GL_RGB,GL_UNSIGNED_BYTE,pixelData);
	}
	else if (srcForamte == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
	{
		glCompressedTexImage2D(GL_TEXTURE_2D,0,srcForamte,width,heigh,0,pixelDataSize,pixelData);
	}
	glBindBuffer(GL_TEXTURE_2D,0);
	//SaveImage("res/texture/test.bmp",pixelData,width,heigh);
	delete imageData;
	return texture;
}
void CheckGLError(const char* file,int line)
{
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
	{
		switch (error)
		{
		case GL_INVALID_ENUM:
			printf("GL Error: GL_INVALID_ENUM %s : %d \n",file,line);
			break;
		case GL_INVALID_VALUE:
			printf("GL Error: GL_INVALID_VALUE %s : %d \n",file,line);
			break;
		case GL_INVALID_OPERATION:
			printf("GL Error: GL_INVALID_OPERATION %s : %d \n",file,line);
			break;
		case GL_STACK_OVERFLOW:
			printf("GL Error: GL_STACK_OVERFLOW %s : %d \n",file,line);
			break;
		case GL_STACK_UNDERFLOW:
			printf("GL Error: GL_STACK_UNDERFLOW %s : %d \n",file,line);
			break;
		case GL_OUT_OF_MEMORY:
			printf("GL Error: GL_OUT_OF_MEMORY %s : %d \n",file,line);
			break;
		default:
			printf("GL Error: 0x%x %s : %d \n",error,file,line);
			break;
		}
	}
}

GLuint CreateVAOWithVBOSettings(std::function<void()> settings)
{
	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	settings();
	glBindVertexArray(0);
	return vao;
}