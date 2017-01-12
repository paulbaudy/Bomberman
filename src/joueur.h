#include "gestion.h"

#define PLAYER_H 48

void init_joueur(t_joueur *joueur, t_reglages *reglages, int manche);
void init_controles(t_controles *c);

int check_controles(t_controles *c, t_partie *p);
int check_controles2(t_controles *c1, t_controles *c2, t_partie *p);

void update_joueur(t_joueur *joueur, t_partie *p);
void verif_bonus(t_partie *p, t_reglages *r);
void verif_mort(t_partie *p);
void anim_mort(t_jeu *jeu, t_joueur *joueur);


