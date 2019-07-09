//gcc main.c -std=c99 -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <stdio.h>

#define uint32 unsigned  int
#define  int32 			 int
#define  uint8 unsigned char
#define NUMBER_OF_CRATES 36

#define LOWER_LIMIT -13.0f
#define UPPER_LIMIT  13.0f
#define RIGHT_LIMIT  13.0f
#define  LEFT_LIMIT -13.0f

typedef struct Texture{
	
	uint32         id;
	int32         width;
	int32         height;
	
} Texture;

/*
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
*/
typedef struct Entity{
	
	float x;
	float y;
	float velx;
	float vely;
	float accx;
	float accy;
	
} Entity;

static void entity_apply(Entity* this, float fx, float fy){
	
	this->accx += fx;
	this->accy += fy;
	
}

static void entity_update(Entity* this, float t){
	
	this->velx += this->accx * t;
	this->vely += this->accy * t;
	
	this->x += this->velx * t;
	this->y += this->vely * t;
	
	this->accx = 0.0f;
	this->accy = 0.0f;
	
}

struct Entity crate[NUMBER_OF_CRATES];
unsigned short int collided[NUMBER_OF_CRATES] = {0};
unsigned short int leftButtonDown = 0;
		 short int selected = -1;

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

void mouse_button_callback(GLFWwindow* w, int button, int action, int mods){
	
	if(button == GLFW_MOUSE_BUTTON_LEFT){
		
		if(action == GLFW_PRESS){
			
			leftButtonDown = 1;
			
		}
		else if(action == GLFW_RELEASE){
			
			leftButtonDown = 0;
			selected = -1;
			
		}
		
		if(leftButtonDown){
			
			//left
			double xpos, ypos;
			xpos = ypos = 0;
			glfwGetCursorPos(w, &xpos, &ypos);
			
			xpos = xpos / 1024;
			xpos = xpos * 26;
			xpos = xpos - 13;
			
			ypos = ypos / 768;
			ypos = ypos * (-26);
			ypos = ypos + 13;
			
			printf("%f\n",ypos);
			
			//ypos = ypos - 13;
			//xpos = xpos - 13;
			//xpos = (1024 / 2) - xpos;
			
			for(int i = 0; i < NUMBER_OF_CRATES; i++){
				
				float dx = abs(xpos - crate[i].x);
				
				//printf("%f %f\n",crate[i].x, crate[i].y);
				
				if(dx < 0.5f){
					
					float dy = abs(ypos - crate[i].y);
					//printf("%f\n",ypos);
					
					if(dy < 1.0f){
						
						//a box has been selected
						//crate[i].vely = crate[i].vely + 5.0f;
						//crate[i].y = ypos;
						selected = i;
						crate[i].y = ypos;
						
					}
					
				}
				
			}
			
		}
		
	}
	
}

void drawBackground(){
	
	Texture background;
	background.width =  940;
	background.height = 627;
	background.id = 	'1';
	background = texture_load("green.png", 1);
	//background = texture_load("crate.png", 1);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	float x = 14.0f;
	float y = 14.0f;
	
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	
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

void drawCrate(){
	
	Texture cratePic;
	cratePic.width = 256;
	cratePic.height = 256;
	cratePic.id = 1;
	cratePic = texture_load("crate.png", 1);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	for(int i = 0; i < NUMBER_OF_CRATES; i++){
		
		glPushMatrix();
		
		//glTranslatef(crate[i].pos[0], crate[i].pos[1], 0.0f);
		glTranslatef(crate[i].x, crate[i].y, 0.0f);
		glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		
		float x = 1.0f;
		float y = 1.0f;
		
		glTexCoord2f(1, 1);
		glVertex3f(x, y, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(-x, y, 0.0f);
		glTexCoord2f(0, 0);
		glVertex3f(-x,-y, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(x, -y, 0.0f);
		
		glEnd();
		glPopMatrix();
		
	}
	
}

void collisionCheckGround(){
	
	for(int i = 0; i < NUMBER_OF_CRATES; i++){
		
		float dy = crate[i].y - LOWER_LIMIT;
		
		if(dy < 0.0f){
			
			crate[i].vely = abs(crate[i].vely);
			collided[i] = 1;
			//collided[i] = 2;
			
		}
		
	}
	
}

void collisionCheckCrates(){
	
	for(int i = 0; i < NUMBER_OF_CRATES; i++){
		
		for(int j = 0; j < NUMBER_OF_CRATES; j++){
			
			if(i == j) break;
			
			float dx = abs(crate[i].x - crate[j].x);
			
			if(dx <= 1.0f){
				
				//same x position
				float dy =  abs(crate[j].y - crate[i].y);
				//printf("%f %f\n",crate[i].x, crate[j].x);
				
				if(dy < 2.0f){
					
					crate[i].vely = abs( crate[i].vely );
					collided[i] = 1;		//<----- problem collided[j]
					
					collided[j] = 1;
					crate[j].vely = abs( crate[j].vely );
					
					//crate[j].vely = abs( crate[i].vely ) * 0.3f;
					//crate[j].vely = -crate[j].vely;
					
				}
				
			}
			
		}
		
	}
	
}

void gravity(GLFWwindow* w){
	
	float dt = glfwGetTime();
	glfwSetTime(0.0f);
	
	for(int i = 0; i < NUMBER_OF_CRATES; i++){
		
		if(selected == i){
			
			double xpos, ypos;
			glfwGetCursorPos(w, &xpos, &ypos);
			xpos = xpos / 1024;
			xpos = xpos * 26;
			xpos = xpos - 13;
			ypos = ypos / 768;
			ypos = ypos * (-26);
			ypos = ypos + 13;
			
			crate[i].y = ypos;
			crate[i].x = xpos;
			crate[i].vely = 0.0f;
			
			continue;
			
		}
		
		entity_update(&crate[i], dt);
		entity_apply(&crate[i], 0.0f, -9.82f);
		
		if(collided[i] == 1){
			
			//collided with crate or ground
			crate[i].vely = crate[i].vely * 0.2f;
			entity_apply(&crate[i], 0.0f, 9.82f);
			entity_update(&crate[i], dt);
			
			collided[i] = 0;
			
		}
		
	}
	
}

int main(){
	
	int width = 1024;
	int height = 768;
	//int width = 1920;
	//int height = 1080;
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "SuperStacker1000", NULL, NULL);
	//GLFWwindow* window = glfwCreateWindow(width, height, "SuperStacker1000", glfwGetPrimaryMonitor(), NULL);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	
	float z = 8.0f;
	{
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120.0f, width/height, 0.1f, 32.0f);
		gluLookAt(
		0,	0,	z,
		0,	0,	0,
		0,	1,	0
		);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
	}
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.1f, 0.1f, 0.1f, 0.5f);
	stbi_set_flip_vertically_on_load(1);
	
	unsigned short int inc = 0;
	
	for(int i = 0; i < NUMBER_OF_CRATES; i++){
		
		//crate[i].x = 0.0f;
		crate[i].x = -12.0f + inc;
		crate[i].y = 0.0f + 10.0f * i;
		
		if(inc > 22) inc = 0;
		inc = inc + 3;
		
	}
	
	glfwSetTime(0.0f);
	
	while(!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		drawBackground();
		drawCrate();
		collisionCheckCrates();
		collisionCheckGround();
		gravity(window);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}