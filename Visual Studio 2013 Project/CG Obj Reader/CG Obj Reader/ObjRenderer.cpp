//
//  ObjRenderer.cpp
//  GC exercicio2
//
//  Created by Ivan Schuaste Cavalheiro on 14/04/15.
//  Copyright (c) 2015 Ivan Schuaste Cavalheiro. All rights reserved.
//

#include "ObjRenderer.h"

void ObjRenderer::Draw(Point p_where)
{
	Object* __object = _object;
	for(int grupo = 0; grupo < __object->groups.size(); grupo++)
    {
        Group* __group = &__object->groups[grupo];
        for(int face = 0; face < __group->faces.size(); face++)
        {
            Face* __face = &__group->faces[face];
            
            auto __drawPointsAction = [&]()
            {
                for (int vertex = 0; vertex < __face->vertexes.size(); vertex++)
                {
                    Vertex* __toDraw = &__face->vertexes[vertex];
                    Vector3 __point = _parent->points[__toDraw->point];
                    //Vector3 __normal = _parent->normals[__toDraw->normal];

					float __x, __y, __z;
					__x = p_where.x + (__point.x * scale);
					__y = p_where.y + (__point.y * scale);
					__z = p_where.z + (__point.z * scale);

					glVertex3f(__x, __y, __z);
                    //glNormal3f(__normal.x, __normal.y, __normal.z);
                }
            };
            
            if(this->_drawType == -1)
            {
                glBegin(GL_POLYGON);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glColor3f(this->_drawColor->r, this->_drawColor->g, this->_drawColor->b);
                __drawPointsAction();
                glEnd();
                
                glBegin(GL_LINE_LOOP);
                glColor3f(this->_drawColor->r * 0.7f, this->_drawColor->g * 0.7f, this->_drawColor->b * 0.7f);
                __drawPointsAction();
                glEnd();
            }
            else
            {
                glColor3f(this->_drawColor->r, this->_drawColor->g, this->_drawColor->b);
                glBegin(this->_drawType);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                __drawPointsAction();
                glEnd();
            }

			glBegin(GL_LINE);
        }
    }
    
    glColor3f(1, 1, 1);
}

void ObjRenderer::SetDrawType(int p_glutDrawType)
{
    this->_drawType = p_glutDrawType;
}

int ObjRenderer::GetCurrentDrawType()
{
    return this->_drawType;
}

void ObjRenderer::SetRandomDrawColor()
{
    float __r = (rand() % 255)/255.0f;
    float __g = (rand() % 255)/255.0f;
    float __b = (rand() % 255)/255.0f;
    
    this->_drawColor->r = __r;
    this->_drawColor->g = __g;
    this->_drawColor->b = __b;
}

void SceneObject::Draw()
{
	this->renderer.Draw(Point::Create(this->position.x, this->position.y, this->position.z));
}