#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <MMSystem.h>
#include <GL/glut.h>
#include <string>
#include <vector>
#include <iostream>
#include "Untitled8.h"
#include "material.h"
#define BLUE    0.529, 0.8, 0.922, 1.0
#define RED		 1.0, 0.0, 0.0, 1.0
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define PINK     1.0, 0.0, 1.0, 0.0
#define BROWN    0.5, 0.35, 0.05, 1.0
#define PI		 3.14159
#define GRAY1    0.2, 0.2, 0.2, 1.0
#define GRAY2    0.93, 0.93, 0.93, 1.0
#define MAX_PARTICLES 1000
#define RAIN	0

typedef struct {

	bool vivo;	
	float vida;	
	float morte; 
	float xpos; 
	float ypos; 
	float zpos;
	float vel;
}sistema;

// Paticle System
sistema particulas[MAX_PARTICLES]; 
float velocidade = 0.0;


// Visualização

GLint		wScreen=800, hScreen=600;
GLfloat		xC=10.0, yC=10.0, zC=10.0;		
GLfloat  rVisao=20, aVisao=0.5*PI, incVisao=0.05;
GLfloat  obsP[] ={rVisao*cos(aVisao), 4.0, rVisao*sin(aVisao)};
GLfloat  incZoom=3;

// outras variáveis

GLfloat ytam= 2;
GLuint  texture[100];
GLuint  tex;
RgbImage imag;
float xtranslated=0;
float ytranslated=0;
GLint   kingkong=0;
GLint   princess=0;
float marioX = 0.0f;
float marioY = 0.0f;
GLint ligaluz=1;

// Luzes

GLfloat luzGlobalCor[4] = { 1,1,1,1 };

GLfloat AmbLightPos[4] = {-20,3.5,0,1 };
GLfloat AmbLightPos1[4] = {0,50,0,0 };
GLfloat AmbLightPos2[4] = {0,40,0,1 };
GLfloat matAmbiente1[] = {1.0,1.0,1.0,1.0};
GLfloat matDifusa1[]   = {1.0,1.0,1.0,1.0};
GLfloat matEspecular1[]= {0.0, 0.0, 0.0, 0.0};



GLfloat down[] = {0,-1,0,1};


 //Sistema de particulas
 
void initParticles(int i) {
    particulas[i].vivo = true;
    particulas[i].vida = 1.0;
    particulas[i].morte = float(rand()%100)/1000.0f+0.003f;
    particulas[i].xpos = (float) (rand() % 100) -50 ;
    particulas[i].ypos = 40.0;
    particulas[i].zpos = (float) (rand() %100) -50 ;
    particulas[i].vel = velocidade;


}

void drawRain() {
	float x, y, z;
	for (int loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
		if (particulas[loop].vivo == true) {			
			x = particulas[loop].xpos;
			y = particulas[loop].ypos;
			z = particulas[loop].zpos;
			
			glColor3f(0.5, 0.5, 1.0);
			glBegin(GL_LINES);
				glVertex3f(x, y, z);
				glVertex3f(x, y+0.5, z);
			glEnd();
	
			particulas[loop].ypos += particulas[loop].vel / (2000);
			particulas[loop].vel += -1;
		
			particulas[loop].vida -= particulas[loop].morte;
			
			if (particulas[loop].ypos <= -50) {
				particulas[loop].vida = -1.0;
			}
	 
			if (particulas[loop].vida < 0.0) {
				initParticles(loop);
			}
		}
	}
}

void initLights(void){
	//…………………………………………………………………………………………………………………………………………… Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);
    
	glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  matAmbiente1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  matDifusa1);
    glLightfv(GL_LIGHT0, GL_SPECULAR,  matEspecular1);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,  0.005);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,  0.06);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.05);
    glEnable(GL_LIGHT0); 
    
    glLightfv(GL_LIGHT1, GL_AMBIENT,  matAmbiente1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  matDifusa1);
    glLightfv(GL_LIGHT1, GL_SPECULAR,  matEspecular1);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION,  0.005);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION,  0.06);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.05);
    glEnable(GL_LIGHT1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);       
	
	glLightfv(GL_LIGHT2, GL_AMBIENT,  matAmbiente1);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  matDifusa1);
    glLightfv(GL_LIGHT2, GL_SPECULAR,  matEspecular1);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION,  0.005);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION,  0.06);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.05);
    glEnable(GL_LIGHT2);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);       
	
}
void criaDefineTexturas()
{   

	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("madeira.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("parede.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	//----------------------------------------- Chao y=0
	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("skinkong.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
		
	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("StormClouds.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	
	glGenTextures(1, &texture[4]);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("lata.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
		
	glGenTextures(1, &texture[5]);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("white.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	
	glGenTextures(1, &texture[6]);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("kingkong.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
		
  	
  	glGenTextures(1, &texture[7]);
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("cima.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());	
	
	glGenTextures(1, &texture[8]);
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("baixo.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());	
		
	glGenTextures(1, &texture[9]);
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("frente.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());		
			
		glGenTextures(1, &texture[10]);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("back.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());	
		
				
		glGenTextures(1, &texture[11]);
	glBindTexture(GL_TEXTURE_2D, texture[11]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("esquerda.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
		
			glGenTextures(1, &texture[12]);
	glBindTexture(GL_TEXTURE_2D, texture[12]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("direita.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());			
	
		glGenTextures(1, &texture[13]);
	glBindTexture(GL_TEXTURE_2D, texture[13]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("princess.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
		
			glGenTextures(1, &texture[14]);
	glBindTexture(GL_TEXTURE_2D, texture[14]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("mariobro.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());					
	
}


void inicializa(void)
{
	 glClearColor(0,0,0,1);
    glShadeModel(GL_SMOOTH);
    //Texturas
    initLights();
    criaDefineTexturas();
	 glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
	for (int loop = 0; loop < MAX_PARTICLES; loop++) {
        initParticles(loop);
    }


	PlaySound("Donkeykong.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);		

	
}


void frontface(float x, float y,float z, int tx){
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[tx]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBegin(GL_QUADS); 
			glBindTexture(GL_TEXTURE_2D,texture[tx]);
		     glTexCoord2f(0.0f,0.0f);glVertex3f( (x),  (y), (z));
	         glTexCoord2f(1.0f,0.0f);glVertex3f(-(x),  (y), (z));
	         glTexCoord2f(1.0f,1.0f);glVertex3f(-(x), -(y), (z));
	         glTexCoord2f(0.0f,1.0f);glVertex3f( (x), -(y), (z));
	    glEnd();
	   glDisable(GL_TEXTURE_2D); 
	   glDisable(GL_COLOR_MATERIAL);
}
		
void drawRectangle(float x, float y,float z, int tx){
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[tx]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBegin(GL_QUADS);               
	      // Top face (y = 1.0f)
	      
		  glTexCoord2f(0.0f,0.0f);glVertex3f( (x), (y), -(z));
	      glTexCoord2f(1.0f,0.0f);glVertex3f(-(x), (y), -(z));
	      glTexCoord2f(1.0f,1.0f);glVertex3f(-(x), (y),  (z));
	      glTexCoord2f(0.0f,1.0f);glVertex3f( (x), (y),  (z));
	 
	      // Bottom face (y = -1.0f)   
	       glTexCoord2f(0.0f,0.0f);glVertex3f( (x), -(y),  (z));
	       glTexCoord2f(1.0f,0.0f);glVertex3f(-(x), -(y),  (z));
	       glTexCoord2f(1.0f,1.0f);glVertex3f(-(x), -(y), -(z));
	       glTexCoord2f(0.0f,1.0f);glVertex3f( (x), -(y), -(z));
	 
	      // Front face  (z = 1.0f) 
		  if(princess ){
			 
	      }else if(kingkong){
	      	 	
	      }
	      else{
		     glBindTexture(GL_TEXTURE_2D,texture[tx]);
		     glTexCoord2f(0.0f,0.0f);glVertex3f( (x),  (y), (z));
	         glTexCoord2f(1.0f,0.0f);glVertex3f(-(x),  (y), (z));
	         glTexCoord2f(1.0f,1.0f);glVertex3f(-(x), -(y), (z));
	         glTexCoord2f(0.0f,1.0f);glVertex3f( (x), -(y), (z));
		 }
	      // Back face (z = -1.0f)
  
	       glTexCoord2f(0.0f,0.0f);glVertex3f((x), -(y), -(z));
	       glTexCoord2f(1.0f,0.0f);glVertex3f(-(x), -(y), -(z));
	       glTexCoord2f(1.0f,1.0f);glVertex3f(-(x),  (y), -(z));
	       glTexCoord2f(0.0f,1.0f);glVertex3f( (x),  (y), -(z));
	 
	      // Left face (x = -1.0f)
	       glTexCoord2f(0.0f,0.0f);glVertex3f(-(x),  (y),  (z));
	       glTexCoord2f(1.0f,0.0f);glVertex3f(-(x),  (y), -(z));
	       glTexCoord2f(1.0f,1.0f);glVertex3f(-(x), -(y), -(z));
	       glTexCoord2f(0.0f,1.0f);glVertex3f(-(x), -(y),  (z));
	 
	      // Right face (x = 1.0f)
	       glTexCoord2f(0.0f,0.0f);glVertex3f((x),  (y), -(z));
	       glTexCoord2f(1.0f,0.0f);glVertex3f((x),  (y),  (z));
	       glTexCoord2f(1.0f,1.0f);glVertex3f((x), -(y),  (z));
	       glTexCoord2f(0.0f,1.0f);glVertex3f((x), -(y), -(z));
	   glEnd();
	   glDisable(GL_COLOR_MATERIAL);
	   glDisable(GL_TEXTURE_2D);  		

}

void drawStairs(float y){
	glPushMatrix();
		drawRectangle(0.2f, y ,0.5f, 0);	
		glTranslatef(4.0f,0.0f,0.0f);
		drawRectangle(0.2f, y ,0.5f, 0);
		glTranslatef(-2.0f,-(y),0.0f);
		for(int i = 0; i < (y)*	2; i++){
			glTranslatef(0.0f,1.0f,0.0f);
			drawRectangle(2.0f,0.1f,0.5f, 0);	
		}
	glPopMatrix();


}



void drawLata(void){
	glTranslatef(xtranslated,ytranslated,0);
	GLUquadricObj*  y = gluNewQuadric ( );
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[4]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK,  GL_AMBIENT, redPlasticAmb);
	   	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlasticDif);
    	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlasticSpec);
    	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, redPlasticCoef);
		glTranslatef(0,-2,0);
		glRotatef(270,1,0,0);
		gluQuadricDrawStyle ( y, GLU_FILL   );
		gluQuadricNormals   ( y, GLU_SMOOTH );
		gluQuadricTexture   ( y, GL_TRUE    );
		gluCylinder(y,1.0f,1.0f,2.0f,100,100);
		
		
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);	
}

void mario(void){
	glTranslatef(marioX,marioY,0);
	AmbLightPos[0] =-20 + marioX; 
	AmbLightPos[1] = 3.5 + marioY;
	glRotatef(180,0,0,1);
	frontface(2,2,2,14);
	
}


void keyboard_s (unsigned char key, int x, int y)
{
   switch (key)
   {

	case 'p':
   		if(luzGlobalCor[0]<1){
   			luzGlobalCor[0] = luzGlobalCor[0] + 0.1;
   		}
   		if(luzGlobalCor[1]<1){
   			luzGlobalCor[1] = luzGlobalCor[1] + 0.1;
   		}
   		if(luzGlobalCor[2]<1){
   			luzGlobalCor[2] = luzGlobalCor[2] + 0.1;
   		}

   		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);
		glutPostRedisplay();
    	break;

	case 'o':
   		if(luzGlobalCor[0]>0){
   			luzGlobalCor[0] = luzGlobalCor[0] - 0.05;
   		}
   		if(luzGlobalCor[1]>0){
   			luzGlobalCor[1] = luzGlobalCor[1] - 0.05;
   		}
   		if(luzGlobalCor[2]>0){
   			luzGlobalCor[2] = luzGlobalCor[2] - 0.05;
   		}

   		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,  luzGlobalCor);
		glutPostRedisplay();
    	break;
	 
      case 'a' : marioX -= 1.00f;
      glutPostRedisplay();
      break;
      case 'd' : marioX  += 1.00f;
      glutPostRedisplay();
      break;
      case 'w' : marioY += 1.00f;
      glutPostRedisplay();
      break;
      case 's' : marioY -= 1.00f;
      glutPostRedisplay();
      break;
      	
    	
		//--------------------------- Iluminacaoda sala
   	case 't':
	case 'T':
		ligaluz=!ligaluz;
		glutPostRedisplay();	
	break;
    case 27:
	exit(0);
	break;		
   }
}


void drawSky(int x, int y, int z){
   glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
   
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[7]);
    glBegin(GL_QUADS);
    //top
     glTexCoord2f(0.0f,0.0f);glVertex3f( (x), (y), -(z));
	 glTexCoord2f(1.0f,0.0f);glVertex3f(-(x), (y), -(z));
	 glTexCoord2f(1.0f,1.0f);glVertex3f(-(x), (y),  (z));
	 glTexCoord2f(0.0f,1.0f);glVertex3f( (x), (y),  (z));
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_COLOR_MATERIAL);
    //bot
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[8]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f);glVertex3f( (x), -(y),  (z));
     	glTexCoord2f(1.0f,0.0f);glVertex3f(-(x), -(y),  (z));
		glTexCoord2f(1.0f,1.0f);glVertex3f(-(x), -(y), -(z));
        glTexCoord2f(0.0f,1.0f);glVertex3f( (x), -(y), -(z));
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_COLOR_MATERIAL);
    //front
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[9]);
    glBegin(GL_QUADS);
    
        glTexCoord2f(0.0f,0.0f);glVertex3f( (x),  (y), (z));
	    glTexCoord2f(1.0f,0.0f);glVertex3f(-(x),  (y), (z));
        glTexCoord2f(1.0f,1.0f);glVertex3f(-(x), -(y), (z));
	    glTexCoord2f(0.0f,1.0f);glVertex3f( (x), -(y), (z));
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_COLOR_MATERIAL);
   //back 
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[10]);
    glBegin(GL_QUADS);
	       glTexCoord2f(0.0f,0.0f);glVertex3f((x), -(y), -(z));
	       glTexCoord2f(1.0f,0.0f);glVertex3f(-(x), -(y), -(z));
	       glTexCoord2f(1.0f,1.0f);glVertex3f(-(x),  (y), -(z));
	       glTexCoord2f(0.0f,1.0f);glVertex3f( (x),  (y), -(z));
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_COLOR_MATERIAL);
    //left
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[11]);
    glBegin(GL_QUADS);
	       glTexCoord2f(0.0f,0.0f);glVertex3f(-(x),  (y),  (z));
	       glTexCoord2f(1.0f,0.0f);glVertex3f(-(x),  (y), -(z));
	       glTexCoord2f(1.0f,1.0f);glVertex3f(-(x), -(y), -(z));
	       glTexCoord2f(0.0f,1.0f);glVertex3f(-(x), -(y),  (z));
    glEnd();
    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_TEXTURE_2D);
    //right
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[12]);
    glBegin(GL_QUADS);
	     glTexCoord2f(0.0f,0.0f);glVertex3f((x),  (y), -(z));
	     glTexCoord2f(1.0f,0.0f);glVertex3f((x),  (y),  (z));
	     glTexCoord2f(1.0f,1.0f);glVertex3f((x), -(y),  (z));
	     glTexCoord2f(0.0f,1.0f);glVertex3f((x), -(y), -(z));
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
     glDisable(GL_COLOR_MATERIAL);
    	      
}
void drawScene(){
	glLightfv(GL_LIGHT0, GL_POSITION, AmbLightPos);
    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,down);
    
    glLightfv(GL_LIGHT1, GL_POSITION, AmbLightPos1);
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,down);
    
    glLightfv(GL_LIGHT2, GL_POSITION, AmbLightPos2);
    glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,down);

    
	glPushMatrix();
	
		glRotatef(180,0,1,0);
		drawSky(50,50,50);
	glPopMatrix();
	
		
	//floor
	glPushMatrix();
		drawRectangle(30.0,2.0,1.0,1);
		glTranslatef(-15.2f, 8.0f, 0.0f); 
		drawRectangle(15.0,2.0,1.0,1);
		glTranslatef(32.4f, 0.0f, 0.0f); 
		drawRectangle(13.0,2.0,1.0,1);
		glTranslatef(-42.4f, 8.0f, 0.0f);
		drawRectangle(5.0,2.0,1.0,1);
		glTranslatef(32.4f, 0.0f, 0.0f);
		drawRectangle(23.0,2.0,1.0,1);
		glTranslatef(10.0f, 8.0f, 0.0f);
		drawRectangle(8.0,2.0,1.0,1);
		glTranslatef(-29.4f, 0.0f, 0.0f);
		drawRectangle(17.0,2.0,1.0,1);
		glTranslatef(7.0f,12.0f, 0.0f);
		drawRectangle(20.0,2.0,1.0,1);
	glPopMatrix();
	
	//stairs
	glPushMatrix();
		glTranslatef(0.0f,6.0f,-0.5f); // onde queres desenhar as escadas
		drawStairs(4);
		glTranslatef(-20.0f,8.0f,0.0f); // onde queres desenhar as escadas
		drawStairs(4);
		glTranslatef(25.0f,8.0f,0.0f); // onde queres desenhar as escadas
		drawStairs(4);
		glTranslatef(10.0f,10.0f,0.0f);
		drawStairs(6);
	glPopMatrix();
	
	//Latas
	

	glPushMatrix();
		glTranslatef(-23.0f,28.0f,0.0f);
		drawLata();
		glTranslatef(6.0f,0.0f,0.0f);
		drawLata();
		glTranslatef(-3.0f,0.0f,0.0f);
		drawLata();
		
	glPopMatrix();
	
	//Mario
	
	
	glPushMatrix();
		glTranslatef(-20.0f,4.0f,-2.0f);	
		mario();
	glPopMatrix();	
	
	//Princess
	glPushMatrix();
		princess=!princess;
		glTranslatef(0.0f,40.0f,0.0f);
		glRotatef(180,0,0,1);
		frontface(2.0f,2.0f,1.0,13);
		princess=0;
	glPopMatrix();
	
	//Kong
	glPushMatrix();
		kingkong=!kingkong;
		glTranslatef(-12.0f,29.0f,-2.0f);
		glRotatef(180,0,0,1);
		frontface(3.0f,3.0f,1.5,6);
		kingkong=0;
		glRotatef(-180,0,0,1);
		glTranslatef(0,-4,0);

	glPopMatrix();
    drawRain();
    glutSwapBuffers();

}


void display(void){
	if (ligaluz)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glViewport (0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(110, (float)wScreen/hScreen, 3, 10*zC);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0],12 + obsP[1], 8 +obsP[2], 0,12,0, 0, 1, 0);
	
	//…………………………………………………………………………………………………………………………………………………………Objectos 
	drawScene();
		
}


void teclasNotAscii(int key, int x, int y){

		if(key == GLUT_KEY_UP) 
			obsP[1] = (obsP[1]+ 0.1) ;
		if(key == GLUT_KEY_DOWN) 
			obsP[1] = (obsP[1]- 0.1) ;
		

		if(key == GLUT_KEY_LEFT) 
			aVisao = (aVisao + 0.05) ;
		if(key == GLUT_KEY_RIGHT) 
			aVisao = (aVisao - 0.05) ;

		obsP[0]=rVisao*cos(aVisao);
		obsP[2]=rVisao*sin(aVisao);
		

	glutPostRedisplay();	

}

void idle ( ) {
  glutPostRedisplay();
}

/*oid update(int value) {
if (xtranslated > 15)
	ytranslated -=0.1;
if( ytranslated <= -3){
	xtranslated -=0.1;
}
else 
	xtranslated+=0.1;
glutPostRedisplay();
glutTimerFunc(25, update, 0);

}*/

void resizeWindow(GLsizei w, GLsizei h)
{
    wScreen=w;
    hScreen=h;
    glutPostRedisplay();
}


int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize (wScreen, hScreen); 
	glutInitWindowPosition (300, 100); 
	glutCreateWindow ("DonkeyKong ");
	inicializa();
	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(keyboard_s); 
	glutIdleFunc(idle);	
//	glutTimerFunc(25, update, 0);
	glutMainLoop();

	return 0;
}



