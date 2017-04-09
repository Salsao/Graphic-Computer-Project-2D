#ifndef LEITOR_H_   /* Include guard */
#define LEITOR_H_
#include "everything.h"
#include "tank.h"

using namespace std;

extern string nomeArquivoSvg;
extern string nomeArquivoPmg;
extern string nomeArquivoPmg2;
extern string variavelNome1;
extern string variavelNome2;
extern string variavelNome3;
extern string variavelNome4;
extern int flag;
extern int flagArquivo;
extern int widthPmg;
extern int heightPmg;
extern float menorx;
extern float menory;
extern int** matrizMobility;
extern int** matrizDistance;
extern float corMax;
extern bool flagMatriz;

extern float shootSpeed;
extern float tankSpeed;
extern float shootsRate;


typedef struct Circulo Circulo;
struct Circulo{
    GLfloat raio;
    GLfloat x;
    GLfloat y;
    GLfloat R;
    GLfloat G;
    GLfloat B;
};

typedef struct Retangulo Retangulo;
struct Retangulo{
    GLfloat altura;
    GLfloat largura;
    GLfloat x;
    GLfloat y;
    GLfloat R;
    GLfloat G;
    GLfloat B;
    bool teletransporte;
};

extern vector <Circulo> vetorCirculo;
extern vector <Retangulo> vetorRetangulo;
extern Circulo circo;
extern Retangulo reto;


static int Getdec(char hexchar);

float HexColorToFloat(string HexColor);

void drawRectangulo (Retangulo r);

void drawCircle(Circulo c);

void putInRectangulo (float height, float width, float x, float y, float R, float G, float B);

void putInCircle(float cx, float cy, float raio, float R, float G, float B);

void seeRect(TiXmlElement* pElement) ;

void seeCircle(TiXmlElement* pElement);

const char * getIndent( unsigned int numIndents );

const char * getIndentAlt( unsigned int numIndents );

int dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent);

void dump_to_stdout( TiXmlNode* pParent, unsigned int indent);

void openSvg(const char* pFilename);

void openPgm(const char* pFilename);

void openXml(const char* pFilename);

#endif
