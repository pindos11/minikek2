#define GLUT_DISABLE_ATEXIT_HACK
#define _WIN32_WINNT 0x0500
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <string>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h> 
#include "read_settings.h"
#include "id_gen.h"
#include "random_gen.h"
#include "height_map.h"

height_map hmap;
long int stime;


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
}

void mouse_move(int x, int y){
	if(x == condition.s_width/2 && y == condition.s_height/2){
		return;
	}
	float dm_x = condition.s_width/2 - x;
	float dm_y = condition.s_height/2 - y;
	condition.x_press = x;
	condition.y_press = y;
	
	dm_x = dm_x/(float)condition.s_width * setting.fov;
	condition.x_cam_ang += dm_x/setting.x_screen_div;
	//std::cout<<dm_x<<": "<<condition.x_cam_ang<<'\n';
	//std::cout<<rad<<'M'<<'\n';
	
	condition.x_cam_view = condition.radius*cos(condition.x_cam_ang);
	condition.y_cam_view = condition.radius*sin(condition.x_cam_ang);
	
	condition.x_cam_view += condition.x_cam_pos;
	condition.y_cam_view += condition.y_cam_pos;
	
	//height
	dm_y = dm_y/(float)condition.s_height * setting.fov;
	condition.y_cam_ang += dm_y*setting.y_screen_mul;
	if(condition.y_cam_ang > setting.fov){
		condition.y_cam_ang = setting.fov;
	}
	if(condition.y_cam_ang < -setting.fov){
		condition.y_cam_ang = -setting.fov;
	}
	
	condition.z_cam_view = condition.y_cam_ang;
	
	glutWarpPointer(condition.s_width/2,condition.s_height/2);
}

void contr_btns(unsigned char key, int x, int y){
	float dmx;
	float dmy;
	float length;
	//std::cout<<(int)key<<'\n';
	switch(key){
		case 246:
		case 119:
			dmx = condition.x_cam_view - condition.x_cam_pos;
			dmy = condition.y_cam_view - condition.y_cam_pos;
			
			length = sqrt(dmx*dmx + dmy*dmy);
			dmx/= length*setting.move_multiplier;
			dmy/= length*setting.move_multiplier;
			
			//std::cout<<dmx<<' '<<dmy<<'\n';
			
			condition.x_cam_view += dmx;
			condition.y_cam_view += dmy;
			
			condition.x_cam_pos += dmx;
			condition.y_cam_pos += dmy;
			break;	
		case 251:
		case 115:
			dmx = condition.x_cam_view - condition.x_cam_pos;
			dmy = condition.y_cam_view - condition.y_cam_pos;
			
			length = sqrt(dmx*dmx + dmy*dmy);
			dmx/= length*setting.move_multiplier;
			dmy/= length*setting.move_multiplier;
			
			condition.x_cam_view -= dmx;
			condition.y_cam_view -= dmy;
			
			condition.x_cam_pos -= dmx;
			condition.y_cam_pos -= dmy;
			break;
		case 244:
		case 97: //a	
			dmx = condition.radius*cos(setting.strafe_angle+condition.x_cam_ang);
			dmy = condition.radius*sin(setting.strafe_angle+condition.x_cam_ang);
			
			length = sqrt(dmx*dmx + dmy*dmy);
			dmx/= length*setting.move_multiplier;
			dmy/= length*setting.move_multiplier;
			
			condition.x_cam_view += dmx;
			condition.y_cam_view += dmy;
			
			condition.x_cam_pos += dmx;
			condition.y_cam_pos += dmy;
			break;
		case 226:
		case 100:
			dmx = condition.radius*cos(setting.strafe_angle+condition.x_cam_ang);
			dmy = condition.radius*sin(setting.strafe_angle+condition.x_cam_ang);
			
			length = sqrt(dmx*dmx + dmy*dmy);
			dmx/= length*setting.move_multiplier;
			dmy/= length*setting.move_multiplier;
			
			condition.x_cam_view -= dmx;
			condition.y_cam_view -= dmy;
			
			condition.x_cam_pos -= dmx;
			condition.y_cam_pos -= dmy;
			break;
		case 122:
			if(condition.pressed==0){
				condition.pressed = 1;
			}
			else{
				condition.pressed = 0;
			}
			break;
		case 27:
			exit(0);
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
	//glRotatef(condition.x_cam_ang,condition.x_cam_pos,condition.y_cam_pos,1);
    
	glPushMatrix();
	glLoadIdentity(); 
	//glScalef(8,8,8);
	//	glTranslatef(condition.x_trans,condition.y_trans,0);
		glScalef(condition.hgt,condition.hgt,condition.hgt);
		gluLookAt(
			condition.x_cam_pos, condition.y_cam_pos, condition.z_cam_pos,
			condition.x_cam_view, condition.y_cam_view, condition.z_cam_view,
			0, 0, 1 );
		glLineWidth(0.2);
		//water
		
		for(int i=0; i<=hmap.triangles.size(); i++){
			map_triangle a = hmap.triangles[i];	
			glBegin(GL_TRIANGLES);
				glColor3f(.65,.65,.45);
				if(condition.pressed==0){
					glNormal3f(a.nx1,a.ny1,a.nz1);
					if(a.z1>1.2){
						glColor3f(.35,.65,.35);
					}
					else{
						glColor3f(.65,.65,.45);
					}
					glVertex3f(a.x1,a.y1,a.z1);
				
					glNormal3f(a.nx2,a.ny2,a.nz2);
					if(a.z2>1.2){
						glColor3f(.35,.65,.35);
					}
					else{
						glColor3f(.65,.65,.45);
					}
					glVertex3f(a.x2,a.y2,a.z2);
				
					glNormal3f(a.nx3,a.ny3,a.nz3);
					if(a.z3>1.2){
						glColor3f(.35,.65,.35);
					}
					else{
						glColor3f(.65,.65,.45);
					}
					glVertex3f(a.x3,a.y3,a.z3);
				}
				else{
					glNormal3f(a.nx,a.ny,a.nz);
					glVertex3f(a.x1,a.y1,a.z1);

					glVertex3f(a.x2,a.y2,a.z2);

					glVertex3f(a.x3,a.y3,a.z3);
				}
			glEnd();
		}
		glEnable(GL_BLEND);
		glBegin(GL_POLYGON);
			glColor4f(.15,.15,.85,0.45);
			glNormal3f(0,0,1);
			glVertex3f(0,0,0);
			glVertex3f(setting.map_size_x,0,0);
			glVertex3f(setting.map_size_x,setting.map_size_y,0);
			glVertex3f(0,setting.map_size_y,0);
		glEnd();
		glDisable(GL_BLEND);
    	GLfloat light0_direction[] = {25.0,25.0,35.0,1.0};
    	glLightfv(GL_LIGHT1, GL_POSITION,light0_direction);
    glPopMatrix();
    
    glutSwapBuffers();  
}
void calculations(){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	ms = abs(ms);
	long int dtime = abs(stime-ms);
	if(dtime>=100){
		condition.z_cam_pos = hmap.get_point_height_walk(condition.x_cam_pos,condition.y_cam_pos)+setting.cam_height;
		display();
	}	
}

void InitGL(){
	glutInitWindowSize( condition.s_width, condition.s_height );     
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Xyeta"); 
	
	//maximize window
	HWND win_handle = FindWindow(0, "Xyeta");
  	ShowWindowAsync(win_handle, SW_SHOWMAXIMIZED);
  	
  	
	glutDisplayFunc(display);
	glutIdleFunc(calculations);
	glutReshapeFunc(resize);
	//glutSpecialFunc(contr_btns);
	glutMouseFunc(contr_mouse);
	glutPassiveMotionFunc(mouse_move);
	glutMouseWheelFunc(zooming);
	glutKeyboardFunc(contr_btns);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,1.0);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	
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
	gluPerspective (setting.fov, GLfloat(condition.s_width/condition.s_height), 0.001, 400);
	
	//a.lWave("ambient.wav");
	//a.play();
	glEnable(GL_LIGHT1);
	GLfloat light0_diffuse[] = {0.9, 0.9, 0.9};
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
    setting.read_settings();
	InitGL();
}
