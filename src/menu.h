#include "struct.h"
#include "multi.h"
#include "gestion.h"
#include "campagne.h"

void draw_menu(t_jeu *jeu);
void draw_menu_jeu(t_jeu *jeu);
void draw_menu_options(t_jeu *jeu);
void draw_menu_aide(t_jeu *jeu);

void draw_menu_jouer1(t_jeu *jeu);
void draw_menu_jouer2(t_jeu *jeu);
void draw_menu_histoire(t_jeu *jeu);

void draw_menu_reglages1(t_jeu *jeu);
void draw_menu_reglages2(t_jeu *jeu);
void lire_nom(t_jeu *jeu, int i_joueur);
void draw_menu_histoire_regles(t_jeu *jeu);
