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


double escala = .18;
const double COLLISION_ACCELERATION = .19*escala;
const double GRAVITY_ACCELERATION = 0.01*escala;
const double SPHERE_RADIUS = 0.5*escala;
const double DISTANCE_BALL_TO_CYLINDER_AXIS = 4*escala;
const double OBSTACLE_RADIUS = 2.5*escala;//CYLINDER_RADIUS+DISTANCE_BALL_TO_CYLINDER_AXIS;
const double PI = 3.14159265359;
const double OBSTACLE_HEIGHT = 1*escala;
const double DISTANCE_BETWEEN_OBSTACLES = 4*escala;
const double MAX_SPEED = .3*escala;
const int OBSTACLE_COUNT = 200005;
const int BOUNCE_ANIMATION_FRAMES = 8;
const int SLICES = 100;
const double STRETCH_BY_SPEED_FACTOR = 6;

pair<double, double> obstacles[200005];
GLuint textura_pedra;
class Bola{
private:
	int slices; //quantidade de slices que dividirão a esfera
	double x, y, z;
	double vx, vy, vz;
	int bounceAnimation;
	
	double calcStretch(int frame){
		frame--;
		double x = PI*(frame/(double)(BOUNCE_ANIMATION_FRAMES-1));
		return 0.5*sin(x);
	}
	
	void applyBounceAnimation(){
		//cout << "bounceAnimation = " << bounceAnimation << endl;
		if(bounceAnimation <= BOUNCE_ANIMATION_FRAMES/2) {
			//double stretch = .5*-2*bounceAnimation/(double)BOUNCE_ANIMATION_FRAMES;
			glScalef(1, 1-calcStretch(bounceAnimation), 1);
		}
		else if(bounceAnimation <= BOUNCE_ANIMATION_FRAMES) {
			//double stretch = -.5*(BOUNCE_ANIMATION_FRAMES-bounceAnimation)/(double)BOUNCE_ANIMATION_FRAMES;
			glScalef(1, 1-calcStretch(bounceAnimation), 1);
		}
		bounceAnimation+=1;
		if(bounceAnimation > BOUNCE_ANIMATION_FRAMES) {
			bounceAnimation = 0;
			move();
		}
	}
	
public:
	double getX(){return x;}
	double getY(){return y;}
	double getZ(){return z;}
	double getVy(){return vy;}
	void upd(double a, double b, double c){x=a,y=b,z=c;} //sets position of the ball to (a, b, c)
	Bola(){
		slices = SLICES;
		bounceAnimation = 0;
		x = -2*escala;
		y = 1*escala;
		z = 0;
		vx = 0;
		vy = 0;
		vz = 0;
	}
	void setRot(double _rot){rot = _rot;}
	void move(){ //applies effects of acceleration to ball
		acelera();
		x += vx;
		y += vy;
		z += vz;
	}
	void colide(){
		vy = COLLISION_ACCELERATION;
		if(!bounceAnimation) bounceAnimation = 1;
	}
	void acelera(){ //accelerates ball
		vy -= GRAVITY_ACCELERATION;
		if(vy < -MAX_SPEED) vy = -MAX_SPEED;
	}
	void desenha(){
		if(!bounceAnimation) move();
		upd(DISTANCE_BALL_TO_CYLINDER_AXIS*cos(rot), y, DISTANCE_BALL_TO_CYLINDER_AXIS*sin(rot));

		GLfloat params[] = {1, 0, 0, .2};
	
		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_AMBIENT, params);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, params);
	 	glLightfv(GL_LIGHT0, GL_SPECULAR, params);
	 	glLightfv(GL_LIGHT0, GL_POSITION, params);
		glEnable(GL_LIGHT0);
		
		glPushMatrix ();
			glTranslatef(x, y, z);
			if(!bounceAnimation) glScalef(1, max(1.0, 1+STRETCH_BY_SPEED_FACTOR*(-vy)), 1);
			else applyBounceAnimation();
			glutSolidSphere (SPHERE_RADIUS, slices, slices);
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

double convertRadToDeg(double rad){
	return rad*180.0/PI;
}

void inicializacao() {
	glEnable(GL_DEPTH_TEST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    textura_pedra = getTexture("textura_pedra.bmp", 1024, 1024);
    //glEnable(GL_TEXTURE_2D);
	glClearColor(0.5, 0.8, 1, 0);
}

void setorCircular(double x, double y, double z, double raio, double anguloIniDeg, double anguloFimDeg, int partes=1002){
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textura_pedra);
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2f(.5, .5);
			glVertex3f(x, y, z);
			glColor3f(.3, .3, .3);
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

	glBindTexture(GL_TEXTURE_2D, textura_pedra);
	glEnable(GL_TEXTURE_2D);

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
	
	glBegin(GL_TRIANGLE_FAN);
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


int getObstacle(double ballPosition){
	int ini = 0;
	int fim = OBSTACLE_COUNT;
	while(fim > ini){
		//cout << "("<<ini<<", "<<fim<<")\n";
		int mid = (ini+fim+1)/2;
		double pos = -DISTANCE_BETWEEN_OBSTACLES*mid;
		if(pos <= ballPosition){
			fim = mid-1;
		}
		else{
			//cout << "pos = " << pos << endl;
			//cout << "ballPosition = " << ballPosition << endl;
			ini = mid;
		}
	}
	return ini;
}

double calcPosObstacle(int index){
	return -DISTANCE_BETWEEN_OBSTACLES*index;
}

void desenhaObstaculos(){
	glColor3f(0, 0, 1);
	int ini = getObstacle(bola.getY());
	for(int i = max(0, ini-4); i < ini+5; i++){
		setorCilindrico(0, calcPosObstacle(i), 0, OBSTACLE_HEIGHT, OBSTACLE_RADIUS, obstacles[i].first, obstacles[i].second);
	}
}

bool checkCollision(double posBall, int indexObstacle, double rotDeg){
	double posObstacle = calcPosObstacle(indexObstacle);
	return posBall <= posObstacle && posBall >= posObstacle-OBSTACLE_HEIGHT && (rotDeg <= obstacles[indexObstacle].first || rotDeg >= obstacles[indexObstacle].second);
}

bool colisao(){
	double infBola = bola.getY()+SPHERE_RADIUS*2;
	double rotgraus = (180.0/PI)*rot;
	int indexObstacle = getObstacle(infBola);
	return checkCollision(infBola, indexObstacle, rotgraus) || checkCollision(infBola+bola.getVy(), getObstacle(infBola+bola.getVy()), rotgraus);
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


	gluLookAt(bola.getX()*(SPHERE_RADIUS+.5), bola.getY()+.07, bola.getZ()*(SPHERE_RADIUS+.5), bola.getX(),bola.getY(),bola.getZ(), 0, 1, 0);
	
	desenhaObstaculos();
	bola.desenha();

	glutSwapBuffers();
	glFlush();
}

bool checkRotation(double rotation){
	int indexObstacle = getObstacle(bola.getY());
	double rotDeg = convertRadToDeg(rotation);
	cout << "y = " << bola.getY() << endl;
	cout << "[" << calcPosObstacle(indexObstacle) << ", " << calcPosObstacle(indexObstacle)-OBSTACLE_HEIGHT << "]\n";
	return !((rotDeg > obstacles[indexObstacle].second || rotDeg < obstacles[indexObstacle].first) && bola.getY() < calcPosObstacle(indexObstacle)-OBSTACLE_HEIGHT && bola.getY() > calcPosObstacle(indexObstacle)-2*OBSTACLE_HEIGHT);
}

void funcaoKeyboard(unsigned char key, int x, int y) {
	if(key == 'z') {
		exit(-1);
	}
	if(key == 'x' && checkRotation(rot+.1)) rot+=0.1;
	if(key == 'c' && checkRotation(rot-.1)) rot-=0.1;
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
	//obstacles[0] = make_pair(0, 90);
	for(int i = 0; i < OBSTACLE_COUNT; i++){
		double a = rand()%300;
		double b = a + 60 + rand()%(301-int(a));
		if(a > b) swap(a, b);
		obstacles[i] = make_pair(a, b);
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

