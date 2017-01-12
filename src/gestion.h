#ifndef GESTION_H
#define GESTION_H

#include "menu.h"
#include "struct.h"
#include "map.h"

#define SPEED 2
#define ANIMATION_SPEED 10

#define MARGE 10
#define TIME_BOMBE 100
#define DEPASSEMENT_TETE 17
#define NB_MANCHES_DEFAUT 3
#define NB_MANCHES_MAX 10
#define NB_BONUS 2
#define NB_BOMBES_BONUS 3
#define TIMER_BOTTES 500
#define TIMER_PARTIE 7200
#define TIME_MORT 60
#define VOLUME_MAX 20

#define SCREEN_WIDTH 672
#define SCREEN_HEIGHT 512


int init_moteur(t_jeu *jeu);
void init_programme();

void draw_all(t_jeu *jeu);
void update_map(t_partie *p, t_reglages *r);
int timer_map(t_jeu *jeu, char *horloge);
void bouger(t_joueur *j, t_partie *p);
int collision(t_joueur *j, t_partie *p);
void poser_bombe(t_joueur *j, t_partie *p);
int explosion(t_partie *p, t_bombe *b, t_reglages *r);
void explose_bombe(int mx, int my, t_partie *p);

void delay(unsigned int fps);
int val_abs(int x);

#endif
