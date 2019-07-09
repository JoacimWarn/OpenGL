//gcc main.c -std=c99 -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static void key_callback(GLFWwindow*,int,int,int,int);
void drawBalls(void);
void moveBalls(void);

#define WIDTH  1280
#define HEIGHT 1024
#define NUMBER_OF_BALLS 4

#define LOWER_LIMIT -13.0f
#define UPPER_LIMIT  13.0f
#define RIGHT_LIMIT  13.0f
#define  LEFT_LIMIT -13.0f

typedef struct Entity {
    float   pos[2];
    float   vel[2];
    float   acc[2];
} Entity;

static void entity_apply(Entity* this, float fx, float fy) {
    this->acc[0] += fx;
    this->acc[1] += fy;
}

static void entity_update(Entity* this, float t) {
    this->vel[0] += this->acc[0] * t;
    this->vel[1] += this->acc[1] * t;

    this->pos[0] += this->vel[0] * t;
    this->pos[1] += this->vel[1] * t;

    this->acc[0] = 0.0f;
    this->acc[1] = 0.0f;
}

Entity ball[NUMBER_OF_BALLS];
float dx[NUMBER_OF_BALLS];
float dy[NUMBER_OF_BALLS];

void checkCollision(){
	
	for(int i = 0; i < NUMBER_OF_BALLS; i++){
		
		for(int j = 0; j < NUMBER_OF_BALLS; j++){
			
			if(i == j) break;
			
			float diffX = abs( ball[i].pos[0] - ball[j].pos[0] );
			
			if(diffX <= 1.5f){
				
				float diffY = abs( ball[i].pos[1] - ball[j].pos[1] );
				
				if(diffY <= 1.5f){
					
					//change direction of dx and dy
					//collision
					
					ball[i].vel[0] = abs(ball[i].vel[0]) * (1.1f);
					ball[i].vel[1] = abs(ball[i].vel[1]) * (1.1f);
					
					dx[i] = -dx[i];
					dy[i] = -dy[i];
					
					//entity_apply(&ball[i], 0.0f, 0.0f);
					//entity_apply(&ball[j], 0.0f, 0.0f);
					
				}
				
			}
			
		}
		
	}
	
}

int main(){
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MultipleBallCollisions", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);
	
	float camera = 8.0f;
	{
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120.0f, WIDTH/HEIGHT, 0.1f, 32.0f);
		gluLookAt(
		0,	0,	camera,
		0,	0,	0,
		0,	1,	0
		);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
	}
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	for(int i = 0; i < NUMBER_OF_BALLS; i++){
		
		ball[i].pos[0] = LEFT_LIMIT + 1.0f + i * 3.0f;
		ball[i].pos[1] = 0.0f;
		dx[i] = 0.3f + i * 0.9f;
		dy[i] = 0.2f + i * 2.0f;
		
	}
	
	glfwSetTime(0.0f);
	
	while(!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT);
		
		//rendering
		drawBalls();
		moveBalls();
		checkCollision();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}

void moveBalls(){
	
	float dt = glfwGetTime();
	glfwSetTime(0.0f);
	
	for(int i = 0; i < NUMBER_OF_BALLS; i++){
		
		if(ball[i].pos[0] >= RIGHT_LIMIT || ball[i].pos[0] <= LEFT_LIMIT){
			
			ball[i].vel[0] = - ball[i].vel[0];
			dx[i] = -dx[i];
			
		}
		if(ball[i].pos[1] >= UPPER_LIMIT || ball[i].pos[1] <= LOWER_LIMIT){
			
			ball[i].vel[1] = - ball[i].vel[1];
			dy[i] = -dy[i];
			
		}
		
		if(ball[i].vel[0] <= 2.0f + i && ball[i].vel[0] >= -1.0f * i) entity_apply(&ball[i], dx[i], 0.0f);
		if(ball[i].vel[1] <= 3.0f && ball[i].vel[1] >= -3.0f) entity_apply(&ball[i], 0.0f, dy[i]);
		
		entity_update(&ball[i], dt);
		
	}
	
}

void drawBalls(){
	
	unsigned short int CIRCLE = 300;
	
	for(int i = 0; i < NUMBER_OF_BALLS; i++){
		
		glPushMatrix();
		glTranslatef(ball[i].pos[0], ball[i].pos[1], 0.0f);
		glColor4f(1.0f, 0.0f, 0.0f, 0.7f);
		glBegin(GL_LINE_LOOP);
		
		for(int j = 0; j < CIRCLE; j++){
			
			float angle = 2.0f * 3.14f * (j/300.0f);
			float x = cos(angle) * 1.0f;
			float y = sin(angle) * 1.0f;
			
			glVertex3f(x, y, 0.0f);
			
		}
		
		glEnd();
		glPopMatrix();
		
	}
	
}

static void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods){
	
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(w, GLFW_TRUE);
	
}