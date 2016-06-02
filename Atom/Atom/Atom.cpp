#include "stdafx.h"
#include "Atom.h"
#include <gl/glut.h>
#include <math.h>
#include <vector>
#include "lodepng.h"

#define GL_PI 3.1415f
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat rotateAngle = 0.0f;

GLuint sunTexID;
GLuint earthTexID;
GLuint moonTexID;

void SetupRC()
{
	GLfloat lightPos[] = { 0.0f, 0.0f, -100.0f, 1.0f };
	
	GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat diffuseLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat spotDir[] = { 0.0f, 1.0f, -1.0f };

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0f);

	glTranslatef(0.0f, 0.0f, -150.0f);

	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 90);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void LoadTexture(GLuint* texID, std::string fileName)
{
	std::vector<unsigned char> image;
	unsigned width, height, error;
	error = lodepng::decode(image, width, height, fileName);

	if (error)
	{
		OutputDebugString(L"texture loading error\n");
		return;
	}

	glGenTextures(1, texID);
	glBindTexture(GL_TEXTURE_2D, *texID);

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
	/*glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);*/

	// light 연산을 위함
	//https://www.opengl.org/archives/resources/faq/technical/texture.htm
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void TimerFunc(int value)
{
	glutPostRedisplay();
	glutTimerFunc(20, TimerFunc, 1);
}

void ChangeSizePerspective(int w, int h)
{
	GLfloat fAspect;
	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, fAspect, 1.0f, 500.0f);

	glTranslatef(0.0f, 0.0f, -100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CreateSun()
{
	static GLfloat sunAngle = 0.0f;
	
	glPushMatrix();
	glLoadIdentity();
	glRotatef(sunAngle, 0.0f, 1.0f, 0.0f);

	glColor3ub(255, 255, 0);
	GLUquadric* sun = gluNewQuadric();

	gluQuadricDrawStyle(sun, GLU_FILL);
	glBindTexture(GL_TEXTURE_2D, sunTexID);
	gluQuadricTexture(sun, GL_TRUE);
	gluQuadricNormals(sun, GLU_SMOOTH);

	gluSphere(sun, 15.0, 32, 16);

	glPopMatrix();

	sunAngle += 0.1f;
	if (sunAngle > 360.0f)
		sunAngle = 0.0f;
}

void CreateSphere(GLuint texID, GLdouble radius, GLint slice, GLint stack)
{
	glPushMatrix();

	glRotatef(rotateAngle, 0.0f, 1.0f, 0.0f);
	GLUquadric* sphere = gluNewQuadric();

	gluQuadricDrawStyle(sphere, GLU_FILL);
	glBindTexture(GL_TEXTURE_2D, texID);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	
	gluSphere(sphere, radius, slice, stack);

	glPopMatrix();
}

void RenderScene()
{
	static GLfloat fElect1 = 0.0f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -100.0f);
	CreateSphere(sunTexID, 40.0f, 40, 20);

	glPushMatrix();
		glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
		glTranslatef(90.0f, 0.0f, 0.0f);
		CreateSphere(earthTexID, 12.0f, 20, 8);

		glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
		glTranslatef(30.0f, 0.0f, 0.0f);
		CreateSphere(moonTexID, 5.0f, 40, 20);
	glPopMatrix();

	//float minRad = 10.0f;
	//float maxRad = 20.0f;

	//for (int i = 0; i < 100; ++i)
	//{
	//	float delta = maxRad - minRad;
	//	double degree = 3.14 / 2;
	//	double a = delta*cosf(modf((rand() * 0.1f * i),&degree));
	//	//float delta2 = modf(delta, &a);
	//	float b = rand();
	//	float random = 3.14 * b * b * 2.0;
	//	

	//	glPushMatrix();
	//	glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
	//	glTranslatef(90.0f, 0.0f, 0.0f);
	//	
	//	glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
	//	glRotatef(fElect1 + random, 0.0f, 1.0f, 0.0f);
	//	glTranslatef(minRad + a, 0.0f, 0.0f);
	//	glColor3ub(200, 200, 200);
	//	glutSolidSphere(0.2f - 0.0001*i, 15, 15);
	//	glPopMatrix();
	//}

	fElect1 += 0.5f;
	if (fElect1 > 360.0f)
		fElect1 = 0.0f;


	rotateAngle += 0.1f;
	if (rotateAngle > 360.0f)
		rotateAngle = 0.0f;

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
	glutCreateWindow("Solar System");

	glutReshapeFunc(ChangeSizePerspective);
	glutTimerFunc(33, TimerFunc, 1);
	glutSpecialFunc(ControlKey); // add
	glutDisplayFunc(RenderScene);
	SetupRC();

	LoadTexture(&sunTexID, "Texture/texture_sun.png");
	LoadTexture(&earthTexID, "Texture/texture_earth_clouds.png");
	LoadTexture(&moonTexID, "Texture/texture_moon.png");

	glutMainLoop();
}