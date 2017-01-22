#include <windows.h>
#include <stdio.h>
#include "glew.h"
#include "misc.h"
#include "model.h"
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"
#include "timer.h"
#include "frustum.h"
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glu32.lib")

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

	RECT rectWin;
	rectWin.left = 0;
	rectWin.right = 800;
	rectWin.top =0;
	rectWin.bottom = 600;
	AdjustWindowRect(&rectWin,WS_OVERLAPPEDWINDOW,FALSE);

	HWND hwnd = CreateWindowEx(NULL, L"OpenGL", L"RenderWindow", WS_OVERLAPPEDWINDOW, 100, 100, rectWin.right-rectWin.left, rectWin.bottom-rectWin.top, NULL, NULL, hInstance, NULL);
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



	int width,height;
	RECT rect;
	GetClientRect(hwnd,&rect);
	width = rect.right-rect.left;
	height = rect.bottom-rect.top;

	glewInit();

	GLuint program = CreateGPUProgram("res/shader/PointSprite.vs", "res/shader/PointSprite.fs");
	GLint posLocation, texcoordLocation,normalLocation, MLocation, VLocation, PLocation,NMLocation,textureLocation;
	posLocation = glGetAttribLocation(program, "pos");
	texcoordLocation = glGetAttribLocation(program, "texcoord");
	normalLocation = glGetAttribLocation(program, "normal");

	MLocation = glGetUniformLocation(program, "M");
	VLocation = glGetUniformLocation(program, "V");
	PLocation = glGetUniformLocation(program, "P");
	NMLocation = glGetUniformLocation(program, "NM");
	textureLocation = glGetUniformLocation(program, "U_MainTexture");

	// load obj model :  vertexes、vertex count、indexes、index count
	unsigned int *indexes = nullptr;
	int vertexCount = 0, indexCount = 0;
	Timer t;
	t.Start();
	VertexData*vertexes = LoadObjModel("res/model/Quad.obj", &indexes, vertexCount, indexCount);
	printf("Load model cost %fs\n",t.GetPassedTime());
	if (vertexes==nullptr)
	{
		printf("load obj model fail\n");
	}

	//float z = 4.0f;
	//float halfFov = 22.5f;
	//float randianHalfFov = 3.14f * halfFov/ 180.0f;
	//float tanHalFov = sin(randianHalfFov)/cos(randianHalfFov);
	//float y  = tanHalFov *z;
	//float aspect = (float)width/(float)height;
	//float x = y*aspect;

	//vertexes[0].position[0] = -x;
	//vertexes[0].position[1] = -y;

	//vertexes[1].position[0] = x;
	//vertexes[1].position[1] = -y;

	//vertexes[2].position[0] = -x;
	//vertexes[2].position[1] = y;

	//vertexes[3].position[0] = x;
	//vertexes[3].position[1] = y;

	vertexes[0].position[0] = 0;
	vertexes[0].position[1] = 0;

	//obj model -> vbo & ibo
	GLuint vbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, GL_STATIC_DRAW, vertexes);
	GLuint ibo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, GL_STATIC_DRAW, indexes);
	GLuint mainTexture = CreateTextureFromFile("res/texture/camera.dds");
	printf("vertex count %d index count %d\n",vertexCount,indexCount);

	glClearColor(0.1f, 0.4f, 0.7f,1.0f);
	glEnable(GL_DEPTH_TEST);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, width, height);
	float identity[] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};


	glm::mat4 model = glm::translate(-1.0f,0.0f,-3.0f)* glm::rotate(-20.0f,0.0f,1.0f,0.0f);
	glm::mat4 projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	//glm::mat4 uiMatrix = glm::ortho(-400.0f, 400.0f,-300.0f,300.0f);
	glm::mat4 normalMatrix = glm::inverseTranspose(model);

	Frustum	frustum;
	frustum.InitProgram();
	frustum.InitOrtho(-0.5f,0.5f,-0.5f,0.5f,0.1f,4.0f);
	//frustum.InitPerspective(45.0f, (float)width / (float)height, 0.1f,4.0f);

	MSG msg;

	auto draw = [&]()-> void
	{
		glUseProgram(program); 
		glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(VLocation, 1, GL_FALSE, identity);
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(NMLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glBindTexture(GL_TEXTURE_2D, mainTexture);
		glUniform1i(texcoordLocation, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(posLocation);
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
		glEnableVertexAttribArray(texcoordLocation);
		glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(normalLocation);
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 5));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);
	};
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
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		frustum.Draw(glm::value_ptr(model),identity,glm::value_ptr(projection));

		draw();
		//glEnable(GL_SCISSOR_TEST);
		//glScissor(0,0,width,100);
		//draw();
		//glScissor(0,200,width,100);
		//draw();
		//glDisable(GL_SCISSOR_TEST);
		SwapBuffers(dc);
	}
	return 0;
}