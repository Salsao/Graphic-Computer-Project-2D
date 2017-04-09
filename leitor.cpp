#include "leitor.h"

//using namespace std;

string nomeArquivoSvg;
string nomeArquivoPmg;
string nomeArquivoPmg2;
string variavelNome1;
string variavelNome2;
string variavelNome3 = ".";
string variavelNome4;
int flag = 0;
int flagArquivo = 0;
int widthPmg;
int heightPmg;
float menorx = 1000;
float menory = 1000;
int** matrizMobility;
int** matrizDistance;
float corMax;
bool flagMatriz = true;

float shootSpeed;
float tankSpeed;
float shootsRate;


vector <Circulo> vetorCirculo;
Circulo circo;

vector <Retangulo> vetorRetangulo;
Retangulo reto;


const unsigned int NUM_INDENTS_PER_SPACE=2;

static int Getdec(char hexchar)
{
	if ((hexchar >= '0') && (hexchar <= '9')) return hexchar - '0';
	if ((hexchar >= 'A') && (hexchar <= 'F')) return hexchar - 'A' + 10;
	if ((hexchar >= 'a') && (hexchar <= 'f')) return hexchar - 'a' + 10;

	return -1; // Wrong character
}

float HexColorToFloat(string HexColor)
{
	string HexColorPtr = HexColor;
	float IntColor;
	IntColor = (Getdec(HexColorPtr[0]) * 16) +
	Getdec(HexColorPtr[1]);

	IntColor = IntColor / 255.0;
	return IntColor;
}



void drawRectangulo (Retangulo r)
{
	glColor3f(r.R,r.G,r.B);
	glBegin(GL_QUADS);
	glVertex2f (r.x - menorx, heightPmg - (-r.y + heightPmg + menory));
	glVertex2f (r.x - menorx, heightPmg - (-r.y - r.altura + heightPmg + menory));
	glVertex2f (r.x - menorx + r.largura, heightPmg - (-r.y - r.altura + heightPmg + menory));
	glVertex2f (r.x - menorx + r.largura, heightPmg -(-r.y + heightPmg + menory));
	glEnd();
}

void drawCircle(Circulo c)
{
	// Player: 0.784314 // player < 0.8 /// inimigo > 0.8 float!!!

	float x = c.raio;
	float y = 0; 
	float theta = 2*3.1415/c.raio; 
	float cose = cos(theta);
	float seno = sin(theta);
	float trans;
	glColor3f(c.R,c.G,c.B);
	glBegin(GL_POLYGON); 
	for(int i = 0; i < 360; i++) 
	{ 
		glVertex2f(x+c.x - menorx, heightPmg - (y+-c.y + heightPmg + menory));
		trans = x;
		x = cose * x - seno * y;
		y = seno * trans + cose * y;
	} 
	glEnd();
}

void putInRectangulo (float height, float width, float x, float y, float R, float G, float B)
{
  	reto.altura = height;
	reto.largura = width;
	reto.x = x;
	reto.y = y;
	reto.R = R;
	reto.G = G;
	reto.B = B;
	if(B > 0.4)
	     reto.teletransporte = true;
	else
	     reto.teletransporte = false;
	vetorRetangulo.push_back(reto);
}

void putInCircle(float cx, float cy, float raio, float R, float G, float B)
{
  	circo.x = cx;
	circo.y = cy;
	circo.raio = raio;
	circo.R = R;
	circo.G = G;
	circo.B = B;
	inicializaTank(circo.x, circo.y, circo.R, circo.raio);
	//cout << "BUIIBVIBUBU\n"; 
	vetorCirculo.push_back(circo);
	
}

void seeRect(TiXmlElement* pElement) {
	float width, height, x, y, R, G, B;
	stringstream t;
	TiXmlAttribute* pAttrib = pElement->FirstAttribute();
	while (pAttrib)
	{
		if (!strcmp(pAttrib->Name(), "style")) {
			string value = pAttrib->Value();
			string colorHex = value.substr(6, 6);
			string Rh = colorHex.substr(0, 2); istringstream(Rh) >> hex >> R;
			string Gh = colorHex.substr(2, 2); istringstream(Gh) >> hex >> G;
			string Bh = colorHex.substr(4, 2); istringstream(Bh) >> hex >> B;
			R = HexColorToFloat(Rh);
			G = HexColorToFloat(Gh);
			B = HexColorToFloat(Bh);
		} else if (!strcmp(pAttrib->Name(), "width")) {
			width = atof(pAttrib->Value());
		} else if (!strcmp(pAttrib->Name(), "height")) {
			height = atof(pAttrib->Value());
		} else if (!strcmp(pAttrib->Name(), "x")) {
			x = atof(pAttrib->Value());
		} else if (!strcmp(pAttrib->Name(), "y")) {
			y = atof(pAttrib->Value());
		}
		pAttrib = pAttrib->Next();
	}
	if (x < menorx)
		menorx = x;
	if (y < menory)
		menory = y;
	//drawRectangulo(height, width, x, y, R, G, B);
	putInRectangulo(height, width, x, y, R, G, B);
}

void seeCircle(TiXmlElement* pElement) {

	float cx, cy, raio, R, G, B;
	TiXmlAttribute* pAttrib = pElement->FirstAttribute();
	while (pAttrib)
	{
		if (!strcmp(pAttrib->Name(), "fill")) {
			string value = pAttrib->Value();
			string colorHex = value.substr(1, 6);
			string Rh = colorHex.substr(0, 2); istringstream(Rh) >> hex >> R;
			string Gh = colorHex.substr(2, 2); istringstream(Gh) >> hex >> G;
			string Bh = colorHex.substr(4, 2); istringstream(Bh) >> hex >> B;
			R = HexColorToFloat(Rh);
			G = HexColorToFloat(Gh);
			B = HexColorToFloat(Bh);
		} else if (!strcmp(pAttrib->Name(), "cx")) {
			cx = atof(pAttrib->Value());
		} else if (!strcmp(pAttrib->Name(), "cy")) {
			cy = atof(pAttrib->Value());
		} else if (!strcmp(pAttrib->Name(), "r")){
			raio = atof(pAttrib->Value());
		}
		pAttrib = pAttrib->Next();
	}
	//drawCircle(cx, cy, raio, R, G, B);
	putInCircle(cx, cy, raio, R, G, B);
}

const char * getIndent( unsigned int numIndents )
{
	static const char * pINDENT="                                      + ";
	static const unsigned int LENGTH=strlen( pINDENT );
	unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
	if ( n > LENGTH ) n = LENGTH;

	return &pINDENT[ LENGTH-n ];
}

// same as getIndent but no "+" at the end
const char * getIndentAlt( unsigned int numIndents )
{
	static const char * pINDENT="                                        ";
	static const unsigned int LENGTH=strlen( pINDENT );
	unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
	if ( n > LENGTH ) n = LENGTH;

	return &pINDENT[ LENGTH-n ];
}

int dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent)
{
	if ( !pElement ) return 0;

	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	int ival;
	double dval;
	const char* pIndent=getIndent(indent);
	//printf("\n");
	while (pAttrib)
	{
		
		// Receber o nome dos arquivos
		if (flagArquivo != 4){
			if (flag == 0){
				variavelNome2 = pAttrib->Value();
				//variavelNome2[0] = toupper(variavelNome2[0]); //Mudei
				flag = 1;
			}
			else if (flag == 1){
				variavelNome4 = pAttrib->Value();
				flag = 2;
			}
			else if (flag == 2){
				variavelNome1 = pAttrib->Value();
				if (flagArquivo == 0){
					nomeArquivoSvg = variavelNome1 + variavelNome2 + variavelNome3 + variavelNome4;
					flagArquivo = 1;
				}
				else if (flagArquivo == 1){
					nomeArquivoPmg = variavelNome1 + variavelNome2 + variavelNome3 + variavelNome4;
					flagArquivo = 2;
				}
				else if (flagArquivo == 2){
					nomeArquivoPmg2 = variavelNome1 + variavelNome2 + variavelNome3 + variavelNome4;
					flagArquivo = 3;
				}
				else if (flagArquivo == 3){
					shootsRate = atof(variavelNome1.c_str());
					shootSpeed = atof(variavelNome2.c_str());
					tankSpeed = atof(variavelNome4.c_str());
					flagArquivo = 4;
				}
				flag = 0;
			}
		}
		
		
		//if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS)    printf( " int=%d", ival);
		//if (pAttrib->QueryDoubleValue(&dval)==TIXML_SUCCESS) printf( " d=%1.1f", dval);
		//printf( "\n" );
		i++;
		pAttrib=pAttrib->Next();
	}
	return i;	
}



void dump_to_stdout( TiXmlNode* pParent, unsigned int indent = 0 )
{
	if ( !pParent ) return;

	TiXmlNode* pChild;
	TiXmlText* pText;
	int t = pParent->Type();
	//printf( "%s", getIndent(indent));
	int num;

	switch ( t )
	{
	case TiXmlNode::TINYXML_DOCUMENT:
		//printf( "Document" );
		break;

	case TiXmlNode::TINYXML_ELEMENT:
		//printf( "Element [%s]", pParent->Value() );

		if (!strcmp(pParent->Value(), "rect")) {
			seeRect(pParent->ToElement());
		}
		
		if (!strcmp(pParent->Value(), "circle")) {
			seeCircle(pParent->ToElement());
		}

		num=dump_attribs_to_stdout(pParent->ToElement(), indent+1);
		switch(num)
		{
			//case 0:  printf( " (No attributes)"); break;
			//case 1:  printf( "%s1 attribute", getIndentAlt(indent)); break;
			//default: printf( "%s%d attributes", getIndentAlt(indent), num); break;
		}
		break;

	case TiXmlNode::TINYXML_COMMENT:
		//printf( "Comment: [%s]", pParent->Value());
		break;

	case TiXmlNode::TINYXML_UNKNOWN:
		//printf( "Unknown" );
		break;

	case TiXmlNode::TINYXML_TEXT:
		pText = pParent->ToText();
		//printf( "Text: [%s]", pText->Value() );
		break;

	case TiXmlNode::TINYXML_DECLARATION:
		//printf( "Declaration" );
		break;
	default:
		break;
	}
	//printf( "\n" );
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		dump_to_stdout( pChild, indent+1 );
	}
}

//Carregar a imagem SVG
void openSvg(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		//printf(" Open Success! \n%s:\n", pFilename);
		dump_to_stdout( &doc ); // defined later in the tutorial
	}
	else
	{
		printf("Failed to load file \"%s\"\n", pFilename);
	}
}

//Carregar a imagem PMG
void openPgm(const char* pFilename)
{
	ifstream doc2;
	doc2.open(pFilename);
	string linha, largura, altura;
	if (doc2.is_open()){
		printf("Show to load file \"%s\"\n", pFilename);
		getline( doc2 , linha );
		getline( doc2 , linha );
		getline( doc2 , linha );
		stringstream  stream(linha.c_str());
		stream >> largura;
		stream >> altura;  
		widthPmg = atof(largura.c_str());
		heightPmg = atof(altura.c_str());
		getline(doc2, linha);
		corMax = atof(linha.c_str());
		if (flagMatriz)
		{
		    matrizMobility = (int**)malloc(sizeof(int*)*(heightPmg));
		    for (int cont = 0; cont < heightPmg; cont++){
			    matrizMobility[cont] = (int*)malloc(sizeof(int)*(widthPmg));
			    for (int cont2 = 0; cont2 < widthPmg; cont2++)
				    {
				    getline(doc2, linha);
				    matrizMobility[cont][cont2] = atof(linha.c_str());
			    }
		    }
		    flagMatriz = false;
		}
		else
		{
		  matrizDistance = (int**)malloc(sizeof(int*)*(heightPmg));
		    for (int cont = 0; cont < heightPmg; cont++){
			    matrizDistance[cont] = (int*)malloc(sizeof(int)*(widthPmg));
			    for (int cont2 = 0; cont2 < widthPmg; cont2++)
				    {
				    getline(doc2, linha);
				    matrizDistance[cont][cont2] = atof(linha.c_str());
			    }
		    }
		}
		}
	else
		printf("Failed to load file \"%s\"\n", pFilename);
		
}


void openXml(const char* pFilename)
{
	TiXmlDocument docX(pFilename);
	bool loadOkay = docX.LoadFile();
	if (loadOkay)
	{
		printf(" Open Success! \n%s:\n", pFilename);
		dump_to_stdout( &docX ); // defined later in the tutorial
	}
	else
	{
		printf("Failed to load file \"%s\"\n", pFilename);
	}
	
}
