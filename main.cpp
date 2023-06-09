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

int mode = FAILED;

void display() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    drawScene();
    if(mode == 0) drawGameStart();
    if(mode == 2) drawGameOver();
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (mode != PLAYING && (key == 80 || key == 112)) {
        switch (mode) {
            case STARTING:
                mode = PLAYING;
                break;
            case FAILED:
                mode = STARTING;
                break;
        }
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutCreateWindow("PACMAN");
    initSprites();
    gluOrtho2D(0.0, 26.0, 26.0, 0.0);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
//    glutTimerFunc(...., ..., 1);
    glutMainLoop();
    return 0;
}