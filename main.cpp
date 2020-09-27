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
#include "bmp_read.h"

height_map hmap;
long int stime;
GLuint textures[10];

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
	
	float z_inertia;
	
	float x_cam_pos;
	float y_cam_pos;
	float z_cam_pos;
	
	float z_pressed;
	float forward_pressed;
	float back_pressed;
	float left_pressed;
	float right_pressed;
	float space_pressed;
	
	float radius;
	win_cond(){
		z_pressed = 0.0;    
		forward_pressed = 0.0;
		back_pressed = 0.0; 
		left_pressed = 0.0; 
		right_pressed = 0.0;
		space_pressed = 0.0;
	}
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
	//condition.x_press = x;
	//condition.y_press = y;
	
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
			condition.forward_pressed = 1.0;
			break;	
		case 251:
		case 115:
			condition.back_pressed = 1.0;
			break;
		case 244:
		case 97: //a
			condition.left_pressed = 1.0;	
			break;
		case 226:
		case 100:
			condition.right_pressed = 1.0;
			break;
		case 122:
			if(condition.z_pressed==0){
				condition.z_pressed = 1;
			}
			else{
				condition.z_pressed = 0;
			}
			break;
		case 27:
			exit(0);
			break;
		case 32:
			condition.space_pressed = 1;
		default:
			break;
	}
	
	
}

void contr_release_btns(unsigned char key, int x, int y){
	float dmx;
	float dmy;
	float length;
	//std::cout<<(int)key<<'\n';
	switch(key){
		case 246:
		case 119:
			condition.forward_pressed = 0.0;
			break;	
		case 251:
		case 115:
			condition.back_pressed = 0.0;
			break;
		case 244:
		case 97: //a
			condition.left_pressed = 0.0;
			break;
		case 226:
		case 100:
			condition.right_pressed = 0.0;
			break;
		case 32:
			condition.space_pressed = 0;
		default:
			break;
	}
	
	
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	glMatrixMode(GL_MODELVIEW);      
    
	glPushMatrix();
	glLoadIdentity(); 
		if(condition.z_pressed == 1){
			glEnable(GL_MULTISAMPLE_ARB);
		}
		else{
			glDisable(GL_MULTISAMPLE_ARB);
		}
		gluLookAt(
			condition.x_cam_pos, condition.y_cam_pos, condition.z_cam_pos,
			condition.x_cam_view, condition.y_cam_view, condition.z_cam_view,
			0, 0, 1 );
		glLineWidth(0.2);
		//water
		//glBindTexture(GL_TEXTURE_2D,textures[0]);
		//glBindTexture( GL_TEXTURE_2D, 0);
		glDepthFunc(GL_LESS);
		for(int i=0; i<=hmap.triangles.size(); i++){
			map_triangle a = hmap.triangles[i];	
			glBegin(GL_TRIANGLES);
				glColor3f(.65,.65,.45);
				if(1){//condition.z_pressed==0){
					glNormal3f(a.nx1,a.ny1,a.nz1);
					if(a.z1>1.2){
						glColor3f(.35,.65,.35);
					}
					else{
						if(a.z1<0.1){
							glColor3f(.45,.45,.25);
						}
						else{
							glColor3f(.65,.65,.45);
						}
					}
					glVertex3f(a.x1,a.y1,a.z1);
				
					glNormal3f(a.nx2,a.ny2,a.nz2);
					if(a.z2>1.2){
						glColor3f(.35,.65,.35);
					}
					else{
						if(a.z2<0.1){
							glColor3f(.45,.45,.25);
						}
						else{
							glColor3f(.65,.65,.45);
						}
					}
					glTexCoord2f(1.0,0.0);
					glVertex3f(a.x2,a.y2,a.z2);
				
					glNormal3f(a.nx3,a.ny3,a.nz3);
					if(a.z3>1.2){
						glColor3f(.35,.65,.35);
					}
					else{
						if(a.z3<0.1){
							glColor3f(.45,.45,.25);
						}
						else{
							glColor3f(.65,.65,.45);
						}
					}
					glVertex3f(a.x3,a.y3,a.z3);
				}
				else{
					glNormal3f(a.nx,a.ny,a.nz);
					if(a.z1>1.2){
						glColor3f(.35,.65,.35);
					}
					else{
						glColor3f(.65,.65,.45);
					}
					glVertex3f(a.x1,a.y1,a.z1);
					
					if(a.z2>1.2){
						glColor3f(.35,.65,.35);
					}
					else{
						glColor3f(.65,.65,.45);
					}
					glVertex3f(a.x2,a.y2,a.z2);
					
					if(a.z3>1.2){
						glColor3f(.35,.65,.35);
					}
					else{
						glColor3f(.65,.65,.45);
					}
					glVertex3f(a.x3,a.y3,a.z3);
				}
			glEnd();
		}
    	//GLfloat light0_direction[] = {25.0,25.0,35.0,1.0};
    	GLfloat light0_direction[] = {condition.x_cam_pos,condition.y_cam_pos,35.0,1.0};
    	glLightfv(GL_LIGHT1, GL_POSITION,light0_direction);
//    	glEnable(GL_BLEND);
//		glBegin(GL_POLYGON);
//			glColor4f(.15,.15,.85,0.45);
//			glNormal3f(0,0,1);
//			glVertex3f(0,0,0);
//			glVertex3f(setting.map_size_x,0,0);
//			glVertex3f(setting.map_size_x,setting.map_size_y,0);
//			glVertex3f(0,setting.map_size_y,0);
//		glEnd();
//		glDisable(GL_BLEND);
    glPopMatrix();
    
    glutSwapBuffers();  
}
void calculations(){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	ms = abs(ms);
	long int dtime = abs(stime-ms);
	if(dtime<5){
		return;
	}
	stime = ms;
	//determine location
	float fb_move = condition.forward_pressed-condition.back_pressed;
	float lr_move = condition.left_pressed-condition.right_pressed;
	float dmx;
	float dmy;
	float length;
	if(fb_move!=0){
		dmx = condition.x_cam_view - condition.x_cam_pos;
		dmy = condition.y_cam_view - condition.y_cam_pos;
			
		length = sqrt(dmx*dmx + dmy*dmy);
		dmx/= length*setting.move_multiplier;
		dmy/= length*setting.move_multiplier;
			
		condition.x_cam_view += (dmx*fb_move);
		condition.y_cam_view += (dmy*fb_move);
			
		condition.x_cam_pos += (dmx*fb_move);
		condition.y_cam_pos += (dmy*fb_move);
	}
	if(lr_move!=0){
		dmx = condition.radius*cos(setting.strafe_angle+condition.x_cam_ang);
		dmy = condition.radius*sin(setting.strafe_angle+condition.x_cam_ang);
			
		length = sqrt(dmx*dmx + dmy*dmy);
		dmx/= length*setting.move_multiplier;
		dmy/= length*setting.move_multiplier;
			
		condition.x_cam_view += (dmx*lr_move);
		condition.y_cam_view += (dmy*lr_move);
			
		condition.x_cam_pos += (dmx*lr_move);
		condition.y_cam_pos += (dmy*lr_move);
	}
	if(condition.space_pressed == 1.0 && condition.z_inertia == 0){
		condition.space_pressed = 0.0;
		condition.z_inertia = setting.jump_power;
	}
	
	//determine height
	float ground_z = hmap.get_point_height_walk(condition.x_cam_pos,condition.y_cam_pos)+setting.cam_height;
	float z_elev = 0;
	//condition.z_cam_pos = hmap.get_point_height_walk(condition.x_cam_pos,condition.y_cam_pos)+setting.cam_height;
	if(condition.z_inertia!=0){
		z_elev = condition.z_inertia*0.001;
		condition.z_cam_pos += z_elev;
		condition.z_inertia -= setting.jump_speed;
		if(condition.z_cam_pos<ground_z){
			condition.z_cam_pos = ground_z;
			condition.z_inertia = 0;
		}
	}
	else{
		condition.z_cam_pos = hmap.get_point_height_walk(condition.x_cam_pos,condition.y_cam_pos)+setting.cam_height;
	}
	//redraw
	display();	
}


void InitGL(){
	glutInitWindowSize( condition.s_width, condition.s_height );     
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
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
	//glutMouseWheelFunc(zooming);
	glutKeyboardFunc(contr_btns);
	glutKeyboardUpFunc(contr_release_btns);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS); 
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,1.0);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	
	textures[0] = LoadTexture("images/dirt.bmp");
	textures[1] = LoadTexture("images/grass.bmp");
	//glDepthRange(1000.0, 1.0);
	//glDepthFunc(GL_GEQUAL);
	glMatrixMode(GL_PROJECTION);  
	//float dwidth = ((float)condition.s_width/(float)condition.s_height)/1.0;
	//glOrtho(-condition.s_width,condition.s_width,-condition.s_height,condition.s_height,-40,40);
	gluPerspective (setting.fov, GLfloat(condition.s_width/condition.s_height), 0.001, 900);
	
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
    setting.read_settings();
	InitGL();
}
