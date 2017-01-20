#include <windows.h>
#include "glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"
#include "misc.h"
#include "model.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glew32.lib")


const int ConstPipline = 1;
const int VFPipline = 2;

//int s_pipline = ConstPipline;
int s_pipline = VFPipline;

struct ShaderData
{
	GLint posLocation,texcoordLocation,normalLocation,MLocation,VLocation,PLocation,NMLocation,TextureLocation;
	glm::mat4 projection;
	glm::mat4 model;
	GLuint vbo;
	GLuint ibo;
	GLuint mainTexture;
	size_t indexCount;
};

float identify[] = {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
};

GLuint s_program;
ShaderData s_shaderData;

/////////////      VF Pipline     ////////////////////////////////

void VFRender()
{

	glUseProgram(s_program);
	
	glm::mat4 normalMatrix = glm::inverseTranspose(s_shaderData.model);
	glUniformMatrix4fv(s_shaderData.MLocation,1,GL_FALSE,glm::value_ptr(s_shaderData.model));
	glUniformMatrix4fv(s_shaderData.VLocation,1,GL_FALSE,identify);
	glUniformMatrix4fv(s_shaderData.PLocation,1,GL_FALSE,glm::value_ptr(s_shaderData.projection));
	glUniformMatrix4fv(s_shaderData.NMLocation,1,GL_FALSE,glm::value_ptr(normalMatrix));
	
	glBindTexture(GL_TEXTURE_2D,s_shaderData.mainTexture);
	glUniform1i(s_shaderData.TextureLocation,0);

	glBindBuffer(GL_ARRAY_BUFFER,s_shaderData.vbo);
	glEnableVertexAttribArray(s_shaderData.posLocation);
	glVertexAttribPointer(s_shaderData.posLocation,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),nullptr); 
	glEnableVertexAttribArray(s_shaderData.texcoordLocation);
	glVertexAttribPointer(s_shaderData.texcoordLocation,2,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)(sizeof(float)*3)); 
	glEnableVertexAttribArray(s_shaderData.normalLocation);
	glVertexAttribPointer(s_shaderData.normalLocation,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)(sizeof(float)*5)); 

	glBindBuffer(GL_ARRAY_BUFFER,0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,s_shaderData.ibo);
	glDrawElements(GL_TRIANGLES,s_shaderData.indexCount,GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glUseProgram(0);
}

void VFPrepare()
{
	glewInit();
	s_program = CreateGPUProgram("res/shader/draw.vs","res/shader/draw.fs");
	

	//Get Attribute in shader
	s_shaderData.posLocation = glGetAttribLocation(s_program,"pos");
	s_shaderData.texcoordLocation = glGetAttribLocation(s_program,"texcoord");
	s_shaderData.normalLocation = glGetAttribLocation(s_program,"normal");

	//Get uniform in shader
	s_shaderData.MLocation = glGetUniformLocation(s_program,"M");
	s_shaderData.VLocation = glGetUniformLocation(s_program,"V");
	s_shaderData.PLocation = glGetUniformLocation(s_program,"P");
	s_shaderData.NMLocation = glGetUniformLocation(s_program,"NM");
	s_shaderData.TextureLocation = glGetUniformLocation(s_program,"U_MainTexture");

	s_shaderData.model = glm::translate(0.0f,0.0f,-4.0f);
	s_shaderData.projection =glm::perspective(45.0f,800.0f/600.0f,0.1f,1000.0f);
	
	//load obj model
	unsigned int * indexes = nullptr;
	int vertexCount = 0,indexCount = 0;
	VertexData* vertexes = LoadObjModel("res/model/Sphere.obj",&indexes,vertexCount,indexCount);

	if(vertexes == nullptr)
	{
		printf("Load Model Fail\n");
	}
	//send vbo to GPU
	s_shaderData.vbo = CreateBufferObject(GL_ARRAY_BUFFER,sizeof(VertexData)*vertexCount,GL_STATIC_DRAW,vertexes);

	//add ibo
	s_shaderData.ibo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indexCount,GL_STATIC_DRAW,indexes);
	// texture
	s_shaderData.mainTexture = CreateTextureFromFile("res/texture/150001.dds");

	s_shaderData.indexCount = indexCount;
	printf("vertex count %d index count %d\n",vertexCount,indexCount);
}

/////////////      Constant Pipline     ////////////////////////////////
void ConstantRenderGL()
{
	glBegin(GL_TRIANGLES);
	glVertex3f(0,0,-100);
	glVertex3f(10,0,-100);
	glVertex3f(0,10,-100);
	glEnd();
}

void ConstantPrepare()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,800.0f/600.0f,0.1f,1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
//////////////////////////////////////////////////////////////////////////

void PrepareForGL(HWND hwnd)
{
	HDC dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd,0,sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER |PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_TYPE_RGBA;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	int pixelForamteID = ChoosePixelFormat(dc,&pfd);
	
	SetPixelFormat(dc,pixelForamteID,&pfd);
	
	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc,rc);
	
	glClearColor(0.1f,0.4f,0.7f,1.0f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	if (s_pipline == VFPipline)
	{
		VFPrepare();
	}
	if(s_pipline == ConstPipline){
		ConstantPrepare();
	}
	
}


LRESULT CALLBACK RenderWindwoProc(HWND hwnd,UINT msg ,WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}

INT WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbSize = sizeof(WNDCLASSEX);	
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = NULL;
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndclass.hIcon = NULL;
	wndclass.hIconSm = NULL;
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = RenderWindwoProc;
	wndclass.lpszClassName = L"MainWindow";
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW |CS_VREDRAW;

	ATOM atom = RegisterClassEx(&wndclass);

	HWND hwnd = CreateWindowEx(NULL,L"MainWindow",L"Render Window",WS_OVERLAPPEDWINDOW,300,300,800,600,NULL,NULL,hInstance,NULL);
	PrepareForGL(hwnd);


	ShowWindow(hwnd,SW_SHOW);
	UpdateWindow(hwnd);

	MSG msg;
	float angle = 0.0f;

	while (true)
	{

		if(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
		{
			if(msg.message == WM_QUIT){
				break;
			}
			
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		angle +=1.0f;
		if(angle >360.0f){
			angle =0;
		}
		s_shaderData.model = glm::translate(0.0f,0.0f,-4.0f)*glm::rotate(angle,0.0f,1.0f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		if(s_pipline == ConstPipline){
			ConstantRenderGL();
		}
		else if(s_pipline == VFPipline)
		{
			VFRender();
		}
		SwapBuffers(GetDC(hwnd));
	}
	return 0;

}