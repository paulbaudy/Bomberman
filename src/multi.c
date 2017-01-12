#include "multi.h"

void init_jeu(t_jeu *jeu)
{
    int i, quitter = 0;
    int max=0, imax=-1;
    SDL_Surface *map_image  = NULL;
    map_image = IMG_Load("./data/sets/tileset.png");
    SDL_Surface *image_gagnant = NULL;
    image_gagnant = IMG_Load("./data/images/gagnantpartie.png");
    SDL_Surface *image_manche = NULL;
    image_manche = IMG_Load("./data/images/gagnantmanche.png");
    SDL_Surface *egal_manche = NULL;
    egal_manche = IMG_Load("./data/images/egalitemanche.png");
    SDL_Surface *egal_partie = NULL;
    egal_partie = IMG_Load("./data/images/egalitepartie.png");
    TTF_Font *font = NULL;
    font = TTF_OpenFont("./data/ARJULIAN.ttf", 50);

    SDL_Rect position;
    position.y=(SCREEN_HEIGHT/2)-32;
    SDL_Color couleur = {255, 255, 0};
    jeu->partie.manche = 0;
    jeu->partie.nb_perso=NB_JOUEURS_MULTI;
    jeu->partie.joueurs = malloc(sizeof(t_joueur)*NB_JOUEURS_MULTI);

    while (jeu->partie.manche < jeu->reglages.nb_manches && !quitter)
    {
        init_partie(jeu, map_image);
        quitter=jouer_partie(jeu);
        if(quitter)
        {
            break;
        }
        i=0;
        if(jeu->partie.nb_vivants>=2 || jeu->partie.nb_vivants==0) // Si le temps est écoulé ou si tout le monde est mort, il y a égalité
        {
            SDL_BlitSurface(egal_manche, NULL, jeu->screen, NULL);
        }else
        {
            /*Recherche du gagnant*/
            while(jeu->partie.joueurs[i].etat == 0 && i<jeu->partie.nb_perso)
            {
                i++;
            }
            if(i<jeu->partie.nb_perso)
            {
                SDL_Surface *texte = TTF_RenderText_Solid(font, jeu->partie.joueurs[i].nom, couleur);
                position.x=(SCREEN_WIDTH/2)-(strlen(jeu->partie.joueurs[i].nom)/2)*25;
                SDL_BlitSurface(image_manche, NULL, jeu->screen, NULL);
                jeu->partie.joueurs[i].score++;
                SDL_BlitSurface(texte, NULL, jeu->screen, &position);
                SDL_FreeSurface(texte);
            }
        }
        Mix_PlayMusic(jeu->reglages.fin_manche, 1);
        SDL_Flip(jeu->screen);
        SDL_Delay(2000);
    }
    if(quitter==0)
    {
        SDL_FillRect(jeu->screen, NULL, SDL_MapRGB(jeu->screen->format, 16, 120, 48));
        /*Calcul du gagnant*/
        i=0;
        while(i<jeu->partie.nb_perso)
        {
            if(jeu->partie.joueurs[i].score>max)
            {
                max=jeu->partie.joueurs[i].score;
                imax=i;
            }else if(jeu->partie.joueurs[i].score==max) // Si on trouve plusieurs fois le même score max, on retient -1 en signe d'égalité
            {
                imax=-1;
            }
            i++;
        }
        if(imax==-1) // Egalité
        {
            SDL_BlitSurface(egal_partie, NULL, jeu->screen, NULL);
        }else
        {
            position.y+=100;
            SDL_Surface *texte = TTF_RenderText_Solid(font, jeu->partie.joueurs[imax].nom, couleur);
            SDL_BlitSurface(image_gagnant, NULL, jeu->screen, NULL);
            SDL_BlitSurface(texte, NULL, jeu->screen, &position);
            SDL_FreeSurface(texte);
        }
        Mix_PlayMusic(jeu->reglages.fin_partie, 1);
        SDL_Flip(jeu->screen);
        SDL_Delay(5000);
    }

    i=0;
    while(i<jeu->partie.nb_perso)
    {
        SDL_FreeSurface(jeu->partie.joueurs[i].avatar);
        SDL_FreeSurface(jeu->partie.joueurs[i].face);
        i++;
    }
    free(jeu->partie.joueurs);
    SDL_FreeSurface(map_image);
    SDL_FreeSurface(image_manche);
    SDL_FreeSurface(image_gagnant);
    SDL_FreeSurface(egal_manche);
    SDL_FreeSurface(egal_partie);
    TTF_CloseFont(font);

    // Retour au menu
    if(quitter)
    {
        return;
    }
    if(jeu->partie.nb_joueurs == 1)
    {
        draw_menu_jouer1(jeu);
    }else
    {
        draw_menu_jouer2(jeu);
    }
}

void init_partie(t_jeu *jeu, SDL_Surface *map_image)
{
    int i=0;
    jeu->partie.nb_vivants=NB_JOUEURS_MULTI;
    jeu->partie.bombes=malloc(sizeof(t_file_bombe));
    jeu->partie.bombes->sommet=malloc(sizeof(t_bombe));
    jeu->partie.bombes->sommet=NULL;
    jeu->partie.bombes->fin=malloc(sizeof(t_bombe));
    jeu->partie.bombes->fin=NULL;
    jeu->partie.map_image=map_image;
    jeu->partie.map_bonus=IMG_Load("./data/sets/bonuset.png");
    load_map_1(&(jeu->partie));
    jeu->partie.manche++;
    jeu->partie.p_timer=TIMER_PARTIE+1;

    i=0;
    while(i<jeu->partie.nb_joueurs) // Joueurs humains
    {
        jeu->partie.joueurs[i].type=(i+1);
        init_joueur(&(jeu->partie.joueurs[i]), &(jeu->reglages), jeu->partie.manche);
        i++;
    }
    init_ordi(&(jeu->partie), jeu->partie.manche);
}

void init_ordi(t_partie *partie, int manche)
{
    int i=partie->nb_joueurs;
    while(i<partie->nb_perso) // On parcourt le reste des joueurs qui ne sont pas humains
    {
        partie->joueurs[i].type=0;
        if(manche == 1)
        {
            strcpy(partie->joueurs[i].nom, "Ordinateur");
            partie->joueurs[i].score=0;
        }
        partie->joueurs[i].inventaire=0;
        partie->joueurs[i].wait_bombe=0;
        partie->joueurs[i].time_bonus=0;
        partie->joueurs[i].etat=1;
        partie->joueurs[i].dest.h=PLAYER_H;
        partie->joueurs[i].dest.w=TILE_SIZE;
        partie->joueurs[i].src.h=PLAYER_H;
        partie->joueurs[i].src.w=TILE_SIZE;
        partie->joueurs[i].src.x=0;
        switch(i)
        {
            case 1: // En haut à droite
                partie->joueurs[i].dest.x=SCREEN_WIDTH-(TILE_SIZE*2)-1;
                partie->joueurs[i].dest.y=15;
                partie->joueurs[i].src.y=2*PLAYER_H;
                partie->joueurs[i].avatar = IMG_Load("./data/sets/persoset1.png");
                partie->joueurs[i].face = IMG_Load("./data/sets/face1.png");
                break;
            case 2: // En bas à gauche
                partie->joueurs[i].dest.x=TILE_SIZE;
                partie->joueurs[i].dest.y=(MAP_SIZE_H-2)*TILE_SIZE-DEPASSEMENT_TETE;
                partie->joueurs[i].src.y=0;
                partie->joueurs[i].avatar = IMG_Load("./data/sets/persoset2.png");
                partie->joueurs[i].face = IMG_Load("./data/sets/face2.png");
                break;
            case 3: //En bas à droite
                partie->joueurs[i].dest.x=SCREEN_WIDTH-(TILE_SIZE*2)-1;
                partie->joueurs[i].dest.y=(MAP_SIZE_H-2)*TILE_SIZE-DEPASSEMENT_TETE;
                partie->joueurs[i].src.y=0;
                partie->joueurs[i].avatar = IMG_Load("./data/sets/persoset3.png");
                partie->joueurs[i].face = IMG_Load("./data/sets/face3.png");
                break;
            default:
                break;
        }
        init_controles(&(partie->joueurs[i].controles));
        i++;
    }
}

void draw_interface(t_jeu *jeu)
{
    int i, j;
    SDL_Surface *texte = NULL;
    TTF_Font *font = NULL;
    font = TTF_OpenFont("./data/ARJULIAN.ttf", 25);
    SDL_Rect dest, src;
    SDL_Color couleur = {225, 255, 0};
    char score[2];
    score[0]='0';
    score[1]='\0';

    /*Joueur 1*/
    // Affichage de la tête
    dest.x=5;
    dest.y=MAP_SIZE_H*TILE_SIZE+3;
    src.w=75;
    src.h=75;
    src.y=0;
    src.x=0;
    if(jeu->partie.joueurs[0].etat==0) // Si le joueur est mort, on change l'image de sa tête
    {
        src.x=75;
    }
    SDL_BlitSurface(jeu->partie.joueurs[0].face, &src, jeu->screen, &dest);

    // Affichage du score
    texte=TTF_RenderText_Solid(font, jeu->partie.joueurs[0].nom, couleur);
    dest.x=92;
    SDL_BlitSurface(texte, NULL, jeu->screen, &dest);
    src.w=TILE_SIZE;
    src.h=TILE_SIZE;
    dest.y+=30;
    score[0]='0'+jeu->partie.joueurs[0].score;
    texte=TTF_RenderText_Solid(font, score, couleur);
    SDL_BlitSurface(texte, NULL, jeu->screen, &dest);

    // Affichage des bonus
    if(jeu->partie.joueurs[0].inventaire>0)
    {
        dest.x+=TILE_SIZE;
        src.x=0;
        src.y=TILE_SIZE;
        SDL_BlitSurface(jeu->partie.map_bonus, &src, jeu->screen, &dest);
    }
    if(jeu->partie.joueurs[0].time_bonus>0 && jeu->partie.joueurs[0].etat!=-1)
    {
        dest.x+=TILE_SIZE;
        src.x=TILE_SIZE;
        src.y=TILE_SIZE;
        SDL_BlitSurface(jeu->partie.map_bonus, &src, jeu->screen, &dest);
    }

    /*Joueur 3*/
    // Idem
    dest.x=5;
    dest.y=SCREEN_HEIGHT-78;
    src.w=75;
    src.h=75;
    src.y=0;
    src.x=0;
    if(jeu->partie.joueurs[2].etat==0)
    {
        src.x=75;
    }
    SDL_BlitSurface(jeu->partie.joueurs[2].face, &src, jeu->screen, &dest);
    texte=TTF_RenderText_Solid(font, jeu->partie.joueurs[2].nom, couleur);
    dest.y=MAP_SIZE_H*TILE_SIZE+125;
    dest.x=92;
    SDL_BlitSurface(texte, NULL, jeu->screen, &dest);
    src.w=TILE_SIZE;
    src.h=TILE_SIZE;
    dest.y-=30;
    score[0]='0'+jeu->partie.joueurs[2].score;
    texte=TTF_RenderText_Solid(font, score, couleur);
    SDL_BlitSurface(texte, NULL, jeu->screen, &dest);
    if(jeu->partie.joueurs[2].inventaire>0)
    {
        dest.x+=TILE_SIZE;
        src.x=0;
        src.y=TILE_SIZE;
        SDL_BlitSurface(jeu->partie.map_bonus, &src, jeu->screen, &dest);
    }
    if(jeu->partie.joueurs[2].time_bonus>0 && jeu->partie.joueurs[2].etat!=-1)
    {
        dest.x+=TILE_SIZE;
        src.x=TILE_SIZE;
        src.y=TILE_SIZE;
        SDL_BlitSurface(jeu->partie.map_bonus, &src, jeu->screen, &dest);
    }

    /*Joueur 2*/
    dest.x=SCREEN_WIDTH-80;
    dest.y=MAP_SIZE_H*TILE_SIZE+3;
    src.w=75;
    src.h=75;
    src.y=0;
    src.x=0;
    if(jeu->partie.joueurs[1].etat==0)
    {
        src.x=75;
    }
    SDL_BlitSurface(jeu->partie.joueurs[1].face, &src, jeu->screen, &dest);
    texte=TTF_RenderText_Solid(font, jeu->partie.joueurs[1].nom, couleur);
    dest.y=MAP_SIZE_H*TILE_SIZE+3;
    dest.x=SCREEN_WIDTH-70-(strlen(jeu->partie.joueurs[1].nom)*15);
    SDL_BlitSurface(texte, NULL, jeu->screen, &dest);
    src.w=TILE_SIZE;
    src.h=TILE_SIZE;
    dest.y+=30;
    dest.x=SCREEN_WIDTH-75-TILE_SIZE;
    score[0]='0'+jeu->partie.joueurs[1].score;
    texte=TTF_RenderText_Solid(font, score, couleur);
    SDL_BlitSurface(texte, NULL, jeu->screen, &dest);
    dest.x-=15;
    if(jeu->partie.joueurs[1].inventaire>0)
    {
        dest.x-=TILE_SIZE;
        src.x=0;
        src.y=TILE_SIZE;
        SDL_BlitSurface(jeu->partie.map_bonus, &src, jeu->screen, &dest);
    }
    if(jeu->partie.joueurs[1].time_bonus>0 && jeu->partie.joueurs[1].etat!=-1)
    {
        dest.x-=TILE_SIZE;
        src.x=TILE_SIZE;
        src.y=TILE_SIZE;
        SDL_BlitSurface(jeu->partie.map_bonus, &src, jeu->screen, &dest);
    }

    /*Joueur 4*/
    dest.x=SCREEN_WIDTH-80;
    dest.y=SCREEN_HEIGHT-78;
    src.w=75;
    src.h=75;
    src.y=0;
    src.x=0;
    if(jeu->partie.joueurs[3].etat==0)
    {
        src.x=75;
    }
    SDL_BlitSurface(jeu->partie.joueurs[3].face, &src, jeu->screen, &dest);
    texte=TTF_RenderText_Solid(font, jeu->partie.joueurs[3].nom, couleur);
    dest.y=MAP_SIZE_H*TILE_SIZE+125;
    dest.x=SCREEN_WIDTH-70-(strlen(jeu->partie.joueurs[3].nom)*15);
    SDL_BlitSurface(texte, NULL, jeu->screen, &dest);
    src.w=TILE_SIZE;
    src.h=TILE_SIZE;
    dest.y-=30;
    dest.x=SCREEN_WIDTH-75-TILE_SIZE;
    score[0]='0'+jeu->partie.joueurs[3].score;
    texte=TTF_RenderText_Solid(font, score, couleur);
    SDL_BlitSurface(texte, NULL, jeu->screen, &dest);
    dest.x-=15;
    if(jeu->partie.joueurs[3].inventaire>0)
    {
        dest.x-=TILE_SIZE;
        src.x=0;
        src.y=TILE_SIZE;
        SDL_BlitSurface(jeu->partie.map_bonus, &src, jeu->screen, &dest);
    }
    if(jeu->partie.joueurs[3].time_bonus>0 && jeu->partie.joueurs[3].etat!=-1)
    {
        dest.x-=TILE_SIZE;
        src.x=TILE_SIZE;
        src.y=TILE_SIZE;
        SDL_BlitSurface(jeu->partie.map_bonus, &src, jeu->screen, &dest);
    }

    /*Affichage des briques sur les côtés de l'horloge*/
    src.x=TILE_SIZE;
    src.y=0;
    src.h=TILE_SIZE;
    src.w=TILE_SIZE;

    for(i=MAP_SIZE_H*TILE_SIZE;i<SCREEN_HEIGHT;i+=TILE_SIZE)
    {
        dest.x=TILE_SIZE*7;
        dest.y=i;
        SDL_BlitSurface(jeu->partie.map_image, &src, jeu->screen, &dest);
        dest.x=TILE_SIZE*13;
        SDL_BlitSurface(jeu->partie.map_image, &src, jeu->screen, &dest);
    }
    src.x=0;
    src.y=0;
    for(j=TILE_SIZE*8; j<TILE_SIZE*13;j+=TILE_SIZE)
    {
        for(i=MAP_SIZE_H*TILE_SIZE;i<SCREEN_HEIGHT;i+=TILE_SIZE)
        {
            dest.x=j;
            dest.y=i;
            SDL_BlitSurface(jeu->partie.map_image, &src, jeu->screen, &dest);
        }
    }
    TTF_CloseFont(font);
    SDL_FreeSurface(texte);
}

int jouer_partie(t_jeu *jeu)
{
    int compute=1, i=0, stilltime=1,quitter=0;
    unsigned int fps = SDL_GetTicks()+16;
    char *horloge = malloc(sizeof(char)*5);
    horloge[0]='0'+jeu->partie.p_timer/3600;
    horloge[1]=':';
    horloge[2]='0'+(jeu->partie.p_timer%3600)/10;
    horloge[3]='0'+(jeu->partie.p_timer%3600)%10;
    horloge[4]='\0';

    while(compute && stilltime && !quitter)
    {
        SDL_FillRect(jeu->screen, NULL, SDL_MapRGB(jeu->screen->format, 16, 120, 48));
        draw_all(jeu);
        if(jeu->partie.nb_joueurs==2)
        {
            quitter=check_controles2(&(jeu->partie.joueurs[0].controles), &(jeu->partie.joueurs[1].controles), &(jeu->partie));
        }else if(jeu->partie.nb_joueurs==1 && jeu->partie.joueurs[0].etat != 0 && jeu->partie.joueurs[0].etat != -1) // Si le joueur n'est pas mort
        {
            quitter=check_controles(&(jeu->partie.joueurs[0].controles), &(jeu->partie));
        }
        i=0;

        while(i<jeu->partie.nb_perso)
        {
            if(jeu->partie.joueurs[i].etat==-1) // En train de mourir
            {
                anim_mort(jeu, &(jeu->partie.joueurs[i]));
            }else if(jeu->partie.joueurs[i].etat!=0) // Si vivant
            {
                if(i>=jeu->partie.nb_joueurs)
                {
                    IA(jeu);
                }
                collision(&(jeu->partie.joueurs[i]), &(jeu->partie));
                update_joueur(&(jeu->partie.joueurs[i]), &(jeu->partie));
            }
            i++;
        }
        if(jeu->partie.nb_vivants<=1) // Fin de la manche
        {
            compute=0;
        }
        draw_interface(jeu);
        stilltime=timer_map(jeu, horloge);
        SDL_Flip(jeu->screen);
        SDL_Delay(1);
        delay(fps);
        fps = SDL_GetTicks()+16;
    }
    free(horloge);
    return quitter;
}

/*Gestion des décisions de l'intelligence artificielle*/
void IA(t_jeu *jeu)
{
    int dist, i, min=2000, id_j=0, j, taille_bombe, trouve_bombe;
    t_bombe* temp = malloc(sizeof(t_bombe));

    /*On parcourt les joueurs qui ne sont pas humains*/
    i=jeu->partie.nb_joueurs;
    while(i<jeu->partie.nb_perso)
    {
        trouve_bombe=0;
        temp=jeu->partie.bombes->sommet;
        while(temp!=NULL && !trouve_bombe) // On vérifie qu'une bombe ne menace pas l'ordinateur
        {
            if(temp->type==0)
            {
                taille_bombe=3;
            }else
            {
                taille_bombe=MAP_SIZE_W/2;
            }
            j=0;
            /*On parcourt les cases de la map en bas de la bombe pour vérifier si un joueur s'y trouve*/
            while(j<=taille_bombe+1 && jeu->partie.map[temp->my+j][temp->mx]!=1 && jeu->partie.map[temp->my+j][temp->mx]!=2 && !trouve_bombe)
            {
                // Si un des quatre coins du personnage est menacé
                if( (((jeu->partie.joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)==temp->my+j && (jeu->partie.joueurs[i].dest.x+(MARGE/2))/TILE_SIZE==temp->mx)
                ||  (((jeu->partie.joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)==temp->my+j && (jeu->partie.joueurs[i].dest.x+jeu->partie.joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE==temp->mx)
                ||  (((jeu->partie.joueurs[i].dest.y+jeu->partie.joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)==temp->my+j && (jeu->partie.joueurs[i].dest.x+(MARGE/2))/TILE_SIZE==temp->mx)
                ||  (((jeu->partie.joueurs[i].dest.y+jeu->partie.joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)==temp->my+j && (jeu->partie.joueurs[i].dest.x+jeu->partie.joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE==temp->mx))
                {
                    if(jeu->partie.joueurs[i].controles.droite || jeu->partie.joueurs[i].controles.bas ) // Si il est déjà en train de l'esquiver
                    {
                        if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==1) // Pas de collision si la fonction renvoie 1.
                        {
                            trouve_bombe=1;
                            break;
                        }
                    }
                    /*On cherche un déplacement possible*/
                    init_controles(&(jeu->partie.joueurs[i].controles));
                    jeu->partie.joueurs[i].controles.gauche=1;
                    if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==-1)
                    {
                        jeu->partie.joueurs[i].controles.droite=1;
                        if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==-1)
                        {
                            jeu->partie.joueurs[i].controles.bas=1;
                            if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==-1)
                            {
                                jeu->partie.joueurs[i].controles.haut=1;
                            }
                        }
                    }
                    trouve_bombe=1;
                }
                j++;
            }
            j=0;
            /*Même chose en haut de la bombe*/
            while(j<=taille_bombe+1 && jeu->partie.map[temp->my-j][temp->mx]!=1 && jeu->partie.map[temp->my-j][temp->mx]!=2 && !trouve_bombe)
            {
                if( (((jeu->partie.joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)==temp->my-j && (jeu->partie.joueurs[i].dest.x+(MARGE/2))/TILE_SIZE==temp->mx)
                ||  (((jeu->partie.joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)==temp->my-j && (jeu->partie.joueurs[i].dest.x+jeu->partie.joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE==temp->mx)
                ||  (((jeu->partie.joueurs[i].dest.y+jeu->partie.joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)==temp->my-j && (jeu->partie.joueurs[i].dest.x+(MARGE/2))/TILE_SIZE==temp->mx)
                ||  (((jeu->partie.joueurs[i].dest.y+jeu->partie.joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)==temp->my-j && (jeu->partie.joueurs[i].dest.x+jeu->partie.joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE==temp->mx))
                {
                    if(jeu->partie.joueurs[i].controles.droite || jeu->partie.joueurs[i].controles.haut )
                    {
                        if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==1)
                        {
                            trouve_bombe=1;
                            break;
                        }
                    }
                    init_controles(&(jeu->partie.joueurs[i].controles));
                    jeu->partie.joueurs[i].controles.gauche=1;
                    if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==-1)
                    {
                        jeu->partie.joueurs[i].controles.droite=1;
                        if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==-1)
                        {
                            jeu->partie.joueurs[i].controles.haut=1;
                            if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==-1)
                            {
                                jeu->partie.joueurs[i].controles.bas=1;
                            }
                        }
                    }
                    trouve_bombe=1;
                }
                j++;
            }
            j=0;
            /*Même chose à droite*/
            while(j<=taille_bombe+1 && jeu->partie.map[temp->my][temp->mx+j]!=1 && jeu->partie.map[temp->my][temp->mx+j]!=2 && !trouve_bombe)
            {

                if( (((jeu->partie.joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)==temp->my && (jeu->partie.joueurs[i].dest.x+(MARGE/2))/TILE_SIZE==temp->mx+j)
                ||  (((jeu->partie.joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)==temp->my && (jeu->partie.joueurs[i].dest.x+jeu->partie.joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE==temp->mx+j)
                ||  (((jeu->partie.joueurs[i].dest.y+jeu->partie.joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)==temp->my && (jeu->partie.joueurs[i].dest.x+(MARGE/2))/TILE_SIZE==temp->mx+j)
                ||  (((jeu->partie.joueurs[i].dest.y+jeu->partie.joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)==temp->my && (jeu->partie.joueurs[i].dest.x+jeu->partie.joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE==temp->mx+j))
                {
                    if(jeu->partie.joueurs[i].controles.droite || jeu->partie.joueurs[i].controles.bas )
                    {
                        if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==1)
                        {
                            trouve_bombe=1;
                            break;
                        }
                    }
                    init_controles(&(jeu->partie.joueurs[i].controles));
                    jeu->partie.joueurs[i].controles.haut=1;
                    if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==-1)
                    {
                        jeu->partie.joueurs[i].controles.bas=1;
                        if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==-1)
                        {
                            jeu->partie.joueurs[i].controles.droite=1;
                            if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==-1)
                            {
                                jeu->partie.joueurs[i].controles.gauche=1;
                            }
                        }
                    }
                    trouve_bombe=1;
                }
                j++;
            }
            j=0;
            /*Même chose à gauche*/
            while(j<=taille_bombe+1 && jeu->partie.map[temp->my][temp->mx-j]!=1 && jeu->partie.map[temp->my][temp->mx-j]!=2 && !trouve_bombe)
            {
                if( (((jeu->partie.joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)==temp->my && (jeu->partie.joueurs[i].dest.x+(MARGE/2))/TILE_SIZE==temp->mx-j)
                ||  (((jeu->partie.joueurs[i].dest.y+DEPASSEMENT_TETE+(MARGE/2))/TILE_SIZE)==temp->my && (jeu->partie.joueurs[i].dest.x+jeu->partie.joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE==temp->mx-j)
                ||  (((jeu->partie.joueurs[i].dest.y+jeu->partie.joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)==temp->my && (jeu->partie.joueurs[i].dest.x+(MARGE/2))/TILE_SIZE==temp->mx-j)
                ||  (((jeu->partie.joueurs[i].dest.y+jeu->partie.joueurs[i].dest.h-(MARGE/2))/TILE_SIZE)==temp->my && (jeu->partie.joueurs[i].dest.x+jeu->partie.joueurs[i].dest.w-1-(MARGE/2))/TILE_SIZE==temp->mx-j))
                {
                    if(jeu->partie.joueurs[i].controles.gauche || jeu->partie.joueurs[i].controles.bas )
                    {
                        if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==1)
                        {
                            trouve_bombe=1;
                            break;
                        }
                    }
                    init_controles(&(jeu->partie.joueurs[i].controles));
                    jeu->partie.joueurs[i].controles.haut=1;
                    if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==-1)
                    {
                        jeu->partie.joueurs[i].controles.bas=1;
                        if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==-1)
                        {
                            jeu->partie.joueurs[i].controles.gauche=1;
                            if(collision(&(jeu->partie.joueurs[i]), &(jeu->partie))==-1)
                            {
                                jeu->partie.joueurs[i].controles.droite=1;
                            }
                        }
                    }
                    trouve_bombe=1;
                }
                j++;
            }
            temp=temp->suivant;
        }
        /*Si le joueur n'est pas menacé*/
        if(!trouve_bombe)
        {

            init_controles(&(jeu->partie.joueurs[i].controles));
            j=0;
            min=2000;
            /*Sélection du joueur le plus proche*/
            while(j<jeu->partie.nb_perso)
            {
                if(j!=i && jeu->partie.joueurs[j].etat!=0 && jeu->partie.joueurs[j].etat!=-1)
                {
                    dist=sqrt(((jeu->partie.joueurs[j].dest.x-jeu->partie.joueurs[i].dest.x)*(jeu->partie.joueurs[j].dest.x-jeu->partie.joueurs[i].dest.x))+((jeu->partie.joueurs[j].dest.y-jeu->partie.joueurs[i].dest.y)*(jeu->partie.joueurs[j].dest.y-jeu->partie.joueurs[i].dest.y)));
                    if(dist<min)
                    {
                        id_j=j;
                        min=dist;
                    }
                }
                j++;
            }
            /*Si il est sur un autre joueur, il l'attaque*/
            if(jeu->partie.joueurs[id_j].dest.x==jeu->partie.joueurs[i].dest.x && jeu->partie.joueurs[id_j].dest.y==jeu->partie.joueurs[i].dest.y)
            {
                poser_bombe(&(jeu->partie.joueurs[i]), &(jeu->partie));
            }else if(val_abs(jeu->partie.joueurs[id_j].dest.x-jeu->partie.joueurs[i].dest.x)<val_abs(jeu->partie.joueurs[id_j].dest.y-jeu->partie.joueurs[i].dest.y))
            {
                /*Il cherche la direction appropriée pour le rejoindre*/
                if(jeu->partie.joueurs[id_j].dest.y-jeu->partie.joueurs[i].dest.y<0)
                {
                    if(jeu->partie.map[(jeu->partie.joueurs[i].dest.y+DEPASSEMENT_TETE-1)/TILE_SIZE][(jeu->partie.joueurs[i].dest.x+(jeu->partie.joueurs[i].dest.w/2))/TILE_SIZE]!=1 && jeu->partie.map[(jeu->partie.joueurs[i].dest.y+DEPASSEMENT_TETE-1)/TILE_SIZE][(jeu->partie.joueurs[i].dest.x+(jeu->partie.joueurs[i].dest.w/2))/TILE_SIZE]!=2)
                    {
                        jeu->partie.joueurs[i].controles.haut=1;
                    }else
                    {
                        poser_bombe(&(jeu->partie.joueurs[i]), &(jeu->partie));
                    }
                }else
                {
                    if(jeu->partie.map[(jeu->partie.joueurs[i].dest.y+jeu->partie.joueurs[i].dest.h+1)/TILE_SIZE][(jeu->partie.joueurs[i].dest.x+(jeu->partie.joueurs[i].dest.w/2))/TILE_SIZE]!=1 && jeu->partie.map[(jeu->partie.joueurs[i].dest.y+jeu->partie.joueurs[i].dest.h+1)/TILE_SIZE][(jeu->partie.joueurs[i].dest.x+(jeu->partie.joueurs[i].dest.w/2))/TILE_SIZE]!=2)
                    {
                        jeu->partie.joueurs[i].controles.bas=1;
                    }else
                    {
                        poser_bombe(&(jeu->partie.joueurs[i]), &(jeu->partie));
                    }
                }
            }else
            {
                if(jeu->partie.joueurs[id_j].dest.x-jeu->partie.joueurs[i].dest.x>0)
                {
                    if(jeu->partie.map[(jeu->partie.joueurs[i].dest.y+(jeu->partie.joueurs[i].dest.h/2))/TILE_SIZE][(jeu->partie.joueurs[i].dest.x+jeu->partie.joueurs[i].dest.w+1)/TILE_SIZE]!=1 && jeu->partie.map[(jeu->partie.joueurs[i].dest.y+(jeu->partie.joueurs[i].dest.h/2))/TILE_SIZE][(jeu->partie.joueurs[i].dest.x+jeu->partie.joueurs[i].dest.w+1)/TILE_SIZE]!=2)
                    {
                        jeu->partie.joueurs[i].controles.droite=1;
                    }else
                    {
                        poser_bombe(&(jeu->partie.joueurs[i]), &(jeu->partie));
                    }
                }else
                {
                    if(jeu->partie.map[(jeu->partie.joueurs[i].dest.y+(jeu->partie.joueurs[i].dest.h/2))/TILE_SIZE][(jeu->partie.joueurs[i].dest.x-1)/TILE_SIZE]!=1 && jeu->partie.map[(jeu->partie.joueurs[i].dest.y+(jeu->partie.joueurs[i].dest.h/2))/TILE_SIZE][(jeu->partie.joueurs[i].dest.x-1)/TILE_SIZE]!=2)
                    {
                        jeu->partie.joueurs[i].controles.gauche=1;
                    }else
                    {
                        poser_bombe(&(jeu->partie.joueurs[i]), &(jeu->partie));
                    }
                }
            }
        }
        i++;
    }
}
