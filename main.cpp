#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

const double ACEL_COLISAO;
const double ACEL_GRAVIDADE;
const double RAIO_ESFERA;
const int SLICES;

class Bola{
private:
	double x, y, z;
	double vx, vy, vz;

public:
	Bola() x(0), y(0), z(100) {}
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
		gluPushMatrix();
			glTranslatef(x, y, z);
			gluSphere(RAIO_ESFERA, SLICES, SLICES);
		gluPopMatrix();
	}

};

void funcaoDisplay() {

	gluLookAt(0, 0, 100, );
	
	
}

void funcaoKeyboard(unsigned char key, int x, int y) {

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
	glutPostRedisplay();
}

void temporizador() {
	glutPostRedisplay();
	if(animacao) {
		tAtual += 0.003*delta;
		if(tAtual > 1) {
			tAtual = 1;
			delta = -1;
		}
		if(tAtual < 0) {
			tAtual = 0;
			delta = 1;
		}
	}
}

void funcaoMouse(int button, int state, int x, int y) {

	GLint viewport[4];
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
	}
}

int main(int argc, char **argv) {
	
	Bola bola = new Bola();

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

