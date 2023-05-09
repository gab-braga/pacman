#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <SOIL.h>

const int SIZE_MAP = 26;
const int SIZE_PAC_MODE = 8;

int pac_state = 0;
int x_pacman = 13.0;
int y_pacman = 15.0;
int direction = 0; // 0 - right / 1 - left / 2 - top / 3 - down

GLint map[SIZE_MAP][SIZE_MAP];
GLint pacman[SIZE_PAC_MODE];
int grafo[SIZE_MAP][SIZE_MAP];

int position = 0;
int x_ax = 0;
int y_ax = 0;

const char* pacman_paths[SIZE_PAC_MODE] = {
        "../images/pacman-1.png",
        "../images/pacman-2.png",
        "../images/pacman-3.png",
        "../images/pacman-4.png",
        "../images/pacman-5.png",
        "../images/pacman-6.png",
        "../images/pacman-7.png",
        "../images/pacman-8.png"
};

const char* scene_paths[13] = {
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

const int scenes_positions_map[SIZE_MAP][SIZE_MAP] = {
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
        {0, 0, 0, 0, 0, 0, 5, 6, 0, 7, 4, 10, 0, 0, 9, 4, 8, 0, 5, 6, 0, 0, 0, 0, 0, 0},
        {4, 4, 4, 4, 8, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 7, 4, 4, 4, 4},
        {0, 0, 0, 0, 3, 0, 7, 8, 0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 7, 8, 0, 3, 0, 0, 0, 0},
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

void initGrafo() {
    for(x_ax = 0; x_ax < SIZE_MAP; x_ax++) {
        for(y_ax = 0; y_ax < SIZE_MAP; y_ax++) {
            position = scenes_positions_map[x_ax][y_ax];
            if(position == 0) {
                grafo[y_ax][x_ax] = 0;
            }
            else {
                grafo[y_ax][x_ax] = -1;
            }
        }
    }
}

int generateTexture(char* filename) {
    int texture = SOIL_load_OGL_texture(filename,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_NTSC_SAFE_RGB);
    if (texture == 0) printf("Erro ao carregar a imagem: %s\n", SOIL_last_result());
    return texture;
}

void initSprites() {
    // Scenes
    for(x_ax = 0; x_ax < SIZE_MAP; x_ax++) {
        for(y_ax = 0; y_ax < SIZE_MAP; y_ax++) {
            position = scenes_positions_map[x_ax][y_ax];
            map[y_ax][x_ax] = generateTexture(const_cast<char *>(scene_paths[position]));
        }
    }
    // Pacman
    for(x_ax = 0; x_ax < SIZE_PAC_MODE; x_ax++)  {
        pacman[x_ax] = generateTexture(const_cast<char *>(pacman_paths[x_ax]));
    }
}

void drawTexture(GLint texture, float x, float y) {
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

void draw_scenes() {
    for(x_ax = 0; x_ax < SIZE_MAP; x_ax++) {
        for(y_ax = 0; y_ax < SIZE_MAP; y_ax++) {
            drawTexture(map[x_ax][y_ax], x_ax, y_ax);
        }
    }
}

void draw_pacman() {
    drawTexture(pacman[pac_state], x_pacman, y_pacman);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    draw_scenes();
    draw_pacman();
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

void alter_direction(int key, int x, int y) {
    switch(key){
        case GLUT_KEY_RIGHT:
            direction = 0; break;
        case GLUT_KEY_LEFT:
            direction = 1; break;
        case GLUT_KEY_UP:
            direction = 2; break;
        case GLUT_KEY_DOWN:
            direction = 3; break;
    }
}

void move_pacman(int value) {
    switch (direction) {
        case 0:
            if(grafo[x_pacman+1][y_pacman] == 0) x_pacman += 1;
            break;
        case 1:
            if(grafo[x_pacman-1][y_pacman] == 0) x_pacman -= 1;
            break;
        case 2:
            if(grafo[x_pacman][y_pacman-1] == 0) y_pacman -= 1;
            break;
        case 3:
            if(grafo[x_pacman][y_pacman+1] == 0) y_pacman += 1;
            break;
    }
    pac_state = pac_state == 0 ? 1 : 0;
    glutPostRedisplay();
    glutTimerFunc(150, move_pacman, 1);
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(600, 600);
    glutCreateWindow("PACMAN");
    gluOrtho2D(0.0, 26.0, 26.0, 0.0);
    initGrafo();
    initSprites();
    glutDisplayFunc(display);
    glutSpecialFunc(alter_direction);
    glutTimerFunc(3000, move_pacman, 1);
//    init();
    glutMainLoop();
    return 0;
}