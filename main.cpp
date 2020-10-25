#define GLUT_DISABLE_ATEXIT_HACK
#define _WIN32_WINNT 0x0500
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sys/time.h>
#include <string>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h> 
#include "obj_reader.h"
#include "read_settings.h"
#include "id_gen.h"
#include "random_gen.h"
#include "height_map.h"
#include "bmp_read.h"

height_map hmap;
long int stime;
int rotat;
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

void draw_model(vec3 pos, vec3 scale, vec3 rot_angs, int model, int texture){
	glBindTexture(GL_TEXTURE_2D,textures[texture]);
	glTranslatef(pos.x,pos.y,pos.z);
	glScalef(scale.x,scale.y,scale.z);
	glRotatef(rot_angs.x,1,0,0);
	glRotatef(rot_angs.y,0,1,0);
	glRotatef(rot_angs.z,0,0,1);
	
	
	
	if(bodies[model].meshes3.size()!=0){
		for(int i=0; i<bodies[model].meshes3.size();i++){
			mesh3 a = bodies[model].meshes3[i];
			glBegin(GL_TRIANGLES);
				glTexCoord2f(bodies[model].textures[a.t1].x,bodies[model].textures[a.t1].y);
				glNormal3f(bodies[model].normals[a.n1].x,bodies[model].normals[a.n1].y,bodies[model].normals[a.n1].z);
				glVertex3f(bodies[model].verticles[a.v1].x,bodies[model].verticles[a.v1].y,bodies[model].verticles[a.v1].z);
				
				glTexCoord2f(bodies[model].textures[a.t2].x,bodies[model].textures[a.t2].y);
				glNormal3f(bodies[model].normals[a.n2].x,bodies[model].normals[a.n2].y,bodies[model].normals[a.n2].z);
				glVertex3f(bodies[model].verticles[a.v2].x,bodies[model].verticles[a.v2].y,bodies[model].verticles[a.v2].z);
				
				glTexCoord2f(bodies[model].textures[a.t3].x,bodies[model].textures[a.t3].y);
				glNormal3f(bodies[model].normals[a.n3].x,bodies[model].normals[a.n3].y,bodies[model].normals[a.n3].z);
				glVertex3f(bodies[model].verticles[a.v3].x,bodies[model].verticles[a.v3].y,bodies[model].verticles[a.v3].z);
			glEnd();
		}
	}
	if(bodies[model].meshes4.size()!=0){
		for(int i=0; i<bodies[model].meshes4.size();i++){
			mesh4 a = bodies[model].meshes4[i];
			glBegin(GL_QUADS);
				glNormal3f(bodies[model].normals[a.n1].x,bodies[model].normals[a.n1].y,bodies[model].normals[a.n1].z);
				glTexCoord2f(bodies[model].textures[a.t1].x,bodies[model].textures[a.t1].y);
				glVertex3f(bodies[model].verticles[a.v1].x,bodies[model].verticles[a.v1].y,bodies[model].verticles[a.v1].z);
				
				glNormal3f(bodies[model].normals[a.n2].x,bodies[model].normals[a.n2].y,bodies[model].normals[a.n2].z);
				glTexCoord2f(bodies[model].textures[a.t2].x,bodies[model].textures[a.t2].y);
				glVertex3f(bodies[model].verticles[a.v2].x,bodies[model].verticles[a.v2].y,bodies[model].verticles[a.v2].z);
				
				glNormal3f(bodies[model].normals[a.n3].x,bodies[model].normals[a.n3].y,bodies[model].normals[a.n3].z);
				glTexCoord2f(bodies[model].textures[a.t3].x,bodies[model].textures[a.t3].y);
				glVertex3f(bodies[model].verticles[a.v3].x,bodies[model].verticles[a.v3].y,bodies[model].verticles[a.v3].z);
				
				glNormal3f(bodies[model].normals[a.n4].x,bodies[model].normals[a.n4].y,bodies[model].normals[a.n4].z);
				glTexCoord2f(bodies[model].textures[a.t4].x,bodies[model].textures[a.t4].y);
				glVertex3f(bodies[model].verticles[a.v4].x,bodies[model].verticles[a.v4].y,bodies[model].verticles[a.v4].z);
			glEnd();
		}
	}
	
	
	glRotatef(-rot_angs.z,0,0,1);
	glRotatef(-rot_angs.y,0,1,0);
	glRotatef(-rot_angs.x,1,0,0);
	glScalef(1.0/scale.x,1.0/scale.y,1.0/scale.z);
	glTranslatef(-pos.x,-pos.y,-pos.z);
	glBindTexture(GL_TEXTURE_2D,0);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	glMatrixMode(GL_MODELVIEW);      
    
	glPushMatrix();
	glLoadIdentity(); 
		gluLookAt(
			condition.x_cam_pos, condition.y_cam_pos, condition.z_cam_pos,
			condition.x_cam_view, condition.y_cam_view, condition.z_cam_view,
			0, 0, 1 );
		
		//light
		GLfloat light1_direction[] = {condition.x_cam_pos,condition.y_cam_pos,condition.z_cam_pos+2,1.0};
		const GLfloat light1_att = 0.1;
    	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, light1_att);
    	glLightfv(GL_LIGHT1, GL_POSITION,light1_direction);
    	glEnable(GL_LIGHT1);
    	
		glBindTexture(GL_TEXTURE_2D,textures[0]);
		//glBindTexture( GL_TEXTURE_2D, 0);
		glDepthFunc(GL_LESS);
		triangles_pack map_draw = hmap.get_to_draw(condition.x_cam_pos,condition.y_cam_pos);
		for(int i=0; i<map_draw.data.size(); i++){
			map_triangle a = map_draw.data[i];	
			glBegin(GL_TRIANGLES);
				glNormal3f(a.nx1,a.ny1,a.nz1);
				glTexCoord2f(a.tx1,a.ty1);
				glVertex3f(a.x1,a.y1,a.z1);
			
				glNormal3f(a.nx2,a.ny2,a.nz2);
				glTexCoord2f(a.tx2,a.ty2);
				glVertex3f(a.x2,a.y2,a.z2);
			
				glNormal3f(a.nx3,a.ny3,a.nz3);
				glTexCoord2f(a.tx3,a.ty3);
				glVertex3f(a.x3,a.y3,a.z3);
			glEnd();
		}
		glBindTexture(GL_TEXTURE_2D,0);
    	//GLfloat light0_direction[] = {25.0,25.0,35.0,1.0};
    	//glLightfv(GL_LIGHT2, GL_POSITION,light2_direction);
		glColor3f(1,0,0);
		glBindTexture(GL_TEXTURE_2D,textures[4]);
		glBegin(GL_POLYGON);
			glTexCoord2f(0,0);
			glVertex3f(-400,-400,-150);
			glTexCoord2f(0,1);
			glVertex3f(-400,-400, 250);
			glTexCoord2f(1,1);
			glVertex3f(400,-400,  250);
			glTexCoord2f(1,0);
			glVertex3f(400,-400, -150);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D,textures[5]);
		glBegin(GL_POLYGON);
			glTexCoord2f(0,0);
			glVertex3f(400,-400,-150);
			glTexCoord2f(0,1);        
			glVertex3f(400,-400, 250);
			glTexCoord2f(1,1);        
			glVertex3f(400,400,  250);
			glTexCoord2f(1,0);        
			glVertex3f(400,400, -150);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D,textures[4]);
		glBegin(GL_POLYGON);
			glTexCoord2f(0,0);
			glVertex3f(400,400, -150);
			glTexCoord2f(0,1);        
			glVertex3f(400,400,  250);
			glTexCoord2f(1,1);        
			glVertex3f(-400,400, 250);
			glTexCoord2f(1,0);        
			glVertex3f(-400,400,-150);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D,textures[5]);
		glBegin(GL_POLYGON);
			glTexCoord2f(0,0);
			glVertex3f(-400,400, -150);
			glTexCoord2f(0,1);         
			glVertex3f(-400,400,  250);
			glTexCoord2f(1,1);         
			glVertex3f(-400,-400, 250);
			glTexCoord2f(1,0);         
			glVertex3f(-400,-400,-150);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D,textures[6]);
		glBegin(GL_POLYGON);
			glTexCoord2f(0,0);
			glVertex3f(-400,-400, 250);
			glTexCoord2f(0,1);         
			glVertex3f(400,-400,  250);
			glTexCoord2f(1,1);         
			glVertex3f(400,400, 250);
			glTexCoord2f(1,0);         
			glVertex3f(-400,400, 250);
		glEnd();
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_LIGHT1);
		
		
		//with opacity
		glDepthMask(GL_FALSE);
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
		const GLfloat light2_att = 7;
    	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, light2_att);
    	glLightfv(GL_LIGHT2, GL_POSITION,light1_direction);
    	glDisable(GL_LIGHT1);
    	glEnable(GL_LIGHT2);
    	grasses_pack grassesd = hmap.get_grass_to_draw(condition.x_cam_pos,condition.y_cam_pos);
		for(int i=0; i<grassesd.data.size(); i++){
			grass_object grass_o = grassesd.data[i];
			draw_model(grass_o.translate, grass_o.scale, grass_o.rotate_angs, grass_o.model, grass_o.texture);
			
		}
		glDisable(GL_LIGHT2);
		glEnable(GL_LIGHT1);
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glDepthMask(GL_TRUE);
    glPopMatrix();
    
    glutSwapBuffers();  
}
void calculations(){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	ms = abs(ms);
	long int dtime = abs(stime-ms);
	if(dtime<setting.frame_time){
		return;
	}
	stime = ms;
	
	rotat+=1;
	
	float speed_mul = float(dtime)/setting.frame_time;
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
		dmx/= length*setting.move_multiplier*speed_mul;
		dmy/= length*setting.move_multiplier*speed_mul;
			
		condition.x_cam_view += (dmx*fb_move);
		condition.y_cam_view += (dmy*fb_move);
			
		condition.x_cam_pos += (dmx*fb_move);
		condition.y_cam_pos += (dmy*fb_move);
	}
	if(lr_move!=0){
		dmx = condition.radius*cos(setting.strafe_angle+condition.x_cam_ang);
		dmy = condition.radius*sin(setting.strafe_angle+condition.x_cam_ang);
			
		length = sqrt(dmx*dmx + dmy*dmy);
		dmx/= length*setting.move_multiplier*speed_mul;
		dmy/= length*setting.move_multiplier*speed_mul;
			
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
		condition.z_inertia -= setting.jump_speed*speed_mul;
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
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutCreateWindow("3ddd"); 
	
	//maximize window
	HWND win_handle = FindWindow(0, "3ddd");
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
	
	glClearColor(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_MULTISAMPLE);
	
	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); 
	//glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	//glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	//glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,1.0);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_ALPHA_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH); 
	
	textures[0] = LoadTexture("images/ground/main2.bmp");
	textures[1] = LoadTexture("images/grass.bmp");
	textures[2] = LoadTexture("images/backg/1.bmp");
	textures[3] = LoadTexture("images/backg/2.bmp");
	textures[4] = LoadTexture("images/backg/3.bmp");
	textures[5] = LoadTexture("images/backg/4.bmp");
	textures[6] = LoadTexture("images/backg/sky.bmp");
	textures[7] = LoadAlphaTexture("models/DownTrees/diffuse.img2");
	//glDepthRange(1000.0, 1.0);
	//glDepthFunc(GL_GEQUAL);
	glMatrixMode(GL_PROJECTION);  
	//float dwidth = ((float)condition.s_width/(float)condition.s_height)/1.0;
	//glOrtho(-condition.s_width,condition.s_width,-condition.s_height,condition.s_height,-40,40);
	gluPerspective (setting.fov, GLfloat(condition.s_width/condition.s_height), 0.001, 900);
	
	//a.lWave("ambient.wav");
	//a.play();
	
	glEnable(GL_LIGHT1);
	GLfloat light0_diffuse[] = {1, 1, 1, 1.0};
	GLfloat light0_ambient[] = {0.3, 0.3, 0.3, 1.0};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light0_diffuse);
    glDisable(GL_LIGHT1);
    
	glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light0_diffuse);
    glDisable(GL_LIGHT2);
	
	glutMainLoop(); 
}


int main(int argc, char** argv) {
	for(int i = 0; i < 12; i++){
		std::string name;
		if(i>=10){
			name = "models/DownTrees/"+std::to_string(i+1)+"_s.obj";
		}
		else{
			name = "models/DownTrees/0"+std::to_string(i+1)+"_s.obj";
		}
		loader(name.c_str(),i);
		std::cout<<name.c_str()<<'\n';
	}
	//loader("models/DownTrees/14.obj",0);
	//loader("models/DownTrees/05_t.obj",1);
	condition.x_cam_pos = 25;
	condition.y_cam_pos = 25;
	condition.z_cam_pos = 5;
	condition.x_cam_view = 75;
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
