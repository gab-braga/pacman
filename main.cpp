#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include "pacman.h"
#include <stdio.h>

#define WINDOW_SIZE 600
#define MAP_SIZE 26
#define STARTING 0
#define PLAYING 1
#define FAILED 2
#define VITORY 3

#define RIGHT 0
#define DOWN  1
#define LEFT  2
#define UP   3

#define PH_ORANGE 0
#define PH_PINK 4
#define PH_CYAN 8
#define PH_RED 12
#define PH_GRAY 16

int mode = STARTING;
Scene *scene = NULL;
Pacman *pacman = NULL;
Phantom *phOrange = NULL;

void display() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    if(scene != NULL) drawScene(scene);
    if(pacman != NULL) drawPacman(pacman);
    if(phOrange != NULL) drawPhantom(phOrange);
    if(mode == STARTING) drawGameStart();
    if(mode == FAILED) drawGameOver();
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

void playGame() {
    scene = generateScene();
    pacman = createPacman(12, 5);
    phOrange = createPhantom(11, 14, PH_ORANGE);
}

void endGame() {
    drawGameOver();
//    destroyScene(scene);
//    destroyPacman(pacman);
//    destroyPhantom(phOrange);
}

void keyboard(unsigned char key, int x, int y) {
    if ((mode == STARTING || mode == FAILED) && (key == 80 || key == 112)) {
        switch (mode) {
            case STARTING:
                mode = PLAYING;
                playGame();
                break;
            case FAILED:
                mode = STARTING;
                break;
        }
        glutPostRedisplay();
    }
}

void keyboardSpecial(int key, int x, int y) {
    switch(key){
        case GLUT_KEY_RIGHT:
            alterDirectionPacman(pacman, RIGHT);
            break;
        case GLUT_KEY_LEFT:
            alterDirectionPacman(pacman, LEFT);
            break;
        case GLUT_KEY_UP:
            alterDirectionPacman(pacman, UP);
            break;
        case GLUT_KEY_DOWN:
            alterDirectionPacman(pacman, DOWN);
            break;
    }
}

void timer(int value) {
    if(pacman != NULL && scene != NULL && mode == PLAYING) {
        if(checkLifePacman(pacman)) {
            movePacman(pacman, scene);
            movePhantom(phOrange, scene, pacman);
        }
        else {
            mode = FAILED;
        }
        glutPostRedisplay();
    }
    glutTimerFunc(200, timer, 1);
}

void init() {
    initSprites();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutCreateWindow("PACMAN");
    init();
    gluOrtho2D(0.0, 26.0, 26.0, 0.0);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(200, timer, 1);
    glutSpecialFunc(keyboardSpecial);
    glutMainLoop();
    return 0;
}