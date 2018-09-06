#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>

float tAtual = 2.0; //um ponto é renderizado na reta
int delta = 1; //= 1 ou -1... variação positiva ou negativa de tAtual, quando animacao = 1
int animacao = 1;
float top = 6, bottom = -4, left = -4, right = 6;
int showGuideLines = true;

using namespace std;

//vetor que contem as abscissas dos pontos
vector<float> vx;
//vetor que contem as ordenadas dos pontos
vector<float> vy;

enum {ADICIONARREMOVER, ALTERAR};
int estado = ADICIONARREMOVER;
int pontoAtual = 0;

const double ACEL_COLISAO = 10;
const double ACEL_GRAVIDADE = 3;
const double RAIO_ESFERA = 1;
const int SLICES = 30; //quantidade de slices que dividirão a esfera
const int RAIO_CILINDRO = 5;

class Bola{
private:
	double x, y, z;
	double vx, vy, vz;

public:

	double getX(){return x;}
	double getY(){return y;}
	double getZ(){return z;}
	
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
		/*glColor3f(1,1,1);
		glPushMatrix();
			//glTranslatef(x, y, z);
			glutSolidSphere(RAIO_ESFERA, SLICES, SLICES);
		glPopMatrix();*/
		
		
		glPushMatrix ();
			glRotatef       (60.0, 1,0,0);
			//glRotatef       (zRotated, 0,0,1);
			glutSolidSphere (RAIO_ESFERA, SLICES, SLICES);
		glPopMatrix ();
	}

};


Bola bola;
double zRotated = 0;

void inicializacao() {
	glClearColor(0.5, 0.5, 0.5, 0);
}

//implementação da função spline (use a b-spline quadrática que consta nas notas de aula)
float g(float t) {
	return 0;
}

//retorna o ponto da spline s(t)
float spline(vector<float> v, float t) {
	return 0;
}


void funcaoDisplay() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, right, bottom, top);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f (0.8, 0.2, 0.1);              // Red ball displaced to left.
	bola.desenha();
    /*glPushMatrix ();
    	glTranslatef    (-1.5, 0.0, 0.0);
    	glRotatef       (60.0, 1,0,0);
    	glRotatef       (zRotated*2.0, 0,0,1);   // Red ball rotates at twice the rate of blue ball.
    	glutSolidSphere (1.0, 50, 50);
    glPopMatrix ();
 
    glColor3f (0.1, 0.2, 0.8);              // Blue ball displaced to right.
    glPushMatrix ();
		glTranslatef    (1.5, 0.0, 0.0);
		glRotatef       (60.0, 1,0,0);
		glRotatef       (zRotated, 0,0,1);
		glutSolidSphere (1.0, 20, 50);
	glPopMatrix ();*/

	glutSwapBuffers();
	glFlush();
}

void funcaoKeyboard(unsigned char key, int x, int y) {
	if(key == 'q') {
		exit(-1);
	}
	glutPostRedisplay();
}

void temporizador() {
	zRotated+=0.3;
	glutPostRedisplay();
}

void funcaoMouseMotion(int x, int y) {

}

void funcaoMouse(int button, int state, int x, int y) {
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	bola = Bola();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Splines");
	glutKeyboardFunc(funcaoKeyboard);
	glutMouseFunc(funcaoMouse);
	glutMotionFunc(funcaoMouseMotion);
	glutDisplayFunc(funcaoDisplay);
	glutIdleFunc(temporizador);
	inicializacao();

	glutMainLoop();
	
	return 0;
}

