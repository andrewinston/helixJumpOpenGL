#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <bits/stdc++.h>
double ycam=0;
double xcam=0;
double zcam=0;
double xbola=0,ybola=0,zbola=0;
using namespace std;
double rot = 0;
//y: pra cima
//x: pra dentro da tela
//z: pra direita


double escala = .2;
const double ACEL_COLISAO = .15*escala;
const double ACEL_GRAVIDADE = 0.01*escala;
const double RAIO_ESFERA = 0.5*escala;
const int SLICES = 100; //quantidade de slices que dividirão a esfera
const double RAIO_CILINDRO = 1*escala;
const double DISTANCIA_BOLA_CILINDRO = 4*escala;
const double RAIO_OBSTACULO = 2.5*escala;//RAIO_CILINDRO+DISTANCIA_BOLA_CILINDRO;
const double PI = 3.14159265359;
const double ALTURA_OBSTACULO = 1*escala;
const double DISTANCIA_ENTRE_OBSTACULOS = 4*escala;
const double VELOCIDADE_MAXIMA = 1*escala;
const int QUANTIDADE_DE_OBSTACULOS = 200;


pair<double, double> obstaculos[QUANTIDADE_DE_OBSTACULOS];
GLuint textura_pedra;
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
		upd(DISTANCIA_BOLA_CILINDRO*cos(rot), y, DISTANCIA_BOLA_CILINDRO*sin(rot));

		GLfloat params[] = {1, 0, 0, .2};
	
		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_AMBIENT, params);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, params);
	 	glLightfv(GL_LIGHT0, GL_SPECULAR, params);
	 	glLightfv(GL_LIGHT0, GL_POSITION, params);
		glEnable(GL_LIGHT0);
		
		glColor3f(1, 0, 0);
		glPushMatrix ();
			glTranslatef(x, y, z);
			glutSolidSphere (RAIO_ESFERA, SLICES, SLICES);
		glPopMatrix ();
		
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	}

};


GLuint getTexture(const char *filename, int width, int height){
	GLuint texture;
	unsigned char *data;
	FILE *file;
	
	file = fopen(filename, "rb");
	if(file == NULL) return 0;
	data = (unsigned char*) malloc(width * height * 4);
     
     // read texture data
    size_t sz = fread(data, width * height * 4, 1, file);
    fclose(file);
     
    // allocate a texture name
    glGenTextures(1, &texture);
    
    // select our current texture
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // select modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);
    
    // when texture area is small, bilinear filter the closest mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
     // when texture area is large, bilinear filter the first mipmap
  	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     
     // texture should tile
   	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     
     // build our texture mipmaps
   	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
     
     // free buffer
   	free(data);
     
   	return texture;
	
	
}

Bola bola;
double zRotated = 0;

void inicializacao() {
	glEnable(GL_DEPTH_TEST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    textura_pedra = getTexture("textura_pedra.bmp", 1024, 1024);
    //glEnable(GL_TEXTURE_2D);
	glClearColor(0.5, 0.5, 0.5, 0);
}

void setorCircular(double x, double y, double z, double raio, double anguloIniDeg, double anguloFimDeg, int partes=1002){
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, textura_pedra);
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2f(.5, .5);
			glVertex3f(x, y, z);
			glColor3f(0, 0, 1);
			double anguloIni = (PI/180.0)*anguloIniDeg;
			double anguloFim = (PI/180.0)*anguloFimDeg;
			double inc = (2*PI)/partes;
			if(anguloIni > anguloFim) swap(anguloIni, anguloFim);
			for(double angle = 0; angle <= anguloIni; angle += inc){
				glTexCoord2f(.5+cos(angle)*.2, .5+sin(angle)*.2);
				glVertex3f(x-cos(angle)*raio, y, z-sin(angle)*raio);
			}
		glEnd();
		glBegin(GL_TRIANGLE_FAN);
			glVertex3f(x, y, z);
			for(double angle = anguloFim; angle <= 2*PI; angle += inc){
				glTexCoord2f(.5+cos(angle)*.2, .5+sin(angle)*.2);
				glVertex3f(x-cos(angle)*raio, y, z-sin(angle)*raio);
			}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
}

void setorCilindrico(double x, double y, double z, double h, double raio, double anguloIniDeg, double anguloFimDeg, int partes=1002){
	double anguloIni = (PI/180.0)*anguloIniDeg;
	double anguloFim = (PI/180.0)*anguloFimDeg;
	double inc = (2*PI)/partes;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textura_pedra);

	glColor3f(1, 0, 1);
	setorCircular(x, y-h, z, raio, anguloIniDeg, anguloFimDeg, partes);
	
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0.1, 0.5);
		glVertex3f(x, y, z);
		glTexCoord2f(0.1, 0.2);
		glVertex3f(x, y-h, z);
		glTexCoord2f(.85, 0.2);
		glVertex3f(x-cos(anguloIni)*raio, y-h, z-sin(anguloIni)*raio);
		glTexCoord2f(.85, .5);
		glVertex3f(x-cos(anguloIni)*raio, y, z-sin(anguloIni)*raio);
	glEnd();
	
	glBegin(GL_TRIANGLES);
		glTexCoord2f(0.1, 0.5);
		glVertex3f(x, y, z);
		glTexCoord2f(0.1, 0.2);
		glVertex3f(x, y-h, z);
		glTexCoord2f(.85, 0.2);
		glVertex3f(x-cos(anguloFim)*raio, y-h, z-sin(anguloFim)*raio);
		glTexCoord2f(.85, .5);
		glVertex3f(x-cos(anguloFim)*raio, y, z-sin(anguloFim)*raio);
	glEnd();
	glDisable(GL_TEXTURE_2D);	
	
	glBegin(GL_TRIANGLE_STRIP);
	for(double angle = 0; angle <= anguloIni; angle += inc){

		glVertex3f(x-cos(angle)*raio, y, z-sin(angle)*raio);
		glVertex3f(x-cos(angle)*raio, y-h, z-sin(angle)*raio);			

	}
	glEnd();
	//glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLE_STRIP);
	for(double angle = anguloFim; angle <= 2*PI-inc; angle += inc){
		glVertex3f(x-cos(angle)*raio, y, z-sin(angle)*raio);
		glVertex3f(x-cos(angle)*raio, y-h, z-sin(angle)*raio);
	
	}
	glEnd();
	
/*	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHTING);*/
	
	setorCircular(x, y, z, raio, anguloIniDeg, anguloFimDeg, partes);	
	

}

//desenha circulo centrado em (x, y, z) com raio, paralelo ao plano XZ
void circulo(double x, double y, double z, double raio, int partes=1002){
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
		if(bola.getY()+RAIO_ESFERA < -DISTANCIA_ENTRE_OBSTACULOS*i-1-ALTURA_OBSTACULO) continue;
		if(bola.getY()+escala*RAIO_ESFERA <= -DISTANCIA_ENTRE_OBSTACULOS*i-1 && (rotgraus <= obstaculos[i].first || rotgraus >= obstaculos[i].second)) {
			return 1;
		}
	}
	return 0;
}

void funcaoDisplay() {
	glCullFace(GL_FRONT);
	if(colisao()){
		bola.colide();
	}
	glLoadIdentity();
	glOrtho(-100, 100, -100, 100, 0, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
	GLfloat params[] = {1, 0, 0, .1};

	glColor3f (1, 0, 0);
	bola.move();

	gluLookAt(bola.getX()*(RAIO_ESFERA+.4), bola.getY()+.1, bola.getZ()*(RAIO_ESFERA+.4), bola.getX(),bola.getY(),bola.getZ(), 0, 1, 0);
	
	desenhaObstaculos();
	bola.desenha();

	glutSwapBuffers();
	glFlush();
}

void funcaoKeyboard(unsigned char key, int x, int y) {
	if(key == 'z') {
		exit(-1);
	}
	if(key == 'x') rot+=0.1;
	if(key == 'c') rot-=0.1;
	if(rot > 2*PI) rot -= 2*PI;
	if(rot < 0) rot += 2*PI;
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
	//obstaculos[0] = make_pair(0, 90);
	for(int i = 0; i < QUANTIDADE_DE_OBSTACULOS; i++){
		double a = rand()%360;
		double b = a + 60 + rand()%(300-int(a));
		if(a > b) swap(a, b);
		obstaculos[i] = make_pair(a, b);
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

