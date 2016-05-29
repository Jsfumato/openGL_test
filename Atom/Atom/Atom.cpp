#include "stdafx.h"
#include "Atom.h"
#include <gl/glut.h>
#include <math.h>

#define GL_PI 3.1415f
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

void SetupRC()
{
	GLfloat lightPos[] = { 0.0f, 0.0f, -100.0f, 1.0f };
	
	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	GLfloat spotDir[] = { 0.0f, 0.0f, -1.0f };

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0f);

	glTranslatef(0.0f, 0.0f, -150.0f);

	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void LoadTexture()
{

}

void TimerFunc(int value)
{
	glutPostRedisplay();
	glutTimerFunc(20, TimerFunc, 1);
}

void ChangeSizeOrtho(int w, int h)
{
	GLfloat nRange = 100.0f;

	glViewport(0, 0, w, h);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange*2.0f, nRange*2.0f);

	else
		glOrtho(-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange*2.0f, nRange*2.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ChangeSizePerspective(int w, int h)
{
	GLfloat fAspect;
	glViewport(0, 0, w, h);


	fAspect = (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0f, fAspect, 1.0f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void RenderScene()
{
	static GLfloat fElect1 = 0.0f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -100.0f);
	glColor3ub(255, 0, 0);
	glutSolidSphere(10.0f, 15, 15);

	glPushMatrix();
		glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
		glTranslatef(90.0f, 0.0f, 0.0f);
		glColor3ub(255, 255, 0);
		glutSolidSphere(6.0f, 15, 15);

		glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
		glTranslatef(30.0f, 0.0f, 0.0f);
		glColor3ub(200, 200, 200);
		glutSolidSphere(2.0f, 15, 15);
	glPopMatrix();

	float minRad = 10.0f;
	float maxRad = 20.0f;

	for (int i = 0; i < 100; ++i)
	{
		float delta = maxRad - minRad;
		double degree = 3.14 / 2;
		double a = delta*cosf(modf((rand() * 0.1f * i),&degree));
		//float delta2 = modf(delta, &a);
		float b = rand();
		float random = 3.14 * b * b * 2.0;
		

		glPushMatrix();
		glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
		glTranslatef(90.0f, 0.0f, 0.0f);
		
		glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(fElect1 + random, 0.0f, 1.0f, 0.0f);
		glTranslatef(minRad + a, 0.0f, 0.0f);
		glColor3ub(200, 200, 200);
		glutSolidSphere(0.2f - 0.0001*i, 15, 15);
		glPopMatrix();
	}

	fElect1 += 0.2f;
	if (fElect1 > 360.0f)
		fElect1 = 0.0f;

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
	glutInitWindowPosition(0, 0); // add
	glutInitWindowSize(512, 512); // add
	glutCreateWindow("Atom");

	//glutReshapeFunc(ChangeSizeOrtho);
	glutReshapeFunc(ChangeSizePerspective);
	glutTimerFunc(33, TimerFunc, 1);
	glutSpecialFunc(ControlKey); // add
	glutDisplayFunc(RenderScene);
	SetupRC();
	LoadTexture();

	glutMainLoop();
}