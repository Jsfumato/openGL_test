#include "stdafx.h"
#include "gl_texture.h"
#include <gl/glut.h>
#include <math.h>
#include "lodepng.h"

#define GL_PI 3.1415f
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zDistance = 0.0f;

GLfloat	amb[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat	dif[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat	lightPos[] = { -50.0f, 50.0f, 100.0f, 1.0f };

GLuint	texID;

void SetupRC()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void LoadTexture()
{
	std::vector<unsigned char> image;
	unsigned width, height, error;

	error = lodepng::decode(image, width, height, "texture.png");

	if (error)
		printf("error &u : %s\n", error, lodepng_error_text(error));

	printf("\nimage size is %i", image.size());

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
}

void ChangeSize(int w, int h)
{
	GLfloat fAspect;
	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, fAspect, 1.0f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -50.0f);
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, zDistance);

		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		glEnable(GL_LIGHT0);

		glColor3f(1.0f, 1.0f, 1.0f);
		glutSolidTeapot(10.0f);
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
	glutInitWindowPosition(0, 0); // add
	glutInitWindowSize(800, 800); // add
	glutCreateWindow("2d Texture");

	glutSpecialFunc(ControlKey); // add
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	SetupRC();
	LoadTexture();

	glutMainLoop();
}