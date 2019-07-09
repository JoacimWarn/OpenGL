//gcc main.c -std=c99 -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <stdio.h>
#include <math.h>

//function headers
void drawBall(void);
void drawRamp(void);
void drawBackground(void);
static void key_callback(GLFWwindow*, int, int, int, int);
//end of function headers

#define uint32 unsigned int
#define  int32			int
#define uint8 unsigned char

#define WIDTH  1680
#define HEIGHT 768

#define LEFT_LIMIT  -28.0f
#define RIGHT_LIMIT  28.0f
#define LOWER_LIMIT -14.0f
#define UPPER_LIMIT  14.0f

#define RAMPLENGTH 10
#define NUMBER_OF_BALLS 1

typedef struct Texture{
	
	uint32	id;
	int32	width;
	int32	height;
	
} Texture;

typedef struct Entity {
    float   pos[2];
    float   vel[2];
    float   acc[2];
} Entity;

static void entity_apply(Entity* this, float fx, float fy) {
    this->acc[0] += fx;
    this->acc[1] += fy;
}

static void entity_update(Entity* this, float t) {
    this->vel[0] += this->acc[0] * t;
    this->vel[1] += this->acc[1] * t;

    this->pos[0] += this->vel[0] * t;
    this->pos[1] += this->vel[1] * t;
	
    this->acc[0] = 0.0f;
    this->acc[1] = 0.0f;
}


Entity ball;
unsigned short int picLoad = 0;
unsigned short int collidedWithGround = 0;
unsigned short int collidedWithRamp = 0;

Texture texture_load(const char* texture_path, int32 is_smooth){
	Texture texture     = {0};
	int32   channels    = 0;
	uint8*  pixels      = NULL;

	//if channels ==  4 -> RGBA else -> RGB
	
	pixels = stbi_load(texture_path, &texture.width, &texture.height, &channels, 0);
	
	assert(pixels);
	
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	
	if(picLoad == 1) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	else if(picLoad == 0)glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
	
	stbi_image_free(pixels);
	
	return texture;
	
}

void gravity(){
	
	float dt = glfwGetTime();
	glfwSetTime(0.0f);
	
	for(int i = 0; i < NUMBER_OF_BALLS; i++){
		
		entity_update(&ball, dt);
		entity_apply(&ball, 0.0f, -9.82f);
		
		if(collidedWithGround){
			
			ball.vel[1] = ball.vel[1] * 0.4f;
			entity_update(&ball, dt);
			entity_apply(&ball, 0.0f, 9.82f);
			
		}
		else if(collidedWithRamp && !collidedWithGround){
			
			entity_apply(&ball, 0.0f, 9.82f);
			
			//~31.30495
			float cosAngle = cos(28.0f/31.30495f);
			float sinAngle = sin(14.0f/31.30495f);
			
			//Fx = m * g * sin(theta)
			float forceX = 1.0f * 9.82f * sinAngle;
			float forceFriction = ball.vel[0] * 0.3f;
			forceX = forceX - forceFriction;
			
			float forceY = 1.0f * 9.82f * cosAngle;
			
			//entity_update(&ball, dt);
			entity_apply(&ball, forceX, forceY);
			//entity_update(&ball, dt);
			
		}
		collidedWithGround = 0;
		collidedWithRamp = 0;
		if(ball.pos[1] <= (LOWER_LIMIT + 1) ){
			
			ball.vel[1] = abs( ball.vel[1] );
			collidedWithGround = 1;
			
		}
		
	}
	
}

void checkCollisionRamp(){
	
	static unsigned short int xCollision = 0;
	
	float y[28 * 2];
	for(int i = 0; i < 28 * 2; i++) y[i] = -14.0f + (i * 0.25f);
	
	xCollision = 0;
	if(ball.pos[0] >= -27.0f && ball.pos[0] < 0.0f){
		
		xCollision = 1;
		
	}
	if(xCollision){
		
		float dy = abs( ball.pos[1] - y[ abs( (int)ball.pos[0] ) * 2 ] );
		
		if(dy <= 0.0f){
			
			collidedWithRamp = 1;
			
		}
		
	}
	
}
//start main method
//
int main(){
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "BallRamp", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	float camera = 8.0f;
	{
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120.0f, WIDTH/HEIGHT, 0.1f, 32.0f);
		gluLookAt(
		0,	0,	camera,
		0,	0,	0,
		0,	1,	0
		);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
	}
	
	glfwSetKeyCallback(window, key_callback);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glClearColor(0.1f, 0.1f, 0.1f, 0.5f);
	stbi_set_flip_vertically_on_load(1);
	
	
	ball.pos[0] = -27.0f;
	ball.pos[1] =	0.0f;
	
	glfwSetTime(0.0f);
	
	while(!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		//rendering
		//drawBackground();
		drawRamp();
		drawBall();
		//drawBackground();
		checkCollisionRamp();
		gravity();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}
//end main method
//
void drawBall(){
	
	unsigned short int CIRCLE = 300;
	
	glPushMatrix();
	glTranslatef(ball.pos[0], ball.pos[1], 0.0f);
	
	glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
	glBegin(GL_LINE_LOOP);
	
	for(int i = 0; i < CIRCLE; i++){
		
		float angle = 2.0f * 3.14f * (i / 300.0f);
		float x = cos(angle) * 1.0f;
		float y = sin(angle) * 1.0f;
		
		glVertex3f(x, y, 0.0f);
		
	}
	
	glEnd();
	glPopMatrix();
	
}

void drawRamp(){
	
	glPushMatrix();
	
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor4f(0.5f, 0.0f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	
	float x = LEFT_LIMIT;
	float y =		0.0f;
	
	glVertex3f(x, y, 0.0f);
	glVertex3f(x + 28.0f, y - 14.0f, 0.0f);
	glVertex3f(x + 28.0f, y - 14.0f, 0.0f);
	glVertex3f(x, y - 14.0f, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}
/*void drawRamp(){
	
	glPushMatrix();
	//glTranslatef(0.0f, 0.0f, 0.0f);
	//glBegin(GL_QUADS);
	
	picLoad = 1;
	
	Texture ramp;
	ramp.width = 1024;
	ramp.height = 1024;
	ramp.id = '2';
	ramp = texture_load("tilable_IMG_0044_lush.png",1);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	
	float x = LEFT_LIMIT;
	float y = 0.0f;
	
	glTexCoord2f(1, 1);
	glVertex3f(x, y, 0.0f);
	glTexCoord2f(0, 1);
	glVertex3f(x + 28.0f, y - 14.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x + 28.0f, y - 14.0f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(x, y - 14.0f, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}
*/
void drawBackground(){
	
	glPushMatrix();
	
	picLoad = 0;
	
	Texture background;
	background.width =  1920;
	background.height = 1080;
	background.id = 	'1';
	background = texture_load("background0.png", 1);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	float x = 28.0f;
	float y = 14.0f;
	
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	
	glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
	
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

static void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods){
	
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(w, GLFW_TRUE);
	
}