//gcc main.c -std=c99 -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//
#include <GL/glu.h>
#include <GLFW/glfw3.h>

//headers definiton
static void key_callback(GLFWwindow*, int, int, int, int);
void drawRectangles(void);
void drawBackground(void);
void mouse_button_callback(GLFWwindow*, int, int, int);
void checkBoard(void);
void drawBoard(void);
void checkIfWinOrFullBoard(void);
//end headers defs

unsigned char mousePressed = '0';
float mouseX = 0.0f;
float mouseY = 0.0f;
unsigned short int playBoard[] =
{
0,	0,	0,
0,	0,	0,
0,	0,	0
};


int main(){
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Three_In_A_Row", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	float camera = 8.0f;
	{
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120.0f, 1024.0f / 768.0f, 0.1f, 32.0f);
		gluLookAt(
		0,	0,	camera,
		0,	0,	0,
		0,	1,	0
		);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
	}
	
	while(!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		//rendering
		drawRectangles();
		checkBoard();
		drawBoard();
		checkIfWinOrFullBoard();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}

void checkIfWinOrFullBoard(){
	
	//if board is full -> clear board
	int i;
	unsigned short int bool;
	for(i = 0; i < 9; i++){
		
		bool = 1;
		if(playBoard[i] == 0) {
			
			bool = 0;
			break;
			
		}
		
	}
	
	if(bool) for(i = 0; i < 9; i++) playBoard[i] = 0;
	
}

void drawBoard(){
	
	float x = -14.0f;
	float y =  10.0f;
	float xInc = 10.0f;
	float yInc = 7.0f;
	
	unsigned short int counter = 0;
	for(int i = 0; i < 3; i++){
		
		for(int j = 0; j < 3; j++){
			
			glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.0f);
			
			if(playBoard[counter] == 1){
				
				glColor4f(1.0f, 0.0f, 0.0f, 0.7f);
				glBegin(GL_LINES);
				
				glVertex3f(x, y, 0.0f);
				glVertex3f(x + xInc, y - yInc, 0.0f);
				
				glVertex3f(x + xInc, y, 0.0f);
				glVertex3f(x, y - yInc, 0.0f);
				
			}
			else if(playBoard[counter] == 2){
				
				glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
				glBegin(GL_LINES);
				
				glVertex3f(x, y, 0.0f);
				glVertex3f(x + xInc, y - yInc, 0.0f);
				
				glVertex3f(x + xInc, y, 0.0f);
				glVertex3f(x, y - yInc, 0.0f);
				
			}
			
			counter++;
			x = x + xInc;
			glEnd();
			glPopMatrix();
			
		}
		
		y = y - yInc;
		x = -14.0f;
		
	}
	
}

void checkBoard(){
	
	static unsigned short int crossColor = 1;
	
	if(mousePressed == '1'){
		
		float x = -14.0f;
		float y =  10.0f;
		float xInc = 10.0f;
		float yInc = 7.0f;
		unsigned short int squarePressed = 0;
		
		for(int i = 0; i < 3; i++){
			
			for(int j = 0; j < 3; j++){
				
				if(mouseX > x && mouseX < x + xInc){
					
					if(mouseY > y - yInc && mouseY < y){
						
						if(playBoard[squarePressed] == 0){
							
							playBoard[squarePressed] = crossColor;
							if(crossColor == 1) crossColor = 2;
							else 				crossColor = 1;
							
						}
						break;
						
					}
					
				}
				
				x = x + xInc;
				squarePressed++;
				
			}
			
			y = y - yInc;
			x = -14.0f;
			
		}
		
		mousePressed = '0';
		
	}
	
}

void mouse_button_callback(GLFWwindow* w, int button, int action, int mods){
	
	if(button == GLFW_MOUSE_BUTTON_LEFT){
		
		if(action == GLFW_PRESS){
			
			mousePressed = '1';
			double xpos, ypos;
			glfwGetCursorPos(w, &xpos, &ypos);
			
			xpos = xpos / 1024;
			xpos = xpos * 26;
			xpos = xpos - 13;
			
			ypos = ypos / 768;
			ypos = ypos * (-26);
			ypos = ypos + 13;
			
			mouseX = xpos;
			mouseY = ypos;
			
			
			
		}
		else if(action == GLFW_RELEASE){
			
			mousePressed = '0';
			
		}
		
	}
	
}


void drawRectangles(){
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor4f(1.0f, 0.0f, 1.0f, 0.7f);
	
	glBegin(GL_LINE_LOOP);
	float x = -14.0f;
	float y =  10.0f;
	float xInc = 10.0f;
	float yInc = 7.0f;
	
	for(int i = 0; i < 3; i++){
		
		for(int j = 0; j < 3; j++){
			glBegin(GL_LINE_LOOP);
			
			glVertex3f(x, y, 0.0f);
			glVertex3f(x + xInc, y, 0.0f);
			glVertex3f(x + xInc, y - yInc, 0.0f);
			glVertex3f(x, y - yInc, 0.0f);
			
			glEnd();
			x = x + xInc;
			
		}
		
		y = y - yInc;
		x = -14.0f;
		
	}
	
	glEnd();
	glPopMatrix();
	
}

static void key_callback(GLFWwindow* w, int key, int action, int scancode, int mods){
	
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(w, GLFW_TRUE);
	
}
