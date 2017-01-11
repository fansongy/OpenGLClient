#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

void RenderGL()
{
	glBegin(GL_TRIANGLES);
	glVertex3f(0,0,-100);
	glVertex3f(10,0,-100);
	glVertex3f(0,10,-100);
	glEnd();

}

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
	glClearColor(41.0/255.0f,71.0f/255.0f,121.0f/255.0f,1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,800.0f/600.0f,0.1f,1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
		RenderGL();
		SwapBuffers(GetDC(hwnd));
	}
	return 0;

}