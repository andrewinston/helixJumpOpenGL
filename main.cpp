#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

const double ACEL_COLISAO = 10;
const double ACEL_GRAVIDADE = 3;
const double RAIO_ESFERA = 1;
const int SLICES = 30; //quantidade de slices que dividirão a esfera

class Bola{
private:
	double x, y, z;
	double vx, vy, vz;

public:
	Bola(){
		x = 0;
		y = 0;
		z = 0;
		vx = 0;
		vy = 0;
		vz = 0;
	}
	void move(){
		acelera();
		x += vx;
		y += vy;
		z += vz;
	}
	void colide(){
		vz -= ACEL_COLISAO;
	}
	void acelera(){
		z += ACEL_GRAVIDADE;
	}
	void desenha(){
		glPushMatrix();
			glTranslatef(x, y, z);
			gluSphere(gluNewQuadric(), RAIO_ESFERA, SLICES, SLICES);
		glPopMatrix();
	}

};

Bola bola;

void inicializacao() {
	glClearColor(0.5, 0.5, 0.5, 0.0);
}


void funcaoDisplay() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-4, 6, -4, 6);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);

	gluLookAt(0, 20, 100, 0, 0, 100, 0, 0, 1);
	
	bola.desenha();
}

void funcaoKeyboard(unsigned char key, int x, int y) {
/*
	if(key == 'q') {
		exit(-1);
	}
	if(!animacao) {
		if(key == 'a') {
			tAtual -= 0.01;
		}
		if(key == 'd') {
			tAtual += 0.01;
		}
	}
	if(key == 's')
		animacao = 1 - animacao;
	glutPostRedisplay();*/
}

void temporizador() {
	glutPostRedisplay();
	/*if(animacao) {
		tAtual += 0.003*delta;
		if(tAtual > 1) {
			tAtual = 1;
			delta = -1;
		}
		if(tAtual < 0) {
			tAtual = 0;
			delta = 1;
		}
	}*/
}

void funcaoMouse(int button, int state, int x, int y) {

	/*GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble worldX, worldY, worldZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	winZ = 0;

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
	if(button == GLUT_LEFT_BUTTON) {
		vx[0] = worldX;
		vy[0] = worldY;
	}
	if(button == GLUT_RIGHT_BUTTON) {
		vx[2] = worldX;
		vy[2] = worldY;
	}*/
}

int main(int argc, char **argv) {
	
	bola = Bola();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hello world");
	glutKeyboardFunc(funcaoKeyboard);
	glutMouseFunc(funcaoMouse);
	glutDisplayFunc(funcaoDisplay);
	glutIdleFunc(temporizador);
	inicializacao();

	glutMainLoop();

	return 0;
}

