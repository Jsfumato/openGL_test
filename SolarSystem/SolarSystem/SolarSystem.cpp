#include "stdafx.h"
#include "SolarSystem.h"
#include <gl/glut.h>
#include <math.h>
#include <vector>
#include <string>
#include "lodepng.h"

#define GL_PI 3.1415f
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat rotateAngle = 0.0f;

GLuint sunTexID;
GLuint mercuryTexID;
GLuint VenusTexID;
GLuint earthTexID;
GLuint moonTexID;
GLuint marsTexID;
GLuint jupiterTexID;
GLuint saturnTexID;
GLuint uranusTexID;
GLuint neptuneTexID;

void SetupRC()
{
	GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };

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
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 360.0f);

	//glTranslatef(0.0f, 0.0f, -150.0f);

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
	gluPerspective(60.0f, fAspect, 1.0f, 1000.0f);

	glTranslatef(0.0f, 0.0f, -300.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	OutputDebugString(L"Perspective");
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

void DrawCircle(float radius)
{
	glPushMatrix();
	
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 0.0f, 0.0f);
	GLfloat delta = 3.14f / 360 * 2;

	for (int i = 0; i < 360; ++i)
	{
		glVertex3f(radius*cosf(delta*i), 0.0f, radius*sinf(delta*i));
		float a = radius*cosf(delta*i);
		float b = radius*sinf(delta*i);
	}
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);

	glPopMatrix();
}

void RenderScene()
{
	static GLfloat fElect1 = 0.0f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	CreateSphere(sunTexID, 20.0f, 40, 20);

	// draw 수성
	glPushMatrix();
		DrawCircle(24.0f);
		glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
		glTranslatef(24.0f, 0.0f, 0.0f);
		CreateSphere(mercuryTexID, 4.0f, 20, 8);
	glPopMatrix();

	// draw 금성
	glPushMatrix();
		DrawCircle(54.0f);
		glRotatef(fElect1*1.1, 0.0f, 1.0f, 0.0f);
		glTranslatef(54.0f, 0.0f, 0.0f);
		CreateSphere(VenusTexID, 9.0f, 20, 8);
	glPopMatrix();

	// draw earth & moon
	glPushMatrix();
		DrawCircle(75.0f);
		glRotatef(fElect1*1.2, 0.0f, 1.0f, 0.0f);
		glTranslatef(75.0f, 0.0f, 0.0f);
		CreateSphere(earthTexID, 10.0f, 20, 8);

		DrawCircle(13.0f);
		glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
		glTranslatef(13.0f, 0.0f, 0.0f);
		CreateSphere(moonTexID, 2.0f, 40, 20);
	glPopMatrix();

	// draw 화성
	glPushMatrix();
		DrawCircle(114.0f);
		glRotatef(fElect1*0.9, 0.0f, 1.0f, 0.0f);
		glTranslatef(114.0f, 0.0f, 0.0f);
		CreateSphere(marsTexID, 5.0f, 20, 8);
	glPopMatrix();

	// draw 목성
	glPushMatrix();
		DrawCircle(150.0f);
		glRotatef(fElect1*0.8, 0.0f, 1.0f, 0.0f);
		glTranslatef(150.0f, 0.0f, 0.0f);
		CreateSphere(jupiterTexID, 20.0f, 20, 8);
	glPopMatrix();

	// draw 토성
	glPushMatrix();
	DrawCircle(200.0f);
	glRotatef(fElect1*1.05, 0.0f, 1.0f, 0.0f);
	glTranslatef(200.0f, 0.0f, 0.0f);
	CreateSphere(saturnTexID, 18.0f, 20, 8);
	glPopMatrix();

	// draw 천왕성
	glPushMatrix();
	DrawCircle(250.0f);
	glRotatef(fElect1*0.95, 0.0f, 1.0f, 0.0f);
	glTranslatef(250.0f, 0.0f, 0.0f);
	CreateSphere(uranusTexID, 8.0f, 20, 8);
	glPopMatrix();

	// draw 해왕성
	glPushMatrix();
	DrawCircle(300.0f);
	glRotatef(fElect1*0.8, 0.0f, 1.0f, 0.0f);
	glTranslatef(300.0f, 0.0f, 0.0f);
	CreateSphere(neptuneTexID, 8.0f, 20, 8);
	glPopMatrix();
	
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
	LoadTexture(&mercuryTexID, "Texture/texture_mercury.png");
	LoadTexture(&VenusTexID, "Texture/texture_venus_surface.png");
	LoadTexture(&earthTexID, "Texture/texture_earth_clouds.png");
	LoadTexture(&moonTexID, "Texture/texture_moon.png");
	LoadTexture(&marsTexID, "Texture/texture_mars.png");
	LoadTexture(&jupiterTexID, "Texture/texture_jupiter.png");
	LoadTexture(&saturnTexID, "Texture/texture_saturn.png");
	LoadTexture(&uranusTexID, "Texture/texture_uranus.png");
	LoadTexture(&neptuneTexID, "Texture/texture_neptune.png");

	glutMainLoop();
}