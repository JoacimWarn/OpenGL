//
//
//gcc main.c -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//
//
#include <GL/glu.h>
#include <GLFW/glfw3.h>
//
//
#define XROW 15
#define YROW 10
//
//
void background(){
	
	static float leftSquare[2] = {0};
	
	glTranslatef(leftSquare[0], leftSquare[1], 0.0f);
	
	glBegin(GL_QUADS);
	
	glColor4f(0.0f, 0.5f, 1.0f, 1.0f);
	
	int i = 0;
	float inc = -40.0f;
	
	//Draw upper and lower row
	for(; i <= XROW; i++){
		
		glVertex3f( inc,		+30.0f, 0.0f);
		glVertex3f( inc + 5,	+30.0f, 0.0f);
		glVertex3f( inc + 5,	+25.0f, 0.0f);
		glVertex3f( inc,		+25.0f, 0.0f);
		
		glVertex3f( inc,		-30.0f, 0.0f);
		glVertex3f( inc + 5,	-30.0f, 0.0f);
		glVertex3f( inc + 5,	-25.0f, 0.0f);
		glVertex3f( inc,		-25.0f, 0.0f);
		
		inc = inc + 5.0f;
		
	}
	
	i = 0;
	inc = 0.0f;
	
	//Draw right and left column
	for(; i <= YROW; i++){
		
		glVertex3f( -40.0f,		+30.0f - inc, 0.0f);
		glVertex3f( -35.0f,		+30.0f - inc, 0.0f);
		glVertex3f( -35.0f,		+25.0f - inc, 0.0f);
		glVertex3f( -40.0f,		+25.0f - inc, 0.0f);
		
		glVertex3f( 40.0f,		-30.0f + inc, 0.0f);
		glVertex3f( 35.0f,		-30.0f + inc, 0.0f);
		glVertex3f( 35.0f,		-25.0f + inc, 0.0f);
		glVertex3f( 40.0f,		-25.0f + inc, 0.0f);
		
		inc = inc + 5.0f;
		
	}
	
	glEnd();
	glPopMatrix();
	
}
//
//
void collisionDetection(GLFWwindow * w, float middleSquare[]){
	
	if (glfwGetKey(w, GLFW_KEY_UP)    == GLFW_PRESS) middleSquare[1] += 0.25f;
	if (glfwGetKey(w, GLFW_KEY_DOWN)  == GLFW_PRESS) middleSquare[1] -= 0.25f;
	if (glfwGetKey(w, GLFW_KEY_LEFT)  == GLFW_PRESS) middleSquare[0] -= 0.25f;
	if (glfwGetKey(w, GLFW_KEY_RIGHT) == GLFW_PRESS) middleSquare[0] += 0.25f;
	
	
	
}
//
//
int main(){
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello world", NULL, NULL);
	
	glfwMakeContextCurrent(window);
	
	// camera z
	float z = 8.0f;
	// rect pos
	float middleSquare[2] = {0};
	
	
	{
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120, 800.0f / 600.0f, 0.1f, 32.0f);
		gluLookAt(
		0, 0, z,  // pos!
		0, 0, 0,  // look at!
		0, 1, 0); // up axis!

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
	}
	
	glScalef(0.5f, 0.5f, 0.5f);
	
	
	
	
	while (!glfwWindowShouldClose(window)){
		float t = glfwGetTime();
		//glfwSetTime(0.0f);
		
		
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
		
		
		collisionDetection(window, middleSquare);
		
		
		// rendering shit:
		{
			glClear(GL_COLOR_BUFFER_BIT);


			glPushMatrix();
			
			glTranslatef(middleSquare[0], middleSquare[1], 0.0f);
			//glRotatef(90.0f * glfwGetTime(), 0, 0, 1);
			
			glBegin(GL_QUADS);
			
			glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
			
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glVertex3f(+1.0f, -1.0f, 0.0f);
			glVertex3f(+1.0f, +1.0f, 0.0f);
			glVertex3f(-1.0f, +1.0f, 0.0f);
			
			glEnd();
			glPopMatrix();
			
			//Drawing the background
			background();
			
			//glEnd();

			//glPopMatrix();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

