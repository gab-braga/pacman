typedef struct TPhantom Phantom;
typedef struct TPacman Pacman;
typedef struct TScene Scene;

void initSprites();

Scene* generateScene();
int checkScoreWon(Scene *scene);

Pacman* createPacman(int x, int y);
void alterDirectionPacman(Pacman *pac, int direction);
void movePacman(Pacman *pac, Scene *scene);
int checkLifePacman(Pacman* pac);

Phantom* createPhantom(int x, int y, int id, int direction);
void movePhantom(Phantom *ph, Scene *scene, Pacman *pac);

void drawGameOver();
void drawGameStart();
void drawGameWon();
void drawScene(Scene *scene);
void drawPacman(Pacman *pac);
void drawPhantom(Phantom *ph, Pacman *pac);

void destroyScene(Scene *scene);
void destroyPacman(Pacman *pac);
void destroyPhantom(Phantom *ph);