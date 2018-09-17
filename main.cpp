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
double ycam=0;
double xcam=0;
double zcam=0;
double xbola=0,ybola=0,zbola=0;
using namespace std;
double rot = 0;
//y: pra cima
//x: pra dentro da tela
//z: pra direita

//vetor que contem as abscissas dos pontos
vector<float> vx;
//vetor que contem as ordenadas dos pontos
vector<float> vy;

enum {ADICIONARREMOVER, ALTERAR};
int estado = ADICIONARREMOVER;
int pontoAtual = 0;

const double ACEL_COLISAO = 10;
const double ACEL_GRAVIDADE = 3;
const double RAIO_ESFERA = 0.5;
const int SLICES = 100; //quantidade de slices que dividirão a esfera
const double RAIO_CILINDRO = 1;
const double DISTANCIA_BOLA_CILINDRO = 1.5;
const double RAIO_OBSTACULO = 0.6;//RAIO_CILINDRO+DISTANCIA_BOLA_CILINDRO;
const double PI = 3.14159265359;
class Bola{
private:
	double x, y, z;
	double vx, vy, vz;

public:

	double getX(){return x;}
	double getY(){return y;}
	double getZ(){return z;}
	void upd(double a, double b, double c){x=a,y=b,z=c;}
	Bola(){
		x = -2;
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
		upd(xbola+DISTANCIA_BOLA_CILINDRO*cos(rot), ybola, zbola+DISTANCIA_BOLA_CILINDRO*sin(rot));
		glColor3f(1, 0,0 );
		glPushMatrix ();
			glTranslatef(x, y, z);
			//glRotatef       (zRotated, 0,0,1);
			glutWireSphere (RAIO_ESFERA, SLICES, SLICES);
		glPopMatrix ();
	}

};


Bola bola;
double zRotated = 0;

void inicializacao() {
	glClearColor(0.5, 0.5, 0.5, 0);
}

void setorCircular(double x, double y, double z, double raio, double anguloIniDeg, double anguloFimDeg, int partes=100){
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x, y, z);
	double anguloIni = (PI/180.0)*anguloIniDeg;
	double anguloFim = (PI/180.0)*anguloFimDeg;
	for(double angle = 0; angle <= anguloIni; angle += (2*PI)/partes){
		glVertex3f(x+sin(angle)*raio, y, z+cos(angle)*raio);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x, y, z);
	for(double angle = anguloFim; angle <= 2*PI; angle += (2*PI)/partes){
		glVertex3f(x+sin(angle)*raio, y, z+cos(angle)*raio);
	}
	glEnd();
}

void setorCilindrico(double x, double y, double z, double h, double raio, double anguloIniDeg, double anguloFimDeg, int partes=100){
	setorCircular(x, y, z, raio, anguloIniDeg, anguloFimDeg, partes);
	setorCircular(x, y-h, z, raio, anguloIniDeg, anguloFimDeg, partes);
	double anguloIni = (PI/180.0)*anguloIniDeg;
	double anguloFim = (PI/180.0)*anguloFimDeg;
	double inc = (2*PI)/partes;
	glColor3f(1, .5, .5);
	for(double angle = 0; angle < anguloIni; angle += inc){
		glBegin(GL_QUADS);
		glVertex3f(x+sin(angle)*raio, y, z+cos(angle)*raio);
		glVertex3f(x+sin(angle)*raio, y-h, z+cos(angle)*raio);
		glVertex3f(x+sin(angle+inc)*raio, y, z+cos(angle+inc)*raio);
		glVertex3f(x+sin(angle+inc)*raio, y-h, z+cos(angle+inc)*raio);
		glEnd();
	}
	glColor3f(0.5, .5, 1);
	for(double angle = anguloIni; angle <= anguloFim; angle += inc){
		glBegin(GL_QUADS);
		glVertex3f(x+sin(angle)*raio, y, z+cos(angle)*raio);
		glVertex3f(x+sin(angle)*raio, y-h, z+cos(angle)*raio);
		glVertex3f(x+sin(angle+inc)*raio, y, z+cos(angle+inc)*raio);
		glVertex3f(x+sin(angle+inc)*raio, y-h, z+cos(angle+inc)*raio);
		glEnd();
	}
	
	glBegin(GL_QUADS);
	glVertex3f(x, y, z);
	glVertex3f(x, y-h, z);
	glVertex3f(x+sin(anguloIni)*raio, y, z+cos(anguloIni)*raio);
	glVertex3f(x+sin(anguloIni)*raio, y-h, z+cos(anguloIni)*raio);
	glEnd();
	
	glBegin(GL_QUADS);
	glVertex3f(x, y, z);
	glVertex3f(x, y-h, z);
	glVertex3f(x+sin(anguloFim)*raio, y, z+cos(anguloFim)*raio);
	glVertex3f(x+sin(anguloFim)*raio, y-h, z+cos(anguloFim)*raio);
	glEnd();
}

void pizza(double x, double y, double z, double raio, double anguloIni, double anguloFim, int partes=100){
	
}

//desenha circulo centrado em (x, y, z) com raio, paralelo ao plano XZ
void circulo(double x, double y, double z, double raio, int partes=100){
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x, y, z);
	for(double angle = 0; angle <= 2*PI; angle+=(2*PI)/partes){
		glVertex3f(x + sin(angle)*raio, y, z+cos(angle)*raio);
	}
	glEnd();
}

//desenha cilindro com topo em (x, y, z) e fundo em (x, y-h, z) com raio
void cilindro(double x, double y, double z, double h, double raio){
	circulo(x, y, z, raio);
	glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), raio, raio, h, SLICES, SLICES);
	glPopMatrix();
	circulo(x, y-h, z, raio);
}

void desenhaCilindro(){
	glColor3f(0, 1, 0);
	cilindro(0, 0, 0, 200, 1);
	/*glPushMatrix();
		glTranslatef(0, 0, 0);
		glRotatef(90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 1, 1, 200, SLICES, SLICES);
	glPopMatrix();*/
}

void desenhaObstaculos(){
	glColor3f(0, 0, 1);
	setorCilindrico(0, -2, 0, 1, RAIO_OBSTACULO, 90, 180);
}

void funcaoDisplay() {
	bola.upd(xbola+DISTANCIA_BOLA_CILINDRO*cos(rot), ybola, zbola+DISTANCIA_BOLA_CILINDRO*sin(rot));
	//glFrustum(-300, 300, -300, 300, 0, 2000);
	gluPerspective(180, 1, 0.001, 3000);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-4, 4, -4, 4);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f (0.8, 0.2, 0.1);              // Red ball displaced to left.
	gluLookAt(bola.getX()*(RAIO_ESFERA), bola.getY()-.1, bola.getZ()*(RAIO_ESFERA), bola.getX(),bola.getY(),bola.getZ(), 0, 1, 0);
	//gluLookAt(bola.getX(),bola.getY()+.1,bola.getZ(), bola.getX(),bola.getY(), bola.getZ(), 0, 1, 0);
	printf("(%f, %f, %f)\n", bola.getX(), bola.getY(), bola.getZ());
	//desenhaCilindro();
	bola.desenha();
	desenhaObstaculos();
	glutSwapBuffers();
	glFlush();
}

void funcaoKeyboard(unsigned char key, int x, int y) {
	if(key == 'z') {
		exit(-1);
	}
	if(key == 'a') xbola-=0.1;
	if(key == 'q') xbola+=0.1;
	if(key == 's') ybola-=0.1;
	if(key == 'w') ybola+=0.1;
	if(key == 'd') zbola-=0.1;
	if(key == 'e') zbola+=0.1;
	if(key == 'f') xcam-=0.1;
	if(key == 'r') xcam+=0.1;
	if(key == 'g') ycam-=0.1;
	if(key == 't') ycam+=0.1;
	if(key == 'h') zcam-=0.1;
	if(key == 'y') zcam+=0.1;
	if(key == 'x') rot-=0.1;
	if(key == 'c') rot+=0.1;
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
	glutCreateWindow("Jogo");
	glutKeyboardFunc(funcaoKeyboard);
	glutMouseFunc(funcaoMouse);
	glutMotionFunc(funcaoMouseMotion);
	glutDisplayFunc(funcaoDisplay);
	glutIdleFunc(temporizador);
	inicializacao();

	glutMainLoop();
	
	return 0;
}

