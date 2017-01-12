#include "joueur.h"


void init_controles(t_controles *c)
{
    c->haut=0;
    c->bas=0;
    c->gauche=0;
    c->droite=0;
    c->action=0;
    c->echap=0;
}

void init_joueur(t_joueur *joueur, t_reglages *reglages, int manche)
{
    switch(joueur->type)
    {
        case 1:
            strcpy(joueur->nom, reglages->nom1);
            joueur->avatar = IMG_Load("./data/sets/persoset0.png"); // Ensemble des animations
            joueur->face = IMG_Load("./data/sets/face0.png"); // Tête pour l'interface

            joueur->dest.x=TILE_SIZE;
            joueur->dest.y=15;
            joueur->dest.h=PLAYER_H;
            joueur->dest.w=TILE_SIZE;
            joueur->src.h=PLAYER_H;
            joueur->src.w=TILE_SIZE;
            joueur->src.x=0;
            joueur->src.y=2*PLAYER_H;
            break;
        case 2:
            if(manche==1)
            {
                strcpy(joueur->nom, reglages->nom2);
                joueur->avatar = IMG_Load("./data/sets/persoset1.png");
                joueur->face = IMG_Load("./data/sets/face1.png");
            }
            joueur->dest.x=SCREEN_WIDTH-(TILE_SIZE*2)-1;
            joueur->dest.y=15;
            joueur->dest.h=PLAYER_H;
            joueur->dest.w=TILE_SIZE;
            joueur->src.h=PLAYER_H;
            joueur->src.w=TILE_SIZE;
            joueur->src.x=0;
            joueur->src.y=2*PLAYER_H;
            break;
        default:
            break;
    }
    if(manche == 1) // Initialisation des scores en début de partie
    {
        joueur->score=0;
    }
    joueur->inventaire=0; // Nombre de bombes rouges
    joueur->wait_bombe=0; // Timer avant de pouvoir poser une autre bombe
    joueur->time_bonus=0; // Temps restant du bonus de vitesse
    joueur->etat=1; // En vie

    init_controles(&(joueur->controles));
}

/*Gestion des touches pour un seul joueur*/
int check_controles(t_controles *c, t_partie *p)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_w:
                        c->haut=1;
                        break;
                    case SDLK_s:
                        c->bas=1;
                        break;
                    case SDLK_a:
                        c->gauche=1;
                        break;
                    case SDLK_d:
                        c->droite=1;
                        break;
                    case SDLK_ESCAPE:
                        c->echap=1;
                        break;
                    case SDLK_SPACE:
                        if(p->joueurs[0].etat!=0 && p->joueurs[0].etat!=-1) // Joueur encore en vie
                        {
                            c->action=1;
                            poser_bombe(&(p->joueurs[0]), p);
                        }else
                        {
                            c->action=0;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym)
                {
                    case SDLK_w:
                        c->haut=0;
                        break;
                    case SDLK_s:
                        c->bas=0;
                        break;
                    case SDLK_a:
                        c->gauche=0;
                        break;
                    case SDLK_d:
                        c->droite=0;
                        break;
                    case SDLK_ESCAPE:
                        c->echap=0;
                        break;
                    case SDLK_SPACE:
                        c->action=0;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                return 1;
                break;
        }
    }
    return 0;
}

/*Gestion des touches pour deux joueurs simultanément*/
int check_controles2(t_controles *c1, t_controles *c2, t_partie *p)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_w:
                        c1->haut=1;
                        break;
                    case SDLK_s:
                        c1->bas=1;
                        break;
                    case SDLK_a:
                        c1->gauche=1;
                        break;
                    case SDLK_d:
                        c1->droite=1;
                        break;
                    case SDLK_ESCAPE:
                        c1->echap=1;
                        break;
                    case SDLK_SPACE:
                        if(p->joueurs[0].etat != 0 && p->joueurs[0].etat != -1)
                        {
                            c1->action=1;
                            poser_bombe(&(p->joueurs[0]), p);
                        }else
                        {
                            c1->action=0;
                        }
                        break;
                    case SDLK_UP:
                        c2->haut=1;
                        break;
                    case SDLK_DOWN:
                        c2->bas=1;
                        break;
                    case SDLK_LEFT:
                        c2->gauche=1;
                        break;
                    case SDLK_RIGHT:
                        c2->droite=1;
                        break;
                    case SDLK_RSHIFT:
                        if(p->joueurs[1].etat != 0 && p->joueurs[1].etat != -1)
                        {
                            c2->action=1;
                            poser_bombe(&(p->joueurs[1]), p);
                        }else
                        {
                            c2->action=0;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym)
                {
                    case SDLK_w:
                        c1->haut=0;
                        break;
                    case SDLK_s:
                        c1->bas=0;
                        break;
                    case SDLK_a:
                        c1->gauche=0;
                        break;
                    case SDLK_d:
                        c1->droite=0;
                        break;
                    case SDLK_ESCAPE:
                        c1->echap=0;
                        break;
                    case SDLK_SPACE:
                        c1->action=0;
                        break;
                    case SDLK_UP:
                        c2->haut=0;
                        break;
                    case SDLK_DOWN:
                        c2->bas=0;
                        break;
                    case SDLK_LEFT:
                        c2->gauche=0;
                        break;
                    case SDLK_RIGHT:
                        c2->droite=0;
                        break;
                    case SDLK_RSHIFT:
                        c2->action=0;
                        break;
                    default:
                        break;
                    }
                break;
            case SDL_QUIT:
                return 1;
                break;
        }
    }
    return 0;
}

void update_joueur(t_joueur *joueur, t_partie *p)
{
    bouger(joueur, p);
    /*Actualisation des compteurs*/
    if(joueur->wait_bombe>0)
    {
        joueur->wait_bombe--;
    }else if(joueur->wait_bombe<0)
    {
        joueur->wait_bombe=0;
    }
    if(joueur->time_bonus>0)
    {
        joueur->time_bonus--;

    }else if(joueur->time_bonus<0)
    {
        joueur->time_bonus=0;
    }
}

void verif_mort(t_partie *p)
{
    int i=0;
    while(i<p->nb_perso)
    {
        /*Même principe que collision, mais vérifie si le joueur n'est pas sur une case mortelle, et n'est pas déjà mort*/
        if(((p->map[((p->joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+(MARGE/2))/TILE_SIZE]>=16 && p->map[((p->joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+(MARGE/2))/TILE_SIZE]<=50)
        || (p->map[((p->joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+p->joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE]>=16 && p->map[((p->joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+p->joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE]<=50)
        || (p->map[((p->joueurs[i].dest.y+p->joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+(MARGE/2))/TILE_SIZE]>=16 && p->map[((p->joueurs[i].dest.y+p->joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+(MARGE/2))/TILE_SIZE]<=50)
        || (p->map[((p->joueurs[i].dest.y+p->joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+p->joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE]>=16 && p->map[((p->joueurs[i].dest.y+p->joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+p->joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE]<=50))
        && (p->joueurs[i].etat!=0 && p->joueurs[i].etat!=-1))
        {
            p->joueurs[i].etat=-1; // En train de mourir
            p->joueurs[i].inventaire=0;
            p->joueurs[i].time_bonus=TIME_MORT; // Plus besoin de time bonus, on l'utilise pour l'animation
        }
        i++;
    }
}

void verif_bonus(t_partie *p, t_reglages *r)
{
    int i=0, bonus=0;
    while(i<p->nb_perso)
    {
        bonus=0;
        /*Même principe que verif_mort, mais vérifie la récolte d'un bonus selon sa case*/
        if(p->map[((p->joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+(MARGE/2))/TILE_SIZE]>=72 && p->map[((p->joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+(MARGE/2))/TILE_SIZE]<=72+NB_BONUS-1)
        {
            bonus=1+p->map[((p->joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+(MARGE/2))/TILE_SIZE]%72;
            p->map[((p->joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+(MARGE/2))/TILE_SIZE]=3;
        }else if((p->map[((p->joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+p->joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE]>=72 && p->map[((p->joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+p->joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE]<=72+NB_BONUS-1))
        {
            bonus=1+p->map[((p->joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+p->joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE]%72;
            p->map[((p->joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+p->joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE]=3;
        }else if(p->map[((p->joueurs[i].dest.y+p->joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+(MARGE/2))/TILE_SIZE]>=72 && p->map[((p->joueurs[i].dest.y+p->joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+(MARGE/2))/TILE_SIZE]<=72+NB_BONUS-1)
        {
            bonus=1+p->map[((p->joueurs[i].dest.y+p->joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+(MARGE/2))/TILE_SIZE]%72;
            p->map[((p->joueurs[i].dest.y+p->joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+(MARGE/2))/TILE_SIZE]=3;
        }else if(p->map[((p->joueurs[i].dest.y+p->joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+p->joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE]>=72 && p->map[((p->joueurs[i].dest.y+p->joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+p->joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE]<=72+NB_BONUS-1)
        {
            bonus=1+p->map[((p->joueurs[i].dest.y+p->joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+p->joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE]%72;
            p->map[((p->joueurs[i].dest.y+p->joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)][(p->joueurs[i].dest.x+p->joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE]=3;
        }
        switch(bonus)
        {
            case 1:
                Mix_PlayMusic(r->bonus, 1);
                p->joueurs[i].inventaire=NB_BOMBES_BONUS;
                break;
            case 2:
                Mix_PlayMusic(r->bonus, 1);
                p->joueurs[i].time_bonus+=TIMER_BOTTES;
                break;
            default:
                break;
        }
        i++;
    }
}

void anim_mort(t_jeu *jeu, t_joueur *joueur)
{
    if(joueur->time_bonus==TIME_MORT) // Initialisation de la mort
    {
        joueur->src.x=0;
        joueur->src.y=4*joueur->src.h; // Emplacement de la première image
    }else if(joueur->time_bonus==0) // Fin de l'animation
    {
        joueur->etat=0;
        jeu->partie.nb_vivants--;
    }else if(joueur->time_bonus%ANIMATION_SPEED==0) // Mise à jour de l'image
    {
        joueur->src.x+=joueur->src.w;
        joueur->src.y+=(joueur->src.x/(3*joueur->src.w))*joueur->src.h; // 3 images par colonne dans le fichier
        joueur->src.x=joueur->src.x%(3*joueur->src.w);
    }
    if(joueur->etat!=0 && joueur->etat!=1) // Toujours en train de mourir
    {
        joueur->time_bonus--;
    }
}
