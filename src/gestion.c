#include "gestion.h"

int init_moteur(t_jeu *jeu)
{
    // Initialisation SDL
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf("Unable to init SDL: %s\n", SDL_GetError() );
        return -1;
    }
    // Initialisation SDL TTF
    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Unable to load TTF_Init : %s\n", TTF_GetError());
        return -1;
    }
    // Initialisation de l'audio
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024)==-1)
    {
        printf("Unable to init Mix_audio : %s\n", Mix_GetError());
        return -1;
    }
    Mix_VolumeMusic(jeu->reglages.volume);
    // Icone de la fenêtre
    SDL_WM_SetIcon(IMG_Load("./data/icone.png"), 0 );

    if ( !jeu->screen )
    {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return -1;
    }
    SDL_WM_SetCaption("Bomberman", NULL);

    // On cache le curseur
    SDL_ShowCursor(SDL_DISABLE);

    SDL_EnableUNICODE(1);

    return 0;
}

void init_programme()
{
    // Initialisation de la variable principale
    t_jeu jeu;
    jeu.screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
    jeu.reglages.nb_manches = NB_MANCHES_DEFAUT;
    strcpy(jeu.reglages.nom1, NOM1_DEFAUT);
    strcpy(jeu.reglages.nom2, NOM2_DEFAUT);
    jeu.reglages.volume=10;
    if(init_moteur(&jeu)!=-1)
    {
        jeu.reglages.explose = Mix_LoadMUS("./data/sons/bom.wav");
        jeu.reglages.put = Mix_LoadMUS("./data/sons/put.wav");
        jeu.reglages.bonus = Mix_LoadMUS("./data/sons/bonus.wav");
        jeu.reglages.fin_manche = Mix_LoadMUS("./data/sons/fin_manche.wav");
        jeu.reglages.fin_partie = Mix_LoadMUS("./data/sons/fin_partie.wav");
        jeu.reglages.entree = Mix_LoadMUS("./data/sons/entree.wav");
        jeu.reglages.perdu = Mix_LoadMUS("./data/sons/perdu.wav");
        draw_menu(&jeu);
    }

    // Libérer les sons + Quitter la SDL
    Mix_FreeMusic(jeu.reglages.explose);
    Mix_FreeMusic(jeu.reglages.put);
    Mix_FreeMusic(jeu.reglages.bonus);
    Mix_FreeMusic(jeu.reglages.fin_manche);
    Mix_FreeMusic(jeu.reglages.fin_partie);
    Mix_FreeMusic(jeu.reglages.entree);
    TTF_Quit();
    SDL_Quit();
    Mix_CloseAudio();
}


int val_abs(int x)
{
    return x<0?-x:x;
}

void bouger(t_joueur *j, t_partie *p)
{
    if(j->controles.haut==1)
    {/*Mise à jour de l'animation*/
        j->src.y=0; // Sélection de l'image de la direction appropriée
        if(p->p_timer%ANIMATION_SPEED==0) // Évolution de l'animation du déplacement
        {
            if(j->src.x==TILE_SIZE)
            {
                j->src.x+=TILE_SIZE;
            }else
            {
                j->src.x=TILE_SIZE;
            }
        }
        /*Déplacement du personnage*/
        if(j->time_bonus>0) // Avec bonus de vitesse
        {
            j->dest.y-=SPEED*2;
        }else
        {
            j->dest.y-=SPEED;
        }
    }else /*Idem pour toutes les autres directions*/
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
        if(j->time_bonus>0)
        {
            j->dest.y+=SPEED*2;
        }else
        {
            j->dest.y+=SPEED;
        }
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
        if(j->time_bonus>0)
        {
            j->dest.x-=SPEED*2;
        }else
        {
            j->dest.x-=SPEED;
        }
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
        if(j->time_bonus>0)
        {
            j->dest.x+=SPEED*2;
        }else
        {
            j->dest.x+=SPEED;
        }
    }else
    {
        j->src.x=0; // Aucun déplacement : personnage immobile
    }
}

/*Collision permet de vérifier si un personnage se heurte (renvoie -1) ou s'il peut avancer (renvoie 1)*/
int collision(t_joueur *j, t_partie *p)
{
    /*Pour pouvoir passer, on vérifie que le haut et le bas de son corps ne touche pas un bloc dur*/
    int mx, my, a, b;
    if(j->controles.droite==1 && !j->controles.bas && !j->controles.haut && !j->controles.gauche)
    {
        // Case de la map où il veut aller par rapport aux pieds
        mx=(j->dest.x+SPEED+j->dest.w-1)/TILE_SIZE;
        my=(j->dest.y+j->dest.h)/TILE_SIZE;
        a=p->map[my][mx]; // Type du bloc trouvé

        // Même chose avec ses épaules
        my=(j->dest.y+DEPASSEMENT_TETE)/TILE_SIZE;
        b=p->map[my][mx];

        // Valeurs des blocs infranchissables
        if(a==1 || (a>=10 && a<=15) || b==1 || (b>=10 && b<=15) || a==2 || (a>=51 && a<=56) || (b>=51 && b<=56) || b==2)
        {
            // S'il ne passe pas, on renouvelle les vérifications en prenant en compte la marge
            mx=(j->dest.x+j->dest.w+SPEED-1)/TILE_SIZE;
            my=(j->dest.y+j->dest.h-MARGE)/TILE_SIZE;
            a=p->map[my][mx];

            my=(j->dest.y+DEPASSEMENT_TETE+MARGE)/TILE_SIZE;
            b=p->map[my][mx];

            if(a==1 || (a>=10 && a<=15) || b==1 || (b>=10 && b<=15) || a==2 || (a>=51 && a<=56) || (b>=51 && b<=56) || b==2)
            {// Impossible de passer
                j->controles.droite=0;
                return -1;
            }else // Il passe avec la marge. On ajuste sa position
            {
                if(a==1 || (a>=10 && a<=15) || a==2) // Les pieds bloquaient
                {
                    j->dest.y=(my-1)*TILE_SIZE - DEPASSEMENT_TETE;
                    return 1;
                }else
                {
                    j->dest.y=(my)*TILE_SIZE - DEPASSEMENT_TETE;
                    return 1;
                }
            }
        }
        return 1;
    }
    // Idem pour les autres directions
    if(j->controles.gauche==1 && !j->controles.droite && !j->controles.bas && !j->controles.haut)
    {
        mx=(j->dest.x-SPEED)/TILE_SIZE;
        my=(j->dest.y+j->dest.h)/TILE_SIZE;
        a=p->map[my][mx];

        my=(j->dest.y+DEPASSEMENT_TETE)/TILE_SIZE;
        b=p->map[my][mx];

        if(a==1 || (a>=10 && a<=15) || b==1 || (b>=10 && b<=15) || a==2 || (a>=51 && a<=56) || (b>=51 && b<=56) || b==2)
        {
            mx=(j->dest.x-SPEED)/TILE_SIZE;
            my=(j->dest.y+j->dest.h-MARGE)/TILE_SIZE;
            a=p->map[my][mx];

            my=(j->dest.y+DEPASSEMENT_TETE+MARGE)/TILE_SIZE;
            b=p->map[my][mx];

            if(a==1 || (a>=10 && a<=15) || b==1 || (b>=10 && b<=15) || a==2 || (a>=51 && a<=56) || (b>=51 && b<=56) || b==2)
            {
                j->controles.gauche=0;
                return -1;
            }else
            {
                if(a==1 || (a>=10 && a<=15) || a==2)
                {
                    j->dest.y=(my-1)*TILE_SIZE - DEPASSEMENT_TETE;
                    return 1;
                }else
                {
                    j->dest.y=(my)*TILE_SIZE - DEPASSEMENT_TETE;
                    return 1;
                }
            }
        }
        return 1;
    }
    if(j->controles.haut==1)
    {
        mx=(j->dest.x)/TILE_SIZE;
        my=(j->dest.y-SPEED+DEPASSEMENT_TETE)/TILE_SIZE;
        a=p->map[my][mx];

        mx=(j->dest.x+j->dest.w-1)/TILE_SIZE;
        b=p->map[my][mx];

        if(a==1 || (a>=10 && a<=15) || b==1 || (b>=10 && b<=15) || a==2 || (a>=51 && a<=56) || (b>=51 && b<=56) || b==2)
        {
            mx=(j->dest.x+MARGE)/TILE_SIZE;
            my=(j->dest.y-SPEED+DEPASSEMENT_TETE)/TILE_SIZE;
            a=p->map[my][mx];

            mx=(j->dest.x+j->dest.w-MARGE-1)/TILE_SIZE;
            b=p->map[my][mx];

            if(a==1 || (a>=10 && a<=15) || b==1 || (b>=10 && b<=15) || a==2 || (a>=51 && a<=56) || (b>=51 && b<=56) || b==2)
            {
                j->controles.haut=0;
                return -1;
            }else
            {
                if(a==1 || (a>=10 && a<=15) || a==2)
                {
                    j->dest.x=(mx-1)*TILE_SIZE;
                    return 1;
                }else
                {
                    j->dest.x=(mx)*TILE_SIZE;
                    return 1;
                }
            }
        }
        return 1;
    }
    if(j->controles.bas==1)
    {
        mx=(j->dest.x)/TILE_SIZE;
        my=(j->dest.y+SPEED+j->dest.h)/TILE_SIZE;
        a=p->map[my][mx];

        mx=(j->dest.x+j->dest.w-1)/TILE_SIZE;
        b=p->map[my][mx];


        if(a==1 || (a>=10 && a<=15) || b==1 || (b>=10 && b<=15) || a==2 || (a>=51 && a<=56) || (b>=51 && b<=56) || b==2)
        {
            mx=(j->dest.x+MARGE)/TILE_SIZE;
            my=(j->dest.y+SPEED+j->dest.h)/TILE_SIZE;
            a=p->map[my][mx];

            mx=(j->dest.x+j->dest.w-MARGE-1)/TILE_SIZE;
            b=p->map[my][mx];

            if(a==1 || (a>=10 && a<=15) || b==1 || (b>=10 && b<=15) || a==2 || (a>=51 && a<=56) || (b>=51 && b<=56) || b==2)
            {
                j->controles.bas=0;
                return -1;
            }else
            {
                if(a==1 || (a>=10 && a<=15) || a==2)
                {
                    j->dest.x=(mx-1)*TILE_SIZE;
                    return 1;
                }else
                {
                    j->dest.x=(mx)*TILE_SIZE;
                    return 1;
                }
            }
        }
        return 1;
    }
    return 0;
}

void poser_bombe(t_joueur *j, t_partie *p)
{
    if(j->etat==0 || j->etat==-1) // Personnage mort : état à 0. En train de mourir : état à -1
    {
        j->controles.action=0;
        return;
    }

    int i=0;
    if(j->wait_bombe>0) // Il doit attendre avant de pouvoir poser une nouvelle bombe
    {
        j->controles.action=0;
        return;
    }else
    {
        if(j->type==0) // Si c'est un ordinateur
        {
            j->wait_bombe=TIME_BOMBE*2; // Pour éviter les suicides
        }else
        {
            j->wait_bombe=TIME_BOMBE;
        }
    }
    /*Initialisation de la bombe*/
    t_bombe *bombe=malloc(sizeof(t_bombe));
    bombe->timer=TIME_BOMBE;
    // Type de la bombe selon l'inventaire
    if(j->inventaire==0)
    {
        bombe->type=0;
    }else
    {
        j->inventaire--;
        bombe->type=1;
    }

    // Définition de la position de la bombe
    if((j->dest.x%TILE_SIZE)>(TILE_SIZE/2))
    {
        bombe->mx=(j->dest.x/TILE_SIZE)+1;

    }else
    {
        bombe->mx=(j->dest.x/TILE_SIZE);
    }

    if(((j->dest.y+DEPASSEMENT_TETE)%TILE_SIZE)>(TILE_SIZE/2))
    {
        bombe->my=((j->dest.y+DEPASSEMENT_TETE)/TILE_SIZE)+1;
    }else
    {
        bombe->my=((j->dest.y+DEPASSEMENT_TETE)/TILE_SIZE);
    }

    if(p->map[bombe->my][bombe->mx]>=10 && p->map[bombe->my][bombe->mx]<=15) // Il y a déjà une bombe à cet endroit
    {// On quitte sans ajouter la bombe à la file
        free(bombe);
        return;
    }
    // Repositionnement du personnage
    j->dest.x=bombe->mx*TILE_SIZE;
    j->dest.y=bombe->my*TILE_SIZE-DEPASSEMENT_TETE;
    if(bombe->type==0)
    {
        bombe->h=5;
        bombe->w=5;
    }else if(bombe->type==1)
    {
        bombe->h=MAP_SIZE_H;
        bombe->w=MAP_SIZE_W;
    }
    /*Initialisation de la map de la bombe selon sa taille. Cases nécessaires à l'explosion dans un ordre optimal*/
    bombe->mapb=malloc(2*sizeof(int*));
    bombe->mapb[0]=malloc(bombe->h*sizeof(int)); // Flammes verticales + centre
    bombe->mapb[1]=malloc((bombe->w-1)*sizeof(int)); // Flammes horizontales
    bombe->mapb[0][0]=46;
    bombe->mapb[0][bombe->h-1]=41;
    for(i=1;i<bombe->h-1;i++)
    {
        bombe->mapb[0][i]=36;
    }
    bombe->mapb[0][(bombe->h/2)]=31;
    for(i=0;i<bombe->w-2;i++)
    {
        bombe->mapb[1][i]=21;
    }
    bombe->mapb[1][((bombe->w-2)/2)]=16;
    bombe->mapb[1][bombe->w-2]=26;

    if(bombe->type==0)
    {
        p->map[bombe->my][bombe->mx]=13;
    }else if(bombe->type==1)
    {
        p->map[bombe->my][bombe->mx]=10;
    }
    bombe->suivant=NULL;
    if(p->bombes->sommet==NULL) // Pas de bombe dans la file
    {
        p->bombes->sommet=bombe;
    }else
    {
        p->bombes->fin->suivant=bombe;
    }
    p->bombes->fin=bombe;
}

void draw_all(t_jeu *jeu)
{
    int i=0;
    update_map(&(jeu->partie), &(jeu->reglages)); // Mise à jour des bombes et des explosions
    map(jeu); // Redessiner la map
    while(i<jeu->partie.nb_perso)
    {
        if(jeu->partie.joueurs[i].etat) // Si il est vivant
        {
            SDL_BlitSurface(jeu->partie.joueurs[i].avatar, &(jeu->partie.joueurs[i].src), jeu->screen, &(jeu->partie.joueurs[i].dest));
        }
        i++;
    }
}

void update_map(t_partie *p, t_reglages *r)
{
    t_bombe *temp=malloc(sizeof(t_bombe));
    if(p->bombes->sommet==NULL)
    {
        temp=NULL; // File vide
    }else
    {
        temp=p->bombes->sommet;
    }

    while(temp!=NULL) // Parcours de la File
    {
        if(temp->timer>0) // Bombe pas encore explosée
        {
            temp->timer=temp->timer-1;
            if(temp->timer%20==0) // L'actualisation de l'animation de la bombe
            {
                p->map[temp->my][temp->mx]++;
                if(p->map[temp->my][temp->mx]>15-(temp->type*3)) // (temp->type*3) est là pour gérer le type de la bombe. Il vaut 0 si c'est une bombe normale
                {
                    p->map[temp->my][temp->mx]=14-(temp->type*3);
                }
            }
        }else // Bombe explosée
        {
            if(explosion(p, temp, r)==1) // Renvoie 1 si la file est vide après explosion
            {
                return;
            }
            temp->timer-=2;
        }
        temp=temp->suivant;
    }
    verif_bonus(p, r);
    free(temp);
}

/*Se lance si une bombe en explose une autre*/
void explose_bombe(int my, int mx, t_partie *p)
{
    t_bombe *temp=malloc(sizeof(t_bombe));
    temp=p->bombes->sommet;
    while(temp!=NULL) // Recherche de la bombe à exploser
    {
        if(temp->mx==mx && temp->my==my)
        {
            temp->timer=0;
            break;
        }
        temp=temp->suivant;
    }
    temp=NULL;
    free(temp);
}

int explosion(t_partie *p, t_bombe *b, t_reglages *r)
{
    int i=1, j=1;
    /*Initialisation de l'explosion, application des flammes sur la map jusqu'aux obstacles*/
    if(b->timer==0)
    {
        Mix_PlayMusic(r->explose, 1);
        p->map[b->my][b->mx]=b->mapb[0][0];
        /*Récupération des images de la map de la bombe pour les afficher*/
        /*Copie vers le haut de la bombe*/
        while(i<(b->h/2)+1 && p->map[b->my-i][b->mx]!=1 && p->map[b->my-i][b->mx]!=2) // 1 : bloc dur, 2 : bloc cassable
        {
            if(p->map[b->my-i][b->mx]>=10 && p->map[b->my-i][b->mx]<=15) // Autre bombe
            {
                explose_bombe(b->my-i, b->mx, p);
            }
            p->map[b->my-i][b->mx]=b->mapb[0][j];
            i++;
            j++;
        }
        if(i<(b->h/2)+1) // Si on a recontré un obstacle, on actualise la case de la bombe pour stopper la suite de l'explosion
        {
            b->mapb[0][j]=-1;
            if(p->map[b->my-i][b->mx]==2) // Bloc cassable, on lance son animation de destruction
            {
                p->map[b->my-i][b->mx]=51;
            }
        }
        i=1;
        j=(b->h/2)+1;
        /*Copie vers le bas de la bombe*/
        while(i<(b->h/2)+1 && p->map[b->my+i][b->mx]!=1 && p->map[b->my+i][b->mx]!=2)
        {
            if(p->map[b->my+i][b->mx]>=10 && p->map[b->my+i][b->mx]<=15)
            {
                explose_bombe(b->my+i, b->mx, p);
            }
            p->map[b->my+i][b->mx]=b->mapb[0][j];
            i++;
            j++;
        }
        if(i<(b->h/2)+1)
        {
            b->mapb[0][j]=-1;
            if(p->map[b->my+i][b->mx]==2)
            {
                p->map[b->my+i][b->mx]=51;
            }
        }
        i=1;
        j=0;
        /*Copie vers la gauche de la bombe*/
        while(i<(b->w/2)+1 && p->map[b->my][b->mx-i]!=1 && p->map[b->my][b->mx-i]!=2)
        {
            if(p->map[b->my][b->mx-i]>=10 && p->map[b->my][b->mx-i]<=15)
            {
                explose_bombe(b->my, b->mx-i, p);
            }
            p->map[b->my][b->mx-i]=b->mapb[1][j];
            i++;
            j++;
        }
        if(i<(b->w/2)+1)
        {
            b->mapb[1][j]=-1;
            if(p->map[b->my][b->mx-i]==2)
            {
                p->map[b->my][b->mx-i]=51;
            }
        }
        i=1;
        j=(b->w/2);
        /*Copie vers la droite de la bombe*/
        while(i<(b->w/2)+1 && p->map[b->my][b->mx+i]!=1 && p->map[b->my][b->mx+i]!=2)
        {
            if(p->map[b->my][b->mx+i]>=10 && p->map[b->my][b->mx+i]<=15)
            {
                explose_bombe(b->my, b->mx+i, p);
            }
            p->map[b->my][b->mx+i]=b->mapb[1][j];
            i++;
            j++;
        }
        if(i<(b->w/2)+1)
        {
            b->mapb[1][j]=-1;
            if(p->map[b->my][b->mx+i]==2)
            {
                p->map[b->my][b->mx+i]=51;
            }
        }
    }else if(b->timer%10==0 && b->timer>-TIME_BOMBE/2) /*Premier stade de l'explosion, grossissement des flammes*/
    {
        i=1;
        j=1;
        b->mapb[0][0]++; // Tileset trié de telle sorte que les animations liées se suivent
        p->map[b->my][b->mx]=b->mapb[0][0];
        while(i<(b->h/2)+1 && b->mapb[0][j]!=-1) // -1 si un obstacle a été rencontré auparavant
        {
            b->mapb[0][j]++;
            p->map[b->my-i][b->mx]=b->mapb[0][j];
            i++;
            j++;
        }
        i=1;
        j=(b->h/2)+1;
        // Idem pour les autres directions
        while(i<(b->h/2)+1 && b->mapb[0][j]!=-1)
        {
            b->mapb[0][j]++;
            p->map[b->my+i][b->mx]=b->mapb[0][j];
            i++;
            j++;
        }
        i=1;
        j=0;
        while(i<(b->w/2)+1 && b->mapb[1][j]!=-1)
        {
            b->mapb[1][j]++;
            p->map[b->my][b->mx-i]=b->mapb[1][j];
            i++;
            j++;
        }
        i=1;
        j=(b->w/2);
        while(i<(b->w/2)+1 && b->mapb[1][j]!=-1)
        {
            b->mapb[1][j]++;
            p->map[b->my][b->mx+i]=b->mapb[1][j];
            i++;
            j++;
        }
    }else if(b->timer%10==0 && b->timer>-(TIME_BOMBE-10)) /*Deuxième stade de l'explosion, atténuation de l'explosion*/
    {
        i=1;
        j=1;
        b->mapb[0][0]--;
        p->map[b->my][b->mx]=b->mapb[0][0];
        while(i<(b->h/2)+1 && b->mapb[0][j]!=-1)
        {
            b->mapb[0][j]--;
            p->map[b->my-i][b->mx]=b->mapb[0][j];
            i++;
            j++;
        }
        if(i<(b->h/2)+1 && p->map[b->my-i][b->mx]!=1)
        {
            p->map[b->my-i][b->mx]++;
        }
        i=1;
        j=(b->h/2)+1;
        while(i<(b->h/2)+1 && b->mapb[0][j]!=-1)
        {
            b->mapb[0][j]--;
            p->map[b->my+i][b->mx]=b->mapb[0][j];
            i++;
            j++;
        }
        if(i<(b->h/2)+1 && p->map[b->my+i][b->mx]!=1)
        {
            p->map[b->my+i][b->mx]++;
        }
        i=1;
        j=0;
        while(i<(b->w/2)+1 && b->mapb[1][j]!=-1)
        {
            b->mapb[1][j]--;
            p->map[b->my][b->mx-i]=b->mapb[1][j];
            i++;
            j++;
        }
        if(i<(b->w/2)+1 && p->map[b->my][b->mx-i]!=1)
        {
            p->map[b->my][b->mx-i]++;
        }
        i=1;
        j=(b->w/2);
        while(i<(b->w/2)+1 && b->mapb[1][j]!=-1)
        {
            b->mapb[1][j]--;
            p->map[b->my][b->mx+i]=b->mapb[1][j];
            i++;
            j++;
        }
        if(i<(b->w/2)+1 && p->map[b->my][b->mx+i]!=1)
        {
            p->map[b->my][b->mx+i]++;
        }
    }else if(b->timer<-(TIME_BOMBE-10)) /*Dernier stade, disparition de l'explosion et de la bombe*/
    {
        p->map[b->my][b->mx]=3;
        i=1;
        j=1;
        while(i<(b->h/2)+1 && b->mapb[0][j]!=-1)
        {
            p->map[b->my-i][b->mx]=3;
            i++;
            j++;
        }
        if(i<(b->h/2)+1 && b->mapb[0][j]==-1 && p->map[b->my-i][b->mx]!=1) // Si c'était un bloc cassable
        {
            if(rand()%100<FREQ_BONUS) // Apparition possible d'un bonus
            {
                p->map[b->my-i][b->mx]=72+(rand()%NB_BONUS);
            }else
            {
                p->map[b->my-i][b->mx]=3;
            }
        }
        i=1;
        j=(b->h/2)+1;
        // Idem pour les autres directions
        while(i<(b->h/2)+1 && b->mapb[0][j]!=-1)
        {
            p->map[b->my+i][b->mx]=3;
            i++;
            j++;
        }
        if(i<(b->h/2)+1 && b->mapb[0][j]==-1 && p->map[b->my+i][b->mx]!=1)
        {
            if(rand()%100<FREQ_BONUS)
            {
                p->map[b->my+i][b->mx]=72+(rand()%NB_BONUS);
            }else
            {
               p->map[b->my+i][b->mx]=3;
            }
        }
        i=1;
        j=0;
        while(i<(b->w/2)+1 && b->mapb[1][j]!=-1)
        {
            p->map[b->my][b->mx-i]=3;
            i++;
            j++;
        }
        if(i<(b->w/2)+1 && b->mapb[1][j]==-1 && p->map[b->my][b->mx-i]!=1)
        {
            if(rand()%100<FREQ_BONUS)
            {
                p->map[b->my][b->mx-i]=72+(rand()%NB_BONUS);
            }else
            {
                p->map[b->my][b->mx-i]=3;
            }
        }
        i=1;
        j=(b->w/2);
        while(i<(b->w/2)+1 && b->mapb[1][j]!=-1)
        {
            p->map[b->my][b->mx+i]=3;
            i++;
            j++;
        }
        if(i<(b->w/2)+1 && b->mapb[1][j]==-1 && p->map[b->my][b->mx+i]!=1)
        {
            if(rand()%100<FREQ_BONUS)
            {
                p->map[b->my][b->mx+i]=72+(rand()%NB_BONUS);
            }else
            {
                p->map[b->my][b->mx+i]=3;
            }
        }

        /*Explosion terminée, on supprime la bombe et on met à jour la file*/
        t_bombe *temp;
        temp=p->bombes->sommet;
        p->bombes->sommet=p->bombes->sommet->suivant;
        free(temp->mapb[0]);
        free(temp->mapb[1]);
        free(temp->mapb);
        free(temp);
        return 1;
    }
    verif_mort(p);
    return 0;
}

int timer_map(t_jeu *jeu, char *horloge)
{
    char *manche=malloc(sizeof(char)*10);
    strcpy(manche, "Manche ");
    SDL_Surface *texte = NULL;
    SDL_Surface *texte_manche = NULL;
    TTF_Font *font = NULL;
    font = TTF_OpenFont("./data/ARJULIAN.ttf", 65);
    TTF_Font *font_manche = NULL;
    font_manche = TTF_OpenFont("./data/ARJULIAN.ttf", 30);

    SDL_Rect positionTexte;
    positionTexte.x=SCREEN_WIDTH/2 - 55;
    positionTexte.y=SCREEN_HEIGHT-120;

    SDL_Color couleur = {255, 0, 10};
    SDL_Color couleur_manche = {255, 255, 0};

    // Affichage du timer de la manche
    if(jeu->partie.p_timer%60==0)
    {
        if(horloge[3]=='0')
        {
            if(horloge[2]=='0')
            {
                if(horloge[0]=='0')
                {
                    return 0;
                }
                horloge[0]--;
                horloge[2]='5';
            }else
            {
                horloge[2]--;
            }
            horloge[3]='9';
        }else
        {
            horloge[3]--;
        }
    }
    texte=TTF_RenderText_Solid(font, horloge,  couleur);
    SDL_BlitSurface(texte, NULL, jeu->screen, &positionTexte);

    // Affichage du numéro de la manche
    positionTexte.y-=TILE_SIZE;
    positionTexte.x-=5;
    manche[7]='0'+jeu->partie.manche;
    manche[8]='\0';
    texte_manche = TTF_RenderText_Solid(font_manche, manche, couleur_manche);
    SDL_BlitSurface(texte_manche, NULL, jeu->screen, &positionTexte);

    TTF_CloseFont(font);
    TTF_CloseFont(font_manche);
    SDL_FreeSurface(texte);
    jeu->partie.p_timer--;
    return 1;
}

/*Gestion des 60 fps pour limiter l'utilisation du processeur*/
void delay(unsigned int fps)
{
    unsigned int instant = SDL_GetTicks();
    if (fps < instant)
    {
        return;
    }
    if (fps > 16+instant)
    {
        SDL_Delay(16);
    }
    else
    {
        SDL_Delay(fps-instant);
    }
}
