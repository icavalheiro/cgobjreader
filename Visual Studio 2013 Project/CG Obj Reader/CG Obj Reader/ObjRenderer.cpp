//
//  ObjRenderer.cpp
//  GC exercicio2
//
//  Created by Ivan Schuaste Cavalheiro on 14/04/15.
//  Copyright (c) 2015 Ivan Schuaste Cavalheiro. All rights reserved.
//

#include "ObjRenderer.h"

void ObjRenderer::Draw(Point p_where, Vector3 p_rotation)
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
                    Vector3 __normal = _parent->normals[__toDraw->normal];

					float __x, __y, __z;
					__x =  (__point.x);
					__y =  (__point.y);
					__z = (__point.z);

					glVertex3f(__x, __y, __z);
                    glNormal3f(__normal.x, __normal.y, __normal.z);
                }
            };
            
			glPushMatrix();
			glTranslatef(p_where.x, p_where.y, p_where.z);
			glScalef(scale, scale, scale);

			glRotatef(p_rotation.x, 1, 0, 0);
			glRotatef(p_rotation.y, 0, 1, 0);
			glRotatef(p_rotation.z, 0, 0, 1);

			if (this->_drawType == POLYGON_WITH_BORDERS)
            {
                glBegin(GL_POLYGON);
                glColor3f(this->_drawColor->r, this->_drawColor->g, this->_drawColor->b);
                __drawPointsAction();
                glEnd();
                
                glBegin(GL_LINE_LOOP);
                glColor3f(this->_drawColor->r * 0.7f, this->_drawColor->g * 0.7f, this->_drawColor->b * 0.7f);
                __drawPointsAction();
                glEnd();
            }
			else if (this->_drawType == POLYGON_WITH_SHADER)
			{
				GLfloat plano_difusa[] = { 0.5, 0.5, 0.0, 1.0 };
				GLfloat plano_especular[] = { 1.0, 1.0, 1.0, 1.0 };
				GLfloat plano_brilho[] = { 50.0 };

				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, plano_difusa);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plano_especular);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, plano_brilho);
				glBegin(GL_POLYGON);
				__drawPointsAction();
				glEnd();
			}
			else
            {
                glColor3f(this->_drawColor->r, this->_drawColor->g, this->_drawColor->b);
                glBegin(this->_drawType);
                __drawPointsAction();
                glEnd();
            }
			glPopMatrix();
        }
    }
    
    glColor3f(1, 1, 1);
}

ObjFile* ObjRenderer::GetObjFile()
{
	return this->_parent;
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
	this->renderer.Draw(Point::Create(this->position.x, this->position.y, this->position.z), this->eulerAngles);
}