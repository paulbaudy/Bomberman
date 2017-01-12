/*Définition des nombres pour placer les fruits sur la carte*/
#define POMME_JAUNE 64
#define POMME_ROUGE 65
#define ORANGE 66
#define FRAISE 67
#define MELON 68
#define POIRE 69
#define COING 70
#define BANANE 71
#define BOMBE_BOOST 72
#define SPEED_BOOST 73
#define CIBLE 74

/*Contantes de la campagne*/
#define SPEED_MONSTRE 1
#define NB_MONSTRES_MAX 10
#define NB_MAP_CAMPAGNE 5
#define NB_FRUITS_MAX 5

/*Chemins des cartes*/
#define MULTIJOUEUR_CLASSIC "./data/maps/multijoueur/multijoueur_classic.txt"
#define MULTIJOUEUR_BRICKSONLY "./data/maps/multijoueur/multijoueur_bricksonly.txt"
#define MULTIJOUEUR_MAP1 "./data/maps/multijoueur/multijoueur_map1.txt"
#define MULTIJOUEUR_MAP2 "./data/maps/multijoueur/multijoueur_map2.txt"
#define MULTIJOUEUR_MAP3 "./data/maps/multijoueur/multijoueur_map3.txt"

#define CAMPAGNE_MAP1 "./data/maps/campagne/campagne_map1.txt"
#define CAMPAGNE_MAP2 "./data/maps/campagne/campagne_map2.txt"
#define CAMPAGNE_MAP3 "./data/maps/campagne/campagne_map3.txt"
#define CAMPAGNE_MAP4 "./data/maps/campagne/campagne_map4.txt"
#define CAMPAGNE_MAP5 "./data/maps/campagne/campagne_map5.txt"

#define CAMPAGNE_BOSS "./data/maps/campagne/campagne_boss.txt"
#define CAMPAGNE_BOSS1 "./data/maps/campagne/campagne_boss1.txt"
#define CAMPAGNE_BOSS2 "./data/maps/campagne/campagne_boss2.txt"
#define CAMPAGNE_BOSS3 "./data/maps/campagne/campagne_boss3.txt"

/*Définitions des prototypes des fonctions principales de la campagne*/
int jouer_campagne(t_jeu *jeu);
void init_campagne(t_jeu *jeu);
void init_partie_campagne(t_jeu *jeu);

/*Prototypes des scenarios des niveaux*/
void scenario1(t_jeu *jeu, int *nb_fruits, int *nb_cibles);
void scenario2(t_jeu *jeu, int *nb_fruits, int *nb_cibles);
void scenario3(t_jeu *jeu, int *nb_fruits, int *nb_cibles);
void scenario4(t_jeu *jeu, int *nb_fruits, int *nb_cibles);
void scenario5(t_jeu *jeu, int *nb_fruits, int *nb_cibles);

/*Prototypes des fonctions qui s'occupent des fruits et des cibles*/
int poser_bonus(t_jeu *jeu, int fruit, int xd, int yd);
void affiche_fruits(t_jeu* jeu, int *fruits_in, int *nb_fruits, int *nb_fruits_in);
int verifier_event(t_jeu *jeu, int* nb_fruits, int* nb_cibles, int* fruits_in, int* nb_fruits_in);

/*Prototypes des fonctions qui s'occupent des monstres*/
void init_joueur_position(t_joueur *joueur, int x, int y);
int ajouter_monstre(t_partie *partie, int x, int y);
void controle_monstre(t_jeu *jeu);
void changer_direction(t_joueur *joueur, int direction);
void verifier_contact_monstres(t_partie *partie);
void bouger_monstre(t_joueur *j, t_partie *p);

/*Chargement des cartes*/
void load_fichier_campagne(t_partie *partie,char* chemin_map);
void load_fichier_multijoueur(t_partie *partie,char* chemin_map);

