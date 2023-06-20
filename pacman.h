typedef struct TPhantom Phantom;
typedef struct TPacman Pacman;
typedef struct TScene Scene;

void initSprites();

Scene* generateScene();
void drawScene(Scene *scene);
void destroyScene(Scene *scene);

Pacman* createPacman(int x, int y);
void drawPacman(Pacman *pac);
void destroyPacman(Pacman *pac);
void alterDirectionPacman(Pacman *pac, int direction);
void movePacman(Pacman *pac, Scene *scene);
int checkLifePacman(Pacman* pac);

Phantom* createPhantom(int x, int y, int id);
void drawPhantom(Phantom *ph);
void destroyPhantom(Phantom *ph);
void movePhantom(Phantom *ph, Scene *scene, Pacman *pac);

void drawGameOver();
void drawGameStart();