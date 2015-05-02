//
//  GL.h
//  GC exercicio2
//
//  Created by Ivan Schuaste Cavalheiro on 14/04/15.
//  Copyright (c) 2015 Ivan Schuaste Cavalheiro. All rights reserved.
//

#ifndef __GC_exercicio2__GL__
#define __GC_exercicio2__GL__

#include <iostream>
#include <stdio.h>
#include <GLUT\GL\glut.h>
#include <vector>

#define OPENGL_WINDOW_NAME "OBJ Reader"
#define SHOW_AXIS true

using namespace std;

class Point
{
public:
    float x;
    float y;
    float z;
    
    static Point Create(float p_x, float p_y, float p_z);
};

class Camera
{
public:
    Point position;
    Point lookPosition;
    Point up;
    
    float angle;
    float aspect;
    
    Camera()
    {
        position.x = 0;
        position.y = 20;
        position.z = 35;
        
        lookPosition.x = 0;
        lookPosition.y = 0;
        lookPosition.z = 0;
        
        up.x = 0;
        up.y = 1;
        up.z = 0;
        
        angle = 60;
    }
    
    void Update(void);
};

class GL
{
private:
    static vector<void (*)(unsigned char p_key, int p_x, int p_y)> _keyboardCallbacks;
    static vector<void (*)(int p_button, int p_state, int p_x, int p_y)> _mouseCallbacks;
    static vector<void (*)(void)> _drawCallbacks;
    
    static void KeyboardCallback(unsigned char p_key, int p_x, int p_y);
    static void MouseCallback(int p_button, int p_state, int p_x, int p_y);
    static void DrawCallback(void);
    static void Update(void);
    
public:
    static float deltaTime;
    
    static Camera mainCamera;
    
    static void SetKeyboardCallback(void (*p_callback)(unsigned char p_key, int p_x, int p_y));
    static void SetMouseCallback(void (*p_callback)(int p_button, int p_state, int p_x, int p_y));
    static void SetDrawCallback(void (*p_callback)(void));
    
    static void Init(int *p_argc, char** p_argv);
    static void StartLoop(void);
    
    static void Label(float p_x, float p_y, float p_z, string p_text);
};


#endif /* defined(__GC_exercicio2__GL__) */
