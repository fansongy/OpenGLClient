#include <windows.h>
#include "glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glew32.lib")


const int ConstPipline = 1;
const int VFPipline = 2;

//int s_pipline = ConstPipline;
int s_pipline = VFPipline;

struct  Vertex
{
	float pos[3];
	float color[4];
};


struct ShaderData
{
	GLint posLocation,colorLoacation,MLocation,VLocation,PLocation;
	glm::mat4 projection;
	GLuint vbo;
};

float identify[] = {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
};

GLuint s_program;
ShaderData s_shaderData;

///////////////   UITL    /////////////////////////////////////////////
char *LoadFileContent(const char* path){
	FILE *pFile =fopen(path,"rb");
	if(pFile)
	{
		fseek(pFile,0,FILE_END);
		int nLen = ftell(pFile);
		char *buffer = new char[nLen+1];
		rewind(pFile);
		fread(buffer,nLen,1,pFile);
		buffer[nLen] = '\0';
		fclose(pFile);
		return buffer;
	}
	fclose(pFile);
	return nullptr;
}


GLuint CreateGPUProgram(const char* vsShaderPath,const char* fsShaderPath)
{
	GLuint vsShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Load
	const char* vsCode = LoadFileContent(vsShaderPath);
	const char* fsCode = LoadFileContent(fsShaderPath);

	//Compile
	glShaderSource(vsShader,1,&vsCode,nullptr);
	glShaderSource(fsShader,1,&fsCode,nullptr);
	glCompileShader(vsShader);
	glCompileShader(fsShader);

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

	return program;
}


/////////////      VF Pipline     ////////////////////////////////

void VFRender()
{
	glUseProgram(s_program);
	glUniformMatrix4fv(s_shaderData.MLocation,1,GL_FALSE,identify);
	glUniformMatrix4fv(s_shaderData.VLocation,1,GL_FALSE,identify);
	glUniformMatrix4fv(s_shaderData.PLocation,1,GL_FALSE,glm::value_ptr(s_shaderData.projection));

	glBindBuffer(GL_ARRAY_BUFFER,s_shaderData.vbo);
	glEnableVertexAttribArray(s_shaderData.posLocation);
	glVertexAttribPointer(s_shaderData.posLocation,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),nullptr); 
	glEnableVertexAttribArray(s_shaderData.colorLoacation);
	glVertexAttribPointer(s_shaderData.colorLoacation,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(sizeof(float)*3)); 

	glDrawArrays(GL_TRIANGLES,0,3);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);
}

void VFPrepare()
{
	glewInit();
	s_program = CreateGPUProgram("draw.vs","draw.fs");
	

	//Get Attribute in shader
	s_shaderData.posLocation = glGetAttribLocation(s_program,"pos");
	s_shaderData.colorLoacation = glGetAttribLocation(s_program,"color");

	//Get uniform in shader
	s_shaderData.MLocation = glGetUniformLocation(s_program,"M");
	s_shaderData.VLocation = glGetUniformLocation(s_program,"V");
	s_shaderData.PLocation = glGetUniformLocation(s_program,"P");

	s_shaderData.projection =glm::perspective(45.0f,800.0f/600.0f,0.1f,1000.0f);
	
	Vertex vertex[3];
	vertex[0].pos[0] = 0;
	vertex[0].pos[1] = 0;
	vertex[0].pos[2] = -100;
	vertex[0].color[0] = 1.0f;
	vertex[0].color[1] = 1.0f;
	vertex[0].color[2] = 1.0f;
	vertex[0].color[3] = 1.0f;

	vertex[1].pos[0] = 10;
	vertex[1].pos[1] = 0;
	vertex[1].pos[2] = -100;
	vertex[1].color[0] = 1.0f;
	vertex[1].color[1] = 1.0f;
	vertex[1].color[2] = 1.0f;
	vertex[1].color[3] = 1.0f;

	vertex[2].pos[0] = 0;
	vertex[2].pos[1] = 10;
	vertex[2].pos[2] = -100;
	vertex[2].color[0] = 1.0f;
	vertex[2].color[1] = 1.0f;
	vertex[2].color[2] = 1.0f;
	vertex[2].color[3] = 1.0f;

	//send vbo to GPU
	glGenBuffers(1,&s_shaderData.vbo);
	glBindBuffer(GL_ARRAY_BUFFER,s_shaderData.vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*3,vertex,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

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
	if (s_pipline == VFPipline)
	{
		VFPrepare();
	}
	glClearColor(41.0/255.0f,71.0f/255.0f,121.0f/255.0f,1.0f);

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
		glClear(GL_COLOR_BUFFER_BIT);
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