

#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 1024
#define HEIGHT 768
#define BUFFER 6

struct point{
	
	float x;
	float y;
	
};

struct point blocks[BUFFER];
struct point thePlayer;
struct point theBall;
struct point gameArea;
unsigned short int gameInit, gameEnd, leftKeyDown, rightKeyDown;

void key_callback(GLFWwindow*, int, int, int, int);
void drawBall();
void drawPlayer();
void drawBackground();
void moveBall();
void drawBlocks();

void drawBlocks(){
	
	float dx, dy;
	dx = 3.0f;
	dy = 2.0f;
	int i;
	
	for(i = 0; i < BUFFER; i++){
		
		glPushMatrix();
		glBegin(GL_QUADS);
		glTranslatef(blocks[i].x, blocks[i].y, 0.0f);
		glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
		//glColor3ub(rand()%255, rand()%255, rand()%255);
		
		glVertex3f(blocks[i].x, blocks[i].y, 0.0f);
		glVertex3f(blocks[i].x + dx, blocks[i].y, 0.0f);
		
		glVertex3f(blocks[i].x + dx, blocks[i].y + dy, 0.0f);
		glVertex3f(blocks[i].x, blocks[i].y + dy, 0.0f);
		
		glEnd();
		glPopMatrix();
		
	}
	
}

void moveBall(){
	
	static float dx = 0.15f;
	static float dy = -0.2f;
	static unsigned short int collidedX = 0;
	static unsigned short int collidedY = 0;
	float xDist, yDist;
	int i;
	
	if(theBall.y < (gameArea.y + 1.0f)){
		//colliding with ceiling
		
		float playerDistToBall;
		playerDistToBall = abs(thePlayer.x - theBall.x);
		
		if(playerDistToBall < 3.0f) collidedY = 1;
		else						gameEnd = 1;
		
	}
	
	for(i = 0; i < BUFFER; i++){
		//colliding with blocks
		
		float playerDistToBlockY;
		playerDistToBlockY = abs(theBall.y - blocks[i].y);
		
		if(playerDistToBlockY < 1.0f){
			
			float playerDistToBlockX = abs(theBall.x - blocks[i].x)/2.0f;
			
			if(playerDistToBlockX < 1.0f){
				
				blocks[i].x = 100.0f;
				blocks[i].y = 100.0f;
				collidedY = 1;
				break;
				
			}
			
		}
		
	}
	
	xDist = abs(theBall.x - gameArea.x);
	yDist = abs(theBall.y - gameArea.y);
	
	if(xDist < 0.1f || xDist > 18.0f) collidedX = 1;
	if(theBall.y > (gameArea.y + 21.0f)) collidedY = 1;
	
	if(collidedX){
		
		dx = -dx * 1.03f;
		
	}
	if(collidedY){
		
		dy = -dy * 1.003f;
		
	}
	
	theBall.x = theBall.x - dx;
	theBall.y = theBall.y - dy;
	
	collidedX = 0;
	collidedY = 0;
	
}

int main(){
	
	gameInit = 0;
	gameEnd = 0;
	
	glfwInit();
	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "BouncingBall", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);
	
	float z = 8.0f;
	{
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120.0f, WIDTH/HEIGHT, 0.1f, 32.0f);
		gluLookAt(
		0, 0, z,
		0, 0, 0,
		0, 1, 0
		);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
	}
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//Defining drawing play area
	gameArea.x = -10.0f;
	gameArea.y = -10.0f;
	
	thePlayer.x = gameArea.x + 10.0f;
	thePlayer.y = gameArea.y;
	
	theBall.x = thePlayer.x;
	theBall.y = thePlayer.y;
	
	leftKeyDown = 0;
	rightKeyDown = 0;
	
	int i;
	for(i = 0; i < BUFFER; i++){
		
		blocks[i].x = gameArea.x + i * 3.1f;
		blocks[i].y = gameArea.y + 15.0f;
		
	}
	
	while(!glfwWindowShouldClose(window) && !gameEnd){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		//rendering objects
		drawBackground();
		drawPlayer();
		drawBall();
		drawBlocks();
		
		if(gameInit) moveBall();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwSetTime(0.0f);
	while(glfwGetTime() < 1.0f);
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}

void drawPlayer(){
	
	if(leftKeyDown) if(thePlayer.x > gameArea.x)			thePlayer.x = thePlayer.x - 0.5f;
	if(rightKeyDown) if((thePlayer.x - 17.0f) < gameArea.x) thePlayer.x = thePlayer.x + 0.5f;
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	
	glBegin(GL_LINES);
	
	glColor4f(1.0f, 0.0f, 1.0f, 0.5f);
	glVertex3f(thePlayer.x, thePlayer.y, 0.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	glVertex3f(thePlayer.x + 3.0f, thePlayer.y, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}

void drawBall(){
	
	if(!gameInit){
		
		theBall.x = thePlayer.x + 1.5f;
		theBall.y = thePlayer.y + 1.0f;
		
	}
	
	unsigned short int circle = 300;
	
	glPushMatrix();
	glTranslatef(theBall.x, theBall.y, 0.0f);
	glBegin(GL_LINE_LOOP);
	glColor4f(0.5f, 0.0f, 0.5f, 0.9f);
	
	for(int i = 0; i < circle; i++){
		
		float angle = 2.0f * 3.14f * (i / 300.0f);
		float x = cos(angle) * 1.0f;
		float y = sin(angle) * 1.0f;
		
		glVertex3f(x, y, 0.0f);
		
	}
	
	glEnd();
	glPopMatrix();
	
}

void key_callback(GLFWwindow* w, int key, int scancode, int action, int mode){
	
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(w, GLFW_TRUE);
	
	if(key == GLFW_KEY_LEFT   && action == GLFW_PRESS){
		
		leftKeyDown = 1;
		//if(thePlayer.x > gameArea.x) thePlayer.x = thePlayer.x - 1.0f;
		
	}
	else if(key == GLFW_KEY_LEFT && action == GLFW_RELEASE){
		
		leftKeyDown = 0;
		
	}
	
	if(key == GLFW_KEY_RIGHT  && action == GLFW_PRESS){
		
		rightKeyDown = 1;
		//if((thePlayer.x - 17.0f) < gameArea.x) thePlayer.x = thePlayer.x + 1.0f;
		
	}
	else if(key == GLFW_KEY_RIGHT && action == GLFW_RELEASE){
		
		rightKeyDown = 0;
		
	}
	
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS){
		
		if(!gameInit){
			/* Game is now enabled */
			
			gameInit = 1;
			
		}
		
	}
	
}

void drawBackground(){
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	
	glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
	glBegin(GL_LINES);
	
	glVertex3f(gameArea.x, gameArea.y, 0.0f);
	glVertex3f(gameArea.x, gameArea.y + 22.0f, 0.0f);
	
	glVertex3f(gameArea.x, gameArea.y + 22.0f, 0.0f);
	glVertex3f(gameArea.x + 20.0f, gameArea.y + 22.0f, 0.0f);
	
	glVertex3f(gameArea.x + 20.0f, gameArea.y + 22.0f, 0.0f);
	glVertex3f(gameArea.x + 20.0f, gameArea.y, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}