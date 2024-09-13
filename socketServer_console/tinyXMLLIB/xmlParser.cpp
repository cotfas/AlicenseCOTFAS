#include "xmlParser.h"


xmlParser::xmlParser(void)
{
}


xmlParser::~xmlParser(void)
{
}


void xmlParser::parse( TiXmlNode* pParent){
	if ( !pParent ) return;
	TiXmlNode* pChild;
	TiXmlAttribute* pAttrib;

	switch (pParent->Type())
	{
	case TiXmlNode::ELEMENT:
		if((string)pParent->Value()=="socket")
			xmlOK=true;

		if(xmlOK==true){
			if((string)pParent->Value()=="module"){
				if(pParent->ToElement()){
					pAttrib=pParent->ToElement()->FirstAttribute();
					if(( (string)pAttrib->Name()=="name" ) && ( (string)pAttrib->Value()=="mysql" || (string)pAttrib->Value()=="server" )){
						getLastModule=pAttrib->Value();
					}else{
						getLastModule="NULL";
					}
				}
			} //module
			if(getLastModule=="mysql" || getLastModule=="server"){
				if((string)pParent->Value()=="param"){
					if(pParent->ToElement()){
						pAttrib=pParent->ToElement()->FirstAttribute();
						if(( (string)pAttrib->Name()=="name" )&&( (string)pAttrib->Next()->Name()=="value" )){
							xmlValues.insert ( pair<string,string>( (string) pAttrib->Value(), (string) pAttrib->Next()->Value() ));
							//cout << pAttrib->Value() << " " << pAttrib->Next()->Value() << endl;///////////////////
						}
					}
				} //param
			}
		}
		break;
	case TiXmlNode::DOCUMENT:
		xmlOK=true;
		break;
	case TiXmlNode::DECLARATION:
		xmlOK=true;
		break;
	default:
		xmlOK=false;
		break;
	}

	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) {
		parse( pChild );
	}
}
void xmlParser::writeToXML(const char* pFilename) { 
	TiXmlDocument doc;  
	TiXmlElement* param;
 	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild( decl );  


	TiXmlElement * root = new TiXmlElement( "socket" );  
	doc.LinkEndChild( root );  
 


	TiXmlElement * moduleSQL = new TiXmlElement( "module" );  
	root->LinkEndChild( moduleSQL );  
	moduleSQL->SetAttribute("name","mysql");

	param = new TiXmlElement( "param" );  
	moduleSQL->LinkEndChild( param );
	param->SetAttribute("name", "dbIP");
	param->SetAttribute("value", xmlValues["dbIP"]);

	param = new TiXmlElement( "param" );  
	moduleSQL->LinkEndChild( param );
	param->SetAttribute("name", "dbPORT");
	param->SetAttribute("value", xmlValues["dbPORT"]);

	param = new TiXmlElement( "param" );  
	moduleSQL->LinkEndChild( param );
	param->SetAttribute("name", "dbUSER");
	param->SetAttribute("value", xmlValues["dbUSER"]);

	param = new TiXmlElement( "param" );  
	moduleSQL->LinkEndChild( param );
	param->SetAttribute("name", "dbPASS");
	param->SetAttribute("value", xmlValues["dbPASS"]);

	param = new TiXmlElement( "param" );  
	moduleSQL->LinkEndChild( param );
	param->SetAttribute("name", "dbNAME");
	param->SetAttribute("value", xmlValues["dbNAME"]);

 


	TiXmlElement * moduleServer = new TiXmlElement( "module" );  
	root->LinkEndChild( moduleServer );  
	moduleServer->SetAttribute("name","server");

	param = new TiXmlElement( "param" );  
	moduleServer->LinkEndChild( param );
	param->SetAttribute("name", "serverPORT");
	param->SetAttribute("value", xmlValues["serverPORT"]);

	param = new TiXmlElement( "param" );  
	moduleServer->LinkEndChild( param );
	param->SetAttribute("name", "autoListening");
	param->SetAttribute("value", "true");



	doc.SaveFile( pFilename ); 
} 
bool xmlParser::loadFile(const char* pFilename){
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	if (loadOkay){
		parse( &doc ); 
		return true;
	}else{
		return false;
	}
}
map<string,string> xmlParser::getXMLValues()
{
	return xmlValues;
}

void xmlParser::setXMLValues(map<string,string> xmlSetVal)
{
	xmlValues=xmlSetVal;
}
