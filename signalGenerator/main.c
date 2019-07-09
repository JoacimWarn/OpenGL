

#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define WIDTH  1024
#define HEIGHT  768
#define BUFFER 24
#define CIRCLE 300
#define NR_OF_KNOBS 4

struct point{
	
	float x;
	float y;
	
};

struct point drawingArea;
struct point knobs[NR_OF_KNOBS];
float knobDetailPos[NR_OF_KNOBS];

void drawKnobDetails();
void drawKnobs();
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_button_callback(GLFWwindow*, int, int, int);

/*
Drawing the curve
*/
void drawWave(){
	
	static float phaseShift = 0.001f;
	float amplitude = 1.0f + knobDetailPos[1] * 1.5f;
	float inc = 0.1f - knobDetailPos[0] * 0.15f;
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	//glBegin(GL_POINTS);
	glColor4f(0.0f, 0.5f, 1.0f, 0.5f);
	
	float i;
	int counter = 2;
	for(i = 0.0f; i < BUFFER;){
		
		if(knobDetailPos[3] < 0.0f){
			/* Sinus wave */
			
			glBegin(GL_POINTS);
			float y = sin(i + phaseShift) * amplitude;
			glVertex3f(i * 1.0f + drawingArea.x, y - 1.0f, 0.0f);
			i = i + inc;
			
		}
		else if(knobDetailPos[3] == 0.0f){
			/* Square wave */
			glBegin(GL_LINES);
			float y = -1.0f * (-amplitude);
			
			if((counter%2) == 0){
				
				glVertex3f(i * 1.0f + drawingArea.x, 		y, 0.0f);
				glVertex3f(i * 1.0f + drawingArea.x + 1.0f, y, 0.0f);
				
				glVertex3f(i * 1.0f + drawingArea.x + 1.0f, y,		  0.0f);
				glVertex3f(i * 1.0f + drawingArea.x + 1.0f, y-(1.0f), 0.0f);
				
			}
			else{
				
				glVertex3f(i * 1.0f + drawingArea.x,		y-(1.0f), 0.0f);
				glVertex3f(i * 1.0f + drawingArea.x + 1.0f, y-(1.0f), 0.0f);
				
				glVertex3f(i * 1.0f + drawingArea.x + 1.0f, y-(1.0f), 0.0f);
				glVertex3f(i * 1.0f + drawingArea.x + 1.0f, y, 0.0f);
				
			}
			//glVertex3f(i * 1.0f + drawingArea.x, 1.0f, 0.0f);
			//float y = sin(i + phaseShift) * amplitude;
			//glVertex3f(i * 1.0f + drawingArea.x, y - 1.0f, 0.0f);
			
			counter++;
			i = i + 1.0f;
			
		}
		else{
			/* Triangle wave */
			
			glBegin(GL_LINES);
			float y = -1.0f * (-amplitude);
			
			glVertex3f(i * 1.0f + drawingArea.x,		y,		  0.0f);
			glVertex3f(i * 1.0f + drawingArea.x + 1.0f, y + 1.0f, 0.0f);
			
			glVertex3f(i * 1.0f + drawingArea.x + 1.0f, y + 1.0f, 0.0f);
			glVertex3f(i * 1.0f + drawingArea.x + 2.0f,		y,		  0.0f);
			
			i = i + 2.0f;
			
		}
		
	}
	
	phaseShift = phaseShift - knobDetailPos[2] * 0.1f;
	
	glEnd();
	glPopMatrix();
	
}

void drawGrid(){
	
	float i;
	int j;
	
	i = drawingArea.x + 12.0f;
	for(j = 24; j > 0; j--){
		
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		
		glColor4f(0.0f, 1.0f, 0.0f, 0.1f);
		glBegin(GL_LINES);
		
		glVertex3f(drawingArea.x - i, drawingArea.y, 0.0f);
		glVertex3f(drawingArea.x - i, drawingArea.y - 12.0f, 0.0f);
		
		if(j > 18){
			
			glVertex3f(drawingArea.x, drawingArea.y + i * 2.0f, 0.0f);
			glVertex3f(drawingArea.x - drawingArea.x * 2.0f, drawingArea.y + i * 2.0f, 0.0f);
			
		}
		
		glEnd();
		glPopMatrix();
		
		i = i - 1.0f;
		
	}
	
	
	
}

void drawDrawingArea(){
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	
	glColor4f(1.0f, 0.0f, 1.0f, 0.5f);
	glBegin(GL_LINE_LOOP);
	
	glVertex3f(drawingArea.x, drawingArea.y, 0.0f);
	glVertex3f(drawingArea.x + 24.0f, drawingArea.y, 0.0f);
	
	glVertex3f(drawingArea.x + 24.0f, drawingArea.y - 12.0f, 0.0f);
	glVertex3f(drawingArea.x, drawingArea.y - 12.0f, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}

int main(){
	
	glfwInit();
	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "SignalGenerator1.0", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	
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
	
	int i;
	
	drawingArea.x = -12.0f;
	drawingArea.y =   4.0f;
	
	for(i = 0; i < NR_OF_KNOBS; i++){
		
		knobs[i].x = -10.0f + (i * 3.0f);
		knobs[i].y = 10.0f;
		
		knobDetailPos[i] = -0.5f;
		
	}
	
	glfwSetTime(0.0f);
	
	while(!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		drawKnobs();
		drawKnobDetails();
		drawDrawingArea();
		drawGrid();
		drawWave();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}

void drawKnobDetails(){
	
	int i;
	
	for(i = 0; i < NR_OF_KNOBS; i++){
		
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		
		glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
		glBegin(GL_LINES);
		
		glVertex3f(knobs[i].x, knobs[i].y, 0.0f);
		glVertex3f(knobs[i].x + knobDetailPos[i], knobs[i].y + 0.8f, 0.0f);
		//glVertex3f(knobs[i].x, knobs[i].y + 0.5f, 0.0f);
		//glVertex3f(knobs[i].x, knobs[i].y + 0.5f, 0.0f);
		
		glEnd();
		glPopMatrix();
		
	}
	
}

void drawKnobs(){
	
	int i, j;
	
	for(j = 0; j < NR_OF_KNOBS; j++){
		
		glPushMatrix();
		glTranslatef(knobs[j].x, knobs[j].y, 0.0f);
		//glRotatef(45.0f * glfwGetTime(), 0, 0, 1);
		float angle, x, y;
		
		glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
		glBegin(GL_LINE_LOOP);
		
		for(i = 0; i < CIRCLE; i++){
			
			angle = 2.0f * 3.14f * (i/300.0f);
			x = cos(angle) * 1.0f;
			y = sin(angle) * 1.0f;
			
			glVertex3f(x, y, 0.0f);
			
		}
		
		glEnd();
		glPopMatrix();
		
	}
	
}

void mouse_button_callback(GLFWwindow* w, int button, int action, int mods){
	
	if(action == GLFW_PRESS){
		
		double xpos, ypos;
		xpos = ypos = 0;
		glfwGetCursorPos(w, &xpos, &ypos);
		
		xpos = xpos / WIDTH;
		xpos = xpos * 26;
		xpos = xpos - 13;
		
		ypos = ypos / HEIGHT;
		ypos = ypos * (-26);
		ypos = ypos + 13;
		
		float dx, dy;
		int i;
		
		for(i = 0; i < NR_OF_KNOBS; i++){
			
			dx = abs(xpos - knobs[i].x);
			
			if(dx < 0.5f){
				
				dy = abs(ypos - knobs[i].y);
				
				if(dy < 1.0f){
					
					if(knobDetailPos[i] >= 0.5f){
						
						knobDetailPos[i] = -0.5f;
						break;
						
					}
					else knobDetailPos[i] = knobDetailPos[i] + 0.5f;
					break;
					
				}
				
			}
			
		}
		
	}
	
}

void key_callback(GLFWwindow* w, int key, int scancode, int action, int mode){
	
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(w, GLFW_TRUE);
	
}

