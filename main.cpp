#define GLUT_DISABLE_ATEXIT_HACK
#define _WIN32_WINNT 0x0500
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h> 
#include "id_gen.h"
#include "random_gen.h"
#include "height_map.h"

height_map hmap;
struct win_cond{
	int s_width;
	int s_height;
	
	int w_height;
	int w_width;
		
	float x_cam_ang;
	float y_cam_ang;
	
	float x_cam_view;
	float y_cam_view;
	float z_cam_view;
	
	
	float x_cam_pos;
	float y_cam_pos;
	float z_cam_pos;
	
	float x_press;
	float y_press;
	float pressed;
	
	float x_trans;
	float y_trans;
	
	float hgt;
	
	float radius;
	
	float world_speed;
};
win_cond condition;

void resize(int width, int height) {
    condition.w_height = height;
    condition.w_width = width;

    glutReshapeWindow( condition.s_width, condition.s_height);
}

void contr_mouse(int button, int state, int x, int y){
	if(button==GLUT_RIGHT_BUTTON){
		return;
	}
	if(state==GLUT_DOWN){
		condition.x_press = x;
		condition.y_press = y;
		condition.pressed = 1;
	}
	else{
		condition.pressed = 0;
	}
}

void mouse_move(int x, int y){
	if(x == condition.s_width/2 && y == condition.s_height/2){
		return;
	}
	float dm_x = condition.s_width/2 - x;
	float dm_y = condition.s_height/2 - y;
	condition.x_press = x;
	condition.y_press = y;
	
	dm_x = dm_x/(float)condition.s_width * 90;
	condition.x_cam_ang += dm_x/6.5;
	//std::cout<<dm_x<<": "<<condition.x_cam_ang<<'\n';
	float dmx;
	float dmy;
	dmx = condition.x_cam_view - condition.x_cam_pos;
	dmy = condition.y_cam_view - condition.y_cam_pos;
	float rad = sqrt(dmx*dmx + dmy*dmy);
	//std::cout<<rad<<'M'<<'\n';
	
	condition.x_cam_view = condition.radius*cos(condition.x_cam_ang);
	condition.y_cam_view = condition.radius*sin(condition.x_cam_ang);
	
	condition.x_cam_view += condition.x_cam_pos;
	condition.y_cam_view += condition.y_cam_pos;
	
	//height
	dm_y = dm_y/(float)condition.s_height * 90;
	condition.y_cam_ang += dm_y*3.5;
	condition.z_cam_view = condition.y_cam_ang;
	
	glutWarpPointer(condition.s_width/2,condition.s_height/2);
}

void contr_btns(int key, int x, int y){
	float dmx;
	float dmy;
	switch(key){
		case GLUT_KEY_F1:
			condition.world_speed += 0.00000005;
			break;
		case GLUT_KEY_F2:
			condition.world_speed -= 0.00000005;
			if(condition.world_speed < 0){
				condition.world_speed = 0;
			}
			break;	
		case GLUT_KEY_UP:
			dmx = condition.x_cam_view - condition.x_cam_pos;
			dmy = condition.y_cam_view - condition.y_cam_pos;
			
			dmx*=0.005;
			dmy*=0.005;
			
			//std::cout<<dmx<<' '<<dmy<<'\n';
			
			condition.x_cam_view += dmx;
			condition.y_cam_view += dmy;
			
			condition.x_cam_pos += dmx;
			condition.y_cam_pos += dmy;
			break;	
		case GLUT_KEY_DOWN:
			dmx = condition.x_cam_view - condition.x_cam_pos;
			dmy = condition.y_cam_view - condition.y_cam_pos;
			
			dmx*=0.005;
			dmy*=0.005;
			
			condition.x_cam_view -= dmx;
			condition.y_cam_view -= dmy;
			
			condition.x_cam_pos -= dmx;
			condition.y_cam_pos -= dmy;
			break;	
		default:
			break;
	}
	
	
}

void zooming(int wheel, int direction, int x, int y){
	float fraction = 0.1;
	switch(direction){
		case 1:
			condition.hgt+=(0.03+condition.hgt*fraction);
			break;
		case -1:
			condition.hgt-=(0.03+condition.hgt*fraction);
			if(condition.hgt<=0){
				condition.hgt = 0.03;
			}
			break;
			
		default:
			break;
	}
}


void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	glMatrixMode(GL_MODELVIEW);      
	condition.z_cam_pos = hmap.get_point_height_walk(condition.x_cam_pos,condition.y_cam_pos)+2.7;
	//glRotatef(condition.x_cam_ang,condition.x_cam_pos,condition.y_cam_pos,1);
    
	glPushMatrix();
	glLoadIdentity(); 
	//glScalef(8,8,8);
	//	glTranslatef(condition.x_trans,condition.y_trans,0);
		glScalef(condition.hgt,condition.hgt,condition.hgt);
		
		gluLookAt(
			condition.x_cam_pos,
			condition.y_cam_pos,
			condition.z_cam_pos,
			condition.x_cam_view,
			condition.y_cam_view,
			condition.z_cam_view,
			0,
			0,
			1 );
		
		glColor4f(.15,.15,.15,.25);
		glLineWidth(0.2);
		for(int i=0; i<=hmap.triangles.size(); i++){
			map_triangle a = hmap.triangles[i];	
			glBegin(GL_TRIANGLES);
				glNormal3f(a.nx1,a.ny1,a.nz1);
				glVertex3f(a.x1,a.y1,a.z1);
				
				glNormal3f(a.nx2,a.ny2,a.nz2);
				glVertex3f(a.x2,a.y2,a.z2);
				
				glNormal3f(a.nx3,a.ny3,a.nz3);
				glVertex3f(a.x3,a.y3,a.z3);
			glEnd();
		}
    GLfloat light0_direction[] = {25.0,25.0,35.0,1.0};
	
    glLightfv(GL_LIGHT1, GL_POSITION,light0_direction);
    glPopMatrix();
    
    glutSwapBuffers(); 
    //glDisable(GL_LIGHT0);
    
}

void InitGL(){
	glutInitWindowSize( condition.s_width, condition.s_height );     
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Xyeta"); 
	
	//maximize window
	HWND win_handle = FindWindow(0, "Xyeta");
  	ShowWindowAsync(win_handle, SW_SHOWMAXIMIZED);
  	
  	
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(resize);
	glutSpecialFunc(contr_btns);
	glutMouseFunc(contr_mouse);
	glutPassiveMotionFunc(mouse_move);
	glutMouseWheelFunc(zooming);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,1.0);
	glEnable(GL_CULL_FACE);
	
	//textures[0] = LoadTexture("models/Sun/p.bmp");
	//textures[1] = LoadTexture("models/Mercury/p.bmp");
	//textures[2] = LoadTexture("models/Venus/p.bmp");
	//textures[3] = LoadTexture("models/Earth/p.bmp");
	//textures[4] = LoadTexture("models/Mars/p.bmp");
	//textures[5] = LoadTexture("models/Jupiter/p.bmp");
	//textures[6] = LoadTexture("models/Saturn/p.bmp");
	//textures[7] = LoadTexture("models/Uranus/p.bmp");
	//textures[8] = LoadTexture("models/Neptune/p.bmp");
	//glDepthRange(1000.0, 1.0);
	//glDepthFunc(GL_GEQUAL);
	glMatrixMode(GL_PROJECTION);  
	//float dwidth = ((float)condition.s_width/(float)condition.s_height)/1.0;
	//glOrtho(-condition.s_width,condition.s_width,-condition.s_height,condition.s_height,-40,40);
	gluPerspective (90, GLfloat(condition.s_width/condition.s_height), 0.001, 400);
	
	//a.lWave("ambient.wav");
	//a.play();
	glEnable(GL_LIGHT1);
	GLfloat light0_diffuse[] = {0.6, 0.6, 0.2};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light0_diffuse);
	
	glutMainLoop(); 
}


int main(int argc, char** argv) {
	condition.x_cam_pos = 5;
	condition.y_cam_pos = 5;
	condition.z_cam_pos = 5;
	condition.hgt = 1;
	condition.x_cam_view = 55;
	condition.y_cam_view = 5;
	condition.radius = 50;
	
	hmap.generate_map(123,1.15);
	glutInit(&argc,argv);
	int size = glutGet(GLUT_SCREEN_WIDTH);
	condition.s_width = size;
	size = glutGet(GLUT_SCREEN_HEIGHT);
	condition.s_height = size;
	
	condition.w_height = condition.s_height;
    condition.w_width = condition.s_width;
    condition.x_press = condition.s_width/2;
    condition.y_press = condition.s_height/2;
	InitGL();
}
