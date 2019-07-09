//gcc main.c -std=c99 -O3 -s -lglfw3 -lopengl32 -lglu32 -lgdi32
//
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>

//function headers
static void key_callback(GLFWwindow*, int, int, int, int);
void drawBackground(void);
void drawPlayer(void);
void gravity(void);
void keyBoardListener(GLFWwindow*);
void drawObjects(void);
void collisionDetection(void);
//end of headers

#define LOWER_LIMIT -14.0f
#define UPPER_LIMIT  14.0f
#define RIGHT_LIMIT  19.0f
#define  LEFT_LIMIT -19.0f

#define NUMBER_OF_OBJECTS 3

struct object{
	
	float x;
	float y;
	
};

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

Entity player;
struct object objects[NUMBER_OF_OBJECTS];
unsigned short int collided = 0;

void collisionDetection(){
	
	int i;
	for(i = 0; i < NUMBER_OF_OBJECTS; i++){
		
		float dx = abs( player.pos[0] - objects[i].x );
		
		if(dx <= 0.0f){
			
			float dy = abs( player.pos[1] - objects[i].y );
			
			if(dy < 2.0f){
				
				//player collided with thing
				player.vel[1] = abs( player.vel[1] );
				collided = 1;
				
			}
			
		}
		
	}
	
}

void drawObjects(){
	
	float x = 1.0f;
	float y = LOWER_LIMIT + 15.0f;
	float inc = 1.0f;
	
	int i;
	for(i = 0; i < NUMBER_OF_OBJECTS; i++){
		
		glPushMatrix();
		glTranslatef(objects[i].x, objects[i].y, 0.0f);
		glColor4f(0.5f, 0.0f, 0.5f, 0.7f);
		glBegin(GL_QUADS);
		
		glVertex3f(x, y + inc, 0.0f);
		glVertex3f(x - inc, y + inc, 0.0f);
		
		glVertex3f(x - inc, y - inc, 0.0f);
		glVertex3f(x, y - inc, 0.0f);
		
		glEnd();
		glPopMatrix();
		
	}
	
}






int main(){
	
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1024, 768, "TwoDScroller", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);
	
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
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.01f, 0.0f, 0.01f, 0.5f);		//background color
	
	player.pos[0] = 0.0f;
	player.pos[1] = 0.0f;
	
	for(int i = 0; i < NUMBER_OF_OBJECTS; i++){
		
		objects[i].x = LEFT_LIMIT + i * 5.0f + 5.0f;
		objects[i].y = LOWER_LIMIT + 2.0f;
		
	}
	
	glfwSetTime(0.0f);
	
	while(!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		
		//rendering
		drawBackground();
		drawObjects();
		drawPlayer();
		gravity();
		keyBoardListener(window);
		collisionDetection();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
	
}

void keyBoardListener(GLFWwindow* w){
	
	if (glfwGetKey(w, GLFW_KEY_UP)    == GLFW_PRESS && collided) player.vel[1] += 75.0f;
	if (glfwGetKey(w, GLFW_KEY_LEFT)  == GLFW_PRESS) player.pos[0] -= 0.25f;
	if (glfwGetKey(w, GLFW_KEY_RIGHT) == GLFW_PRESS) player.pos[0] += 0.25f;
	
}

void gravity(){
	
	float g = -(9.82f * 2.0f);
	float dt = glfwGetTime();
	glfwSetTime(0.0f);
	
	entity_update(&player, dt);
	entity_apply(&player, 0.0f, g);
	
	if(collided){
		
		player.vel[1] = player.vel[1] * 0.1f;
		entity_update(&player, dt);
		entity_apply(&player, 0.0f, -g);
		
	}
	collided = 0;
	if(player.pos[1] <= LOWER_LIMIT + 2.0f){
		
		player.vel[1] = abs( player.vel[1] );
		collided = 1;
		
	}
	
}

void drawPlayer(void){
	
	glPushMatrix();
	glTranslatef(player.pos[0], player.pos[1], 0.0f);
	glColor4f(0.5f, 0.5f, 0.5f, 0.7f);
	glBegin(GL_QUADS);
	
	float x = 1.0f;
	float y = 1.0f;
	float inc = 1.0f;
	
	glVertex3f(x, y, 0.0f);
	glVertex3f(x + inc, y, 0.0f);
	glVertex3f(x + inc, y - inc, 0.0f);
	glVertex3f(x, y - inc, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}

void drawBackground(){
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor4f(0.5f, 0.0f, 0.5f, 0.7f);
	glBegin(GL_QUADS);
	
	float x = LEFT_LIMIT;
	float y =LOWER_LIMIT + 2.0f;
	
	glVertex3f(x, y, 0.0f);
	glVertex3f(x + RIGHT_LIMIT*2.0f, y, 0.0f);
	glVertex3f(x + RIGHT_LIMIT*2.0f, y - 5.0f, 0.0f);
	glVertex3f(x, y - 5.0f, 0.0f);
	
	glEnd();
	glPopMatrix();
	
}

static void key_callback(GLFWwindow* w, int key, int action, int scancode, int mods){
	
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(w, GLFW_TRUE);
	
}