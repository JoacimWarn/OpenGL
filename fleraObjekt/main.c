//gcc main.c -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//
//
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

#pragma ("glw3.lib")
#pragma ("opengl32.lib")
#pragma ("glu32.lib")
#pragma ("gdi32.lib")
//
//
#define NUMBEROFSHAPES 5

#define CIRCLE 300
#define LOWER_LIMIT -1.8f
#define UPPER_LIMIT  1.8f
#define RIGHT_LIMIT  2.8f
#define  LEFT_LIMIT -2.8f
//
//
struct shape{
	
	float x;
	float y;
	float dx;
	float dy;
	
};
//
//
struct shape shapes[NUMBEROFSHAPES];
//
//
void drawShapes(){
	
	for(int j = 0; j < NUMBEROFSHAPES; j++){
		
		//glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		glTranslatef(shapes[j].x, shapes[j].y, 0.0f);
		
		glColor4f(0.0f, 0.0f, 1.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
		
		for(int i = 0; i < CIRCLE; i++){
			
			float angle = 2.0f * 3.14f * (i/300.0f);
			float x = cos(angle) * 1.0f;
			float y = sin(angle) * 1.0f;
			glVertex3f(x, y, 0.0f);
			
		}
		
		//glPushMatrix();
		glEnd();
		glPopMatrix();
		
	}
	
}
//
//
void moveShapes(){
	
	for(int i = 0; i < NUMBEROFSHAPES; i++){
		
		if( shapes[i].x <= LEFT_LIMIT || shapes[i].x >= RIGHT_LIMIT )  shapes[i].dx = -shapes[i].dx;
		if( shapes[i].y <= LOWER_LIMIT || shapes[i].y >= UPPER_LIMIT ) shapes[i].dy = -shapes[i].dy;
		
		if((i%2) == 0){
			shapes[i].x = shapes[i].x + shapes[i].dx;
			shapes[i].y = shapes[i].y + shapes[i].dy;
		}
		else{
			shapes[i].x = shapes[i].x - shapes[i].dx;
			shapes[i].y = shapes[i].y - shapes[i].dy;
		}
		
	}
	
}
//
//
int main(){
	
	glfwInit();
	
	GLFWwindow* window = glfwCreateWindow(1024,768,"Multiple shapes",NULL,NULL);
	glfwMakeContextCurrent(window);
	
	float z = 0.8f;
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
	
	glScalef(0.5f, 0.5f, 1.0f);
	
	for(int i = 0; i < NUMBEROFSHAPES; i++){
		
		shapes[i].x = 0.0f + i * 0.2f;
		shapes[i].y = 0.0f + i * 0.2f;
		shapes[i].dx = 0.02f + i * 0.002f;
		shapes[i].dy = 0.03f + i * 0.002f;
		
	}
	
	//Refreshing drawing table
	while(!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		drawShapes();
		moveShapes();
		
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		//counter++;
		//if(counter >= NUMBEROFSHAPES) counter = 0;
		
		//printf("%d\n",counter);
		
	}
	
	return 0;
	
}

