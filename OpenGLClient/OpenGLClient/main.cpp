#include <windows.h>
#include "glew.h"
#include <stdio.h>
#include <math.h>
#include "utils.h"
#include "GPUProgram.h"
#include "ObjModel.h"
#include "FBO.h"
#include "FullScreenQuad.h"
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")

FBO *fbo=nullptr;
FullScreenQuad fsq; 
GPUProgram *verticalProgram, *HorizontalProgram;

void Blur(GLuint texture,int blurCount)
{
	fbo[0].Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(HorizontalProgram->mProgram);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(HorizontalProgram->GetLocation("U_MainTexture"), 0);
	fsq.Draw(HorizontalProgram->GetLocation("pos"), HorizontalProgram->GetLocation("texcoord"));
	fbo[1].Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(verticalProgram->mProgram);
	glBindTexture(GL_TEXTURE_2D, fbo[0].GetBuffer("color"));
	glUniform1i(verticalProgram->GetLocation("U_MainTexture"), 0);
	fsq.Draw(verticalProgram->GetLocation("pos"), HorizontalProgram->GetLocation("texcoord"));
	fbo[1].Unbind();

	for (int i=1;i<blurCount;i++)
	{
		fbo[0].Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(HorizontalProgram->mProgram);
		glBindTexture(GL_TEXTURE_2D, fbo[1].GetBuffer("color"));
		glUniform1i(HorizontalProgram->GetLocation("U_MainTexture"), 0);
		fsq.Draw(HorizontalProgram->GetLocation("pos"), HorizontalProgram->GetLocation("texcoord"));
		fbo[1].Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(verticalProgram->mProgram);
		glBindTexture(GL_TEXTURE_2D, fbo[0].GetBuffer("color"));
		glUniform1i(verticalProgram->GetLocation("U_MainTexture"), 0);
		fsq.Draw(verticalProgram->GetLocation("pos"), HorizontalProgram->GetLocation("texcoord"));
		fbo[1].Unbind();
	}
}

LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}


INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = NULL;
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hIcon = NULL;
	wndClass.hIconSm = NULL;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc=GLWindowProc;
	wndClass.lpszClassName = L"OpenGL";
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;
	ATOM atom = RegisterClassEx(&wndClass);

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 1280;
	rect.bottom = 720;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hwnd = CreateWindowEx(NULL, L"OpenGL", L"RenderWindow", WS_OVERLAPPEDWINDOW, 100, 100, rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, hInstance, NULL);
	HDC dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_TYPE_RGBA | PFD_DOUBLEBUFFER;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int pixelFormatID = ChoosePixelFormat(dc, &pfd);

	SetPixelFormat(dc,pixelFormatID,&pfd);

	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);
	GetClientRect(hwnd, &rect);
	int viewportWidth = rect.right - rect.left, viewportHeight = rect.bottom - rect.top;
	glewInit();

	GPUProgram originalProgram;
	originalProgram.AttachShader(GL_VERTEX_SHADER, "res/shader/fog.vs");
	originalProgram.AttachShader(GL_FRAGMENT_SHADER, "res/shader/fog.fs");
	originalProgram.Link();
	originalProgram.DetectAttribute("pos");
	originalProgram.DetectAttribute("texcoord");
	originalProgram.DetectAttribute("normal");
	originalProgram.DetectUniform("M");
	originalProgram.DetectUniform("V");
	originalProgram.DetectUniform("P");
	originalProgram.DetectUniform("NM");
	originalProgram.DetectUniform("U_AmbientLightColor");
	originalProgram.DetectUniform("U_AmbientMaterial");
	originalProgram.DetectUniform("U_DiffuseLightColor");
	originalProgram.DetectUniform("U_DiffuseMaterial");
	originalProgram.DetectUniform("U_SpecularLightColor");
	originalProgram.DetectUniform("U_SpecularMaterial");
	originalProgram.DetectUniform("U_LightPos");
	originalProgram.DetectUniform("U_EyePos");
	originalProgram.DetectUniform("U_LightDirection");
	originalProgram.DetectUniform("U_Cutoff");
	originalProgram.DetectUniform("U_DiffuseIntensity");
	originalProgram.DetectUniform("U_FogStart");
	originalProgram.DetectUniform("U_FogEnd");
	originalProgram.DetectUniform("U_FogColor");

	GPUProgram combineProgram;
	combineProgram.AttachShader(GL_VERTEX_SHADER, "res/shader/fullscreenquad.vs");
	combineProgram.AttachShader(GL_FRAGMENT_SHADER, "res/shader/smooth.fs");
	combineProgram.Link();
	combineProgram.DetectAttribute("pos");
	combineProgram.DetectAttribute("texcoord");
	combineProgram.DetectUniform("U_BaseTexture");
	combineProgram.DetectUniform("U_BlendTexture");

	GPUProgram combineProgram1;
	combineProgram1.AttachShader(GL_VERTEX_SHADER, "res/shader/fullscreenquad.vs");
	combineProgram1.AttachShader(GL_FRAGMENT_SHADER, "res/shader/sharpen.fs");
	combineProgram1.Link();
	combineProgram1.DetectAttribute("pos");
	combineProgram1.DetectAttribute("texcoord");
	combineProgram1.DetectUniform("U_BaseTexture");
	combineProgram1.DetectUniform("U_BlendTexture");

	GPUProgram combineProgram2;
	combineProgram2.AttachShader(GL_VERTEX_SHADER, "res/shader/fullscreenquad.vs");
	combineProgram2.AttachShader(GL_FRAGMENT_SHADER, "res/shader/edgeDetect.fs");
	combineProgram2.Link();
	combineProgram2.DetectAttribute("pos");
	combineProgram2.DetectAttribute("texcoord");
	combineProgram2.DetectUniform("U_BaseTexture");
	combineProgram2.DetectUniform("U_BlendTexture");

	GPUProgram combineProgram3;
	combineProgram3.AttachShader(GL_VERTEX_SHADER, "res/shader/fullscreenquad.vs");
	combineProgram3.AttachShader(GL_FRAGMENT_SHADER, "res/shader/edgeDetect.fs");
	combineProgram3.Link();
	combineProgram3.DetectAttribute("pos");
	combineProgram3.DetectAttribute("texcoord");
	combineProgram3.DetectUniform("U_BaseTexture");
	combineProgram3.DetectUniform("U_BlendTexture");


	HorizontalProgram = new GPUProgram;
	HorizontalProgram->AttachShader(GL_VERTEX_SHADER, "res/shader/fullscreenquad.vs");
	HorizontalProgram->AttachShader(GL_FRAGMENT_SHADER, "res/shader/GaussianHorizontal.fs");
	HorizontalProgram->Link();
	HorizontalProgram->DetectAttribute("pos");
	HorizontalProgram->DetectAttribute("texcoord");
	HorizontalProgram->DetectUniform("U_MainTexture");

	verticalProgram = new GPUProgram;
	verticalProgram->AttachShader(GL_VERTEX_SHADER, "res/shader/fullscreenquad.vs");
	verticalProgram->AttachShader(GL_FRAGMENT_SHADER, "res/shader/GaussianVertical.fs");
	verticalProgram->Link();
	verticalProgram->DetectAttribute("pos");
	verticalProgram->DetectAttribute("texcoord");
	verticalProgram->DetectUniform("U_MainTexture");

	//init 3d model
	ObjModel cube;
	cube.Init("res/model/Cube.obj");

	float identity[] = {
		1.0f,0,0,0,
		0,1.0f,0,0,
		0,0,1.0f,0,
		0,0,0,1.0f
	};
	float ambientLightColor[] = { 0.4f,0.4f,0.4f,1.0f };
	float ambientMaterial[] = { 0.2f,0.2f,0.2f,1.0f };
	float diffuseLightColor[] = { 1.0f,1.0f,1.0f,1.0f };
	float diffuseMaterial[] = { 0.1f,0.4f,0.7f,1.0f };
	float diffuseIntensity = 1.0f;
	float specularLightColor[] = { 1.0f,1.0f,1.0f,1.0f };
	float specularMaterial[] = { 1.0f,1.0f,1.0f,1.0f };
	float lightPos[] = { -1.0f,1.0f,0.0f,0.0f };
	float spotLightDirection[] = { 0.0f,-1.0f,0.0f,128.0f };
	float spotLightCutoff = 0.0f;// 15.0f;//degree
	float eyePos[] = { 0.0f,0.0f,0.0f };
	float fogStart = 2.0f;
	float fogEnd = 30.0f;
	float fogColor[] = { 0.6f, 0.6f, 0.6f,1.0 };

	glm::mat4 cubeModel = glm::translate<float>(-2.0f, 0.0f, -4.0f)*glm::rotate(-30.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 cubeModel2 = glm::translate<float>(-2.0f, 0.0f, -12.0f)*glm::rotate(-30.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 cubeModel3 = glm::translate<float>(-1.0f, 0.0f, -20.0f)*glm::rotate(-30.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 cubeNormalMatrix = glm::inverseTranspose(cubeModel);
	glm::mat4 cubeNormalMatrix2 = glm::inverseTranspose(cubeModel2);
	glm::mat4 cubeNormalMatrix3 = glm::inverseTranspose(cubeModel3);

	glm::mat4 projectionMatrix = glm::perspective(50.0f, (float)viewportWidth / (float)viewportHeight, 0.1f, 1000.0f);

	fsq.Init();
	fbo = new FBO[2];
	for (int i = 0; i < 2; i++)
	{
		fbo[i].AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, viewportWidth, viewportHeight);
		fbo[i].AttachDepthBuffer("depth", viewportWidth, viewportHeight);
		fbo[i].Finish();
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//normal rgba
		glUseProgram(originalProgram.mProgram);
		glUniformMatrix4fv(originalProgram.GetLocation("M"), 1, GL_FALSE, glm::value_ptr(cubeModel));
		glUniformMatrix4fv(originalProgram.GetLocation("NM"), 1, GL_FALSE, glm::value_ptr(cubeNormalMatrix));
		glUniformMatrix4fv(originalProgram.GetLocation("V"), 1, GL_FALSE, identity);
		glUniformMatrix4fv(originalProgram.GetLocation("P"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniform4fv(originalProgram.GetLocation("U_AmbientLightColor"), 1, ambientLightColor);
		glUniform4fv(originalProgram.GetLocation("U_AmbientMaterial"), 1, ambientMaterial);
		glUniform4fv(originalProgram.GetLocation("U_DiffuseLightColor"), 1, diffuseLightColor);
		glUniform4fv(originalProgram.GetLocation("U_DiffuseMaterial"), 1, diffuseMaterial);
		glUniform4fv(originalProgram.GetLocation("U_SpecularLightColor"), 1, specularLightColor);
		glUniform4fv(originalProgram.GetLocation("U_SpecularMaterial"), 1, specularMaterial);
		glUniform4fv(originalProgram.GetLocation("U_LightPos"), 1, lightPos);
		glUniform3fv(originalProgram.GetLocation("U_EyePos"), 1, eyePos);
		glUniform4fv(originalProgram.GetLocation("U_LightDirection"), 1, spotLightDirection);
		glUniform1f(originalProgram.GetLocation("U_Cutoff"), spotLightCutoff);
		glUniform1f(originalProgram.GetLocation("U_DiffuseIntensity"), diffuseIntensity);
		glUniform1f(originalProgram.GetLocation("U_FogStart"), fogStart);
		glUniform1f(originalProgram.GetLocation("U_FogEnd"), fogEnd);
		glUniform4fv(originalProgram.GetLocation("U_FogColor"), 1, fogColor);
		cube.Bind(originalProgram.GetLocation("pos"), originalProgram.GetLocation("texcoord"), originalProgram.GetLocation("normal"));
		cube.Draw();

		glUniformMatrix4fv(originalProgram.GetLocation("M"), 1, GL_FALSE, glm::value_ptr(cubeModel2));
		glUniformMatrix4fv(originalProgram.GetLocation("NM"), 1, GL_FALSE, glm::value_ptr(cubeNormalMatrix2));
		cube.Bind(originalProgram.GetLocation("pos"), originalProgram.GetLocation("texcoord"), originalProgram.GetLocation("normal"));
		cube.Draw();

		glUniformMatrix4fv(originalProgram.GetLocation("M"), 1, GL_FALSE, glm::value_ptr(cubeModel3));
		glUniformMatrix4fv(originalProgram.GetLocation("NM"), 1, GL_FALSE, glm::value_ptr(cubeNormalMatrix3));
		cube.Bind(originalProgram.GetLocation("pos"), originalProgram.GetLocation("texcoord"), originalProgram.GetLocation("normal"));
		cube.Draw();
		glUseProgram(0);
		glFlush();
		SwapBuffers(dc);
	}
	return 0;
}