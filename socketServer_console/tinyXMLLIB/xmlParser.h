#pragma once

#include "TICPP\ticpp.h"
#pragma comment(lib, "ticppd.lib")

#include <iostream>
#include <map>    
#include <string>
using namespace std;






class xmlParser
{
public:
	xmlParser(void);
	virtual ~xmlParser(void);


public:
	void writeToXML(const char* pFilename);
	bool loadFile(const char* pFilename);
	void setXMLValues(map<string,string> xmlSetVal);
	map<string,string> getXMLValues();


private:
	void parse( TiXmlNode* pParent);

private:
	map<string,string> xmlValues;
	string getLastModule;
	bool xmlOK;
};

