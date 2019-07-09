//gcc main.c -std=c99 -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <stdio.h>

#define uint32 unsigned 	  int
#define int32				  int
#define uint8  unsigned		 char
//
//
typedef struct Texture{
	
    uint32         id;
    int32         width;
    int32         height;
	
} Texture;
//
//
Texture texture_load(const char* texture_path, int32 is_smooth){
    Texture texture     = {0};
    int32   channels    = 0;
    uint8*  pixels      = NULL;

	//if channels ==  4 -> RGBA else -> RGB
	
    pixels = stbi_load(texture_path, &texture.width, &texture.height, &channels, 0);
	
    assert(pixels);
	
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);

    stbi_image_free(pixels);

    return texture;
	
}


static void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods){
	
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(w, GLFW_TRUE);
	
}


void drawBackground(){
	
	Texture alpaca;
	alpaca.width = 940;
	alpaca.height = 627;
	alpaca.id = '1';
	//alpaca = texture_load("min_bild_21.png",1);
	alpaca = texture_load("greentrees.png", 1);
	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glBindTexture(GL_TEXTURE_2D, alpaca);
	
	float x = 14.0f;
	float y = 14.0f;
	
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	glTexCoord2f(1, 1);
	glVertex3f( x,  y, 0.0f);
	glTexCoord2f(0, 1);
	glVertex3f(-x,  y, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(-x, -y, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f( x, -y, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}


void drawPlayer(){
	
	Texture alpaca;
	alpaca.width = 940;
	alpaca.height = 627;
	alpaca.id = '1';
	alpaca = texture_load("moresolidyellow.png", 1);
	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glBindTexture(GL_TEXTURE_2D, alpaca);
	
	float x = 1.0f;
	float y = 1.0f;
	
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	glTexCoord2f(1, 1);
	glVertex3f( x,  y, 0.0f);
	glTexCoord2f(0, 1);
	glVertex3f(-x,  y, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(-x, -y, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f( x, -y, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}


int main(){
	
	unsigned int WIDTH = 1024;
	unsigned int HEIGHT = 768;
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Picture", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);
	
	float camera = 8.0f;
	{
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120 ,WIDTH/HEIGHT, 0.1f, 32.0f);
		gluLookAt(
		0,	0,	camera,
		0,	0,	0,
		0,	1,	0
		);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
	}
	/*
	glEnable(GL_TEXTURE_2D);
	
	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.1f, 0.1f, 0.1f, 0.5f);
	stbi_set_flip_vertically_on_load(1);
	
	while(!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		//rendering
		drawBackground();
		drawPlayer();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}

