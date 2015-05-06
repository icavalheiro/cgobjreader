#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <thread>
#include <chrono>

#include "ObjReader.h"
#include "GL.h"
#include "ObjRenderer.h"
#include "ObjWriter.h"


double contador = 0;
float cameraDistance = 35;
bool autoRotate = true;
string objsDirectory;
vector<SceneObject> sceneObjects;
int currentSelected = 0;

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
	//define a handler for console input
	auto __consoleHandler = []()
	{
		cout << "Set the working directory (where the obj files are): ";
		getline(cin, objsDirectory);
		cout << endl;

		this_thread::sleep_for(chrono::seconds(1));

		string __reading = "";
		while (__reading != "q")
		{
			cout << "> ";
			getline(cin, __reading);
			cout << endl;
			this_thread::sleep_for(chrono::seconds(1));

			if (__reading == "save")
			{
				string __saveLocation = objsDirectory + "/saved_scene.obj";
				ObjWriter::WriteToFile(sceneObjects, __saveLocation);
				cout << "File saved: " << __saveLocation << endl;
				continue;
			}

			string __fileName = objsDirectory + "/" + __reading + ".obj";
			if (exists(__fileName) == false)
			{
				cout << "Cannot load file: " + __fileName << endl;
				continue;
			}

			ObjFile* __file = (new ObjReader())->ReadFile(__fileName);
			for (int i = 0; i < __file->objects.size(); i++)
			{
				ObjRenderer* __renderer = new ObjRenderer(&__file->objects[i], __file);
				__renderer->name = __fileName + ((i > 0) ? ("(" + to_string(i) + ")") : "");
				SceneObject* __newSceneObject = new SceneObject();
				__newSceneObject->renderer = *__renderer;
				__newSceneObject->position = Vector3(0, 0, 0);

				sceneObjects.push_back(*__newSceneObject);
			}
		}
		exit(0);
	};

	//pass important variables and callbacks to the GL class
    GL::Init(&argc, argv);
    GL::SetDrawCallback([]()
                        {
                            //draw loaded obj
							for (int i = 0; i < sceneObjects.size(); i++)
							{
								sceneObjects[i].Draw();

								if (i == currentSelected)
								{
									float __lineHeigh = 0.7f;

									GL::Label(sceneObjects[i].position.x, sceneObjects[i].position.y - (__lineHeigh * 0), sceneObjects[i].position.z,
										sceneObjects[i].renderer.name);

									GL::Label(sceneObjects[i].position.x, sceneObjects[i].position.y - (__lineHeigh * 1), sceneObjects[i].position.z,
										"position: (" + to_string(sceneObjects[i].position.x) + ", " + to_string(sceneObjects[i].position.y) + ", " + to_string(sceneObjects[i].position.z) + ")");

									GL::Label(sceneObjects[i].position.x, sceneObjects[i].position.y - (__lineHeigh * 2), sceneObjects[i].position.z,
										"scale: " + to_string(sceneObjects[i].renderer.scale));

									GL::Label(sceneObjects[i].position.x, sceneObjects[i].position.y - (__lineHeigh * 3), sceneObjects[i].position.z,
										"faces: " + to_string(sceneObjects[i].renderer.faceCount));
								}
							}

                            
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
								if (p_key == 'z') // zoom in
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].renderer.scale /= 1.1f;
								}

								if (p_key == 'x') // zoom out
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].renderer.scale *= 1.1f;
								}

                                if(p_key == 'm') // change draw type mode
                                {
									if (currentSelected < sceneObjects.size())
									{
										if (sceneObjects[currentSelected].renderer.GetCurrentDrawType() == GL_LINE_LOOP)
											sceneObjects[currentSelected].renderer.SetDrawType(GL_POLYGON);
										else if (sceneObjects[currentSelected].renderer.GetCurrentDrawType() == GL_POLYGON)
											sceneObjects[currentSelected].renderer.SetDrawType(POLYGON_WITH_BORDERS);
										else
											sceneObjects[currentSelected].renderer.SetDrawType(GL_LINE_LOOP);
									}
                                }
                                    
								if (p_key == 'c') //change draw color
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].renderer.SetRandomDrawColor();
								}
                                    
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

								if (p_key == '\'')
								{
									currentSelected++;
									if (currentSelected >= sceneObjects.size())
										currentSelected = 0;
								}

								if (p_key == 'i')
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].position.x += 0.3f;
								}

								if (p_key == 'I')
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].position.x -= 0.3f;
								}

								if (p_key == 'o')
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].position.y += 0.3f;
								}

								if (p_key == 'O')
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].position.y -= 0.3f;
								}

								if (p_key == 'p')
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].position.z += 0.3f;
								}

								if (p_key == 'P')
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].position.z -= 0.3f;
								}

								if (p_key == 'j')
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].eulerAngles.x += 10;
								}

								if (p_key == 'J')
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].eulerAngles.x -= 10;
								}

								if (p_key == 'k')
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].eulerAngles.y += 10;
								}

								if (p_key == 'K')
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].eulerAngles.y -= 10;
								}

								if (p_key == 'l')
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].eulerAngles.z += 10;
								}

								if (p_key == 'L')
								{
									if (currentSelected < sceneObjects.size())
										sceneObjects[currentSelected].eulerAngles.z -= 10;
								}
                                    
                                GL::mainCamera.position.x = cos(contador) * cameraDistance;
                                GL::mainCamera.position.z = sin(contador) * cameraDistance;
                                
                                GL::mainCamera.Update();
                            });

	//start the GL loop, showing the window and starting the console handler in a separate thread
	thread __consoleThread(__consoleHandler);
	GL::StartLoop();
}


