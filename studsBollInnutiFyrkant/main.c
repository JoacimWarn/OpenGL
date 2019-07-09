//gcc main.c -std=c99 -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <math.h>

#define HEIGHT 1024
#define WIDTH 1280
#define ASPECT_RATIO WIDTH/HEIGHT
#define CIRCLE 300
#define NUMBER_OF_CIRCLES 1

struct circle{
	
	float x;
	float y;
	float dx;
	float dy;
	
};

struct circle circles[NUMBER_OF_CIRCLES];
//
//
void drawSquare(){
	
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(45.0f * glfwGetTime(), 0, 0, 1);
	
	glColor4f(1.0f, 0.0f, 1.0f, 0.0f);
	
	glBegin(GL_LINE_LOOP);
	
	float squareX = 8.0f;
	float squareY = 8.0f;
	
	glVertex3f( squareX, squareY, 0.0f);
	glVertex3f(-squareX, squareY, 0.0f);
	glVertex3f(-squareX,-squareY, 0.0f);
	glVertex3f( squareX,-squareY, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}
//
//
void drawCircle(){
	
	glPushMatrix();
	
	glTranslatef(circles[0].x, circles[0].y, 0.0f);
	glColor4f(1.0f, 0.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	
	for(int i = 0; i < CIRCLE; i++){
		
		float angle = 2.0f * 3.14f * (i/300.0f);
		float x = cos(angle) * 2.0f;
		float y = sin(angle) * 2.0f;
		glVertex3f(x, y, 0.0f);
		
	}
	
	glEnd();
	glPopMatrix();
	
}
//
//
void moveCircles(){
	
	for(int i = 0; i < NUMBER_OF_CIRCLES; i++){
		
		circles[i].x = circles[i].x - circles[i].dx;
		circles[i].y = circles[i].y - circles[i].dy;
		
	}
	
}
//
//
int main(){
	
	//glRotatef(90.0f * glfwGetTime(), 0, 0, 1);
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bouncing inside square", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	float z = 8.0f;
	{
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120, ASPECT_RATIO, 0.1f, 32.0f);
		gluLookAt(
		0,	0,	z,
		0,	0,	0,
		0,	1,	0
		);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
	}
	
	glScalef(1.0f, 1.0f, 1.0f);
	
	for(int i = 0; i < NUMBER_OF_CIRCLES; i++){
		
		circles[i].x = 1.0f + i;
		circles[i].y = 1.0f + i;
		circles[i].dx = 0.025f;
		circles[i].dy = 0.025f;
		
	}
	
	while(!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		//Draw objects
		drawSquare();
		drawCircle();
		moveCircles();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}