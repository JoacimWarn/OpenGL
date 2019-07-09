// gcc main.c -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <stdio.h>

#define CIRCLE 300

#pragma ("glw3.lib")
#pragma ("opengl32.lib")
#pragma ("glu32.lib")
#pragma ("gdi32.lib")

struct food f;

struct ball{
	
	int collision;
	int size;
	float x;
	float y;
	float dx;
	float dy;
	
};
struct food{
	
	int collision;
	float x;
	float y;
	
};
//
//
#define NROFBALLS 3
//
//
#define XROW 15
#define YROW 10
//
//
#define LOWER_LIMIT -24.0f
#define UPPER_LIMIT  24.0f
#define RIGHT_LIMIT  36.0f
#define  LEFT_LIMIT -36.0f
//
//
struct ball balls[NROFBALLS];
//
//
void collision(GLFWwindow* window){
	
	for(int i = 0; i < NROFBALLS; i++){
		
		if( balls[i].x <= LEFT_LIMIT || balls[i].x >= RIGHT_LIMIT ) balls[i].dx = -balls[i].dx;
		if( balls[i].y <= LOWER_LIMIT || balls[i].y >= UPPER_LIMIT )balls[i].dy = -balls[i].dy;
		
		balls[i].x = balls[i].x - balls[i].dx;
		balls[i].y = balls[i].y - balls[i].dy;
		
	}
	
}
//
//
void keyBoard(GLFWwindow* window, float pos[]){
	
	static unsigned short int keyPressed = 0;
	
	//if(pos[1] <= LOWER_LIMIT || pos[1] >= UPPER_LIMIT) b.collision = 1;
	//if(pos[0] <= LEFT_LIMIT  || pos[0] >= RIGHT_LIMIT) b.collision = 1;
	
	if(glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS && keyPressed != 1) keyPressed = 0;
	if(glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS && keyPressed != 0) keyPressed = 1;
	if(glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS && keyPressed != 3) keyPressed = 2;
	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && keyPressed != 2) keyPressed = 3;
	
	if(keyPressed == 0) pos[1] = pos[1] + 0.25f;
	else if(keyPressed == 1) pos[1] = pos[1] - 0.25f;
	else if(keyPressed == 2) pos[0] = pos[0] - 0.25f;
	else if(keyPressed == 3) pos[0] = pos[0] + 0.25f;
	
	/*if(glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS && pos[1] <= UPPER_LIMIT) pos[1] = pos[1] + 0.5f;
	if(glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS && pos[1] >= LOWER_LIMIT) pos[1] = pos[1] - 0.5f;
	if(glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS && pos[0] >= LEFT_LIMIT)  pos[0] = pos[0] - 0.5f;
	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && pos[0] <= RIGHT_LIMIT) pos[0] = pos[0] + 0.5f;*/
	
}
//
//
void background(float downRow[], float upperRow[]){
	
	glTranslatef(downRow[0], downRow[1], 0.0f);
	
	glBegin(GL_QUADS);
	
	glColor4f(1.0f, 0.5f, 0.0f, 0.0f);
	
	int i = 0;
	float inc = -40.0f;
	
	//Draw lower row
	for(; i <= XROW; i++){
		
		//Downrow
		glVertex3f( inc,		-30.0f, 0.0f);
		glVertex3f( inc + 5,	-30.0f, 0.0f);
		glVertex3f( inc + 5,	-25.0f, 0.0f);
		glVertex3f( inc,		-25.0f, 0.0f);
		
		glVertex3f( inc,		+30.0f, 0.0f);
		glVertex3f( inc + 5,	+30.0f, 0.0f);
		glVertex3f( inc + 5,	+25.0f, 0.0f);
		glVertex3f( inc,		+25.0f, 0.0f);
		
		inc = inc + 5.0f;
		
	}
	
	glEnd();
	glPopMatrix();
	
}
//
//
void generateFood(float foodPos[]){
	
	static int first = 1;
	
	//Rendering
	glTranslatef(foodPos[0], foodPos[1], 0.0f);
	glBegin(GL_QUADS);
	glColor4f(0.0f, 0.5f, 0.0f, 0.0f);
	
	glVertex3f(-0.5f,		-0.5f, 0.0f);
	glVertex3f(+0.5f,		-0.5f, 0.0f);
	glVertex3f(+0.5f,		+0.5f, 0.0f);
	glVertex3f(-0.5f,		+0.5f, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}
//
//
void drawBalls(){
	
	//glTranslatef();
	
	//for(int i = 0; i < NROFBALLS; i++) glTranslatef(balls[i].x, balls[i].y, 0.0f);
	
	//glColor4f(1.0f, 0.5f, 0.0f, 0.0f);
	//glBegin(GL_LINE_LOOP);
	
	for(int ballNumber = 0; ballNumber < NROFBALLS; ballNumber++){
		
		glTranslatef(balls[ballNumber].x, balls[ballNumber].y, 0.0f);
		glColor4f(1.0f, 0.5f, 0.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
		
		for(int i = 0; i < CIRCLE; i++){
			
			float angle = 2.0f * 3.14f * (i/300.0f);
			float x = cos(angle) * 2.0f;
			float y = sin(angle) * 2.0f;
			glVertex3f(x, y, 0.0f);
			
		}
		
		glEnd();
		//glPopMatrix();
		
	}
	
	glEnd();
	glPopMatrix();
	
}
//
//
int main(){
	
	glfwInit();
	
	//Create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Snake", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	//Create camera
	float z = 8.0f;
	{
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120, 800.0f/600.0f, 0.1f, 32.0f);
		gluLookAt(
		0,	0,	z,
		0,	0,	0,
		0,	1,	0
		);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
	}
	
	glScalef(0.5f, 0.5f, 0.5f);
	
	float foodPos[2] = {0};
	float downRow[2] = {0};
	float upperRow[2] = {0};
	
	for(int i = 0; i < NROFBALLS; i++){
		
		balls[i].x = 0.25f + i;
		balls[i].y = 0.25f;
		balls[i].collision = 0;
		balls[i].size = 1;
		balls[i].dx = 0.25f;
		balls[i].dy = 0.25f;
		
	}
	
	while(!glfwWindowShouldClose(window)){
		
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, 1);
		
		/*
		if(glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS) pos[1] = pos[1] + 0.2f;
		if(glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS) pos[1] = pos[1] - 0.2f;
		if(glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS) pos[0] = pos[0] - 0.2f;
		if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) pos[0] = pos[0] + 0.2f;
		*/
		
		//Rendering
		//glClear(GL_COLOR_BUFFER_BIT);
		//glPushMatrix();
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		//Drawing the ball
		drawBalls();
		
		//Drawing the background
		background(downRow, upperRow);
		//generateFood(foodPos);
		
		collision(window);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	return 0;
	
}

