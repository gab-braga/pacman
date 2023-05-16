#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <SOIL.h>
#include <ctime>

const int WINDOW_SIZE = 600;

const int SIZE_MAP = 26;
const int SIZE_PAC_MODE = 8;

const int D_RIGHT = 0;
const int D_LEFT  = 1;
const int D_UP    = 2;
const int D_DOWN  = 3;

const int PAC_STATE_RIGHT = 0;
const int PAC_STATE_LEFT  = 2;
const int PAC_STATE_UP    = 4;
const int PAC_STATE_DOWN  = 6;

const int N_GHOSTS = 4;

const int MOVE_SIZE = 1;

int pac_state = 0;
int x_pacman = 13;
int y_pacman = 15;
int direction = D_RIGHT;

int x_ghosts[N_GHOSTS]      = { 11, 12, 13, 14 };
int y_ghosts[N_GHOSTS]      = { 13, 13, 13, 13 };
int direc_ghosts[N_GHOSTS]  = { D_UP, D_UP, D_UP, D_UP };
int ghosts_states[N_GHOSTS] = { 0, 0, 0, 0 };

GLint map[SIZE_MAP][SIZE_MAP];
GLint pacman[SIZE_PAC_MODE];
GLint ghost[20];
int grafo[SIZE_MAP][SIZE_MAP];

int position = 0;
int x_ax = 0;
int y_ax = 0;
int gh_index = 0;

int index_random = 0;

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

const char* ghost_paths[20] = {
        "../images/ghost-1.png",
        "../images/ghost-2.png",
        "../images/ghost-3.png",
        "../images/ghost-4.png",
        "../images/ghost-5.png",
        "../images/ghost-6.png",
        "../images/ghost-7.png",
        "../images/ghost-8.png",
        "../images/ghost-9.png",
        "../images/ghost-10.png",
        "../images/ghost-11.png",
        "../images/ghost-12.png",
        "../images/ghost-13.png",
        "../images/ghost-14.png",
        "../images/ghost-15.png",
        "../images/ghost-16.png",
        "../images/ghost-17.png",
        "../images/ghost-18.png",
        "../images/ghost-19.png",
        "../images/ghost-20.png"
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

void init_grafo() {
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

int create_texture(char* filename) {
    int texture = SOIL_load_OGL_texture(filename,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_NTSC_SAFE_RGB);
    if (texture == 0) printf("ERROR: NOT FOUND IMAGE: %s\n", SOIL_last_result());
    return texture;
}

void init_sprites() {
    // Scenes
    for(x_ax = 0; x_ax < SIZE_MAP; x_ax++) {
        for(y_ax = 0; y_ax < SIZE_MAP; y_ax++) {
            position = scenes_positions_map[x_ax][y_ax];
            map[y_ax][x_ax] = create_texture(const_cast<char *>(scene_paths[position]));
        }
    }
    // Pac-Man
    for(x_ax = 0; x_ax < SIZE_PAC_MODE; x_ax++)  {
        pacman[x_ax] = create_texture(const_cast<char *>(pacman_paths[x_ax]));
    }
    // Ghost
    for(x_ax = 0; x_ax < 20; x_ax++) {
        ghost[x_ax] = create_texture(const_cast<char *>(ghost_paths[x_ax]));
    }
}

void draw_texture(GLint texture, float x, float y) {
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
            draw_texture(map[x_ax][y_ax], x_ax, y_ax);
        }
    }
}

void draw_ghosts() {
    draw_texture(ghost[0], x_ghosts[0], y_ghosts[0]);
    draw_texture(ghost[4], x_ghosts[1], y_ghosts[1]);
    draw_texture(ghost[8], x_ghosts[2], y_ghosts[2]);
    draw_texture(ghost[12], x_ghosts[3], y_ghosts[3]);
}

void draw_pacman() {
    draw_texture(pacman[pac_state], x_pacman, y_pacman);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    draw_scenes();
    draw_ghosts();
    draw_pacman();
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

void alter_direction(int key, int x, int y) {
    switch(key){
        case GLUT_KEY_RIGHT:
            direction = D_RIGHT;
            pac_state = PAC_STATE_RIGHT;
            break;

        case GLUT_KEY_LEFT:
            direction = D_LEFT;
            pac_state = PAC_STATE_LEFT;
            break;
        case GLUT_KEY_UP:
            direction = D_UP;
            pac_state = PAC_STATE_UP;
            break;
        case GLUT_KEY_DOWN:
            direction = D_DOWN;
            pac_state = PAC_STATE_DOWN;
            break;
    }
}

int random_index() {
    return rand()%(3-0+1) + 0;
}

void update_position_ghosts() {
    for(gh_index = 0; gh_index < N_GHOSTS; gh_index++) {
        index_random = random_index();
//    printf("%d", index_random);
        switch (index_random) {
            case D_RIGHT:
                if(x_ghosts[gh_index] == 25 && y_ghosts[gh_index] == 11) {
                    x_ghosts[gh_index] = 0;
                }
                else {
                    if(grafo[x_ghosts[gh_index]+1][y_ghosts[gh_index]] == 0) x_ghosts[gh_index] += MOVE_SIZE;
                }
                break;
            case D_LEFT:
                if(x_ghosts[gh_index] == 0 && y_ghosts[gh_index] == 11) {
                    x_ghosts[gh_index] = 25;
                }
                else {
                    if(grafo[x_ghosts[gh_index]-1][y_ghosts[gh_index]] == 0) x_ghosts[gh_index] -= MOVE_SIZE;
                }
                break;
            case D_UP:
                if(grafo[x_ghosts[gh_index]][y_ghosts[gh_index]-1] == 0) y_ghosts[gh_index] -= MOVE_SIZE;
                break;
            case D_DOWN:
                if(grafo[x_ghosts[gh_index]][y_ghosts[gh_index]+1] == 0) y_ghosts[gh_index] += MOVE_SIZE;
                break;
        }
    }
}

void move_pacman(int value) {
    switch (direction) {
        case D_RIGHT:
            if(x_pacman == 25 && y_pacman == 11) {
                x_pacman = 0;
            }
            else {
                if(grafo[x_pacman+1][y_pacman] == 0) x_pacman += MOVE_SIZE;
            }
            break;
        case D_LEFT:
            if(x_pacman == 0 && y_pacman == 11) {
                x_pacman = 25;
            }
            else {
                if(grafo[x_pacman-1][y_pacman] == 0) x_pacman -= MOVE_SIZE;
            }
            break;
        case D_UP:
            if(grafo[x_pacman][y_pacman-1] == 0) y_pacman -= MOVE_SIZE;
            break;
        case D_DOWN:
            if(grafo[x_pacman][y_pacman+1] == 0) y_pacman += MOVE_SIZE;
            break;
    }

    // Update mode Pac-Man
    pac_state = (pac_state % 2 == 0) ? pac_state+1 : pac_state-1;
    update_position_ghosts();
    // Update view screen
    glutPostRedisplay();
    glutTimerFunc(200, move_pacman, 1);
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
    srand((unsigned)time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutCreateWindow("PACMAN");
    gluOrtho2D(0.0, 26.0, 26.0, 0.0);
    init_grafo();
    init_sprites();
    glutDisplayFunc(display);
    glutSpecialFunc(alter_direction);
    glutTimerFunc(2000, move_pacman, 1);
//    init();
    glutMainLoop();
    return 0;
}