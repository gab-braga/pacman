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
#define FREE 0
#define SALT 30

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
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {3, 0, 7, 4, 8, 0, 7, 4, 4, 4, 8, 0, 3, 3, 0, 7, 4, 4, 4, 8, 0, 7, 4, 8, 0, 3},
        {3, 0, 3, 0, 3, 0, 3, 0, 0, 0, 3, 0, 3, 3, 0, 3, 0, 0, 0, 3, 0, 3, 0, 3, 0, 3},
        {3, 0, 5, 4, 6, 0, 5, 4, 4, 4, 6, 0, 5, 6, 0, 5, 4, 4, 4, 6, 0, 5, 4, 6, 0, 3},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {3, 0, 9, 4, 10, 0, 7, 8, 0, 9, 4, 4, 8, 7, 4, 4, 10, 0, 7, 8, 0, 9, 4, 10, 0, 3},
        {3, 0, 0, 0, 0,  0, 3, 3, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 3},
        {5, 4, 4, 4, 8, 0, 3, 5, 4, 4, 8, 0, 3, 3, 0, 7, 4, 4, 6, 3, 0, 7, 4, 4, 4, 6},
        {0, 0, 0, 0, 3, 0, 3, 7, 4, 4, 6, 0, 5, 6, 0, 5, 4, 4, 8, 3, 0, 3, 0, 0, 0, 0},
        {4, 4, 4, 4, 6, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 5, 4, 4, 4, 4},
        {0, 0, 0, 0, 0, 0, 5, 6, 0, 9, 10, 0, 0, 0, 0, 9, 10, 0, 5, 6, 0, 0, 0, 0, 0, 0},
        {4, 4, 4, 4, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 4, 4, 4, 4},
        {0, 0, 0, 0, 3, 0, 7, 8, 0, 7, 4, 4, 4, 4, 4, 4, 8, 0, 7, 8, 0, 3, 0, 0, 0, 0},
        {7, 4, 4, 4, 6, 0, 3, 3, 0, 5, 4, 4, 4, 4, 4, 4, 6, 0, 3, 3, 0, 5, 4, 4, 4, 8},
        {3, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 3},
        {3, 0, 7, 4, 10, 0, 5, 6, 0, 9, 4, 4, 8, 7, 4, 4, 10, 0, 5, 6, 0, 9, 4, 8, 0, 3},
        {3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 3},
        {3, 0, 5, 4, 4, 8, 0, 7, 4, 4, 8, 0, 3, 3, 0, 7, 4, 4, 8, 0, 7, 4, 4, 6, 0, 3},
        {3, 0, 0, 0, 0, 3, 0, 5, 4, 4, 6, 0, 5, 6, 0, 5, 4, 4, 6, 0, 3, 0, 0, 0, 0, 3},
        {5, 4, 4, 8, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 7, 4, 4, 6},
        {7, 4, 4, 6, 0, 11, 0, 7, 8, 0, 9, 4, 8, 7, 4, 10, 0, 7, 8, 0, 11, 0, 5, 4, 4, 8},
        {3, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 3, 3, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 3},
        {3, 0, 9, 4, 4, 4, 4, 6, 5, 4, 10, 0, 5, 6, 0, 9, 4, 6, 5, 4, 4, 4, 4, 10, 0, 3},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
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
        {0, 1}, // TOP
        {-1,0}, // LEFT
        {0, -1} // DOWN
};

// DEFINE A ESTRURA DE UM VÉRTICE NO GRAFO
struct TVertex {
    int x, y; // POSIÇÕES DO VERTICE NO GRAFO
    int border[4]; // POSIÇÕES DE BORDA VO VERTICE NO GRAFO
};

// DEFINE A ESTRUTURA DO CENÁRIO DO JOGO
struct TScene {
    int map[MAP_SIZE][MAP_SIZE]; // MAPEAMENTO DO CENÁRIO (...)
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
    int status; // INDICA O MODO DO FANTASMA (0, 1 MORTO, 2)
    int xi, yi; // POSIÇÕES INCIAIS
    int x, y; // POSIÇÕES DINÂMICAS
    int direction, step, parcial;
    int isCrossing; // INDICA DECISÃO DE EFETUAR CRUZAMENTO
    int isReturn; // INDICA DECIÃO DE EFETUAR RETORNO
    int *path; // MAPEAMENTO DO CAMINHO
    int indexCurrent; // INDICE ATUAL
        int id; // IDENTIFICAÇÃO DO FANTASMA
};

// ================================== FUNCIONALIDADES ==================================

int createTexture(char* filename);
void drawSprite(GLuint texture, float x, float y);
void drawFullScreen(GLuint texture);
void buildSceneGrafo(Scene *scene);
int checkPower(Pacman *pac);
int checkCrossing(int x, int y, Scene *scene);
int checkDirection(int map[MAP_SIZE][MAP_SIZE], int x, int y, int direction);
int chekcDirectionGrafo(Phantom *ph, Scene *scene);
int checkProximityPacmanPhantom(Phantom *ph, Pacman *pac, Scene *scene, int direction);
int checkDirectionPhantomAlive(Phantom *ph, Pacman *pac, Scene *scene);
int checkDirectionPhantomDead(Phantom *ph, Scene *scene);
void collectPointsPhantom(Pacman *pac);
int generateRandomPhantomDirection(Phantom *ph, Scene *scene);
int calculateDistanceGrafo(Scene *scene, int nodeA, int nodeB);
void findMinPath(Phantom *ph, Scene *scene);
void verifyDirectionPhantom(Phantom *ph, Scene *scene, Pacman *pac);
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
            if(position == 0) {
                scene->map[x][y] = FREE; // DEFINE CAMINHO LIVRE
                scene->coins++;
            }
            else {
                scene->map[y][x] = OBSTACLE; // DEFINE OBSTACULO
            }
        }
    }
    buildSceneGrafo(scene);
    return scene;
}

// RENDERIZA NA JANELA DE VISUALIZAÇÃO O CENARIO COM BASE NO MAPA DE POSIÇÕES
void drawScene() {
    int line, column;
    for(line = 0; line < MAP_SIZE; line++) {
        for(column = 0; column < MAP_SIZE; column++) {
            drawSprite(sceneTex2d[SCENES_POSITION[column][line]], line, column);
        }
    }
}

// LIBERAR MEMORIA ALOCADA PARA O CENARIO
void destroyScene(Scene *scene) {
    if(scene->grafo != NULL)
        free(scene->grafo);
    free(scene);
}

Pacman* createPacman(int x, int y) {
    Pacman *pac = static_cast<Pacman *>(malloc(sizeof(Pacman)));
    if(pac != NULL) {
        pac->power = 0;
        pac->points = 0;
        pac->step = 4;
        pac->life = 1;
        pac->status = 0;
        pac->direction = 0;
        pac->parcial = 0;
        pac->xi = x;
        pac->yi = y;
        pac->x = x;
        pac->y = y;
    }
    return pac;
}

void drawPacman(Pacman *pac) {
    float line, column;
    float step = pac->parcial / SALT;
    if(pac->direction == 0 || pac->direction == 2) {
        line = pac->x + step;
        column = pac->y;
    }
    else {
        line = pac->x;
        column = pac->y + step;
    }

    if(pac->life) {
        int idx = 2 * pac->direction;
        if(pac->status = 0) {
            // desenha boca fechada
        }
        else {
            // desenha boca fechada
        }
        pac->status = (pac->status % 2 == 0) ? pac->status+1 : pac->status-1;
        drawSprite(pacmanTex2d[pac->status], line, column);
    }
}

void destroyPacman(Pacman *pac) {
    free(pac);
}

void alterDirectionPacman(Pacman *pac, int direction, Scene *scene) {
    if(scene->map[pac->x + DIRECTIONS[direction].x][pac->y + DIRECTIONS[direction].y] == 0) {
        int di = abs(direction - pac->direction);
        if(di != 2 && di != 0) {
            pac->parcial = 0;
        }
        pac->direction = direction;
    }
}

void movePacman(Pacman *pac, Scene *scene) {
    if(pac->life == 0)
        return;

    if(scene->map[pac->x + DIRECTIONS[pac->direction].x][pac->y + DIRECTIONS[pac->direction].y] == 0) {
        if(pac->direction < 2) {
            pac->parcial += pac->step;
            if (pac->parcial >= SALT) {
                pac->x = DIRECTIONS[pac->direction].x;
                pac->y = DIRECTIONS[pac->direction].y;
                pac->parcial = 0;
            }
        }
        else {
            pac->parcial -= pac->step;
            if(pac->parcial <= -SALT) {
                pac->x += DIRECTIONS[pac->direction].x;
                pac->y += DIRECTIONS[pac->direction].y;
                pac->parcial = 0;
            }
        }
    }

    if(scene->map[pac->x][pac->y] == 1) {
        pac->points += 10;
        scene->coins--;
    }
    if(scene->map[pac->x][pac->y] == 2) {
        pac->points += 50;
        pac->power = 1000;
        scene->coins--;
    }
    scene->map[pac->x][pac->y] = 0;
}

int checkLifePacman(Pacman *pac) {
    if(pac->life) {
        return 1;
    }
    else {
        if(pac->deadAnimation > 60) {
            return 0;
        }
        else {
            return 1;
        }
    }
}

Phantom* createPhantom(int x, int y, int id) {
    Phantom *ph = static_cast<Phantom *>(malloc(sizeof(Phantom)));
    if(ph != NULL) {
        ph->step = 3;
        ph->isCrossing = 0;
        ph->isReturn = 0;
        ph->indexCurrent = 0;
        ph->status = 0;
        ph->direction = 0;
        ph->parcial = 0;
        ph->path = NULL;
        ph->xi = x;
        ph->yi = y;
        ph->x = x;
        ph->y = y;
        ph->id = id;
    }
    return ph;
}

void drawPhantom(Phantom *ph) {
    float line, column;
    float step = ph->parcial / SALT;
    if(ph->direction == 0 || ph->direction == 2) {
        line = ph->x;
        column = ph->y + step;
    }
    else {
        line = ph->x + step;
        column = ph->y;
    }
    int idx = ph->id + ph->status;
    drawSprite(phantomTex2d[idx], line, column);
}

void destroyPhantom(Phantom *ph) {
    if(ph->path != NULL)
        free(ph->path);
    free(ph);
}

void movePhantom(Phantom *ph, int direction, Scene *scene) {
    int xi = ph->x;
    int yi = ph->y;

    if(xi != ph->x || yi != ph->y)
        ph->isCrossing = 0;

    if(scene->map[ph->x + DIRECTIONS[direction].x][ph->y + DIRECTIONS[direction].y] == 0) {
        if(direction == ph->direction) {
            if(ph->direction < 2) {
                ph->parcial += ph->step;
                if(ph->parcial >= SALT) {
                    ph->x += DIRECTIONS[direction].x;
                    ph->y += DIRECTIONS[direction].y;
                    ph->parcial = 0;
                }
            }
            else {
                ph->parcial -= ph->step;
                if(ph->parcial <= -SALT) {
                    ph->x += DIRECTIONS[direction].x;
                    ph->y += DIRECTIONS[direction].y;
                    ph->parcial = 0;
                }
            }
        }
        else {
            if(abs(direction - ph->direction) != 2)
                ph->parcial = 0;
            ph->direction = direction;
        }
    }
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

void buildSceneGrafo(Scene *scene) {
    int i, x, y, k, idx, count;
    int mat[MAP_SIZE][MAP_SIZE];
    for(x = 0; x < MAP_SIZE-1; x++) {
        for(y = 0; y < MAP_SIZE-1; y++) {
            if(scene->map[x][y] == -1) { // INDICATIVO DE OBSTACULO
                if(checkCrossing(x, y, scene)) {
                    count++;
                    mat[x][y] = count;
                }
                else {
                    mat[x][y] = 0;
                }
            }
            else {
                mat[x][y] = -1;
            }
        }
    }

    for(i=0; i < MAP_SIZE; i++){
        mat[0][i] = -1;
        mat[i][0] = -1;
        mat[MAP_SIZE-1][i] = -1;
        mat[i][MAP_SIZE-1] = -1;
    }

    scene->vertexCount = count;
    scene->grafo = static_cast<TVertex *>(malloc(count * sizeof(struct TVertex)));

    for(x = 0; x < MAP_SIZE-1; x++) {
        for(y = 0; y < MAP_SIZE-1; y++) {
            if(mat[x][y] > 0) {
                idx = mat[x][y] - 1;
                scene->grafo[idx].x = x;
                scene->grafo[idx].x = y;
            }
        }
    }
}

int checkPower(Pacman *pac) {
    return pac->power > 0;
}

int checkCrossing(int x, int y, Scene *scene) {
    int i, count = 0;
    int border[4];
    for(i = 0; i < 4; i++) {
        if(scene->map[x + DIRECTIONS[i].x][y + DIRECTIONS[i].y] == -1) { // INDICATIVO DE OBSTACULO NA DIREÇÃO ESPECIFICADA
            border[i] = -1; // DEFINIR OBSTACULO NA DIREÇÃO ESPECIFICADA
            count++; //
        }
        else {
            border[i] = 0;
        }
    }
    if(count > 1) {
        if(count == 2) {
            if((border[0] == border[2] && border[0]) || (border[1] == border[3] && border[1]))
                return 0;
            return 1;
        }
        return 1;
    }
    return 0;
}

int checkDirection(int map[MAP_SIZE][MAP_SIZE], int x, int y, int direction) {
    int xi = x, yi = y;
    while(map[xi+DIRECTIONS[direction].x][yi+DIRECTIONS[direction].y] == 0) {
        xi += DIRECTIONS[direction].x;
        yi += DIRECTIONS[direction].y;
    }
    if(map[xi+DIRECTIONS[direction].x][yi+DIRECTIONS[direction].y])
        return -1;
    else
        return map[xi+DIRECTIONS[direction].x][yi+DIRECTIONS[direction].y] - 1;
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
    int direction;
    if(!ph->isReturn) {
        if(checkCrossing(ph->x, ph->y, scene)) {
            ph->isReturn = 1;
            findMinPath(ph, scene);
            ph->isCrossing = 1;
            direction = chekcDirectionGrafo(ph, scene);
        }
        else {
            direction = ph->direction;
            if(scene->map[ph->x + DIRECTIONS[direction].x][ph->y + DIRECTIONS[direction].y] > 2)
                direction = (direction + 2) % 4;
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

int calculateDistanceGrafo(Scene *scene, int nodeA, int nodeB) {
    return fabs(scene->grafo[nodeA].x - scene->grafo[nodeB].x) + fabs(scene->grafo[nodeA].y - scene->grafo[nodeB].y);
}

void findMinPath(Phantom *ph, Scene *scene) {
    int i, k, indexNode, direction;
    int flag;
    int *distance = static_cast<int *>(malloc(scene->vertexCount * sizeof(int)));
    if(ph->path == NULL)
        ph->path = static_cast<int *>(malloc(scene->vertexCount * sizeof(int)));
    for(i = 0; i < scene->vertexCount; i++) {
        distance[i] = 10000;
        ph->path[i] = -1;
        if(scene->grafo[i].x == ph->xi && scene->grafo[i].y == ph->yi)
            indexNode = i;
        if(scene->grafo[i].x == ph->xi && scene->grafo[i].y == ph->yi)
            ph->indexCurrent = i;
    }
    distance[indexNode] = 0;
    flag = 1;
    while(flag) {
        flag = 0;
        for(i = 0; i < scene->vertexCount; i++) {
            for(k = 0; k < 4; k++) {
                if(scene->grafo[i].border[k] >= 0) {
                    direction = calculateDistanceGrafo(scene, i, scene->grafo[i].border[k]);
                    if(distance[scene->grafo[i].border[k]] > (distance[i] + direction)) {
                        distance[scene->grafo[i].border[k]] = (distance[i] + direction);
                        ph->path[scene->grafo[i].border[k]] = 1;
                        flag = 1;
                    }
                }
            }
        }
    }
}

void verifyDirectionPhantom(Phantom *ph, Scene *scene, Pacman *pac) {
    int direction;
    if(ph->status == 1) {
        direction = checkDirectionPhantomAlive(ph, pac, scene);
    }
    else {
        if(checkPower(pac))
            ph->status = 2;
        else
            ph->status = 0;
        direction = checkDirectionPhantomDead(ph, scene);
        if(pac->x == ph->x && pac->y == ph->y) {
            if(checkPower(pac)) {
                ph->status = 1;
                collectPointsPhantom(pac);
                ph->isReturn = 0;
            }
            else
            if(checkLifePacman(pac))
                gameOver(pac);
        }
    }
    movePhantom(ph, direction, scene);
}

void deadAnimeationPacman(Pacman *pac) {
    switch(pac->deadAnimation) {
        case 0:
            drawSprite(pacmanTex2d[8], pac->x, pac->y);
            break;
        case 1:
            drawSprite(pacmanTex2d[9], pac->x, pac->y);
            break;
        case 2:
            drawSprite(pacmanTex2d[10], pac->x, pac->y);
            break;
        case 3:
            drawSprite(pacmanTex2d[11], pac->x, pac->y);
            break;
        case 4:
            drawGameOver();
            break;
    }
    pac->deadAnimation++;
}

void gameOver(Pacman *pac) {
    if(pac->life) {
        pac->life = 0;
        pac->deadAnimation = 0;
        deadAnimeationPacman(pac);
    }
}