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
					if(( (string)pAttrib->Name()=="name" ) && ( ((string)pAttrib->Value()=="socketClient" ) || ((string)pAttrib->Value()=="mainForm" ) ) ){
						getLastModule=pAttrib->Value();
					}else{
						getLastModule="NULL";
					}
				}
			} //module
			if((getLastModule=="socketClient")||(getLastModule=="mainForm")){
				if((string)pParent->Value()=="param"){
					if(pParent->ToElement()){
						pAttrib=pParent->ToElement()->FirstAttribute();
						if(( (string)pAttrib->Name()=="name" )&&( (string)pAttrib->Next()->Name()=="value" )){
							xmlValues.insert ( pair<string,string>( (string) pAttrib->Value(), (string) pAttrib->Next()->Value() ));
							cout << pAttrib->Value() << " " << pAttrib->Next()->Value() << endl;///////////////////
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
void xmlParser::writeToXML(const char* pFilename,map<string,string> Values) { 
	TiXmlDocument doc;  
	TiXmlElement* param;
 	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild( decl );  


	TiXmlElement * root = new TiXmlElement( "socket" );  
	doc.LinkEndChild( root );  


	TiXmlElement * moduleClient = new TiXmlElement( "module" );  
	root->LinkEndChild( moduleClient );  
	moduleClient->SetAttribute("name","socketClient");

	param = new TiXmlElement( "param" );  
	moduleClient->LinkEndChild( param );
	param->SetAttribute("name", "ID_IP");
	param->SetAttribute("value", Values["ID_IP"]);

	param = new TiXmlElement( "param" );  
	moduleClient->LinkEndChild( param );
	param->SetAttribute("name", "ID_PORT");
	param->SetAttribute("value", Values["ID_PORT"]);

	param = new TiXmlElement( "param" );  
	moduleClient->LinkEndChild( param );
	param->SetAttribute("name", "ID_USERNAME");
	param->SetAttribute("value", Values["ID_USERNAME"]);


	param = new TiXmlElement( "param" );  
	moduleClient->LinkEndChild( param );
	param->SetAttribute("name", "ID_PASSWORD");
	param->SetAttribute("value", Values["ID_PASSWORD"]);

	/*param = new TiXmlElement( "param" );  
	moduleClient->LinkEndChild( param );
	param->SetAttribute("name", "ID_AUTH");
	param->SetAttribute("value", Values["ID_AUTH"]);
	*/
	param = new TiXmlElement( "param" );  
	moduleClient->LinkEndChild( param );
	param->SetAttribute("name", "autoEnableListen");
	param->SetAttribute("value", Values["autoEnableListen"]);

	param = new TiXmlElement( "param" );  
	moduleClient->LinkEndChild( param );
	param->SetAttribute("name", "autoEnableChat");
	param->SetAttribute("value", Values["autoEnableChat"]);
 

	TiXmlElement * moduleRect = new TiXmlElement( "module" );  
	root->LinkEndChild( moduleRect );  
	moduleRect->SetAttribute("name","mainForm");

	param = new TiXmlElement( "param" );  
	moduleRect->LinkEndChild( param );
	param->SetAttribute("name", "mainRectBottom");
	param->SetAttribute("value", Values["mainRectBottom"]);

	param = new TiXmlElement( "param" );  
	moduleRect->LinkEndChild( param );
	param->SetAttribute("name", "mainRectTop");
	param->SetAttribute("value", Values["mainRectTop"]);

	param = new TiXmlElement( "param" );  
	moduleRect->LinkEndChild( param );
	param->SetAttribute("name", "mainRectLeft");
	param->SetAttribute("value", Values["mainRectLeft"]);

	param = new TiXmlElement( "param" );  
	moduleRect->LinkEndChild( param );
	param->SetAttribute("name", "mainRectRight");
	param->SetAttribute("value", Values["mainRectRight"]);

	param = new TiXmlElement( "param" );  
	moduleRect->LinkEndChild( param );
	param->SetAttribute("name", "mainMaximized");
	param->SetAttribute("value", Values["mainMaximized"]);



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
