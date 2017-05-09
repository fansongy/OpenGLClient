#include <stdio.h>
#include <windows.h>
#include "utils.h"

#pragma comment(lib,"SOIL.lib")

char* LoadFileContent(const char*path)
{
	char *pFileContent = NULL;
	FILE*pFile = fopen(path, "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		if (nLen > 0)
		{
			rewind(pFile);
			pFileContent = new char[nLen + 1];
			fread(pFileContent, 1, nLen, pFile);
			pFileContent[nLen] = '\0';
		}
		fclose(pFile);
	}
	return pFileContent;
}



unsigned char* LoadBMP(const char*path, int &width, int &height)
{
	unsigned char*imageData = nullptr;
	FILE *pFile = fopen(path, "rb");
	if (pFile)
	{
		BITMAPFILEHEADER bfh;
		fread(&bfh, sizeof(BITMAPFILEHEADER), 1, pFile);
		if (bfh.bfType == 0x4D42)
		{
			BITMAPINFOHEADER bih;
			fread(&bih, sizeof(BITMAPINFOHEADER), 1, pFile);
			width = bih.biWidth;
			height = bih.biHeight;
			int pixelCount = width*height * 3;
			imageData = new unsigned char[pixelCount];
			fseek(pFile, bfh.bfOffBits, SEEK_SET);
			fread(imageData, 1, pixelCount, pFile);

			unsigned char temp;
			for (int i = 0; i < pixelCount; i += 3)
			{
				temp = imageData[i + 2];
				imageData[i + 2] = imageData[i];
				imageData[i] = temp;
			}
		}
		fclose(pFile);
	}
	return imageData;
}

GLuint CreateTextureFromFile(const char*filePath)
{
	GLuint texture = SOIL_load_OGL_texture(filePath, 0, 0, SOIL_FLAG_POWER_OF_TWO|SOIL_FLAG_INVERT_Y);
	return texture;
}

void CheckGLError(const char*file, int line)
{

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		switch (error)
		{
		case  GL_INVALID_ENUM:
			printf("GL Error GL_INVALID_ENUM %s : %d\n", file, line);
			break;
		case  GL_INVALID_VALUE:
			printf("GL Error GL_INVALID_VALUE %s : %d\n", file, line);
			break;
		case  GL_INVALID_OPERATION:
			printf("GL Error GL_INVALID_OPERATION %s : %d\n", file, line);
			break;
		case  GL_STACK_OVERFLOW:
			printf("GL Error GL_STACK_OVERFLOW %s : %d\n", file, line);
			break;
		case  GL_STACK_UNDERFLOW:
			printf("GL Error GL_STACK_UNDERFLOW %s : %d\n", file, line);
			break;
		case  GL_OUT_OF_MEMORY:
			printf("GL Error GL_OUT_OF_MEMORY %s : %d\n", file, line);
			break;
		default:
			printf("GL Error 0x%x %s : %d\n", error, file, line);
			break;
		}
	}
}