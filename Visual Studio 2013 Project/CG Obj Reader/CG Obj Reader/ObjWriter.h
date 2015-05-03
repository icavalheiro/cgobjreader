#ifndef __GC_ecercicio2__ObjWriter__
#define __GC_ecercicio2__ObjWriter__

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "ObjRenderer.h"

namespace ObjWriter
{
	void WriteToFile(vector<SceneObject> p_objects, string p_where);
}

#endif