//gcc main.c -std=c99 -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <math.h>

#define CIRCLE 300
#define LOWER_LIMIT -13.0f
#define UPPER_LIMIT  13.0f
#define RIGHT_LIMIT  13.0f
#define  LEFT_LIMIT -13.0f

typedef struct Entity {
    float   pos[2];
    float   vel[2];
    float   acc[2];
} Entity;

static void entity_apply(Entity* this, float fx, float fy){
    this->acc[0] += fx;
    this->acc[1] += fy;
}

static void entity_update(Entity* this, float t){
    this->vel[0] += this->acc[0] * t;
    this->vel[1] += this->acc[1] * t;

    this->pos[0] += this->vel[0] * t;
    this->pos[1] += this->vel[1] * t;

    this->acc[0] = 0.0f;
    this->acc[1] = 0.0f;
}
//
//global variables
//
struct Entity theBall;
unsigned short int collided = 0;
//
//
static void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods){
	
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(w, GLFW_TRUE);
	
}


void drawShapes(){
	
	glPushMatrix();
	
	//glTranslatef(theCircle.x, theCircle.y, 0.0f);
	glTranslatef(theBall.pos[0], theBall.pos[1], 0.0f);
	
	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	
	for(int i = 0; i < CIRCLE; i++){
		
		float angle = 2.0f * 3.14f * (i/300.0f);
		float x = cos(angle) * 1.0f;
		float y = sin(angle) * 1.0f;
		glVertex3f(x, y, 0.0f);
		
	}
	
	glEnd();
	glPopMatrix();
	
}


void collisionCheck(){
	
	float dt = glfwGetTime();
	glfwSetTime(0.0f);
	
	entity_update(&theBall, dt);
	entity_apply(&theBall, 0.0f, -9.82f);
	
	if(collided){
		
		theBall.vel[1] = theBall.vel[1] * 0.4f;
		entity_apply(&theBall, 0.0f, 9.82f);
		
	}
	collided = 0;
	if(theBall.pos[1] < LOWER_LIMIT){
		
		theBall.vel[1] = abs( theBall.vel[1] );
		collided = 1;
		
	}
	
}


int main(){
	
	int width = 1024;
	int height = 768;
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "Physics", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);
	
	float camera = 8.0f;
	{
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120.0f, width/height, 0.1f, 32.0f);
		gluLookAt(
		0,	0,	camera,
		0,	0,	0,
		0,	1,	0
		);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
	}
	
	glScalef(1.0f, 1.0f, 1.0f);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	theBall.pos[0] = 0.0f;
	theBall.pos[1] = 10.0f;
	
	theBall.vel[0] = 0.0f;
	theBall.vel[1] = 0.0f;
	
	theBall.acc[0] = 0.0f;
	theBall.acc[1] = 0.0f;
	
	glfwSetTime(0.0f);
	
	while(!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT);
		//rendering
		drawShapes();
		collisionCheck();
		
		//if ball hits the ground -> time = 0
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}

