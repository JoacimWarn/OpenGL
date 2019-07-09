

#include <gl/glu.h>
#include <glfw/glfw3.h>

#include <stdio.h>
#include <math.h>

#define WIDTH 1024.0f
#define HEIGHT 768.0f
#define NR_OF_OBJECTS 5

void key_callback(GLFWwindow*, int, int, int, int);

struct planet{
	
	float x;
	float y;
	float mass;
	
};

struct planet planets[NR_OF_OBJECTS];

/*
Fg = (G * (m1*m2)) / (r*r)
Fc = (v*v) / r
*/
void movePlanets(){
	
	static int nrOfTimesCalled = 1;
	
	float x, y, angle;
	int i;
	
	for(i = 0; i < NR_OF_OBJECTS; i++){
		
		//velocity
		angle = 2.0f * 3.14f * (nrOfTimesCalled / 300.0f) / (1.0f + i);
		x = cos(angle) * 4.0f * (i + 1.0f);
		y = sin(angle) * 4.0f * (i + 1.5f);
		
		planets[i].x = x;
		planets[i].y = y;
		
	}
	
	nrOfTimesCalled++;
	
	//if(nrOfTimesCalled > (300 * NR_OF_OBJECTS)) nrOfTimesCalled = 1;
	
}

void drawPlanets(){
	
	unsigned short int circle = 300;
	int i, j;
	
	for(i = 0; i < NR_OF_OBJECTS; i++){
		
		glPushMatrix();
		glTranslatef(planets[i].x, planets[i].y, 0.0f);
		glBegin(GL_LINE_LOOP);
		glColor4f(0.5f, 0.5f, 0.3f, 0.7f);
		
		for(j = 0; j < circle; j++){
			
			float angle = 2.0f * 3.14f * (j / 300.0f);
			float x = cos(angle) * 1.0f;
			float y = sin(angle) * 1.0f;
			
			glVertex3f(x, y, 0.0f);
			
		}
		
		glEnd();
		glPopMatrix();
		
	}
	
}

void drawSun(){
	
	float middleX = (WIDTH / WIDTH)   * 1.0f;
	float middleY = (HEIGHT / HEIGHT) * 1.0f;
	
	float dx = -1.0f;
	float dy = -1.0f;
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 0.0f, 0.8f);
	
	glVertex3f(middleX, middleY, 0.0f);
	glVertex3f(middleX + dx, middleY, 0.0f);
	
	glVertex3f(middleX + dx,    middleY - dy, 0.0f);
	glVertex3f(middleX,			middleY - dy, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}

int main(){
	
	glfwInit();
	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "Circulating", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);
	
	float z = 16.0f;
	{
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120.0f, WIDTH/HEIGHT, 0.1f, 32.0f);
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
	
	int i;
	
	for(i = 0; i < NR_OF_OBJECTS; i++){
		
		planets[i].x = (WIDTH - WIDTH) + 1.0f + (3.0f * i);
		planets[i].y = (HEIGHT - HEIGHT);
		planets[i].mass = i + 1.0f;
		
	}
	
	while(!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		drawSun();
		drawPlanets();
		movePlanets();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}


void key_callback(GLFWwindow* w, int key, int scancode, int action, int mode){
	
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(w, GLFW_TRUE);
	
}