//gcc main.c -std=c99 -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#define WIDTH 1024
#define HEIGHT 768
#define BUFFER 10
//
//
struct point{
	
	float dx;
	float dy;
	float dz;
	float x;
	float y;
	float z;
	
};
//
struct point squareLoc[BUFFER];
//
//
void drawSquares(){
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	
	for(int i = 0; i < BUFFER; i++){
		
		glColor4f(1.0f, 0.0f, 1.0f, 0.1f * i);
		
		squareLoc[i].x += 2.0f * i;
		
		glVertex3f(squareLoc[i].x - 1.0f,  squareLoc[i].y, squareLoc[i].z);
		glVertex3f(squareLoc[i].x + 1.0f,  squareLoc[i].y, squareLoc[i].z);
		glVertex3f(squareLoc[i].x + 1.0f, -squareLoc[i].y, squareLoc[i].z);
		glVertex3f(squareLoc[i].x - 1.0f, -squareLoc[i].y, squareLoc[i].z);
		
		squareLoc[i].x = -12.0f;
		
	}
	
	glEnd();
	glPopMatrix();
	
}
//
//
void moveSquares(){
	
	for(int i = 0; i < BUFFER; i++){
		
		squareLoc[i].z -= squareLoc[i].dz;
		
		squareLoc[i].y -= squareLoc[i].dy * i * 0.1f;
		
			 if(squareLoc[i].y >= 3.0f) squareLoc[i].dy = -squareLoc[i].dy;
		else if(squareLoc[i].y <=-3.0f) squareLoc[i].dy = -squareLoc[i].dy;
		
			 if(squareLoc[i].z >= 5.0f) squareLoc[i].dz = -squareLoc[i].dz;
		else if(squareLoc[i].z <=-5.0f) squareLoc[i].dz = -squareLoc[i].dz;
		
	}
	
}
//
//
int main(){
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Randomstuff", NULL, NULL);
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
	
	for(int i = 0; i <= BUFFER; i++){
		
		squareLoc[i].x = -12.0f;
		squareLoc[i].y =   1.0f;
		squareLoc[i].z =   0.0f;
		squareLoc[i].dx =  0.0f;
		squareLoc[i].dy = 0.01f;
		squareLoc[i].dz = 0.01f;
		
	}
	
	glfwSetTime(0.0f);
	
	while(!(glfwWindowShouldClose(window))){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		//rendering
		drawSquares();
		//moveSquares();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}

