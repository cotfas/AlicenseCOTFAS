#pragma once



// XML Parser lib
#include "../tinyXMLLIB/xmlParser.h"
#pragma comment(lib, "Debug//tinyXMLLIB.lib")



// database lib
#include "../MyDatabaseLib/dataBase.h"
#pragma comment(lib, "Debug//MyDatabaseLib.lib")

#pragma comment(lib, "Debug//connectionLib.lib")



#pragma comment(lib, "wsock32.lib")

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <hash_map>
#include <time.h>


#include "consoleColor.h"


using namespace std;

bool startXML();