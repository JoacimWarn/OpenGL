//gcc main.c -std=c99 -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//

#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <stdio.h>

#define WIDTH  800
#define HEIGHT 600
#define LOWER_LIMIT -11.0f
#define UPPER_LIMIT  12.0f
#define RIGHT_LIMIT  36.0f
#define  LEFT_LIMIT -36.0f
#define BUFFER 10
//
//
struct point{
	
	float x;
	float y;
	
};

struct point projectileLoc[BUFFER];
struct point enemyLoc;
struct point playerLoc;
unsigned short int playerHit = 0;
unsigned short int playerHealth = 5;
unsigned short int shieldOn = 1;
//
//
void drawSheild(){
	
	if(shieldOn){
		
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
		
		glVertex3f(playerLoc.x - 0.5f, playerLoc.y - 0.5f, 0.0f);
		glVertex3f(playerLoc.x - 0.5f, playerLoc.y + 0.5f, 0.0f);
		
		glPopMatrix();
		glEnd();
		
	}
	
}
//
//
void drawBackground(){
	
	float inc = 0.5f;
	static float x = -14.0f;
	static float y = -13.0f;
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	
	glVertex3f(x, y, 0.0f);
	glVertex3f(x + 28.0f, y, 0.0f);
	glVertex3f(x + 28.0f, y - 1.0f, 0.0f);
	glVertex3f(x, y - 1.0f, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}
//
//
void drawEnemy(){
	
	static float inc = 0.01f;
	static float x = -13.0f;
	static float phaseShift = 0.01f;
	static float amplitude = 12.0f;
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glColor4f(0.5f, 0.0f, 1.0f, 0.8f);
	
	float z;
	
	for(float i = -15.0f; i < 14.0f;){
		
		float y = sin(i - phaseShift) * amplitude;
		i = i + inc;
		
		z = y;
		
		//glVertex3f(i, y, 0.0f);
		//glVertex3f(i + 0.5f, y, 0.0f);
		
	}
	
	glVertex3f(-13.0f, z, 0.0f);
	glVertex3f(-13.0f + 0.5f, z, 0.0f);
	glVertex3f(-13.0f + 0.5f, z - 0.5f, 0.0f);
	glVertex3f(-13.0f, z - 0.5f, 0.0f);
	
	enemyLoc.x = -13.0f;
	enemyLoc.y = z;
	
	//inc = inc + 0.01f;
	if(glfwGetTime() > 5.0f) phaseShift = phaseShift - 0.05f;
	else phaseShift = phaseShift - 0.02f;
	
	glEnd();
	glPopMatrix();
	
}
//
//
void drawProjectile(){
	
	static float dx[BUFFER] = {-13.0f, -11.0f, -9.0f, -7.0f, -5.0f, -3.0f, -1.0f, 1.0f, 3.0f, 5.0f};
	
	for(int i = 0; i < BUFFER; i++){
		
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
		glBegin(GL_LINES);
		
		projectileLoc[i].x = dx[i];
		dx[i] = dx[i] + 0.5f;
		
		if(dx[i] >= 14.0f){
			
			dx[i] = -13.0f;
			projectileLoc[i].y = enemyLoc.y - 0.25f;
			
		}
		
		glVertex3f(projectileLoc[i].x + 0.5f, projectileLoc[i].y, 0.0f);
		glVertex3f(projectileLoc[i].x + 1.5f, projectileLoc[i].y, 0.0f);
		
		glEnd();
		glPopMatrix();
		
	}
	
}
//
//
void drawPlayer(GLFWwindow* w){
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	
	glColor4f(0.0f, 0.5f, 1.0f, 0.8f);
	if(playerHit){
		
		glColor4f(0.0f, 0.5f, 1.0f, 0.0f);
		playerHit = 0;
		
	}
	
	
		 if(glfwGetKey(w, GLFW_KEY_UP)   == GLFW_PRESS && playerLoc.y < UPPER_LIMIT) playerLoc.y = playerLoc.y + 0.25f;
	else if(glfwGetKey(w, GLFW_KEY_DOWN) == GLFW_PRESS && playerLoc.y > LOWER_LIMIT) playerLoc.y = playerLoc.y - 0.25f;
	
	
	glVertex3f(playerLoc.x, playerLoc.y, 0.0f);
	glVertex3f(playerLoc.x + 0.5f, playerLoc.y, 0.0f);
	glVertex3f(playerLoc.x + 0.5f, playerLoc.y - 0.5f, 0.0f);
	glVertex3f(playerLoc.x, playerLoc.y - 0.5f, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}
//
//
void hitDetection(){
	
	for(int i = 0; i < BUFFER; i++){
		
		//check if shield is active first
		
		if(playerLoc.x >= projectileLoc[i].x - 0.5f && playerLoc.x <= projectileLoc[i].x + 0.5f){
			
			if(playerLoc.y >= projectileLoc[i].y - 0.5f && playerLoc.y <= projectileLoc[i].y + 0.5f){
				
				if(shieldOn){
					
					shieldOn = 0;
					break;
					
				}
				else if(!shieldOn){
					
					playerHit = 1;
					playerHealth = playerHealth - 1;
					
				}
				
			}
			
		}
		
		
	}
	
}
//
//
int main(){
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Dodge", NULL, NULL);
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
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.1f, 0.1f, 0.1f, 0.5f);
	
	glScalef(1.0f, 1.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.01f, 0.0f);
	
	enemyLoc.x = 0.0f;
	enemyLoc.y = 0.0f;
	playerLoc.x = 10.0f;
	playerLoc.y = 5.0f;
	
	for(int i = 0; i < BUFFER; i++){
		
		projectileLoc[i].x = 0.0f;
		projectileLoc[i].y = 0.0f;
		
	}
	
	glfwSetTime(0.0f);
	
	while(!glfwWindowShouldClose(window) && (playerHealth > 0)){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		//Rendering
		drawBackground();		//drawing background
		drawEnemy();			//drawing the oscillating enemy
		drawProjectile();		//drawing projectice from enemy
		drawPlayer(window);		//drawing the player
		drawSheild(shieldOn);
		hitDetection();			//checks if the player has been hit by projectile
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}

