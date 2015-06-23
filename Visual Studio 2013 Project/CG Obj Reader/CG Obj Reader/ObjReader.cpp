//
//  ObjReader.cpp
//  GC exercicio2
//
//  Created by Ivan Schuaste Cavalheiro on 07/04/15.
//  Copyright (c) 2015 Ivan Schuaste Cavalheiro. All rights reserved.
//

#include "ObjReader.h"

vector<string> objReader_split_string(string p_str, char p_delimiter)
{
    //fazer não retorna strings vazias
    vector<string> __internal;
    stringstream __ss(p_str);
    string __tok;
    
    while(getline(__ss, __tok, p_delimiter))
        if(!(__tok == "" && p_delimiter == ' '))
            __internal.push_back(__tok);
    
    return __internal;
}

ObjFile* ObjReader::ReadFile(string p_fileName)
{
    string __line;
    ObjFile* __toReturn = new ObjFile();
    Object* __currentObject = NULL;
	Group* __currentGroup = NULL;

	//read texure
	{
		string __textureName = p_fileName + ".jpg";
		cout << "Trying to find texture " << __textureName << endl;
		ifstream __testForTexture(__textureName);
		if (__testForTexture.is_open() == true)
		{
			__testForTexture.close();
			cout << "Texture found, loading it..." << endl;
			//read texture

			//http://www.lonesock.net/soil.html
			//https://open.gl/textures
			try
			{

				GLuint __texture;
				glGenTextures(1, &__texture);
				glBindTexture(GL_TEXTURE_2D, __texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				int width, height;
				unsigned char* image = SOIL_load_image(__textureName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
				free(image);
				
				__toReturn->texture = &__texture;
				glBindTexture(GL_TEXTURE_2D, 0);

				/*GLuint __texture = SOIL_load_OGL_texture(
				(__textureName).c_str(),
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA);

				//if (__texture == 0)
				//	cout << "\nError: " << SOIL_last_result() << endl;

				//__toReturn->texture = &__texture;*/
			}
			catch (exception e)
			{
				cout << "\nError:" << e.what() << endl;
			}

			/*
			
			GLuint __texture;
	unsigned char * data;
	FILE * file;

	file = fopen(filename, "rb");
	if (file == NULL) exit(1);
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	glGenTextures(1, &__texture);
	glBindTexture(GL_TEXTURE_2D, __texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
		GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	textura =  __texture;
			*/

		}
		else
		{
			cout << "Texture not found." << endl;
			__testForTexture.close();
		}

		cout << endl;
	}


	//read .obj
	ifstream __fileStream(p_fileName);
    if(__fileStream.is_open())
    {
        vector<Vector3> __points;
        vector<Vector2> __textureCoordenates;
        vector<Vector3> __normals;
        vector<Vector3> __parameterSpaces;
        
        while(getline(__fileStream, __line))
        {
            if(__line.length() > 1 && __line[0] == '#')
                continue;
            
            vector<string> __lineSplit = objReader_split_string(__line, ' ');
            
            //for debugging
            if(SHOW_OBJ_READER_DEBUG)
            {
                for(int ih = 0; ih < __lineSplit.size(); ih++)
                {
                    cout << __lineSplit[ih] << " | " ;
                }
                cout << "-> ";
            }
            
            if(__lineSplit.size() < 1)
            {//empty line
                if(SHOW_OBJ_READER_DEBUG)
                    cout << "empty line" << endl;
                
                if(__currentGroup != NULL)
                {
                    if(__currentObject == NULL)
                        __currentObject = new Object();
                    
                    __currentObject->groups.push_back(*__currentGroup);
                    __currentGroup = NULL;
                }

                continue;
            }
            
            //###### Go through all possible line types ######
            
            //point
            if(__lineSplit[0] == "v")
            {
                if(SHOW_OBJ_READER_DEBUG)
                    cout << "point" << endl;
                
                Vector3 __newPoint(stof(__lineSplit[1]), stof(__lineSplit[2]), stof(__lineSplit[3]));
                
                __points.push_back(__newPoint);
                
                continue;
            }
            
            //texture coordenate
            if(__lineSplit[0] == "vt")
            {
                if(SHOW_OBJ_READER_DEBUG)
                    cout << "texture coordinate" << endl;
                
                Vector2 __newTextureCoordenate(stof(__lineSplit[1]), stof(__lineSplit[2]));
                
                __textureCoordenates.push_back(__newTextureCoordenate);
                continue;
            }
            
            //normal
            if(__lineSplit[0] == "vn")
            {
                if(SHOW_OBJ_READER_DEBUG)
                    cout << "normal" << endl;
                
                Vector3 __newNormal(stof(__lineSplit[1]), stof(__lineSplit[2]), stof(__lineSplit[3]));
                
                __normals.push_back(__newNormal);
                continue;
            }
            
            //parameter space
            if(__lineSplit[0] == "vp")
            {
                if(SHOW_OBJ_READER_DEBUG)
                    cout << "parameter space" << endl;
                
                Vector3 __newParameterSpace;
                __newParameterSpace.x = stof(__lineSplit[1]);
                if(__lineSplit.size() > 2)
                    __newParameterSpace.y = stof(__lineSplit[2]);
                if(__lineSplit.size() > 3)
                    __newParameterSpace.z = stof(__lineSplit[3]);
                
                __parameterSpaces.push_back(__newParameterSpace);
                continue;
            }
            
            //face
            if(__lineSplit[0] == "f")
            {
                if(SHOW_OBJ_READER_DEBUG)
                    cout << "face ";
                
                Face __newFace;
                for(int i = 1; i < __lineSplit.size(); i++)
                {
                    if(SHOW_OBJ_READER_DEBUG)
                        cout << "(";
                    
                    //lê vertex
                    Vertex __newVertex;
                    string::size_type __doubleSlashIndex = __lineSplit[i].find("//");
                    
                    if(__doubleSlashIndex != string::npos)
                        __lineSplit[i].replace(__doubleSlashIndex, 2, "/ /");
                    
                    vector<string> __vertexSplit = objReader_split_string(__lineSplit[i], '/');
                    
                    int __pointToUse;
                    try
                    {
                        __pointToUse = stoi(__vertexSplit[0]);
                    }
                    catch (exception p_e)
                    {
                        if(SHOW_OBJ_READER_DEBUG)
                            cout << "Extra space) ";
                        
                        continue;
                    }
                    
                    //point
                    if(SHOW_OBJ_READER_DEBUG)
                        cout << __vertexSplit[0];
                    
                    __newVertex.point = __pointToUse-1;
                    
                    //texture?
                    if(__vertexSplit.size() > 1)
                    {
                        if(SHOW_OBJ_READER_DEBUG)
                            cout << " - " << __vertexSplit[1];
                        
                        if(__vertexSplit[1] != " ")
                        {
                            int __textureCoordinateToUse = stoi(__vertexSplit[1]);
                            __newVertex.textureCoordinate = __textureCoordinateToUse-1;
                        }
                    }
                    
                    //normal?
                    if(__vertexSplit.size() > 2)
                    {
                        if(SHOW_OBJ_READER_DEBUG)
                            cout << " - " << __vertexSplit[2];
                        int __normalToUse = stoi(__vertexSplit[2]);
                        __newVertex.normal = __normalToUse-1;
                    }
                    
                    __newFace.vertexes.push_back(__newVertex);
                    
                    if(SHOW_OBJ_READER_DEBUG)
                        cout << ") ";
                }
                
                if(__currentGroup == NULL)
                    __currentGroup = new Group();
                
                if(SHOW_OBJ_READER_DEBUG)
                    cout << endl;
                
                __currentGroup->faces.push_back(__newFace);
            }
            
            //object
            if(__lineSplit[0] == "o")
            {
                if(SHOW_OBJ_READER_DEBUG)
                    cout << "new object" << endl;
                
                if(__currentObject != NULL)
                {
                    if(__currentGroup != NULL)
                    {
                        __currentObject->groups.push_back(*__currentGroup);
                        __currentGroup = NULL;
                    }
                    
                    __toReturn->objects.push_back(*__currentObject);
                    __currentObject = NULL;
                }
                
                __currentObject = new Object();
                __currentObject->name = __lineSplit[1];
            }
            
            //group
			if (__lineSplit[0] == "g")
            {
                if(SHOW_OBJ_READER_DEBUG)
                    cout << "new group" << endl;
                
                if(__currentGroup != NULL)
                {
                    if(__currentObject == NULL)
                        __currentObject = new Object();
                    
                    __currentObject->groups.push_back(*__currentGroup);
                    __currentGroup = NULL;
                }
                
                __currentGroup = new Group();
                __currentGroup->name = __lineSplit[1];
            }
            
            //NOT USING <begin>
            if(__lineSplit[0] == "s")
            {
                if(SHOW_OBJ_READER_DEBUG)
                    cout << "ignored" << endl;
                
                continue;
            }
            
            if(__lineSplit[0] == "mtllib")
            {
                if(SHOW_OBJ_READER_DEBUG)
                    cout << "ignored" << endl;
                
                continue;
            }
            
            if(__lineSplit[0] == "usemtl")
            {
                if(SHOW_OBJ_READER_DEBUG)
                    cout << "ignored" << endl;
                    
                continue;
            }
            //NOT USING <end>
        }
        
        __toReturn->points = __points;
        __toReturn->textureCoordinates = __textureCoordenates;
        __toReturn->normals = __normals;
        __toReturn->parameterSpaces = __parameterSpaces;
    }
    
    if(__currentGroup != NULL)
    {
        if(__currentObject == NULL)
            __currentObject = new Object();
        __currentObject->groups.push_back(*__currentGroup);
        __currentGroup = NULL;
    }
    
    if(__currentObject != NULL)
    {
        __toReturn->objects.push_back(*__currentObject);
        __currentObject = NULL;
    }
    
    return __toReturn;
};