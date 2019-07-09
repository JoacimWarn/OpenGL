#include <GL/glu.h>
#include <GLFW/glfw3.h>

#define HEIGHT 768
#define WIDTH 1024
#define ASPECT_RATIO WIDTH/HEIGHT
#define nrOfSqrs 5

void key_callback(GLFWwindow*, int, int, int, int);

struct square{
	
	float x;
	float y;
	
};

struct square squares[nrOfSqrs];

void drawSquares(){
	
	static float nrOfTimesCalled = 0.01f;
	
	float increment;
	int i, j;
	
	increment = -8.0f;
	
	for(i = 0; i < nrOfSqrs; i++){
		
		glPushMatrix();
		glTranslatef(squares[i].x, squares[i].y, 0.0f);
		glRotatef(1.0f * glfwGetTime() + (i * nrOfTimesCalled), 0, 0, 1);
		
		glColor4f(1.0f, 0.0f, 1.0f, 0.5f);
		glBegin(GL_LINE_LOOP);
		
		for(j = 0; j < 4; j++){
			
			//4 edges of square
			glVertex3f(squares[j].x, squares[j].y, 0.0f);
			glVertex3f(squares[j].x + increment, squares[j].y, 0.0f);
			
			glVertex3f(squares[j].x + increment, squares[j].y - increment, 0.0f);
			glVertex3f(squares[j].x, squares[j].y - increment, 0.0f);
			
		}
		
		glEnd();
		glPopMatrix();
		
	}
	
	nrOfTimesCalled = nrOfTimesCalled + 0.5f;
	
}

void moveSquares(){
	
	/*
	int i;
	
	for(i = 0; i < nrOfSqrs; i++){
		
		squares[i].x = squares[i].x + 0.01f;
		squares[i].y = squares[i].y + 0.01f;
		
	}
	*/
	
}

int main(){
	
	//glRotatef(90.0f * glfwGetTime(), 0, 0, 1);
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bouncing inside square", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);
	
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
	
	int i;
	for(int i = 0; i < nrOfSqrs; i++){
		
		squares[i].x = 0.0f;
		squares[i].y = 0.0f;
		
	}
	
	glfwSetTime(0.0f);
	
	while(!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		//Draw objects
		drawSquares();
		moveSquares();
		
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