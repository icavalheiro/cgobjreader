#include "ObjWriter.h"

void ObjWriter::WriteToFile(vector<SceneObject> p_objects, string p_location)
{
	ofstream __file;
	__file.open(p_location);

	if (__file.is_open() == false)
	{
		cout << "Failed to create/open file for writing" << endl;
		return;
	}

	int __contPoints = 0;
	int __contNormals = 0;
	int __contTexture = 0;
	int __contParameterSpace = 0;

	for (int i = 0; i < p_objects.size(); i++)
	{
		SceneObject* __currentObject = &p_objects[i];
		ObjRenderer* __currentRenderer = &__currentObject->renderer;
		Vector3 __currentPosition = __currentObject->position;
		
		ObjFile* __fileToWrite = __currentRenderer->GetObjFile();

		//write points
		vector<Vector3> __points = __fileToWrite->points;
		for (int j = 0; j < __points.size(); j++)
		{
			string __toWrite = "v " + to_string(__points[j].x) + " " + to_string(__points[j].y) + " " + to_string(__points[j].z) + "\n";
			__file << __toWrite;
		}
		__file << "\n";

		//write normals
		vector<Vector3> __normals = __fileToWrite->normals;
		for (int j = 0; j < __normals.size(); j++)
		{
			string __toWrite = "vn " + to_string(__normals[j].x) + " " + to_string(__normals[j].y) + " " + to_string(__normals[j].z) + "\n";
			__file << __toWrite;
		}
		__file << "\n";

		//write texture coordinates
		vector<Vector2> __textureCoodinates = __fileToWrite->textureCoordinates;
		for (int j = 0; j < __textureCoodinates.size(); j++)
		{
			string __toWrite = "vt " + to_string(__textureCoodinates[j].x) + " " + to_string(__textureCoodinates[j].y) + "\n";
			__file << __toWrite;
		}
		__file << "\n";

		//write parameter space
		vector<Vector3> __parameterSpaces = __fileToWrite->parameterSpaces;
		for (int j = 0; j < __parameterSpaces.size(); j++)
		{
			string __toWrite = "vp " + to_string(__parameterSpaces[j].x) + " " + to_string(__parameterSpaces[j].y) + " " + to_string(__parameterSpaces[j].z) + "\n";
			__file << __toWrite;
		}
		__file << "\n";

		//write the objects
		for (int o = 0; o < __fileToWrite->objects.size(); o++)
		{
			string __objLine = "o " + __fileToWrite->objects[o].name + "\n";
			__file << __objLine;

			for (int g = 0; g < __fileToWrite->objects[o].groups.size(); g++)
			{
				Group __currentGroup = __fileToWrite->objects[o].groups[g];

				string __groupLine = "g " + __currentGroup.name + "\n";
				__file << __groupLine;

				for (int f = 0; f < __currentGroup.faces.size(); f++)
				{
					Face __currentFace = __currentGroup.faces[f];
					string __toWrite = "f ";
					for (int fl = 0; fl < __currentFace.vertexes.size(); fl++)
					{
						__toWrite += to_string(__currentFace.vertexes[fl].point+1 + __contPoints) + "/" + to_string(__currentFace.vertexes[fl].textureCoordinate+1+__contTexture) + "/" + to_string(__currentFace.vertexes[fl].normal+1+__contNormals) + " ";
					}
					__toWrite += "\n";
					__file << __toWrite;
				}
			}

			__file << "\n";
		}

		__contNormals += __normals.size();
		__contPoints += __points.size();
		__contParameterSpace += __parameterSpaces.size();
		__contTexture += __textureCoodinates.size();
	}

	__file.close();
}