//
//  ObjRenderer.h
//  GC exercicio2
//
//  Created by Ivan Schuaste Cavalheiro on 14/04/15.
//  Copyright (c) 2015 Ivan Schuaste Cavalheiro. All rights reserved.
//

#ifndef __GC_exercicio2__ObjRenderer__
#define __GC_exercicio2__ObjRenderer__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GL.h"
#include "ObjReader.h"

class Color
{
public:
    float r;
    float g;
    float b;
    float a;
    
    Color(float p_r, float p_g, float p_b, float p_a)
    {
        r = p_r;
        g = p_g;
        b = p_b;
        a = p_a;
    }
};

class ObjRenderer
{
private:
    Object *_object;
    ObjFile *_parent;
    int _drawType;
    Color *_drawColor;
    
public:
    float scale;
	string name;
	int faceCount;
    
	ObjRenderer(){} // please dont use this one
    ObjRenderer(Object *p_object, ObjFile *p_parent)
    {
        srand ((int)time(NULL));
        _object = p_object;
        _parent = p_parent;
        scale = 1;
        _drawType = GL_LINE_LOOP;
        _drawColor = new Color(1,1,1,1);
		faceCount = 0;
		for (int i = 0; i < p_object->groups.size(); i++)
			faceCount += p_object->groups[i].faces.size();
    }
    
    void Draw(Point p_where, Vector3 p_rotation);
    
    void SetDrawType(int p_glutDrawType);
    int GetCurrentDrawType();

    void SetRandomDrawColor();

	ObjFile* GetObjFile(void);
};

class SceneObject
{
public:
	ObjRenderer renderer;
	Vector3 position;
	Vector3 eulerAngles;

	void Draw(void);
};

#endif /* defined(__GC_exercicio2__ObjRenderer__) */
