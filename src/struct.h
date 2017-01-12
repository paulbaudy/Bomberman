#ifndef STRUCT_H
#define STRUCT_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAP_SIZE_W 21
#define MAP_SIZE_H 11
#define NB_CARAC_MAX 11
#define NOM1_DEFAUT ((const char *)"Joueur 1")
#define NOM2_DEFAUT ((const char *)"Joueur 2")

typedef struct t_bombe t_bombe;
struct t_bombe{
    int **mapb; // Tableau des images de l'animation des bombes
    int h,w;
    int mx, my;
    int timer;
    int type;
    t_bombe *suivant;
};

typedef struct{
    t_bombe *sommet;
    t_bombe *fin;
}t_file_bombe;

typedef struct {
    int haut, bas, gauche, droite, echap, action;
}t_controles;

typedef struct {
    char nom[NB_CARAC_MAX];
    int score;
    SDL_Surface* avatar; // Ensemble des images des animations
    SDL_Surface* face; // Tête pour l'interface
    SDL_Rect dest;
    SDL_Rect src;
    int inventaire; // Nombre de bombes bonus
    int etat; // 0 si mort, 1 si vivant, -1 si en train de mourir
    int type;//0 pour l'ordinateur, 1 pour le joueur 1 et 2 pour le joueur 2
    t_controles controles;
    int wait_bombe; // Temps d'attente avant de pouvoir poser une bombe
    int time_bonus; // Temps qu'il reste pour le bonus de vitesse
}t_joueur;

typedef struct {
    int map[MAP_SIZE_H][MAP_SIZE_W];
    t_file_bombe* bombes;
    SDL_Surface* map_image;
    SDL_Surface* map_bonus;
    int nb_joueurs;
    int nb_perso; // Nombre de joueurs humains et d'ordinateurs
    t_joueur *joueurs;
    int manche;
    int nb_vivants;
    int p_timer;
}t_partie;

typedef struct {
    char nom1[NB_CARAC_MAX];
    char nom2[NB_CARAC_MAX];
    int nb_manches;
    Mix_Music *explose;
    Mix_Music *put;
    Mix_Music *bonus;
    Mix_Music *fin_partie;
    Mix_Music *fin_manche;
    Mix_Music *entree;
    Mix_Music *perdu;
    int volume;
}t_reglages;

typedef struct {
    t_partie partie;
    SDL_Surface* screen;
    t_reglages reglages;
}t_jeu;

#endif
