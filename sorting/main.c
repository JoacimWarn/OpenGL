//gcc main.c -std=c99 -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <time.h>
#include <math.h>

#define HEIGHT 1024
#define WIDTH  1280
#define BUFFER 10

struct point{
	
	float x;
	float y;
	
};

struct point points[BUFFER];
float inc = 10.0f;
//
//
void drawCoordinateSystem(){
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	
	//Draw x line
	glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
	glVertex3f(-inc, 0.0f, 0.0f);
	glVertex3f( inc, 0.0f, 0.0f);
	
	//Draw y line
	glColor4f(0.0f, 0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -inc, 0.0f);
	glVertex3f(0.0f,  inc, 0.0f);
	
	/*
	//Draw z line
	glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
	glVertex3f(-inc, -inc, 0.0f);
	glVertex3f( inc,  inc, 0.0f);
	*/
	
	glEnd();
	glPopMatrix();
	
}
//
//
void drawData(){
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
	
	for(int i = 0; i < BUFFER; i++){
		
		glVertex3f(i * 1.0f + 1.0f, 0.0f, 0.0f);
		glVertex3f(points[i].x, points[i].y, 0.0f);
		
	}
	
	glEnd();
	glPopMatrix();
	
}
//
//
void drawSinusCurve(){
	
	static float inc = 0.01f;
	static float amplitude = 5.0f;
	static float phaseShift = 0.001f;
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glColor4f(1.0f, 0.0f, 1.0f, 0.0f);
	
	float z = 0.0f;
	
	for(float i = -10.0f; i < BUFFER;){
		
		float y = sin(i + phaseShift) * amplitude;
		glVertex3f(i * 1.0f, y, 0.0f);
		i = i + inc;
		if(i >= 2.0f && i <= 2.0f) z = y;
		
	}
	
	for(int i = 0; i < 300; i++){
		
		float angle = 2.0f * 3.14f * (i/300.0f);
		float x = cos(angle)/2.0f;
		float y = sin(angle)/2.0f;
		glVertex3f(x + 4.0f, y + z, 0.0f);
		
	}
	
	//amplitude = amplitude + inc;				//här ändrar vi
	
	//phaseShift = phaseShift - 0.05f;
	//inc = inc + 0.001f;						//här ändrar
	if(inc>0.1f) inc = 0.01f;
	
	glEnd();
	glPopMatrix();
	
}
//
//
int main(){
	
	time_t t;
	srand((unsigned) time(&t));
	
	for(int i = 0; i < BUFFER; i++){
		
		int random = (rand() % 21) - 10;
		
		points[i].x = i * 1.0f + 1.0f;
		points[i].y = random * 1.0f;
		
	}
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT,"sorting",NULL,NULL);
	glfwMakeContextCurrent(window);
	
	float z = 8.0f;
	{
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120, WIDTH/HEIGHT, 0.1f, 32.0f);
		gluLookAt(
		0,	0,	z,
		0,	0,	0,
		0,	1,	0
		);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
	}
	
	glScalef(1.0f, 1.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.01f, 0.0f);
	
	glfwSetTime(0.0f);
	
	while(!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		//Rendering
		//glRotatef(90.0f * glfwGetTime(), 0, 0, 1);
		drawCoordinateSystem();
		//drawData();
		drawSinusCurve();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}

