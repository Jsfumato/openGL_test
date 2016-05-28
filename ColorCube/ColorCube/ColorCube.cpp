#include "stdafx.h"
#include "ColorCube.h"
#include <gl/glut.h>
#include <math.h>
#include <vector>
#include <functional>

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat width = 10.0f;

std::vector<std::function<void(void)>> vertexVector;

void SetVertex()
{
	vertexVector.clear();
	vertexVector.push_back(
		[]() {
		glColor3ub((GLubyte)0, (GLubyte)255, (GLubyte)0);
		glVertex3f(0.0f, width, 0.0f);
	});
	vertexVector.push_back(
		[]() {
		glColor3ub((GLubyte)0, (GLubyte)255, (GLubyte)255);
		glVertex3f(0.0f, width, width);
	});
	vertexVector.push_back(
		[]() {
		glColor3ub((GLubyte)255, (GLubyte)255, (GLubyte)255);
		glVertex3f(width, width, width);
	});
	vertexVector.push_back(
		[]() {
		glColor3ub((GLubyte)255, (GLubyte)255, (GLubyte)0);
		glVertex3f(width, width, 0.0f);
	});
	vertexVector.push_back(
		[]() {
		glColor3ub((GLubyte)0, (GLubyte)0, (GLubyte)0);
		glVertex3f(0.0f, 0.0f, 0.0f);
	});
	vertexVector.push_back(
		[]() {
		glColor3ub((GLubyte)0, (GLubyte)0, (GLubyte)255);
		glVertex3f(0.0f, 0.0f, width);
	});
	vertexVector.push_back(
		[]() {
		glColor3ub((GLubyte)255, (GLubyte)0, (GLubyte)255);
		glVertex3f(width, 0.0f, width);
	});
	vertexVector.push_back(
		[]() {
		glColor3ub((GLubyte)255, (GLubyte)0, (GLubyte)0);
		glVertex3f(width, 0.0f, 0.0f);
	});
}

void SetupRC()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
}

void ChangeSize(int w, int h)
{
	//glViewport(0, 0, w, h);
	//GLfloat fAspect = (GLfloat)w / (GLfloat)h;
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//gluPerspective(60.0f, fAspect, 1.0f, 500.0f);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-100.0f, 100.0f, -100.0f*h / w, 100.0f*h / w, 100.0f, -100.0f);
	else
		glOrtho(-100.0f*w / h, 100.0f*w / h, -100.0f, 100.0f, 100.0f, -100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);

	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	glBegin(GL_QUADS);
		vertexVector[0]();
		vertexVector[1]();
		vertexVector[2]();
		vertexVector[3]();
	glEnd();		   
					   
	glBegin(GL_QUADS); 
		vertexVector[4]();
		vertexVector[7]();
		vertexVector[6]();
		vertexVector[5]();
	glEnd();		   
					   
	glBegin(GL_QUAD_STRIP);
		vertexVector[1]();
		vertexVector[5]();
		vertexVector[2]();
		vertexVector[6]();
		vertexVector[3]();
		vertexVector[7]();
		vertexVector[0]();
		vertexVector[4]();
		vertexVector[1]();
		vertexVector[5]();
	glEnd();

	glPopMatrix();

	glutSwapBuffers();
}

void ControlKey(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;
	if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;
	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;
	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;
	glutPostRedisplay();
}

int APIENTRY wWinMain(_In_ HINSTANCE		hInstance,
	_In_opt_ HINSTANCE	hPrevInstance,
	_In_ LPWSTR			lpCmdLine,
	_In_ int			nCmdShow)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(512, 512);
	glutCreateWindow("ColorCube");

	SetVertex();
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(ControlKey);
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();
}