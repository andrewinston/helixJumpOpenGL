#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <bits/stdc++.h>
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

pair<double, double> obstaculos[50];

double escala = .2;
const double ACEL_COLISAO = .15*escala;
const double ACEL_GRAVIDADE = 0.01*escala;
const double RAIO_ESFERA = 0.5*escala;
const int SLICES = 100; //quantidade de slices que dividirão a esfera
const double RAIO_CILINDRO = 1*escala;
const double DISTANCIA_BOLA_CILINDRO = 3*escala;
const double RAIO_OBSTACULO = 2*escala;//RAIO_CILINDRO+DISTANCIA_BOLA_CILINDRO;
const double PI = 3.14159265359;
const double ALTURA_OBSTACULO = 1*escala;
const double DISTANCIA_ENTRE_OBSTACULOS = 8*escala;
const double VELOCIDADE_MAXIMA = .2*escala;
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
	void setRot(double _rot){rot = _rot;}
	void move(){
		acelera();
		x += vx;
		y += vy;
		z += vz;
	}
	void colide(){
		vy = ACEL_COLISAO;
	}
	void acelera(){
		vy -= ACEL_GRAVIDADE;
		if(vy < -VELOCIDADE_MAXIMA) vy = -VELOCIDADE_MAXIMA;
	}
	void desenha(){
		/*glColor3f(1,1,1);
		glPushMatrix();
			//glTranslatef(x, y, z);
			glutSolidSphere(RAIO_ESFERA, SLICES, SLICES);
		glPopMatrix();*/
		upd(DISTANCIA_BOLA_CILINDRO*cos(rot), y, DISTANCIA_BOLA_CILINDRO*sin(rot));
		glColor3f(1, 0, 0);
		glPushMatrix ();
			glTranslatef(x, y, z);
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

void setorCircular(double x, double y, double z, double raio, double anguloIniDeg, double anguloFimDeg, int partes=100){
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x, y, z);
	glColor3f(0, 0, 1);
	double anguloIni = (PI/180.0)*anguloIniDeg;
	double anguloFim = (PI/180.0)*anguloFimDeg;
	double inc = (2*PI)/partes;
	if(anguloIni > anguloFim) swap(anguloIni, anguloFim);
	for(double angle = 0; angle <= anguloIni; angle += inc){
		glVertex3f(x+sin(angle)*raio, y, z+cos(angle)*raio);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x, y, z);
	for(double angle = anguloFim; angle <= 2*PI; angle += inc){
		glVertex3f(x+sin(angle)*raio, y, z+cos(angle)*raio);
	}
	glEnd();
}

void setorCilindrico(double x, double y, double z, double h, double raio, double anguloIniDeg, double anguloFimDeg, int partes=100){
	double anguloIni = (PI/180.0)*anguloIniDeg;
	double anguloFim = (PI/180.0)*anguloFimDeg;
	double inc = (2*PI)/partes;

	setorCircular(x, y, z, raio, anguloIniDeg, anguloFimDeg, partes);
	setorCircular(x, y-h, z, raio, anguloIniDeg, anguloFimDeg, partes);
	glColor3f(0, 0, 1);
	glBegin(GL_TRIANGLE_STRIP);
	for(double angle = 0; angle <= anguloIni; angle += inc){

		glVertex3f(x+sin(angle)*raio, y, z+cos(angle)*raio);
		glVertex3f(x+sin(angle)*raio, y-h, z+cos(angle)*raio);			
		/*glBegin(GL_TRIANGLES);
			glVertex3f(x+sin(angle)*raio, y, z+cos(angle)*raio);
			glVertex3f(x+sin(angle+inc)*raio, y-h, z+cos(angle+inc)*raio);
			glVertex3f(x+sin(angle+2*inc)*raio, y, z+cos(angle+inc)*raio);
			glVertex3f(x+sin(angle)*raio, y-h, z+cos(angle)*raio);
			glVertex3f(x+sin(angle+inc)*raio, y, z+cos(angle+inc)*raio);
			glVertex3f(x+sin(angle+2*inc)*raio, y-h, z+cos(angle+2*inc)*raio);
			
		glEnd();*/
	}
	glEnd();
	//glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLE_STRIP);
	for(double angle = anguloFim; angle < 2*PI-inc; angle += inc){
		glVertex3f(x+sin(angle)*raio, y, z+cos(angle)*raio);
		glVertex3f(x+sin(angle)*raio, y-h, z+cos(angle)*raio);
		
		/*glBegin(GL_TRIANGLES);
			glVertex3f(x+sin(angle)*raio, y, z+cos(angle)*raio);
			glVertex3f(x+sin(angle+inc)*raio, y-h, z+cos(angle+inc)*raio);
			glVertex3f(x+sin(angle+2*inc)*raio, y, z+cos(angle+inc)*raio);
			glVertex3f(x+sin(angle)*raio, y-h, z+cos(angle)*raio);
			glVertex3f(x+sin(angle+inc)*raio, y, z+cos(angle+inc)*raio);
			glVertex3f(x+sin(angle+2*inc)*raio, y-h, z+cos(angle+2*inc)*raio);
		glEnd();*/
	}
	glEnd();
	
	//glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLES);
		glVertex3f(x, y, z);
		glVertex3f(x, y-h, z);
		glVertex3f(x+sin(anguloIni)*raio, y-h, z+cos(anguloIni)*raio);
		glVertex3f(x, y, z);
		glVertex3f(x+sin(anguloIni)*raio, y, z+cos(anguloIni)*raio);
		glVertex3f(x+sin(anguloIni)*raio, y-h, z+cos(anguloIni)*raio);
	glEnd();
	
	//glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
		glVertex3f(x, y, z);
		glVertex3f(x, y-h, z);
		glVertex3f(x+sin(anguloFim)*raio, y-h, z+cos(anguloFim)*raio);
		glVertex3f(x, y, z);
		glVertex3f(x+sin(anguloFim)*raio, y, z+cos(anguloFim)*raio);
		glVertex3f(x+sin(anguloFim)*raio, y-h, z+cos(anguloFim)*raio);
	glEnd();

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
	circulo(x, y-h, z, raio);
	glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), raio, raio, h, SLICES, SLICES);
	glPopMatrix();
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
	for(int i = 0; i < 50; i++){
		setorCilindrico(0, -DISTANCIA_ENTRE_OBSTACULOS*i-1, 0, ALTURA_OBSTACULO, RAIO_OBSTACULO, obstaculos[i].first, obstaculos[i].second);
	}
}

bool colisao(){
	bool colidiu = 0;
	double rotgraus = (180.0/PI)*rot;
	for(int i = 0; i < 50; i++){
		/*if(bola.getY() <= -DISTANCIA_ENTRE_OBSTACULOS*i-1+.1*escala && bola.getY() >= -DISTANCIA_ENTRE_OBSTACULOS*i-1){
			return 1;
		}*/
		if(bola.getY()+RAIO_ESFERA < -DISTANCIA_ENTRE_OBSTACULOS*i-1-ALTURA_OBSTACULO) continue;
		if(bola.getY()+RAIO_ESFERA <= -DISTANCIA_ENTRE_OBSTACULOS*i-1 && (rotgraus <= obstaculos[i].first || rotgraus >= obstaculos[i].second)) return 1;
	}
	return 0;
}

void funcaoDisplay() {
	//bola.upd(xbola+DISTANCIA_BOLA_CILINDRO*cos(rot), ybola, zbola+DISTANCIA_BOLA_CILINDRO*sin(rot));
	//bola.setRot(rot);
	bola.move();
	if(colisao()){
		bola.colide();
	}
	//glFrustum(-300, 300, -300, 300, 0, 2000);
	//gluPerspective(60, 1, 0, 300);
	//glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100, 100, -100, 100, 0, 100);
//	gluOrtho2D(-8, 8, -8, 8);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f (0.8, 0.2, 0.1);              // Red ball displaced to left.
	gluLookAt(bola.getX()*(RAIO_ESFERA+.4), bola.getY()+.1, bola.getZ()*(RAIO_ESFERA+.4), bola.getX(),bola.getY(),bola.getZ(), 0, 1, 0);
	//gluLookAt(bola.getX(),bola.getY()+.1,bola.getZ(), bola.getX(),bola.getY(), bola.getZ(), 0, 1, 0);
	printf("(%f, %f, %f)\n", bola.getX(), bola.getY(), bola.getZ());
	desenhaObstaculos();
	//desenhaCilindro();
	bola.desenha();
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
	if(key == 'x') rot+=0.1;
	if(key == 'c') rot-=0.1;
	if(rot >= 2*PI) rot = 0;
	if(key == ' ') {
		bola.colide();
	}
	bola.setRot(rot);
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
	obstaculos[0] = make_pair(90, 180);
	for(int i = 1; i < 50; i++){
		double a = rand()/double(RAND_MAX);
		double b = rand()/double(RAND_MAX);
		if(a > b) swap(a, b);
		obstaculos[i] = make_pair(a*360, max(a+45, b*360));
	}
	
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

