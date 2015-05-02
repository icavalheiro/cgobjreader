#include <iostream>
#include <string>
#include <math.h>
#include <fstream>

#include "ObjReader.h"
#include "GL.h"
#include "ObjRenderer.h"


ObjRenderer* obj;
double contador = 0;
float cameraDistance = 35;
bool autoRotate = true;
string loadedObjPath;

inline bool exists (const std::string& name) 
{
	ifstream __file(name.c_str());
	bool __toReturn = __file.is_open();
	__file.close();
	return __toReturn;
}

// Programa Principal
int main(int argc, char** argv)
{
	string __path = "D:\\Mac LAN\\chimp.obj";

    if(argc < 2)
    {
		if (exists(__path) == false)
		{
			cout << endl << "No file to load." << endl;
			return 0;
		}
    }
	else
		__path = argv[1];
    
    try
    {
		loadedObjPath = __path;
        
        if(exists(loadedObjPath) == false)
        {
            cout << "Specified .obj does not exists... closing..." << endl;
            return 0;
        }
        
        ObjFile* fileToDraw = (new ObjReader())->ReadFile(loadedObjPath);
    
        obj = new ObjRenderer(&fileToDraw->objects[0], fileToDraw);
    
        GL::Init(&argc, argv);
        GL::SetDrawCallback([]()
                            {
                                //draw loaded obj
                                obj->Draw(Point::Create(0, 0, 0));
                            
                                //draw label
                                GL::Label(2, 0, loadedObjPath);
                                GL::Label(2, -0.7f, "camera position: (" + to_string(GL::mainCamera.position.x) + ", " + to_string(GL::mainCamera.position.y) + ", " + to_string(GL::mainCamera.position.z) + ")");
                                GL::Label(2, -1.4f, "scale: " + to_string(obj->scale));
                                
                            
                                //rotate camera
                                if(autoRotate)
                                {
                                    contador += GL::deltaTime;
                                    GL::mainCamera.position.x = cos(contador) * cameraDistance;
                                    GL::mainCamera.position.z = sin(contador) * cameraDistance;
                                }
                            });
    
    
        GL::SetKeyboardCallback([](unsigned char p_key, int p_x, int p_y)
                                {
                                    if(p_key == 'z') // zoom in
                                        obj->scale -= obj->scale*0.1f;
                                
                                    if(p_key == 'x') // zoom out
                                        obj->scale += obj->scale*0.1f;
                                    
                                    if(p_key == 'm') // change draw type mode
                                    {
                                        if(obj->GetCurrentDrawType() == GL_LINE_LOOP)
                                            obj->SetDrawType(GL_POLYGON);
                                        else if(obj->GetCurrentDrawType() == GL_POLYGON)
                                            obj->SetDrawType(-1);//-1 represent both
                                        else
                                            obj->SetDrawType(GL_LINE_LOOP);
                                    }
                                    
                                    if(p_key == 'c') //change draw color
                                        obj->SetRandomDrawColor();
                                    
                                    if(p_key == ',')
                                    {
                                        autoRotate = false;
                                        contador -= 0.1f;
                                    }
                                    
                                    if(p_key == '.')
                                    {
                                        autoRotate = false;
                                        contador += 0.1f;
                                    }
                                    
                                    if(p_key == ' ')
                                        autoRotate = true;
                                    
                                    GL::mainCamera.position.x = cos(contador) * cameraDistance;
                                    GL::mainCamera.position.z = sin(contador) * cameraDistance;
                                
                                    GL::mainCamera.Update();
                                });
    
        GL::SetMouseCallback([] (int p_button, int p_state, int p_x, int p_y)
                             {
                             
                             });
        
        GL::StartLoop();
    }
    catch(exception p_exception)
    {
        cout << "Something went wrong: " << p_exception.what() << endl;
        return 0;
    }
}


