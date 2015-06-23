//
//  GL.cpp
//  GC exercicio2
//
//  Created by Ivan Schuaste Cavalheiro on 14/04/15.
//  Copyright (c) 2015 Ivan Schuaste Cavalheiro. All rights reserved.
//

#include "GL.h"

float GL::deltaTime;
Camera GL::mainCamera;
vector<void (*)(unsigned char p_key, int p_x, int p_y)> GL::_keyboardCallbacks;
vector<void (*)(int p_button, int p_state, int p_x, int p_y)> GL::_mouseCallbacks;
vector<void (*)(void)> GL::_drawCallbacks;

Point Point::Create(float p_x, float p_y, float p_z)
{
    Point __toReturn;
    __toReturn.x = p_x;
    __toReturn.y = p_y;
    __toReturn.z = p_z;
    
    return __toReturn;
}

void Camera::Update(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle,aspect,0.1,500);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(position.x,position.y,position.z, 0,0,0, 0,1,0);
}


void GL::SetKeyboardCallback(void (*p_callback)(unsigned char, int, int))
{
    GL::_keyboardCallbacks.push_back(p_callback);
}


void GL::SetMouseCallback(void (*p_callback)(int, int, int, int))
{
    GL::_mouseCallbacks.push_back(p_callback);
}


void GL::SetDrawCallback(void (*p_callback)())
{
    GL::_drawCallbacks.push_back(p_callback);
}

void GL::KeyboardCallback(unsigned char p_key, int p_x, int p_y)
{
    for(int i = 0; i < GL::_keyboardCallbacks.size(); i++)
        GL::_keyboardCallbacks[i](p_key, p_x, p_y);
}

void GL::MouseCallback(int p_button, int p_state, int p_x, int p_y)
{
    for(int i = 0; i < GL::_mouseCallbacks.size(); i++)
        GL::_mouseCallbacks[i](p_button, p_state, p_x, p_y);
}

void GL::DrawCallback(void)
{
    GL::Update();
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for(int i = 0; i < GL::_drawCallbacks.size(); i++)
        GL::_drawCallbacks[i]();
    
    if(SHOW_AXIS)
    {
        glPushMatrix();
        glBegin(GL_LINE_LOOP);
        glColor3f(1.0f,0,0);
        glVertex3d(0, 0, 0);
        glVertex3d(1, 0, 0);
        glEnd();
    
        glBegin(GL_LINE_LOOP);
        glColor3f(0,0,1);
        glVertex3d(0, 0, 0);
        glVertex3d(0, 1, 0);
        glEnd();
    
        glBegin(GL_LINE_LOOP);
        glColor3f(0,1,0);
        glVertex3d(0, 0, 0);
        glVertex3d(0, 0, 1);
        glEnd();
        glPopMatrix();
    }
    
    glutSwapBuffers();
    glutPostRedisplay();
    
}

float __timeInLastFrame;
void GL::Update(void)
{
    float __timePassedSinceStart = glutGet(GLUT_ELAPSED_TIME);
    GL::deltaTime = (__timePassedSinceStart - __timeInLastFrame)/1000;
    __timeInLastFrame = __timePassedSinceStart;
    
    GL::mainCamera.Update();
}

void GL::Init(int *p_argc, char **p_argv)
{
    glutInit(p_argc, p_argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);//
	glShadeModel(GL_FLAT);//
	glDisable(GL_CULL_FACE);//
    glutInitWindowSize(800,600);
    glutCreateWindow(OPENGL_WINDOW_NAME);
    glutDisplayFunc(GL::DrawCallback);
    glutMouseFunc(GL::MouseCallback);
    glutKeyboardFunc(GL::KeyboardCallback);
    glutReshapeFunc([] (GLsizei p_w, GLsizei p_h)
                    {
                        if(p_h == 0) p_h = 1;
                        
                        glViewport(0, 0, p_w, p_h);
                        
                        GL::mainCamera.aspect = (GLfloat)p_w/(GLfloat)p_h;
                        GL::Update();
                    });
    
    glClearColor(0,0,0,1);


	/*
	GLfloat posicao_luz0[] = { 0.0, 10.0, 0.0, 1.0 };
	GLfloat cor_luz0[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat cor_luz0_amb[] = { 0.3, 0.3, 0.3, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz0_amb);
	glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);


	GLfloat posicao_luz1[] = { 0.0, 10.0, 5.0, 1.0 };
	GLfloat cor_luz1[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat direcao_luz1[] = { 0.0, -10.0, -5.0, 1.0 };
	GLint   spot_luz1 = 30;
	glLightfv(GL_LIGHT1, GL_DIFFUSE, cor_luz1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, cor_luz1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spot_luz1);
	glLightfv(GL_LIGHT1, GL_POSITION, posicao_luz1);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direcao_luz1);*/

	//glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

}

void GL::StartLoop(void)
{
    __timeInLastFrame = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();
}

void GL::Label(float p_x, float p_y, float p_z, string p_text)
{
    glRasterPos3f(p_x, p_y, p_z);
    for(int i = 0; i < p_text.size(); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, (int)p_text[i]);
}