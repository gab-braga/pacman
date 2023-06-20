#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <SOIL.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "pacman.h"

#define WINDOW_SIZE 600
#define MAP_SIZE 26
#define OBSTACLE (-1)
#define FREE_WAY 0
#define COIN_WAY 1
#define POWER_WAY 2

#define SALT 30
#define STEP 5

#define POWER 0
#define NO_POWER (-1)

#define LIFE 0
#define DEAD (-1)

#define RIGHT 0
#define DOWN  1
#define LEFT  2
#define UP   3

// ================================== IMAGENS ==================================

const char* PACMAN_PATHS[12] = {
        "../images/pacman-1.png",
        "../images/pacman-2.png",
        "../images/pacman-3.png",
        "../images/pacman-4.png",
        "../images/pacman-5.png",
        "../images/pacman-6.png",
        "../images/pacman-7.png",
        "../images/pacman-8.png",
        "../images/pacman-9.png",
        "../images/pacman-10.png",
        "../images/pacman-11.png",
        "../images/pacman-12.png"
};

const char* PHANTOM_PATHS[20] = {
        "../images/phantom-1.png",
        "../images/phantom-2.png",
        "../images/phantom-3.png",
        "../images/phantom-4.png",
        "../images/phantom-5.png",
        "../images/phantom-6.png",
        "../images/phantom-7.png",
        "../images/phantom-8.png",
        "../images/phantom-9.png",
        "../images/phantom-10.png",
        "../images/phantom-11.png",
        "../images/phantom-12.png",
        "../images/phantom-13.png",
        "../images/phantom-14.png",
        "../images/phantom-15.png",
        "../images/phantom-16.png",
        "../images/phantom-17.png",
        "../images/phantom-18.png",
        "../images/phantom-19.png",
        "../images/phantom-20.png"
};

const char* SCENES_PATHS[13] = {
        "../images/empty.png",
        "../images/coin.png",
        "../images/power.png",
        "../images/vertical.png",
        "../images/horizontal.png",
        "../images/curve-base-left.png",
        "../images/curve-base-right.png",
        "../images/curve-top-left.png",
        "../images/curve-top-right.png",
        "../images/end-left.png",
        "../images/end-right.png",
        "../images/end-base.png",
        "../images/end-top.png"
};

// ================================== MAPA DE POSIÇÕES ==================================

const int SCENES_POSITION[MAP_SIZE][MAP_SIZE] = {
        {7, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 8, 7, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 8},
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},
        {3, 1, 7, 4, 8, 1, 7, 4, 4, 4, 8, 1, 3, 3, 1, 7, 4, 4, 4, 8, 1, 7, 4, 8, 1, 3},
        {3, 2, 3, 0, 3, 1, 3, 0, 0, 0, 3, 1, 3, 3, 1, 3, 0, 0, 0, 3, 1, 3, 0, 3, 2, 3},
        {3, 1, 5, 4, 6, 1, 5, 4, 4, 4, 6, 1, 5, 6, 1, 5, 4, 4, 4, 6, 1, 5, 4, 6, 1, 3},
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},
        {3, 1, 9, 4, 10, 1, 7, 8, 1, 9, 4, 4, 8, 7, 4, 4, 10, 1, 7, 8, 1, 9, 4, 10, 1, 3},
        {3, 1, 1, 1, 1,  1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 3},
        {5, 4, 4, 4, 8, 1, 3, 5, 4, 4, 8, 1, 3, 3, 1, 7, 4, 4, 6, 3, 1, 7, 4, 4, 4, 6},
        {0, 0, 0, 0, 3, 1, 3, 7, 4, 4, 6, 1, 5, 6, 1, 5, 4, 4, 8, 3, 1, 3, 0, 0, 0, 0},
        {4, 4, 4, 4, 6, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 5, 4, 4, 4, 4},
        {1, 1, 1, 1, 1, 1, 5, 6, 1, 7, 4, 4, 4, 4, 4, 4, 8, 1, 5, 6, 1, 1, 1, 1, 1, 1},
        {4, 4, 4, 4, 8, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 1, 7, 4, 4, 4, 4},
        {0, 0, 0, 0, 3, 1, 7, 8, 1, 5, 4, 4, 4, 4, 4, 4, 6, 1, 7, 8, 1, 3, 0, 0, 0, 0},
        {7, 4, 4, 4, 6, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 5, 4, 4, 4, 8},
        {3, 1, 1, 1, 1, 1, 3, 3, 1, 7, 4, 4, 4, 4, 4, 4, 8, 1, 3, 3, 1, 1, 1, 1, 1, 3},
        {3, 1, 7, 4, 10, 1, 5, 6, 1, 5, 4, 4, 8, 7, 4, 4, 6, 1, 5, 6, 1, 9, 4, 8, 1, 3},
        {3, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 3},
        {3, 1, 5, 4, 4, 8, 1, 7, 4, 4, 8, 1, 3, 3, 1, 7, 4, 4, 8, 1, 7, 4, 4, 6, 1, 3},
        {3, 1, 1, 1, 1, 3, 1, 5, 4, 4, 6, 1, 5, 6, 1, 5, 4, 4, 6, 1, 3, 1, 1, 1, 1, 3},
        {5, 4, 4, 8, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 7, 4, 4, 6},
        {7, 4, 4, 6, 1, 11, 1, 7, 8, 1, 9, 4, 8, 7, 4, 10, 1, 7, 8, 1, 11, 1, 5, 4, 4, 8},
        {3, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 3, 3, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 3},
        {3, 2, 9, 4, 4, 4, 4, 6, 5, 4, 10, 1, 5, 6, 1, 9, 4, 6, 5, 4, 4, 4, 4, 10, 2, 3},
        {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},
        {5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 6}
};

// ================================== ESTRUTURAS ==================================

// DEFINE A ESTRUTURA DE UM PONTO DE DIREÇÃO
struct TPoint {
    int x, y;
};

// DEFINE AS POSSIBILIDADES DE DIREÇÕES
const struct TPoint DIRECTIONS[4] = {
        {1,0}, // RIGHT
        {0, 1}, // DOWN
        {-1,0}, // LEFT
        {0, -1} // UP
};

// DEFINE A ESTRURA DE UM VÉRTICE NO GRAFO
struct TVertex {
    int x, y; // POSIÇÕES DO VERTICE NO GRAFO
    int border[4]; // POSIÇÕES DE BORDA VO VERTICE NO GRAFO
};

// DEFINE A ESTRUTURA DO CENÁRIO DO JOGO
struct TScene {
    int map[MAP_SIZE][MAP_SIZE]; // MAPEAMENTO DO CENÁRIO -1 (OBSTACLE), 0 (FREE WAY), 1 (COIN), 2 (POWER)
    int coins; // MOEDAS/PASTILHAS COLETADAS
    int vertexCount; // QUANTIDADE DE VERTICES NO GRAFO
    struct TVertex *grafo; // GRAFO COM MAPEAMENTO DOS CAMINHOS NO GAME
};

// DEFINE A ESTRUTURA DO PACMAN
struct TPacman {
    int status; // INDICA O MODO DO PACMAN
    int xi, yi; // POSIÇÕES INCIAIS
    int x, y; // POSIÇÕES DINÂMICAS
    int direction, step, parcial; // DIREÇÃO, PASSO, PASSO PARCIAL
    int points; // PONTOS COLETADOS
    int power; //INDICA MODO INVENCIVEL
    int life; // INDICA A VITALIDADE
    int deadAnimation; // INDICA O STATUS DA ANIMAÇÃO DE DERROTA
};

// DEFINE A ESTRUTURA DO FANTASMA
struct TPhantom {
    int status; // INDICA O MODO DO FANTASMA (-1 MORTO, 0 CAPTURA, 1 FUGA, 2 PAUSA)
    int xi, yi; // POSIÇÕES INCIAIS
    int x, y; // POSIÇÕES DINÂMICAS
    int direction, step, parcial;
    int isCrossing; // INDICA DECISÃO DE EFETUAR CRUZAMENTO
    int isReturn; // INDICA DECIÃO DE EFETUAR RETORNO
    int *path; // MAPEAMENTO DO CAMINHO
    int indexCurrent; // INDICE ATUAL
        int id; // IDENTIFICAÇÃO DO FANTASMA
};

// ================================== FUNCIONALIDADES UTEIS ==================================

int createTexture(char* filename);
void drawSprite(GLuint texture, float x, float y);
void drawFullScreen(GLuint texture);
void buildSceneGrafo(Scene *scene);
int determineDirectionVertex(int map[MAP_SIZE][MAP_SIZE], int x, int y, int direction);
int checkCrossing(int x, int y, Scene *scene);
int checkPower(Pacman *pac);
int chekcDirectionGrafo(Phantom *ph, Scene *scene);
int checkProximityPacmanPhantom(Phantom *ph, Pacman *pac, Scene *scene, int direction);
int checkDirectionPhantomAlive(Phantom *ph, Pacman *pac, Scene *scene);
int checkDirectionPhantomDead(Phantom *ph, Scene *scene);
void collectPointsPhantom(Pacman *pac);
int generateRandomPhantomDirection(Phantom *ph, Scene *scene);
int calculateDistanceGrafo(Scene *scene, int nodeA, int nodeB);
void findMinPath(Phantom *ph, Scene *scene);
void movePhantomByDirection(Phantom *ph, int direction, Scene *scene);
void deadAnimeationPacman(Pacman *pac);
void gameOver(Pacman *pac);

// ================================== TEXTURAS ==================================

GLuint sceneTex2d[13];
GLuint pacmanTex2d[8];
GLuint phantomTex2d[20];
GLuint screenStart, screenGameOver;

// ================================== IMPLEMENTAÇÃO DAS FUNCIONALIDADES DE PACMAN.H ==================================

// CARREGA TODAS AS TEXTURAS NECESSARIAS (PACMAN, FANTASMAS, CENÁRIOS E INICIO E FIM DO JOGO)
void initSprites() {
    int i = 0;
    // CENÁRIO
    for(i = 0; i < 13; i++) {
        sceneTex2d[i] = createTexture(const_cast<char *>(SCENES_PATHS[i]));
    }
    // PACMAN
    for(i = 0; i < 12; i++) {
        pacmanTex2d[i] = createTexture(const_cast<char *>(PACMAN_PATHS[i]));
    }
    // FANTAMAS
    for(i = 0; i < 20; i++) {
        phantomTex2d[i] = createTexture(const_cast<char *>(PHANTOM_PATHS[i]));
    }
    // GAME START E GAME OVER
    screenStart = createTexture("../images/game-start.png");
    screenGameOver = createTexture("../images/game-over.png");
}

// CRIA UM NOVO CENARIO COM DADOS INICIAIS
Scene* generateScene() {
    int x, y, position;
    Scene *scene = static_cast<Scene *>(malloc(sizeof(Scene)));
    scene->coins = 0;
    for(x = 0; x < MAP_SIZE; x++) {
        for(y = 0; y < MAP_SIZE; y++) {
            position = SCENES_POSITION[y][x];
            scene->map[x][y] = position;
            if(position == 0) // INDICATIVO DE CAMINHO LIVRE
                scene->coins++;
        }
    }
    buildSceneGrafo(scene);
    return scene;
}

// RENDERIZA NA JANELA DE VISUALIZAÇÃO O CENARIO COM BASE NO MAPA DE POSIÇÕES
void drawScene(Scene *scene) {
    int line, column;
    for(line = 0; line < MAP_SIZE; line++) {
        for(column = 0; column < MAP_SIZE; column++) {
            drawSprite(sceneTex2d[scene->map[line][column]], line, column);
        }
    }
}

// LIBERAR MEMORIA ALOCADA PARA O CENARIO E/OU GRAFO
void destroyScene(Scene *scene) {
    if(scene->grafo != NULL)
        free(scene->grafo);
    free(scene);
}

// GERA O PACMAN COM DADOS INICIAIS
Pacman* createPacman(int x, int y) {
    Pacman *pac = static_cast<Pacman *>(malloc(sizeof(Pacman)));
    if(pac != NULL) {
        pac->power = NO_POWER;
        pac->points = 0;
        pac->step = STEP;
        pac->life = LIFE;
        pac->status = 0; //
        pac->direction = 0;
        pac->parcial = 0;
        pac->xi = x;
        pac->yi = y;
        pac->x = x;
        pac->y = y;
    }
    return pac;
}

// RENDERIZA NA JANELA DE VISUALIZAÇÃO O PACMAN COM BASE NAS SUAS CORDENADAS X, Y
void drawPacman(Pacman *pac) {
    if(pac->life != LIFE)
        return;
    drawSprite(pacmanTex2d[pac->status], pac->x, pac->y);
}

// LIBERAR MEMORIA ALOCADA PARA O PACMAN
void destroyPacman(Pacman *pac) {
    free(pac);
}

// ALTERAR DIREÇÃO DO PACMAN BASEADO NO DIREÇÃO RECEBIDA
void alterDirectionPacman(Pacman *pac, int direction) {
    pac->direction = direction;
    switch(direction){
        case RIGHT:
            pac->status = 0;
            break;
        case LEFT:
            pac->status = 2;
            break;
        case UP:
            pac->status = 4;
            break;
        case DOWN:
            pac->status = 6;
            break;
    }
}

// MOVIMENTA O PACMAN BASEADO NA DIREÇÃO ATUAL COLETA AS MOEDAS
void movePacman(Pacman *pac, Scene *scene) {
    int position;
    if(pac->life != LIFE)
        return;
    // VERIFICA CAMINHO LIVRE NA DIREÇÃO E MODA POSIÇÃO DO PACMAN
    position = scene->map[pac->x + DIRECTIONS[pac->direction].x][pac->y + DIRECTIONS[pac->direction].y];
    if(position == FREE_WAY || position == COIN_WAY || position == POWER_WAY) {
        pac->x += DIRECTIONS[pac->direction].x;
        pac->y += DIRECTIONS[pac->direction].y;
    }
    // COLETA MOEDA
    if(scene->map[pac->x][pac->y] == COIN_WAY) { // INDICATIVO DE COIN
        pac->points += 10;
        scene->coins--;
    }
    // COLETA PODER
    else if(scene->map[pac->x][pac->y] == POWER_WAY) { // INDICATIVO DE POWER
        pac->points += 50;
        pac->power = POWER;
        scene->coins--;
    }
    // REMOVE MOEADA DO MAPA
    scene->map[pac->x][pac->y] = FREE_WAY;
    // ALTERNA STATUS DO PACMAN (BOCA FECHADA/ABERTA)
    pac->status = (pac->status % 2 == 0) ? pac->status + 1 : pac->status - 1;
    // LOG DE PONTUAÇÃO
    printf("Pontos -> %d\n", pac->points);
    printf("Moedas -> %d\n", scene->coins);
}

// CHECAR O STATUS DA VITALIDADE DO PACMAN
int checkLifePacman(Pacman *pac) {
    if(pac->life == LIFE) {
        return 1;
    }
    else {
        return 0;
//        if(pac->deadAnimation > 60) {
//            return 0;
//        }
//        else {
//            return 1;
//        }
    }
}

// CRIAR UM NOVO FANSTASMA COM INFORMAÇÕES INICIAIS
Phantom* createPhantom(int x, int y, int id) {
    Phantom *ph = static_cast<Phantom *>(malloc(sizeof(Phantom)));
    if(ph != NULL) {
        ph->isCrossing = 0;
        ph->isReturn = 0;
        ph->indexCurrent = 0;
        ph->status = 0;
        ph->direction = 0;
        ph->path = NULL;
        ph->xi = x;
        ph->yi = y;
        ph->x = x;
        ph->y = y;
        ph->id = id;
    }
    return ph;
}

// RENDERIZA NA JANELA DE VISUALIZAÇÃO O FANTASMA COM BASE NAS SUAS CORDENADAS X, Y
void drawPhantom(Phantom *ph) {
    int idx = ph->id + ph->status;
    drawSprite(phantomTex2d[idx], ph->x, ph->y);
}

// LIBERAR MEMORIA ALOCADA PARA O FANTASMA
void destroyPhantom(Phantom *ph) {
    if(ph->path != NULL)
        free(ph->path);
    free(ph);
}

// COMPUTA A DIREÇÃO E VERIFICA O STATUS DO FANTASMA
void movePhantom(Phantom *ph, Scene *scene, Pacman *pac) {
    int direction;
    if(ph->status == DEAD) {
        direction = checkDirectionPhantomDead(ph, scene);
    }
    else {
        if(checkPower(pac))
            ph->status = 1; // INDICATIVO DE MODO DE FUGA
        else
            ph->status = 0; // INDICATIVO DE MODO DE CAPTURA
        direction = checkDirectionPhantomAlive(ph, pac, scene);
        if(pac->x == ph->x && pac->y == ph->y) {
//            if(checkPower(pac)) {
            if(0) {
                ph->status = DEAD; // INDICATIVO DE MORTE DO FANSTASMA
                ph->isReturn = 0;
                collectPointsPhantom(pac);
            }
            else if(checkLifePacman(pac)) gameOver(pac);
        }
    }
    movePhantomByDirection(ph, direction, scene);
}

// ================================== IMPLEMENTAÇÃO DAS FUNCIONALIDADES UTEIS ==================================

// GERA UMA NOVA TEXTURA COM BASE NO CAMINHO DA IMAGEN
int createTexture(char* filename) {
    int texture = SOIL_load_OGL_texture(filename,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    if (texture == 0) printf("ERROR: NOT FOUND IMAGE: %s\n", SOIL_last_result());
    return texture;
}

// RENDERIZA NA JANELA DE VISUALIZAÇÃO UMA UNIDADE DE SPRITE DO JOGO, INDICANDA A TEXTURA E SUA POSIÇÃO X, Y
void drawSprite(GLuint texture, float x, float y) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f + x, 0.0f + y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1.0f + x, 0.0f + y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1.0f + x, 1.0f + y);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0.0f + x, 1.0f + y);
    glEnd();
}

// RENDERIZA EM TELA CHEIA NA JANELA DE VISUALIZAÇÃO INIDICANDO A TEXTURA
void drawFullScreen(GLuint texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(26.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(26.0f, 26.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0.0f, 26.0f);
    glEnd();
}

// RENDERIZA A VISUALIZAÇÃO DE GAME START EM TELA CHEIA
void drawGameStart() {
    drawFullScreen(screenStart);
}

// RENDERIZA A VISUALIZAÇÃO DE GAME OVER EM TELA CHEIA
void drawGameOver() {
    drawFullScreen(screenGameOver);
}

// CONSTROI O GRAFO COM BASE NOS CRUZAMENTOS EXISTENTES NO MAPA (CADA CRUZAMENTO OU CURVA É UM VERTICE DO GRAFO)
void buildSceneGrafo(Scene *scene) {
    int x, y, k, position, idx, vertexCount = 0;
    int matrizAux[MAP_SIZE][MAP_SIZE];
    // CONSTROI MAPA AUXILIAR. CRUZAMENTO (POSIÇÃO), RETA (CAMINHO LIVRE), PAREDE (OBSTACULO)
    for(x = 0; x < MAP_SIZE; x++) {
        for(y = 0; y < MAP_SIZE; y++) {
            position = scene->map[x][y];
            if(position == FREE_WAY || position == COIN_WAY || position == POWER_WAY) { // INDICATIVO DE CAMINHO LIVRE
                if(checkCrossing(x, y, scene)) {
                    vertexCount++;
                    matrizAux[x][y] = vertexCount;
                }
                else {
                    matrizAux[x][y] = FREE_WAY;
                }
            }
            else {
                matrizAux[x][y] = OBSTACLE;
            }
        }
    }
    // DEFINE TAMANHO DO GRAFO E ALOCA A MEMORIA
    scene->vertexCount = vertexCount;
    scene->grafo = static_cast<TVertex *>(malloc(vertexCount * sizeof(struct TVertex)));
    // PREECHIMENTO DO GRAFO E POSIÇÕES DAS BORDAS
    for(x = 0; x < MAP_SIZE; x++) {
        for(y = 0; y < MAP_SIZE; y++) {
            if(matrizAux[x][y] > 0) { // INDICATIVO DE CRUZAMENTO
                idx = matrizAux[x][y] - 1;
                scene->grafo[idx].x = x;
                scene->grafo[idx].y = y;
                for(k = 0; k < 4; k++)
                    scene->grafo[idx].border[k] = determineDirectionVertex(matrizAux, x, y, k); // OBSTACLE, vertexCount
            }
        }
    }
}

// CHECA SE HÁ CRUZAMENTO OU CURVA NA POSSIÇÃO X, Y DETERMINADA
int checkCrossing(int x, int y, Scene *scene) {
    int i, position, vertexCount = 0;
    int border[4];
    for(i = 0; i < 4; i++) {
        position = scene->map[x + DIRECTIONS[i].x][y + DIRECTIONS[i].y];
        if(position == FREE_WAY || position == COIN_WAY || position == POWER_WAY) { // INDICATIVO DE CAMINHO LIVRE NA DIREÇÃO ESPECIFICADA
            border[i] = FREE_WAY; // DEFINIR CAMINHO LIVRE NA DIREÇÃO ESPECIFICADA
            vertexCount++; //
        }
        else {
            border[i] = OBSTACLE;
        }
    }
    if(vertexCount > 1) { // INDICATIVO DE CRUZAMENTO
        if(vertexCount == 2) {
            if((border[0] == border[2] && border[0] == FREE_WAY) || (border[1] == border[3] && border[1] == FREE_WAY)) // INDICATIVO DE RETA
                return 0; // RETA
            return 1; // CURVA
        }
        return 1; // CRUZAMENTO
    }
    return 0; // SEM SAÍDA
}

// MOVIMENTA O FASNTASMA ALTERANDO OS VALORES DA POSIÇÃO X, Y RELATIVA A ELE
void movePhantomByDirection(Phantom *ph, int direction, Scene *scene) {
    int position;
    int xi = ph->x;
    int yi = ph->y;

    position = scene->map[ph->x + DIRECTIONS[direction].x][ph->y + DIRECTIONS[direction].y];
    if(position == FREE_WAY || position == COIN_WAY || position == POWER_WAY) {
        if(ph->direction == direction) {
            ph->x += DIRECTIONS[direction].x;
            ph->y += DIRECTIONS[direction].y;
            ph->parcial = 0;
        }
        else {
            ph->direction = direction;
        }
    }

    if(xi != ph->x || yi != ph->y) // INDICATIVO QUE O FANTASMA NÃO ESTÁ NO CRUZAMENTO
        ph->isCrossing = 0;
}

// CHECA A INVECIBILIDADE DO PACMAN
int checkPower(Pacman *pac) {
    return pac->power == POWER;
}

// DETERMINA O VALOR DE CADA DIREÇÃO DE UM VERTICE. OBSTACULO (-1) OU CRUZAMENTO (N-1)
int determineDirectionVertex(int map[MAP_SIZE][MAP_SIZE], int x, int y, int direction) {
    int xi = x, yi = y;
    while(map[xi+DIRECTIONS[direction].x][yi+DIRECTIONS[direction].y] == FREE_WAY) { // INDICATIVO DE CAMINHO LIVRE
        xi += DIRECTIONS[direction].x;
        yi += DIRECTIONS[direction].y;
    }
    if(map[xi+DIRECTIONS[direction].x][yi+DIRECTIONS[direction].y] == OBSTACLE) // INDICATIVO DE OBSTACULO
        return OBSTACLE;
    else
        return map[xi+DIRECTIONS[direction].x][yi+DIRECTIONS[direction].y] - 1; // INDICATIVO DE SER OUTRO CRUZAMENTO NA DIREÇÃO
}

int chekcDirectionGrafo(Phantom *ph, Scene *scene) {
    if(scene->grafo[ph->indexCurrent].x == scene->grafo[ph->path[ph->indexCurrent]].x) {
        if(scene->grafo[ph->indexCurrent].y > scene->grafo[ph->path[ph->indexCurrent]].y)
            return 3;
        else
            return 1;
    }
    else {
        if(scene->grafo[ph->indexCurrent].x > scene->grafo[ph->path[ph->indexCurrent]].x)
            return 2;
        else
            return 0;
    }
}

int checkProximityPacmanPhantom(Phantom *ph, Pacman *pac, Scene *scene, int direction) {
    int flag = 0;
    if(direction == 0 || direction == 2) {
        if(pac->x == ph->x)
            flag = 1;
    }
    else {
        if (pac->y == ph->y)
            flag = 1;
    }
    if(flag) {
        int xi = ph->x;
        int yi = ph->y;
        while(scene->map[yi + DIRECTIONS[direction].y][xi + DIRECTIONS[direction].x] <= 2) {
            xi += DIRECTIONS[direction].x;
            yi += DIRECTIONS[direction].y;
            if(xi == pac->x && yi == pac->y)
                return 1;
        }
    }
    return 0;
}

int checkDirectionPhantomAlive(Phantom *ph, Pacman *pac, Scene *scene) {
    int direction, i;
    if(checkCrossing(ph->x, ph->y, scene)) {
        if(!ph->isCrossing) {
            direction = -1;
            for(i = 0; i < 4; i++)
                if(checkProximityPacmanPhantom(ph, pac, scene, direction))
                    direction = i;
            if(direction == -1)
                direction = generateRandomPhantomDirection(ph, scene);
            else
            if(checkPower(pac)) {
                i = direction;
                while(i == direction)
                    direction = generateRandomPhantomDirection(ph, scene);
            }
            ph->isCrossing = 1;
        }
        else
            direction = ph->direction;
    }
    else {
        ph->isCrossing = 0;
        direction = ph->direction;
        if(checkPower(pac) && checkProximityPacmanPhantom(ph, pac, scene, direction))
            direction = (direction + 2) % 4;
        if(scene->map[ph->x + DIRECTIONS[direction].x][ph->y + DIRECTIONS[direction].y] > 2)
            direction = (direction + 2) % 4;
    }
    return direction;
}

int checkDirectionPhantomDead(Phantom *ph, Scene *scene) {
    int direction, position;
    if(!ph->isReturn) {
        if(checkCrossing(ph->x, ph->y, scene)) {
            ph->isCrossing = 1;
            ph->isReturn = 1;
            findMinPath(ph, scene);
            direction = chekcDirectionGrafo(ph, scene);
        }
        else {
            direction = ph->direction;
            position = scene->map[ph->x + DIRECTIONS[direction].x][ph->y + DIRECTIONS[direction].y];
            if(position != FREE_WAY && position != COIN_WAY && position != POWER_WAY) // INDICATIVO DE OBSTACULO
                direction = (direction + 2) % 4; // RETORNA REFERENTE A DIREÇÃO ATUAL
        }
    }
    else {
        if(ph->x != ph->xi || ph->y != ph->yi) {
            if(checkCrossing(ph->x, ph->y, scene)) {
                if(ph->isCrossing)
                    direction = ph->direction;
                else {
                    ph->indexCurrent = ph->path[ph->indexCurrent];
                    direction = chekcDirectionGrafo(ph, scene);
                    ph->isCrossing = 1;
                }
            }
            else {
                direction = ph->direction;
                ph->isCrossing = 0;
            }
        }
        else {
            ph->status = 0;
            direction = ph->direction;
        }
    }
    return direction;
}

// PACMAN RECEBE MAIS 100 PONTOS COM A MORTE DO FANTASMA
void collectPointsPhantom(Pacman *pac) {
    pac->points += 100;
}

int generateRandomPhantomDirection(Phantom *ph, Scene *scene) {
    int i, j, k, max;
    int weight[4], border[4];
    for(i = 0; i < 4; i++)
        weight[i] = (rand() % 10 + 1);
    weight[ph->direction] = 7;
    weight[(ph->direction + 2) % 4] = 3;
    for(i = 0; i < 4; i++) {
        max = 0;
        for(j = 0; j < 4; j++) {
            if(weight[j] > max) {
                max = weight[j];
                k = j;
            }
        }
        border[i] = k;
        weight[k] = 0;
    }
    i = 0;
    while(scene->map[ph->x + DIRECTIONS[border[i]].x][ph->y + DIRECTIONS[border[i]].y] > 2)
        i++;

    return border[i];
}

void findMinPath(Phantom *ph, Scene *scene) {
    int i, k, indexNode, steps;
    int flag;
    int *distance = static_cast<int *>(malloc(scene->vertexCount * sizeof(int)));
    if(ph->path == NULL)
        ph->path = static_cast<int *>(malloc(scene->vertexCount * sizeof(int)));
    for(i = 0; i < scene->vertexCount; i++) {
        distance[i] = 10000;
        ph->path[i] = -1;
        if(scene->grafo[i].x == ph->xi && scene->grafo[i].y == ph->yi)
            indexNode = i;
        if(scene->grafo[i].x == ph->x && scene->grafo[i].y == ph->y)
            ph->indexCurrent = i;
    }
    distance[indexNode] = 0;
    flag = 1;
    while(flag) {
        flag = 0;
        for(i = 0; i < scene->vertexCount; i++) {
            for(k = 0; k < 4; k++) {
                if(scene->grafo[i].border[k] >= 0) { // INDICATIVO QUE NÃO É OBSTACULO
                    steps = calculateDistanceGrafo(scene, i, scene->grafo[i].border[k]);
                    if(distance[scene->grafo[i].border[k]] > (distance[i] + steps)) {
                        distance[scene->grafo[i].border[k]] = (distance[i] + steps);
                        ph->path[scene->grafo[i].border[k]] = 1;
                        flag = 1;
                    }
                }
            }
        }
    }
}

// CALCULA E RETORNA A DISTANCIA ENTRE DOIS PONTOS NO GRAFO (AUXILIA NA ESCOLHA DO CAMINHO QUANDO FANTASMA MORRE)
int calculateDistanceGrafo(Scene *scene, int nodeA, int nodeB) {
    return fabs(scene->grafo[nodeA].x - scene->grafo[nodeB].x) + fabs(scene->grafo[nodeA].y - scene->grafo[nodeB].y);
}

// PACMAN MORRE A INICIA ANIMAÇÃO DE MORTE
void gameOver(Pacman *pac) {
    if(pac->life == LIFE) {
        pac->life = DEAD;
        drawGameOver();
//        pac->deadAnimation = 0;
//        deadAnimeationPacman(pac);
    }
}

//void deadAnimeationPacman(Pacman *pac) {
//    switch(pac->deadAnimation) {
//        case 0:
//            drawSprite(pacmanTex2d[8], pac->x, pac->y);
//            break;
//        case 1:
//            drawSprite(pacmanTex2d[9], pac->x, pac->y);
//            break;
//        case 2:
//            drawSprite(pacmanTex2d[10], pac->x, pac->y);
//            break;
//        case 3:
//            drawSprite(pacmanTex2d[11], pac->x, pac->y);
//            break;
//        case 4:
//            drawGameOver();
//            break;
//    }
//    pac->deadAnimation++;
//}