#ifndef TANK_H_   /* Include guard */
#define TANK_H_

#include "everything.h"
#include "leitor.h"

using namespace std;


typedef struct Bullet Bullet;
struct Bullet{
    float angulo;
    float posicao;
    float x;
    float y;
    float posX;
    float posY;
};




typedef struct Tank Tank;
struct Tank{
    bool tes;
    GLfloat qwerty;
    GLfloat tt;
    GLfloat gX;
    GLfloat gY;
    GLfloat gg;
    GLfloat angleCanhao;
    GLfloat mouseX;
    GLfloat mouseY;
    GLfloat esteira;
    GLfloat ladoD;
    GLfloat ladoE;
    bool contrarioD;
    bool contrarioE;
    bool player;
    int height;
    int width;
    float raio;
    GLfloat alturaEsteira;
    GLfloat larguraEsteira;
    GLfloat canhao;
    bool vivo;
    float raioBase;
    vector <Bullet*> bullets;
    int qtdBullets;
    int id;
  
};

extern vector <Tank> vetorTank;
extern Tank tanque;
 

void teletransporte(float alturaR, float larguraR, float x, float y, float x2, float y2, Tank *tanque);
 
void inicializaTank(GLfloat circle_x, GLfloat circle_y, float circle_R, float circle_raio);

void moveInX(GLfloat dx, Tank tanque);

void moveInY(GLfloat dy, Tank tanque);

void moveAngle(GLfloat gg, Tank tanque);

void Idle();

void desenhaCanhao (Tank tanque);

void desenhaBaseTank (Tank tanque);

void desenhaBaseCanhao(Tank tanque) ;

void desenhaFormatoBullet(float raio, float R, float G, float B) ;

void desenhaEsteira (Tank tanque, bool de);

void apontaCanhao(Tank alvo, Tank *inimigo);

void moveCanhao (int x, int y);

void updateCanhao();

void desenhaBullet(int x, int y, GLfloat tt, GLfloat bocaCanhao, Tank *tanque, int i);

void tiro(Tank *tanque);

void movemove();

void update(int value);

void inimigoAtira(int value);

void shootClick(int button, int state, int x, int y);

void Tecl(unsigned char key, int x, int y);

void killKey();

void keyUP(unsigned char key, int x, int y);

void desenhaTank(Tank tanque);


#endif
