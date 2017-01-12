#include "gestion.h"
#include "campagne.h"
#include "map.h"
#include "multi.h"
#include "struct.h"
#include "joueur.h"

int jouer_campagne(t_jeu *jeu)
{
    int compute=1, i=0;
    unsigned int fps = SDL_GetTicks()+16;

    SDL_Surface *gagne = NULL;
    gagne=IMG_Load("./data/images/gagne.png");
    SDL_Surface *perd = NULL;
    perd=IMG_Load("./data/images/perdu.png");

	/*Variables pour compter les fruits et les cibles pour faire les vérifications*/
    int nb_fruits=0;
    int nb_cibles=0;
    int *fruits_in = calloc(sizeof(int), NB_FRUITS_MAX);
    int *nb_fruits_in=malloc(sizeof(int));
    *nb_fruits_in=0;

	/*La manche représente le niveau*/
    int niveau = jeu->partie.manche;

    char *horloge = malloc(sizeof(char)*5);
    horloge[0]='0'+jeu->partie.p_timer/3600;
    horloge[1]=':';
    horloge[2]='0'+(jeu->partie.p_timer%3600)/10;
    horloge[3]='0'+(jeu->partie.p_timer%3600)%10;
    horloge[4]='\0';

	/*Chargement des niveaux en fonction de la manche*/
    switch(niveau)
    {
        case 1:
            scenario1(jeu,&nb_fruits,&nb_cibles);
            break;
        case 2:
            scenario2(jeu,&nb_fruits,&nb_cibles);
            break;
        case 3:
            scenario3(jeu,&nb_fruits,&nb_cibles);
            break;
        case 4:
            scenario4(jeu,&nb_fruits,&nb_cibles);
            break;
        case 5:
            scenario5(jeu,&nb_fruits,&nb_cibles);
            break;
        default:
            compute=0;
            break;
    }

    jeu->partie.nb_vivants=jeu->partie.nb_perso;
    SDL_FillRect(jeu->screen, NULL, SDL_MapRGB(jeu->screen->format, 16, 120, 48));
    SDL_Flip(jeu->screen);

    while(compute)
    {
        SDL_FillRect(jeu->screen, NULL, SDL_MapRGB(jeu->screen->format, 16, 120, 48));
        /*Calculer et dessiner la carte à chaque boucle*/
        draw_all(jeu);

        /*Renvoie 1 si on veut quitter*/
        if(check_controles(&(jeu->partie.joueurs[0].controles), &(jeu->partie)))
        {
            free(horloge);
            free(nb_fruits_in);
            free(fruits_in);
            SDL_FreeSurface(gagne);
            SDL_FreeSurface(perd);
            return -1;
        }
        controle_monstre(jeu);


        /*Gérer les fruits*/
        if(verifier_event(jeu, &nb_fruits, &nb_cibles, fruits_in, nb_fruits_in)==-1)
        {/*On a détruit un fruit*/
            Mix_PlayMusic(jeu->reglages.perdu, 1);
            SDL_BlitSurface(perd, NULL, jeu->screen, NULL);
            SDL_Flip(jeu->screen);
            SDL_Delay(1000);
            compute=0;
        }else if(nb_fruits==0)
        {/*On a ramassé tous les fruits*/
            Mix_PlayMusic(jeu->reglages.fin_manche, 1);
            SDL_BlitSurface(gagne, NULL, jeu->screen, NULL);
            SDL_Flip(jeu->screen);
            SDL_Delay(2000);
            free(horloge);
            free(nb_fruits_in);
            free(fruits_in);
            SDL_FreeSurface(gagne);
            SDL_FreeSurface(perd);
            return 1;
        }

        /*Mise à jour du joueur et des monstres*/
        i=0;
        while(i<jeu->partie.nb_perso)
        {
            if(jeu->partie.joueurs[i].etat==-1) // Si il est en train de mourir
            {
                anim_mort(jeu, &(jeu->partie.joueurs[i]));
            }else if(jeu->partie.joueurs[i].etat!=0)
            {
                collision(&(jeu->partie.joueurs[i]), &(jeu->partie));
                verifier_contact_monstres(&(jeu->partie));
                if(jeu->partie.joueurs[i].type==1) update_joueur(&(jeu->partie.joueurs[i]), &(jeu->partie));
                else bouger_monstre(&(jeu->partie.joueurs[i]), &(jeu->partie));
            }
            i++;
        }

        if(jeu->partie.joueurs[0].etat==0 || jeu->partie.p_timer==0)
        {/*Affichage de la défaite*/
            Mix_PlayMusic(jeu->reglages.perdu, 1);
            SDL_BlitSurface(perd, NULL, jeu->screen, NULL);
            SDL_Flip(jeu->screen);
            SDL_Delay(1000);
            compute=0;
        }
        timer_map(jeu, horloge);
        affiche_fruits(jeu, fruits_in, &nb_fruits, nb_fruits_in);

        SDL_Flip(jeu->screen);
        SDL_Delay(1);
        delay(fps);
        fps = SDL_GetTicks() + 16;
    }
    free(horloge);
    free(nb_fruits_in);
    free(fruits_in);
    SDL_FreeSurface(gagne);
    SDL_FreeSurface(perd);
    /*Perdu*/
    return 0;
}

/*Fonction similaire à bouger mais les monstres sont moins rapides*/
void bouger_monstre(t_joueur *j, t_partie *p)
{
    if(j->controles.haut==1)
    {
        j->src.y=0; // Sélection de l'image de la direction appropriée
        if(p->p_timer%ANIMATION_SPEED==0) // Mise à jour de l'animation du mouvement
        {
            if(j->src.x==TILE_SIZE)
            {
                j->src.x+=TILE_SIZE;
            }else
            {
                j->src.x=TILE_SIZE;
            }
        }
		/*Les monstres n'accélèrent pas lorsqu'ils ramassent un bonus*/
        j->dest.y-=SPEED_MONSTRE;
    }else /*Idem pour les autres directions*/
    if(j->controles.bas==1)
    {
        j->src.y=2*j->src.h;
        if(p->p_timer%ANIMATION_SPEED==0)
        {
            if(j->src.x==TILE_SIZE)
            {
                j->src.x+=TILE_SIZE;
            }else
            {
                j->src.x=TILE_SIZE;
            }
        }
        j->dest.y+=SPEED_MONSTRE;
    }else
    if(j->controles.gauche==1 && !j->controles.droite)
    {
        j->src.y=j->src.h;
        if(p->p_timer%ANIMATION_SPEED==0)
        {
            if(j->src.x==TILE_SIZE)
            {
                j->src.x+=TILE_SIZE;
            }else
            {
                j->src.x=TILE_SIZE;
            }
        }
        j->dest.x-=SPEED_MONSTRE;
    }else
    if(j->controles.droite==1 && !j->controles.gauche)
    {
        j->src.y=3*j->src.h;
        if(p->p_timer%ANIMATION_SPEED==0)
        {
            if(j->src.x==TILE_SIZE)
            {
                j->src.x+=TILE_SIZE;
            }else
            {
                j->src.x=TILE_SIZE;
            }
        }
        j->dest.x+=SPEED_MONSTRE;
    }else
    {
        j->src.x=0; // Aucun déplacement : personnage immobile
    }
}

/*Calcule le mouvement que doit faire le monstre en fonction des cubes qui l'entourent*/
void controle_monstre(t_jeu *jeu)
{
	/*Initialisation et définition des variables pour simplifier la lecture*/
    int i;

    int haut=0;
    int bas=1;
    int droite=2;
    int gauche=3;

    int y_case_joueur;
    int y_case_dessus_joueur;
    int y_case_dessous_joueur;

    int x_case_joueur;
    int x_case_droite_joueur;
    int x_case_gauche_joueur;

    int case_dessus_joueur;
    int case_dessous_joueur;
    int case_droite_joueur;
    int case_gauche_joueur;

    for(i=jeu->partie.nb_joueurs;i<jeu->partie.nb_perso;i++)
    {
        if(jeu->partie.joueurs[i].etat!=0 && jeu->partie.joueurs[i].etat!=-1) // Encore en vie
        {
            /*On prend la position des cases au contact*/
            y_case_dessus_joueur=jeu->partie.joueurs[i].dest.y+DEPASSEMENT_TETE-(MARGE/4);
            y_case_dessous_joueur=jeu->partie.joueurs[i].dest.y+jeu->partie.joueurs[i].dest.h+(MARGE/4);
            y_case_joueur=jeu->partie.joueurs[i].dest.y+DEPASSEMENT_TETE+TILE_SIZE/2;

            x_case_joueur=jeu->partie.joueurs[i].dest.x+TILE_SIZE/2;
            x_case_droite_joueur=jeu->partie.joueurs[i].dest.x+jeu->partie.joueurs[i].dest.w+(MARGE/4);
            x_case_gauche_joueur=jeu->partie.joueurs[i].dest.x-(MARGE/4);

            /*Cube au dessus*/
            case_dessus_joueur=jeu->partie.map[y_case_dessus_joueur/TILE_SIZE][x_case_joueur/TILE_SIZE];
            /*Cube en dessous*/
            case_dessous_joueur=jeu->partie.map[y_case_dessous_joueur/TILE_SIZE][x_case_joueur/TILE_SIZE];
            /*Cube à droite*/
            case_droite_joueur=jeu->partie.map[y_case_joueur/TILE_SIZE][x_case_droite_joueur/TILE_SIZE];
            /*Cube à gauche*/
            case_gauche_joueur=jeu->partie.map[y_case_joueur/TILE_SIZE][x_case_gauche_joueur/TILE_SIZE];

			/*On le dirige vers le bas par défaut*/
            if(jeu->partie.joueurs[i].controles.haut==0 && jeu->partie.joueurs[i].controles.bas==0 && jeu->partie.joueurs[i].controles.droite==0 && jeu->partie.joueurs[i].controles.gauche==0)
                changer_direction(&(jeu->partie.joueurs[i]),bas);

            /*Cas où le joueur est entouré de trois blocs*/
            if((case_dessous_joueur==1 || case_dessous_joueur==2 || (5<case_dessous_joueur && case_dessous_joueur<64)) && (case_droite_joueur==1 || case_droite_joueur==2 || (5<case_droite_joueur && case_droite_joueur<64)) && (case_gauche_joueur==1 || case_gauche_joueur==2 || (5<case_gauche_joueur && case_gauche_joueur<64)))
                changer_direction(&(jeu->partie.joueurs[i]),haut);
            else if((case_dessous_joueur==1 || case_dessous_joueur==2 || (3<case_dessous_joueur && case_dessous_joueur<64)) && (case_dessus_joueur==1 || case_dessus_joueur==2 || (3<case_dessus_joueur && case_dessus_joueur<64)) && (case_gauche_joueur==1 || case_gauche_joueur==2 || (3<case_gauche_joueur && case_gauche_joueur<64)))
                changer_direction(&(jeu->partie.joueurs[i]),droite);
            else if((case_dessous_joueur==1 || case_dessous_joueur==2 || (3<case_dessous_joueur && case_dessous_joueur<64)) && (case_droite_joueur==1 || case_droite_joueur==2 || (3<case_droite_joueur && case_droite_joueur<64)) && (case_dessus_joueur==1 || case_dessus_joueur==2 || (3<case_dessus_joueur && case_dessus_joueur<64)))
                changer_direction(&(jeu->partie.joueurs[i]),gauche);
            else if((case_dessus_joueur==1 || case_dessus_joueur==2 || (3<case_dessus_joueur && case_dessus_joueur<64)) && (case_droite_joueur==1 || case_droite_joueur==2 || (3<case_droite_joueur && case_droite_joueur<64)) && (case_gauche_joueur==1 || case_gauche_joueur==2 || (3<case_gauche_joueur && case_gauche_joueur<64)))
                changer_direction(&(jeu->partie.joueurs[i]),bas);


            /*Cas où le joueur est entouré de deux blocs*/
            /*Blocs en haut et à droite*/
            else if((case_dessus_joueur==1 || case_dessus_joueur==2 || (3<case_dessus_joueur && case_dessus_joueur<64)) && (case_droite_joueur==1 || case_droite_joueur==2 || (3<case_droite_joueur && case_droite_joueur<64)))
            {
                if(jeu->partie.joueurs[i].controles.droite==1)
                    changer_direction(&(jeu->partie.joueurs[i]),bas);
                else if(jeu->partie.joueurs[i].controles.bas!=1)
                    changer_direction(&(jeu->partie.joueurs[i]),gauche);
            }
            /*Blocs en haut et à gauche*/
            else if((case_dessus_joueur==1 || case_dessus_joueur==2 || (3<case_dessus_joueur && case_dessus_joueur<64)) && (case_gauche_joueur==1 || case_gauche_joueur==2 || (3<case_gauche_joueur && case_gauche_joueur<64)))
            {
                if(jeu->partie.joueurs[i].controles.gauche==1)
                    changer_direction(&(jeu->partie.joueurs[i]),bas);
                else if(jeu->partie.joueurs[i].controles.bas!=1)
                    changer_direction(&(jeu->partie.joueurs[i]),droite);
            }
            /*Blocs en bas et à droite*/
            else if((case_dessous_joueur==1 || case_dessous_joueur==2 || (3<case_dessous_joueur && case_dessous_joueur<64)) && (case_droite_joueur==1 || case_droite_joueur==2 || (3<case_droite_joueur && case_droite_joueur<64)))
            {
                if(jeu->partie.joueurs[i].controles.bas==1)
                    changer_direction(&(jeu->partie.joueurs[i]),gauche);
                else if(jeu->partie.joueurs[i].controles.gauche!=1)
                    changer_direction(&(jeu->partie.joueurs[i]),haut);
            }
            /*Blocs en bas et à gauche*/
            else if((case_dessous_joueur==1 || case_dessous_joueur==2 || (3<case_dessous_joueur && case_dessous_joueur<64)) && (case_gauche_joueur==1 || case_gauche_joueur==2 || (3<case_gauche_joueur && case_gauche_joueur<64)))
            {
                if(jeu->partie.joueurs[i].controles.bas==1)
                    changer_direction(&(jeu->partie.joueurs[i]),droite);
                else if(jeu->partie.joueurs[i].controles.droite!=1)
                    changer_direction(&(jeu->partie.joueurs[i]),haut);
            }

            /*Si il n'y a qu'un bloc à côté de lui*/
            /*Bloc au-dessous*/
            else if((case_dessous_joueur==1 || case_dessous_joueur==2 || (3<case_dessous_joueur && case_dessous_joueur<64)))
            {
                if(jeu->partie.joueurs[i].controles.bas==1)
                    changer_direction(&(jeu->partie.joueurs[i]),droite);
            }
            /*Bloc au-dessus*/
            else if((case_dessus_joueur==1 || case_dessus_joueur==2 || (3<case_dessus_joueur && case_dessus_joueur<64)))
            {
                if(jeu->partie.joueurs[i].controles.haut==1)
                    changer_direction(&(jeu->partie.joueurs[i]),droite);
            }
            /*Bloc à droite*/
            else if((case_droite_joueur==1 || case_droite_joueur==2 || (3<case_droite_joueur && case_droite_joueur<64)))
            {
                if(jeu->partie.joueurs[i].controles.droite==1)
                    changer_direction(&(jeu->partie.joueurs[i]),haut);
            }
            /*Bloc à gauche*/
            else if((case_gauche_joueur==1 || case_gauche_joueur==2 || (3<case_gauche_joueur && case_gauche_joueur<64)))
            {
                if(jeu->partie.joueurs[i].controles.gauche==1)
                    changer_direction(&(jeu->partie.joueurs[i]),bas);
            }
        }
    }
}

void changer_direction(t_joueur *joueur, int direction)
{
    /*Aller vers le haut*/
    if(direction==0)
    {
        joueur->controles.haut=1;
        joueur->controles.bas=0;
        joueur->controles.droite=0;
        joueur->controles.gauche=0;
    }

    /*Aller vers le bas*/
    if(direction==1)
    {
        joueur->controles.haut=0;
        joueur->controles.bas=1;
        joueur->controles.droite=0;
        joueur->controles.gauche=0;
    }

    /*Aller vers la droite*/
    if(direction==2)
    {
        joueur->controles.haut=0;
        joueur->controles.bas=0;
        joueur->controles.droite=1;
        joueur->controles.gauche=0;
    }

    /*Aller vers la gauche*/
    if(direction==3)
    {
        joueur->controles.haut=0;
        joueur->controles.bas=0;
        joueur->controles.droite=0;
        joueur->controles.gauche=1;
    }
}

/*Charger une carte de la campagne*/
void load_fichier_campagne(t_partie *partie,char* chemin_map)
{
    int x=0, y=0;
    int caractereActuel=0;
    FILE* fichier_map=NULL;

	/*Ouvrir le fichier de la carte*/
    fichier_map = fopen(chemin_map,"r+");

	/*Si on a réussi à l'ouvrir*/
    if (fichier_map != NULL)
    {
		/*On prend le premier caractère*/
        caractereActuel=fgetc(fichier_map);
        while(caractereActuel != EOF) //Tant que l'on est pas arrivé à la fin du fichier
        {
			/*On met des blocs incassables tout autour du terrain*/
            if(x==0 || x==MAP_SIZE_W-1 || y==0 || y==MAP_SIZE_H-1)
            {
                partie->map[y][x]=1;
            }else
            {
                partie->map[y][x]=caractereActuel-'0';
            }
            x++;

			/*On prend le caractère suivant*/
            caractereActuel=fgetc(fichier_map);

			/*Si on revient à la ligne*/
            if(caractereActuel=='\n')
            {
                caractereActuel=fgetc(fichier_map);
                y++;
                x=0;
            }
        }
        fclose(fichier_map);
    }
}

/*Charger une carte multijoueur de la même manière que pour charger la campagne mais en mettant des murs cassables aléatoirement*/
void load_fichier_multijoueur(t_partie *partie,char* chemin_map)
{
    int x=0, y=0;
    int caractereActuel=0;
    FILE* fichier_map=NULL;
    fichier_map = fopen(chemin_map,"r+");
    if (fichier_map != NULL)
    {
        caractereActuel=fgetc(fichier_map);
        while(caractereActuel != EOF)
        {
            if(x==0 || x==MAP_SIZE_W-1 || y==0 || y==MAP_SIZE_H-1)
            {
                partie->map[y][x]=1;
            }else
            {
                partie->map[y][x]=caractereActuel-'0';

                if(partie->map[y][x]!=1)
                {
                    partie->map[y][x]=3;
					/* On place des briques cassables en dehors des spawns aléatoirement*/
                    if(rand()%100<FREQ_BRICK && (y>2 || x>2) && (y<MAP_SIZE_H-3 || x<MAP_SIZE_W-3) && (y<MAP_SIZE_H-3 || x>2) && (y>2 || x<MAP_SIZE_W-3))
                    {
                        partie->map[y][x]=2;
                    }
                }
            }
            x++;
            caractereActuel=fgetc(fichier_map);
            if(caractereActuel=='\n')
            {
                caractereActuel=fgetc(fichier_map);
                y++;
                x=0;
            }
        }
        fclose(fichier_map);
    }
}

/*Initialisation des différents niveaux de la campagne*/
void scenario1(t_jeu *jeu, int *nb_fruits, int *nb_cibles)
{
    load_fichier_campagne(&(jeu->partie),CAMPAGNE_MAP1);
    init_joueur_position(&(jeu->partie.joueurs[0]),1,1);
    *nb_fruits+=poser_bonus(jeu, POMME_JAUNE, 19, 1);
    *nb_fruits+=poser_bonus(jeu, POMME_ROUGE, 19, 9);
    *nb_fruits+=poser_bonus(jeu, ORANGE, 1, 9);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 19, 1);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 19, 9);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 1, 9);
}

void scenario2(t_jeu *jeu, int *nb_fruits, int *nb_cibles)
{
    load_fichier_campagne(&(jeu->partie),CAMPAGNE_MAP2);
    init_joueur_position(&(jeu->partie.joueurs[0]),1,1);
    *nb_fruits+=poser_bonus(jeu, FRAISE, 19,1);
    *nb_fruits+=poser_bonus(jeu, COING, 1,9);
    *nb_fruits+=poser_bonus(jeu, BANANE, 19,9);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 19, 1);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 19, 9);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 5, 4);
}

void scenario3(t_jeu *jeu, int *nb_fruits, int *nb_cibles)
{
    load_fichier_campagne(&(jeu->partie),CAMPAGNE_MAP3);
    init_joueur_position(&(jeu->partie.joueurs[0]),1,1);
    *nb_fruits+=poser_bonus(jeu, ORANGE, 19, 9);
    *nb_fruits+=poser_bonus(jeu, MELON, 19, 1);
    *nb_fruits+=poser_bonus(jeu, POMME_ROUGE, 1, 9);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 17, 9);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 5, 1);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 19, 3);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 8, 4);
    *nb_cibles+=poser_bonus(jeu, CIBLE, 9, 7);
}

void scenario4(t_jeu *jeu, int *nb_fruits, int *nb_cibles)
{
    load_fichier_campagne(&(jeu->partie),CAMPAGNE_MAP4);
    init_joueur_position(&(jeu->partie.joueurs[0]),1,1);
    *nb_fruits+=poser_bonus(jeu, FRAISE, 19, 9);
    *nb_fruits+=poser_bonus(jeu, BANANE, 17, 5);
    *nb_fruits+=poser_bonus(jeu, POMME_JAUNE, 8, 6);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 17, 9);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 15, 5);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 1, 9);
    *nb_cibles+=poser_bonus(jeu, CIBLE, 10, 5);
    *nb_cibles+=poser_bonus(jeu, CIBLE, 1, 9);
    *nb_cibles+=poser_bonus(jeu, CIBLE, 17, 1);
}

void scenario5(t_jeu *jeu, int *nb_fruits, int *nb_cibles)
{
    load_fichier_campagne(&(jeu->partie),CAMPAGNE_MAP5);
    init_joueur_position(&(jeu->partie.joueurs[0]),1,1);
    *nb_fruits+=poser_bonus(jeu, MELON, 19,9);
    *nb_fruits+=poser_bonus(jeu, COING, 19,1);
    *nb_fruits+=poser_bonus(jeu, BANANE, 19,5);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 5, 5);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 8, 5);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 15, 1);
    jeu->partie.nb_perso+=ajouter_monstre(&(jeu->partie), 15, 9);
    *nb_cibles+=poser_bonus(jeu, CIBLE, 7, 8);
    *nb_cibles+=poser_bonus(jeu, CIBLE, 7, 2);
    *nb_cibles+=poser_bonus(jeu, CIBLE, 19, 2);
    *nb_cibles+=poser_bonus(jeu, CIBLE, 19, 8);
}

/*Initialisation d'un monstre*/
int ajouter_monstre(t_partie *partie, int x, int y)
{
    int i=partie->nb_perso;

    strcpy(partie->joueurs[i].nom, "Monstre");
    partie->joueurs[i].type=0;
    partie->joueurs[i].score=0;
    partie->joueurs[i].inventaire=0;
    partie->joueurs[i].wait_bombe=0;
    partie->joueurs[i].time_bonus=0;
    partie->joueurs[i].etat=1;
    partie->joueurs[i].avatar = IMG_Load("./data/sets/persoset1.png");

    init_joueur_position(&(partie->joueurs[i]),x,y);
    partie->joueurs[i].dest.h=PLAYER_H;
    partie->joueurs[i].dest.w=TILE_SIZE;
    partie->joueurs[i].src.h=PLAYER_H;
    partie->joueurs[i].src.w=TILE_SIZE;
    partie->joueurs[i].src.x=0;
    partie->joueurs[i].src.y=0;

    init_controles(&(partie->joueurs[i].controles));

    return 1;
}

/*Réinitialise la campagne entre chaque niveau*/
void init_partie_campagne(t_jeu *jeu)
{
    jeu->partie.nb_joueurs=1;
    jeu->partie.nb_vivants=1;
    jeu->partie.nb_perso=1;
    jeu->reglages.nb_manches=NB_MAP_CAMPAGNE;

    jeu->partie.bombes->sommet=NULL;
    jeu->partie.bombes->fin=NULL;

    jeu->partie.p_timer=(3*TIMER_PARTIE)+1;

    jeu->partie.joueurs = malloc(sizeof(t_joueur)*NB_MONSTRES_MAX);
    jeu->partie.joueurs[0].type=1;

    init_joueur(&(jeu->partie.joueurs[0]), &(jeu->reglages), jeu->partie.manche);
}

/*Initialiser les paramètres propres à la campagne et lancer la partie*/
void init_campagne(t_jeu *jeu)
{
    int compute;

    jeu->partie.bombes=malloc(sizeof(t_file_bombe));
    jeu->partie.bombes->sommet=malloc(sizeof(t_bombe));
    jeu->partie.bombes->fin=malloc(sizeof(t_bombe));
    init_partie_campagne(jeu);

    jeu->partie.map_image=IMG_Load("./data/sets/tileset.png");
    jeu->partie.map_bonus=IMG_Load("./data/sets/bonuset.png");

    jeu->partie.manche=1;

    while(jeu->partie.manche<=jeu->reglages.nb_manches)
    {
        init_partie_campagne(jeu);
        compute=jouer_campagne(jeu);
		free(jeu->partie.joueurs);
        if(compute==-1)
        {
            free(jeu->partie.bombes->sommet);
            free(jeu->partie.bombes->fin);
            free(jeu->partie.bombes);
            return;
        }else
        {
            jeu->partie.manche+=compute;
        }
    }
    draw_menu_histoire(jeu);
}

void init_joueur_position(t_joueur *joueur, int x, int y)
{
    joueur->dest.x=x*TILE_SIZE;
    joueur->dest.y=y*TILE_SIZE-DEPASSEMENT_TETE;
}

int poser_bonus(t_jeu *jeu, int bonus, int xd, int yd)
{
    /*Vérifier qu'il n'y a pas déjà un bonus sur la case*/
    if(jeu->partie.map[yd][xd]<64)
    {
        jeu->partie.map[yd][xd]=bonus;
        return 1;
    }
    else return 0;
}

/*Tuer un joueur si il est en contact avec un monstre*/
void verifier_contact_monstres(t_partie *partie)
{
	/*Initialisation de variables pour faciliter la lecture*/
    int i;

    int y_cote_dessus_joueur;
    int y_cote_dessous_joueur;

    int x_cote_droit_joueur;
    int x_cote_gauche_joueur;

    int y_cote_dessus_monstre;
    int y_cote_dessous_monstre;

    int x_cote_droit_monstre;
    int x_cote_gauche_monstre;

	/*Prendre la position des cotés du joueur*/
    y_cote_dessus_joueur=(partie->joueurs[0].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE;
    y_cote_dessous_joueur=(partie->joueurs[0].dest.y+partie->joueurs[0].dest.h-1-(MARGE/2))/TILE_SIZE;

    x_cote_droit_joueur=(partie->joueurs[0].dest.x+partie->joueurs[0].dest.w-1-(MARGE/2))/TILE_SIZE;
    x_cote_gauche_joueur=(partie->joueurs[0].dest.x+(MARGE/2))/TILE_SIZE;

    for(i=partie->nb_joueurs;i<partie->nb_perso;i++)
    {
		/*Si le monstre est vivant*/
        if(partie->joueurs[i].etat==0)
        {
			/*Prendre la postion des cotés du monstre*/
            y_cote_dessus_monstre=(partie->joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE;
            y_cote_dessous_monstre=(partie->joueurs[i].dest.y+partie->joueurs[i].dest.h-1-(MARGE/2))/TILE_SIZE;

            x_cote_droit_monstre=(partie->joueurs[i].dest.x+partie->joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE;
            x_cote_gauche_monstre=(partie->joueurs[i].dest.x+(MARGE/2))/TILE_SIZE;

			/*Si il y a contact*/
            if(( (x_cote_droit_joueur==x_cote_gauche_monstre && y_cote_dessous_joueur==y_cote_dessous_monstre)
               || (x_cote_gauche_joueur==x_cote_droit_monstre && y_cote_dessus_joueur==y_cote_dessus_monstre)
               || (x_cote_gauche_joueur==x_cote_gauche_monstre && y_cote_dessus_joueur==y_cote_dessous_monstre)
               || (x_cote_gauche_joueur==x_cote_gauche_monstre && y_cote_dessous_joueur==y_cote_dessus_monstre))
                && partie->joueurs[0].etat!=0)
            {
                partie->joueurs[0].etat=0;
                partie->nb_vivants--;
            }
        }
    }
}

/*Verifier quand on ramasse un fruit, marche sur une cible ou si on détruit l'un ou l'autre*/
int verifier_event(t_jeu *jeu, int *nb_fruits, int *nb_cibles, int *fruits_in, int *nb_fruits_in)
{
    int x,y;

    int nb_f=0;/*Nombre de fruits*/
    int nb_c=0;/*Nombre de cibles*/

    int niveau = jeu->partie.manche;

	/*Recompter les fruits et les cibles*/
    for(x=0; x<MAP_SIZE_W; x++)
    {
        for(y=0;y<MAP_SIZE_H;y++)
        {
            if(jeu->partie.map[y][x]>=64 && jeu->partie.map[y][x]<72)
            {
                nb_f++;
            }else if (jeu->partie.map[y][x]==CIBLE)
            {
                nb_c++;
            }
        }
    }

	/*Gerer lorsqu'on marche sur une cible*/
    if(*nb_cibles==nb_c)
    {
        x=jeu->partie.joueurs[0].dest.x/TILE_SIZE;
        y=(jeu->partie.joueurs[0].dest.y+DEPASSEMENT_TETE)/TILE_SIZE;

        if(jeu->partie.map[y][x]==CIBLE)
        {
				/*En fonction de la cible et du niveau faire disparaitre des cubes incassables*/
                switch(niveau)
                {
                    case 1: // Pas de cible
                        break;
                    case 2:
                        break;
                    case 3:
                        if(x==9 && y==7) jeu->partie.map[9][18]=3;
                        break;
                    case 4:
                        if(x==10 && y==5) jeu->partie.map[9][18]=3;
                        if(x==1 && y==9) jeu->partie.map[5][16]=3;
                        if(x==17 && y==1) jeu->partie.map[1][18]=3;
                        break;
                    case 5:
                        if(x==7 && y==2) jeu->partie.map[1][16]=3;
                        if(x==7 && y==8) jeu->partie.map[9][16]=3;
                        if(x==19 && y==2) jeu->partie.map[5][11]=3;
                        if(x==19 && y==8) jeu->partie.map[5][12]=3;
                        break;
                    default:
                        break;
                }
                jeu->partie.map[y][x]=3;
                *nb_cibles=*nb_cibles-1;
        }

    }else return -1; /*On a détruit la cible*/

	/*Gerer lorsqu'on marche sur un fruit*/
    if(*nb_fruits==nb_f)
    {
        /*Si on touche un fruit on l'enlève*/
        if(jeu->partie.map[((jeu->partie.joueurs[0].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+(MARGE/2))/TILE_SIZE]>=64 && jeu->partie.map[((jeu->partie.joueurs[0].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+(MARGE/2))/TILE_SIZE]<72)
        {
            fruits_in[*nb_fruits_in]=jeu->partie.map[((jeu->partie.joueurs[0].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+(MARGE/2))/TILE_SIZE];
            jeu->partie.map[((jeu->partie.joueurs[0].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+(MARGE/2))/TILE_SIZE]=3;
            *nb_fruits=*nb_fruits-1;
            *nb_fruits_in=*nb_fruits_in+1;
        }else if((jeu->partie.map[((jeu->partie.joueurs[0].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+jeu->partie.joueurs[0].dest.w-1-(MARGE/2))/TILE_SIZE]>=64 && jeu->partie.map[((jeu->partie.joueurs[0].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+jeu->partie.joueurs[0].dest.w-1-(MARGE/2))/TILE_SIZE]<72))
        {
            fruits_in[*nb_fruits_in]=jeu->partie.map[((jeu->partie.joueurs[0].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+jeu->partie.joueurs[0].dest.w-1-(MARGE/2))/TILE_SIZE];
            jeu->partie.map[((jeu->partie.joueurs[0].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+jeu->partie.joueurs[0].dest.w-1-(MARGE/2))/TILE_SIZE]=3;
            *nb_fruits=*nb_fruits-1;
            *nb_fruits_in=*nb_fruits_in+1;
        }else if(jeu->partie.map[((jeu->partie.joueurs[0].dest.y+jeu->partie.joueurs[0].dest.h-(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+(MARGE/2))/TILE_SIZE]>=64 && jeu->partie.map[((jeu->partie.joueurs[0].dest.y+jeu->partie.joueurs[0].dest.h-(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+(MARGE/2))/TILE_SIZE]<72)
        {
            fruits_in[*nb_fruits_in]=jeu->partie.map[((jeu->partie.joueurs[0].dest.y+jeu->partie.joueurs[0].dest.h-(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+(MARGE/2))/TILE_SIZE];
            jeu->partie.map[((jeu->partie.joueurs[0].dest.y+jeu->partie.joueurs[0].dest.h-(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+(MARGE/2))/TILE_SIZE]=3;
            *nb_fruits=*nb_fruits-1;
            *nb_fruits_in=*nb_fruits_in+1;
        }else if(jeu->partie.map[((jeu->partie.joueurs[0].dest.y+jeu->partie.joueurs[0].dest.h-(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+jeu->partie.joueurs[0].dest.w-1-(MARGE/2))/TILE_SIZE]>=64 && jeu->partie.map[((jeu->partie.joueurs[0].dest.y+jeu->partie.joueurs[0].dest.h-(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+jeu->partie.joueurs[0].dest.w-1-(MARGE/2))/TILE_SIZE]<72)
        {
            fruits_in[*nb_fruits_in]=jeu->partie.map[((jeu->partie.joueurs[0].dest.y+jeu->partie.joueurs[0].dest.h-(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+jeu->partie.joueurs[0].dest.w-1-(MARGE/2))/TILE_SIZE];
            jeu->partie.map[((jeu->partie.joueurs[0].dest.y+jeu->partie.joueurs[0].dest.h-(MARGE/2))/TILE_SIZE)][(jeu->partie.joueurs[0].dest.x+jeu->partie.joueurs[0].dest.w-1-(MARGE/2))/TILE_SIZE]=3;
            *nb_fruits=*nb_fruits-1;
            *nb_fruits_in=*nb_fruits_in+1;
        }

        /*Si on a ramassé tous les fruits*/
        if(*nb_fruits==0)
        {
            return 1;
        }else  return 0; /*Si il reste des fruits, on continue*/

    }else return -1; /*Sinon si on a détruit un fruit */

}

void affiche_fruits(t_jeu* jeu, int *fruits_in, int *nb_fruits, int *nb_fruits_in)
{
    int i = 0;
    SDL_Rect dest;
    SDL_Rect src;

	/*Position où afficher les fruits*/
    dest.x=SCREEN_WIDTH/2 - 55 -TILE_SIZE;
    dest.y=SCREEN_HEIGHT-120+2*TILE_SIZE;
    dest.h=TILE_SIZE;
    dest.w=TILE_SIZE;
    src.y=0;
    src.h=TILE_SIZE;
    src.w=TILE_SIZE;

    while(i<*nb_fruits_in)
    {
        src.x=(fruits_in[i]-64)*TILE_SIZE;
        dest.x+=TILE_SIZE;
        SDL_BlitSurface(jeu->partie.map_bonus, &src, jeu->screen, &dest);
        i++;
    }

    /* Briques sur les côtés de l'horloge et des fruits ramassés*/
    src.x=TILE_SIZE;
    src.y=0;
    src.h=TILE_SIZE;
    src.w=TILE_SIZE;
    dest.w=TILE_SIZE;
    dest.h=TILE_SIZE;

    for(i=MAP_SIZE_H*TILE_SIZE;i<SCREEN_HEIGHT;i+=TILE_SIZE)
    {
        dest.x=TILE_SIZE*7;
        dest.y=i;
        SDL_BlitSurface(jeu->partie.map_image, &src, jeu->screen, &dest);
        dest.x=TILE_SIZE*13;
        SDL_BlitSurface(jeu->partie.map_image, &src, jeu->screen, &dest);
    }
}
