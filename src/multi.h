#include "gestion.h"
#include "joueur.h"

#define NB_JOUEURS_MAX 2
#define NB_JOUEURS_MULTI 4

void init_jeu(t_jeu *jeu);
void init_partie(t_jeu *jeu, SDL_Surface *map_image);
void init_ordi(t_partie *partie, int manche);
void IA(t_jeu *jeu);

int jouer_partie(t_jeu *jeu);
void draw_interface(t_jeu *jeu);

