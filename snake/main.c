//gcc main.c -std=c99 -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <stdio.h>

#define NUMBER_OF_SHAPES 50
#define MAX_BUFFER 1200

#define LOWER_LIMIT -14.0f
#define UPPER_LIMIT  14.0f
#define RIGHT_LIMIT  18.5f
#define  LEFT_LIMIT -18.5f
//
//
struct shape{
	
	float x;
	float y;
	float prevx;
	float prevy;
	float dx;
	float dy;
	
};
struct food{
	
	float x;
	float y;
	
};
//
//
struct food apple;
struct shape shapes[MAX_BUFFER];
short unsigned int collision = 0;
short unsigned int numberEaten = 0;
short unsigned int eaten = 0;
short unsigned int inc = 10;		//hur mycket längre den ska bli varje gång den äter

float velocityX = 0.25f;
float velocityY = 0.25f;
//
//
void readFromKeyboard(GLFWwindow* window){
	
	static unsigned short int keyPressed = 0;
	
	if(glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS && keyPressed != 1) keyPressed = 0;
	else if(glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS && keyPressed != 0) keyPressed = 1;
	else if(glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS && keyPressed != 3) keyPressed = 2;
	else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && keyPressed != 2) keyPressed = 3;
	
	shapes[0].prevx = shapes[0].x;
	shapes[0].prevy = shapes[0].y;
	
	if(keyPressed == 0) shapes[0].y = shapes[0].y + shapes[0].dy;
	else if(keyPressed == 1) shapes[0].y = shapes[0].y - shapes[0].dy;
	else if(keyPressed == 2) shapes[0].x = shapes[0].x - shapes[0].dx;
	else if(keyPressed == 3) shapes[0].x = shapes[0].x + shapes[0].dx;
	
	if( (shapes[0].x <= apple.x + 1.0f && shapes[0].x >= apple.x - 1.0f) && (shapes[0].y >= apple.y - 1.0f && shapes[0].y <= apple.y + 1.0f && !eaten) ){
		
		apple.x = (rand() % 34) - 17;
		apple.y = (rand() % 24) - 12;
		eaten = 1;
		numberEaten++;
		
	}
	
	for(int i = 1; i < NUMBER_OF_SHAPES + inc * numberEaten; i++){
		
		if((shapes[0].x == shapes[i].x && shapes[0].y == shapes[i].y) && glfwGetTime() > 1.0f){
			
			collision = 1;
			break;
			
		}
		
		shapes[i].prevx = shapes[i].x;
		shapes[i].prevy = shapes[i].y;
		
		shapes[i].x = shapes[i - 1].prevx;
		shapes[i].y = shapes[i - 1].prevy;
		
	}
	
	eaten = 0;
	
}
//
//
void collisionCheck(){
	
	if( shapes[0].x <=  LEFT_LIMIT || shapes[0].x >= RIGHT_LIMIT ) collision = 1;
	if( shapes[0].y <= LOWER_LIMIT || shapes[0].y >= UPPER_LIMIT ) collision = 1;
	
}
//
//
void drawShape(){
	
	for(int i = 0; i < (NUMBER_OF_SHAPES + inc * numberEaten); i++){
		
		glPushMatrix();
		glTranslatef(shapes[i].x, shapes[i].y, 0.0f);
		glColor4f(1.0f, 0.0f, 1.0f, 0.0f);
		glBegin(GL_QUADS);
		
		glVertex3f( 0.25f,  0.25f, 0.0f);
		glVertex3f(-0.25f,  0.25f, 0.0f);
		glVertex3f(-0.25f, -0.25f, 0.0f);
		glVertex3f( 0.25f, -0.25f, 0.0f);
		
		glEnd();
		glPopMatrix();
		
	}
	
}
//
//
void drawFood(){
	
	glPushMatrix();
	glTranslatef(apple.x, apple.y, 0.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
	
	glBegin(GL_QUADS);
	
	glVertex3f( 0.5f,  0.5f, 0.0f);
	glVertex3f(-0.5f,  0.5f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f( 0.5f, -0.5f, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}
//
//
void moveShape(){
	
	for(int i = 0; i < NUMBER_OF_SHAPES; i++){
		
		if( shapes[i].x <= LEFT_LIMIT || shapes[i].x >= RIGHT_LIMIT )  shapes[i].dx = -shapes[i].dx;
		if( shapes[i].y <= LOWER_LIMIT || shapes[i].y >= UPPER_LIMIT ) shapes[i].dy = -shapes[i].dy;
		
		shapes[i].x = shapes[i].x - shapes[i].dx;
		shapes[i].y = shapes[i].y - shapes[i].dy;
		
	}
	
}
//
//
int main(){
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1024,768,"Snake",NULL,NULL);
	glfwMakeContextCurrent(window);
	glfwSetTime(0.0f);
	
	float z = 8.0f;
	{
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120, 1024.0f/768.0f, 0.1f, 32.0f);
		gluLookAt(
		0,	0,	z,
		0,	0,	0,
		0,	1,	0
		);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
	}
	
	glScalef(1.0f, 1.0f, 1.0f);
	
	for(int i = 0; i < NUMBER_OF_SHAPES; i++){
		
		shapes[i].x = 0.0f;
		shapes[i].y = 0.0f;
		shapes[i].prevx = shapes[i].x;
		shapes[i].prevy = shapes[i].y;
		shapes[i].dx = velocityX;
		shapes[i].dy = velocityY;
		
	}
	
	apple.x = 2.0f;
	apple.y = -5.0f;
	
	while(!glfwWindowShouldClose(window) && !collision){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		drawShape();
		drawFood();
		readFromKeyboard(window);
		
		//moveShape();
		collisionCheck();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwSetTime(0.0f);
	while(glfwGetTime() < 0.5f);
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}

