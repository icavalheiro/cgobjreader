//
//  ObjReader.h
//  GC exercicio2
//
//  Created by Ivan Schuaste Cavalheiro on 07/04/15.
//  Copyright (c) 2015 Ivan Schuaste Cavalheiro. All rights reserved.
//

#ifndef __GC_exercicio2__ObjReader__
#define __GC_exercicio2__ObjReader__

#define SHOW_OBJ_READER_DEBUG false

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <sstream>
#include <vector>
#include <GLUT\GL\SOIL.h>
#include "GL.h"

using namespace std;

class Vector3
{
public:
    float x;
    float y;
    float z;
    
    Vector3(float p_x, float p_y, float p_z)
    {
        x = p_x;
        y = p_y;
        z = p_z;
    }
    
    Vector3()
    {
    }
};

class Vector2
{
public:
    float x;
    float y;
    
    Vector2(float p_x, float p_y)
    {
        x = p_x;
        y = p_y;
    }
    
    Vector2()
    {
    }
};

class Vertex
{
public:
    int point;
    int textureCoordinate;
    int normal;
    int parameterSpace;
};

class Face
{
public:
    vector<Vertex> vertexes;
};

class Group
{
public:
    string name;
    vector<Face> faces;
};

class Object
{
public:
    string name;
    vector<Group> groups;
};

class ObjFile
{
public:
    vector<Vector3> points;
    vector<Vector2> textureCoordinates;
    vector<Vector3> normals;
    vector<Vector3> parameterSpaces;
    
    vector<Object> objects;

	GLuint* texture;
};

class ObjReader
{
public:
    ObjFile* ReadFile(string p_fileName);
};

#endif /* defined(__GC_exercicio2__ObjReader__) */
