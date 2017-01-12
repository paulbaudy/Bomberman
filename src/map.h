#include "gestion.h"

#define TILE_SIZE 32
#define FREQ_BRICK 70
#define FREQ_BONUS 25
#define NB_TILES 8 // 8 colonnes dans les fichiers tileset et bonuset

void load_map_1(t_partie *partie);
void tile(SDL_Surface* image, int dx, int dy, int sx, int sy, t_jeu *jeu);
void map(t_jeu *jeu);
