#include "stdafx.h"
#include "Collision.h"
#include <gl/glut.h>
#include <math.h>
#include <vector>
#include <tuple>

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

std::vector<std::tuple<GLfloat, GLfloat, GLfloat, GLfloat, GLsizei>> rectVector;

// 윈도우크기선언
GLfloat window_width;
GLfloat window_height;

void Initialize()
{
	rectVector.clear();
	rectVector.push_back(std::make_tuple(0.0f, 0.0f, 1.0f, 1.0f, 40.0f));
	rectVector.push_back(std::make_tuple(-100.0f, -100.0f, 1.0f, 1.0f, 40.0f));
	rectVector.push_back(std::make_tuple(-50.0f, -50.0f, 1.0f, 1.0f, 40.0f));
	rectVector.push_back(std::make_tuple(50.0f, 50.0f, 1.0f, 1.0f, 40.0f));
}

void SetupRC()
{
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void CheckCollision()
{
	for (auto& iter = rectVector.begin(); iter < rectVector.end(); ++iter)
	{
		GLfloat& xPos = std::get<0>(*iter);
		GLfloat& yPos = std::get<1>(*iter);
		GLfloat& xStep = std::get<2>(*iter);
		GLfloat& yStep = std::get<3>(*iter);
		GLsizei& rsize = std::get<4>(*iter);

		// x축범위(window 안에서돌아다니도록설정)
		if (xPos > window_width - rsize	|| xPos < -window_width)
			xStep = -xStep;

		// y축범위(window 안에서돌아다니도록설정)
		if (yPos>window_height - rsize	|| yPos< -window_height)
			yStep = -yStep;

		// 윈도우가변경되어경계를넘어갔을때
		if (xPos > window_width - rsize)
			xPos = window_width - rsize - 1;
		if (yPos > window_height - rsize)
			yPos = window_height - rsize - 1;

		xPos += xStep;
		yPos += yStep;
	}
}

void CheckCollisionRect()
{
	for (auto& iter = rectVector.begin(); iter < (rectVector.end() - 1); ++iter)
	{
		GLfloat& xPos = std::get<0>(*iter);
		GLfloat& yPos = std::get<1>(*iter);
		GLfloat& xStep = std::get<2>(*iter);
		GLfloat& yStep = std::get<3>(*iter);
		GLsizei rsize = std::get<4>(*iter);

		for (auto& iterNext = (iter + 1); iterNext < rectVector.end(); ++iterNext)
		{
			GLfloat& xPosNext = std::get<0>(*iterNext);
			GLfloat& yPosNext = std::get<1>(*iterNext);
			GLfloat& xStepNext = std::get<2>(*iterNext);
			GLfloat& yStepNext = std::get<3>(*iterNext);
			//GLsizei rsizeNext = std::get<4>(*iterNext);

			//if (xPos < xPosNext)
			//	rsize = rsize;
			//if (xPosNext < xPos)
			//	rsize = rsizeNext;

			if ((abs(xPos - xPosNext) > rsize) || (abs(yPos - yPosNext) > rsize))
				continue;

			if (abs(xPos - xPosNext) > abs(yPos - yPosNext))
			{
				xStep = -xStep;
				xStepNext = -xStepNext;
			}

			if (abs(xPos - xPosNext) < abs(yPos - yPosNext))
			{
				yStep = -yStep;
				yStepNext = -yStepNext;
			}
		}
	}
}

void TimerFunction(int value)
{  
	CheckCollisionRect();
	CheckCollision();
	
	glutPostRedisplay(); // 다시그리기
	glutTimerFunc(33, TimerFunction, 1);
}

void ChangeSize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
	{
		window_width = 100.0f;
		window_height = 100.0f*h / w;
		glOrtho(-100.0f, 100.0f, -window_height, window_height, 1.0, -1.0);
	}
	else
	{
		window_width = 100.0f*w / h;
		window_height = 100.0f;
		glOrtho(-window_width, window_width, -100.0f, 100.0f, 1.0, -1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	for (auto& iter : rectVector)
	{
		GLfloat	xPos = std::get<0>(iter);
		GLfloat	yPos = std::get<1>(iter);
		GLsizei rsize = std::get<4>(iter);
		glRectf(xPos, yPos, xPos + rsize, yPos + rsize);
	}
	glutSwapBuffers();
}

int APIENTRY wWinMain(_In_ HINSTANCE		hInstance,
	_In_opt_ HINSTANCE	hPrevInstance,
	_In_ LPWSTR			lpCmdLine,
	_In_ int			nCmdShow)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	Initialize();

	glutInitWindowPosition(0, 0); // add
	glutInitWindowSize(800, 600); // add
	glutCreateWindow("Rect_Collision");

	glutReshapeFunc(ChangeSize);
	glutTimerFunc(2000, TimerFunction, 1);
	glutDisplayFunc(RenderScene);
	SetupRC();

	glutMainLoop();
}