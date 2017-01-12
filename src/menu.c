#include "menu.h"

/*Menu principal*/
void draw_menu(t_jeu *jeu)
{
    int compute=1, choix=0, entree=0;
    SDL_Surface *jouer = NULL;
    jouer=IMG_Load("./data/images/imageMenu0.png");
    SDL_Surface *moptions = NULL;
    moptions=IMG_Load("./data/images/imageMenu1.png");
    SDL_Surface *maide = NULL;
    maide=IMG_Load("./data/images/imageMenu2.png");
    SDL_Surface *quitter = NULL;
    quitter=IMG_Load("./data/images/imageMenu3.png");

    SDL_Rect positionMenu;
    positionMenu.x=0;
    positionMenu.y=0;

    SDL_BlitSurface(jouer,NULL,jeu->screen,&positionMenu);
    SDL_Flip(jeu->screen);
    SDL_Event event;

    while(compute)
    {
        SDL_Flip(jeu->screen);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                        if(choix==0) // Pour ne pas dépasser vers le haut
                        {
                            choix=3;
                        }else
                        {
                            choix=(choix-1)%4;
                        }
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        break;
                    case SDLK_DOWN:
                        choix=(choix+1)%4;
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        break;
                    case SDLK_ESCAPE:
                        compute=0;
                        break;
                    case SDLK_RETURN:
                        Mix_PlayMusic(jeu->reglages.entree, 1);
                        compute=0;
                        entree=1;
                        break;
                    default:
                        break;
                }
                break;

            case SDL_QUIT:
                compute=0;
                break;
            default:
                break;

        }
        switch(choix) //Actualisation de l'image
        {
            case 0:
                SDL_BlitSurface(jouer, NULL, jeu->screen, &positionMenu);
                break;
            case 1:
                SDL_BlitSurface(moptions, NULL, jeu->screen, &positionMenu);
                break;
            case 2:
                SDL_BlitSurface(maide, NULL, jeu->screen, &positionMenu);
                break;
            case 3:
                SDL_BlitSurface(quitter, NULL, jeu->screen, &positionMenu);
                break;
            default:
                break;
        }
    }
    SDL_FreeSurface(jouer);
    SDL_FreeSurface(moptions);
    SDL_FreeSurface(maide);
    SDL_FreeSurface(quitter);

    if(entree)
    {
        switch(choix) //Sélection du menu approprié
        {
            case 0:
                draw_menu_jeu(jeu);
                break;
            case 1:
                draw_menu_options(jeu);
                break;
            case 2:
                draw_menu_aide(jeu);
                break;
            default:
                break;
        }
    }
}

void draw_menu_jeu(t_jeu *jeu)
{
    int compute=1, choix=0, entree=0;
    SDL_Surface *jouer0 = NULL;
    jouer0=IMG_Load("./data/images/imageJouer0.png");
    SDL_Surface *jouer1 = NULL;
    jouer1=IMG_Load("./data/images/imageJouer1.png");
    SDL_Surface *jouer2 = NULL;
    jouer2=IMG_Load("./data/images/imageJouer2.png");
    SDL_Surface *jouer3 = NULL;
    jouer3=IMG_Load("./data/images/imageJouer3.png");


    SDL_Rect positionMenu;
    positionMenu.x=0;
    positionMenu.y=0;

    SDL_BlitSurface(jouer0,NULL,jeu->screen,&positionMenu);
    SDL_Flip(jeu->screen);
    SDL_Event event;

    while(compute)
    {
        SDL_Flip(jeu->screen);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                        if(choix==0)
                        {
                            choix=3;
                        }else
                        {
                            choix=(choix-1)%4;
                        }
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        break;
                    case SDLK_DOWN:
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        choix=(choix+1)%4;
                        break;
                    case SDLK_ESCAPE:
                        break;
                    case SDLK_RETURN:
                        Mix_PlayMusic(jeu->reglages.entree, 1);
                        compute=0;
                        entree=1;
                        break;
                    default:
                        break;
                }
                break;

            case SDL_QUIT:
                compute=0;
                break;
            default:
                break;

        }
        switch(choix)
        {
            case 0:
                SDL_BlitSurface(jouer0, NULL, jeu->screen, &positionMenu);
                break;
            case 1:
                SDL_BlitSurface(jouer1, NULL, jeu->screen, &positionMenu);
                break;
            case 2:
                SDL_BlitSurface(jouer2, NULL, jeu->screen, &positionMenu);
                break;
            case 3:
                SDL_BlitSurface(jouer3, NULL, jeu->screen, &positionMenu);
                break;
            default:
                break;
        }

    }
    SDL_FreeSurface(jouer0);
    SDL_FreeSurface(jouer1);
    SDL_FreeSurface(jouer2);
    SDL_FreeSurface(jouer3);
    if(entree)
    {
        switch(choix)
        {
            case 0:
                draw_menu_jouer1(jeu);
                break;
            case 1:
                draw_menu_jouer2(jeu);
                break;
            case 2:
                draw_menu_histoire(jeu);
                break;
            case 3:
                draw_menu(jeu);
                break;
            default:
                break;
        }
    }

}

/*Jouer à un joueur contre l'ordinateur*/
void draw_menu_jouer1(t_jeu *jeu)
{
    int compute=1, choix=0, entree=0;
    SDL_Surface *jouer1vsIA0 = NULL;
    jouer1vsIA0=IMG_Load("./data/images/imageJouer1vsIA0.png");
    SDL_Surface *jouer1vsIA1 = NULL;
    jouer1vsIA1=IMG_Load("./data/images/imageJouer1vsIA1.png");
    SDL_Surface *jouer1vsIA2 = NULL;
    jouer1vsIA2=IMG_Load("./data/images/imageJouer1vsIA2.png");

    SDL_Rect positionMenu;
    positionMenu.x=0;
    positionMenu.y=0;

    SDL_BlitSurface(jouer1vsIA0,NULL,jeu->screen,&positionMenu);
    SDL_Flip(jeu->screen);
    SDL_Event event;

    while(compute)
    {
        SDL_Flip(jeu->screen);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                        if(choix==0)
                        {
                            choix=2;
                        }else
                        {
                            choix=(choix-1)%3;
                        }
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        break;
                    case SDLK_DOWN:
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        choix=(choix+1)%3;
                        break;
                    case SDLK_ESCAPE:
                        break;
                    case SDLK_RETURN:
                        Mix_PlayMusic(jeu->reglages.entree, 1);
                        compute=0;
                        entree=1;
                        break;
                    default:
                        break;
                }
                break;

            case SDL_QUIT:
                compute=0;
                break;
            default:
                break;

        }
        switch(choix)
        {
            case 0:
                SDL_BlitSurface(jouer1vsIA0, NULL, jeu->screen, &positionMenu);
                break;
            case 1:
                SDL_BlitSurface(jouer1vsIA1, NULL, jeu->screen, &positionMenu);
                break;
            case 2:
                SDL_BlitSurface(jouer1vsIA2, NULL, jeu->screen, &positionMenu);
                break;
            default:
                break;
        }

    }
    SDL_FreeSurface(jouer1vsIA0);
    SDL_FreeSurface(jouer1vsIA1);
    SDL_FreeSurface(jouer1vsIA2);

    if(entree)
    {
        switch(choix)
        {
            case 0: // Lancement de la partie
                jeu->partie.nb_joueurs = 1;
                init_jeu(jeu);
                break;
            case 1:
                draw_menu_reglages1(jeu);
                break;
            case 2:
                draw_menu_jeu(jeu);
                break;
            default:
                break;
        }
    }
}

/*Jouer à deux joueurs contre l'ordinateur*/
void draw_menu_jouer2(t_jeu *jeu)
{
    int compute=1, choix=0, entree=0;
    SDL_Surface *jouer2vsIA0 = NULL;
    jouer2vsIA0=IMG_Load("./data/images/imageJouer2vsIA0.png");
    SDL_Surface *jouer2vsIA1 = NULL;
    jouer2vsIA1=IMG_Load("./data/images/imageJouer2vsIA1.png");
    SDL_Surface *jouer2vsIA2 = NULL;
    jouer2vsIA2=IMG_Load("./data/images/imageJouer2vsIA2.png");

    SDL_Rect positionMenu;
    positionMenu.x=0;
    positionMenu.y=0;

    SDL_BlitSurface(jouer2vsIA0,NULL,jeu->screen,&positionMenu);
    SDL_Flip(jeu->screen);
    SDL_Event event;

    while(compute)
    {
        SDL_Flip(jeu->screen);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                        if(choix==0)
                        {
                            choix=2;
                        }else
                        {
                            choix=(choix-1)%3;
                        }
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        break;
                    case SDLK_DOWN:
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        choix=(choix+1)%3;
                        break;
                    case SDLK_ESCAPE:
                        break;
                    case SDLK_RETURN:
                        Mix_PlayMusic(jeu->reglages.entree, 1);
                        compute=0;
                        entree=1;
                        break;
                    default:
                        break;
                }
                break;

            case SDL_QUIT:
                compute=0;
                break;
            default:
                break;

        }
        switch(choix)
        {
            case 0:
                SDL_BlitSurface(jouer2vsIA0, NULL, jeu->screen, &positionMenu);
                break;
            case 1:
                SDL_BlitSurface(jouer2vsIA1, NULL, jeu->screen, &positionMenu);
                break;
            case 2:
                SDL_BlitSurface(jouer2vsIA2, NULL, jeu->screen, &positionMenu);
                break;
            default:
                break;
        }

    }
    SDL_FreeSurface(jouer2vsIA0);
    SDL_FreeSurface(jouer2vsIA1);
    SDL_FreeSurface(jouer2vsIA2);

    if(entree)
    {
        switch(choix)
        {
            case 0:
                jeu->partie.nb_joueurs = 2;
                init_jeu(jeu);
                break;
            case 1:
                draw_menu_reglages2(jeu);
                break;
            case 2:
                draw_menu_jeu(jeu);
                break;
            default:
                break;
        }
    }
}

void draw_menu_histoire(t_jeu *jeu)
{
    int compute=1, choix=0, entree=0;
    SDL_Surface *histoire0 = NULL;
    histoire0=IMG_Load("./data/images/imageHistoire0.png");
    SDL_Surface *histoire1 = NULL;
    histoire1=IMG_Load("./data/images/imageHistoire1.png");
    SDL_Surface *histoire2 = NULL;
    histoire2=IMG_Load("./data/images/imageHistoire2.png");

    SDL_Rect positionMenu;
    positionMenu.x=0;
    positionMenu.y=0;

    SDL_BlitSurface(histoire0,NULL,jeu->screen,&positionMenu);
    SDL_Flip(jeu->screen);
    SDL_Event event;

    while(compute)
    {
        SDL_Flip(jeu->screen);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                        if(choix==0)
                        {
                            choix=2;
                        }else
                        {
                            choix=(choix-1)%3;
                        }
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        break;
                    case SDLK_DOWN:
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        choix=(choix+1)%3;
                        break;
                    case SDLK_ESCAPE:
                        break;
                    case SDLK_RETURN:
                        Mix_PlayMusic(jeu->reglages.entree, 1);
                        compute=0;
                        entree=1;
                        break;
                    default:
                        break;
                }
                break;

            case SDL_QUIT:
                compute=0;
                break;
            default:
                break;

        }
        switch(choix)
        {
            case 0:
                SDL_BlitSurface(histoire0, NULL, jeu->screen, &positionMenu);
                break;
            case 1:
                SDL_BlitSurface(histoire1, NULL, jeu->screen, &positionMenu);
                break;
            case 2:
                SDL_BlitSurface(histoire2, NULL, jeu->screen, &positionMenu);
                break;
            default:
                break;
        }

    }
    SDL_FreeSurface(histoire0);
    SDL_FreeSurface(histoire1);
    SDL_FreeSurface(histoire2);

    if(entree)
    {
        switch(choix)
        {
            case 0:
                init_campagne(jeu);
                break;
            case 1:
                draw_menu_histoire_regles(jeu);
                break;
            case 2:
                draw_menu_jeu(jeu);
                break;
            default:
                break;
        }
    }
}

void draw_menu_histoire_regles(t_jeu *jeu)
{
    int compute=1, choix=0, entree=0;
    SDL_Surface *aide = NULL;
    aide=IMG_Load("./data/images/imageHistoireRegles0.png");

    SDL_Rect positionMenu;
    positionMenu.x=0;
    positionMenu.y=0;

    SDL_BlitSurface(aide,NULL,jeu->screen,&positionMenu);
    SDL_Flip(jeu->screen);
    SDL_Event event;

    while(compute)
    {
        SDL_Flip(jeu->screen);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                        if(choix==0)
                        {
                            choix=0;
                        }else
                        {
                            choix=(choix-1)%1;
                        }
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        break;
                    case SDLK_DOWN:
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        break;
                    case SDLK_ESCAPE:
                        break;
                    case SDLK_RETURN:
                        Mix_PlayMusic(jeu->reglages.entree, 1);
                        compute=0;
                        entree=1;
                        break;
                    default:
                        break;
                }
                break;

            case SDL_QUIT:
                compute=0;
                break;
            default:
                break;

        }
    }
    SDL_FreeSurface(aide);
    if(entree)
    {
        switch(choix)
        {
            case 0:
                draw_menu_histoire(jeu);
                break;
            default:
                break;
        }
    }
}

void draw_menu_aide(t_jeu *jeu)
{
    int compute=1, choix=0, entree=0;
    SDL_Surface *aide = NULL;
    aide=IMG_Load("./data/images/imageAide.png");

    SDL_Rect positionMenu;
    positionMenu.x=0;
    positionMenu.y=0;

    SDL_BlitSurface(aide,NULL,jeu->screen,&positionMenu);
    SDL_Flip(jeu->screen);
    SDL_Event event;

    while(compute)
    {
        SDL_Flip(jeu->screen);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                        if(choix==0)
                        {
                            choix=0;
                        }else
                        {
                            choix=(choix-1)%1;
                        }
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        break;
                    case SDLK_DOWN:
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        break;
                    case SDLK_ESCAPE:
                        break;
                    case SDLK_RETURN:
                        Mix_PlayMusic(jeu->reglages.entree, 1);
                        compute=0;
                        entree=1;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                compute=0;
                break;
            default:
                break;
        }
    }
    SDL_FreeSurface(aide);
    if(entree)
    {
        switch(choix)
        {
            case 0:
                draw_menu(jeu);
                break;
            default:
                break;
        }
    }
}

/*Reglages d'une partie à un joueur*/
void draw_menu_reglages1(t_jeu *jeu)
{
    char manche[3];
    int compute=1, choix=3, entree=0;
    SDL_Surface *reglage0 = NULL;
    reglage0=IMG_Load("./data/images/image_Reglages0.png");
    SDL_Surface *reglage1 = NULL;
    reglage1=IMG_Load("./data/images/image_Reglages1bis.png");
    SDL_Surface *reglage3 = NULL;
    reglage3=IMG_Load("./data/images/image_Reglages3.png");
    SDL_Surface *reglage4 = NULL;
    reglage4=IMG_Load("./data/images/image_Reglages4.png");

    SDL_Color couleur = {0, 0, 0};
    SDL_Surface *texte1 = NULL;
    SDL_Surface *texte2 = NULL;
    TTF_Font *font = NULL;
    font = TTF_OpenFont("./data/ARJULIAN.ttf", 50);
    texte1=TTF_RenderText_Solid(font, jeu->reglages.nom1, couleur);
    manche[0]= '0'+(jeu->reglages.nb_manches/10);
    manche[1]='0'+(jeu->reglages.nb_manches%10);
    manche[2]='\0';
    texte2=TTF_RenderText_Solid(font, manche, couleur);

    SDL_Rect position1;
    position1.x=(SCREEN_WIDTH/2)-(strlen(jeu->reglages.nom1)/2)*30; // On adapte la position en fonction de la longueur du nom
    position1.y=120;

    SDL_Rect position2;
    position2.x=(SCREEN_WIDTH/2)-30;
    position2.y=215;

    SDL_BlitSurface(reglage4,NULL,jeu->screen,NULL);
    SDL_Flip(jeu->screen);
    SDL_Event event;

    while(compute)
    {
        SDL_Flip(jeu->screen);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                        if(choix==0)
                        {
                            choix=3;
                        }else
                        {
                            choix=(choix-1)%4;
                        }
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        break;
                    case SDLK_DOWN:
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        choix=(choix+1)%4;
                        break;
                    case SDLK_ESCAPE:
                        break;
                    case SDLK_RETURN:
                        Mix_PlayMusic(jeu->reglages.entree, 1);
                        compute=0;
                        entree=1;
                        break;
                    case SDLK_LEFT:
                        if(choix==1)
                        {
                            Mix_PlayMusic(jeu->reglages.put, 1);
                            jeu->reglages.nb_manches--;
                            if(jeu->reglages.nb_manches==0)
                            {
                                jeu->reglages.nb_manches=1;
                            }
                        }
                        break;
                    case SDLK_RIGHT:
                        if(choix==1)
                        {
                            Mix_PlayMusic(jeu->reglages.put, 1);
                            jeu->reglages.nb_manches++;
                            if(jeu->reglages.nb_manches==NB_MANCHES_MAX+1)
                            {
                                jeu->reglages.nb_manches=NB_MANCHES_MAX;
                            }
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                compute=0;
                break;
            default:
                break;
        }
        switch(choix)
        {
            case 0:
                SDL_BlitSurface(reglage0, NULL, jeu->screen, NULL);
                break;
            case 1:
                SDL_BlitSurface(reglage1, NULL, jeu->screen, NULL);
                break;
            case 2:
                SDL_BlitSurface(reglage3, NULL, jeu->screen, NULL);
                break;
            case 3:
                SDL_BlitSurface(reglage4, NULL, jeu->screen, NULL);
                break;
            default:
                break;
        }
        // Mise à jour de l'affichage des reglages
        SDL_BlitSurface(texte1, NULL, jeu->screen, &position1);
        manche[0]= '0'+(jeu->reglages.nb_manches/10);
        manche[1]='0'+(jeu->reglages.nb_manches%10);
        manche[2]='\0';
        texte2=TTF_RenderText_Solid(font, manche, couleur);
        SDL_BlitSurface(texte2, NULL, jeu->screen, &position2);
    }

    SDL_FreeSurface(reglage0);
    SDL_FreeSurface(reglage1);
    SDL_FreeSurface(reglage3);
    SDL_FreeSurface(reglage4);
    SDL_FreeSurface(texte1);
    SDL_FreeSurface(texte2);
    TTF_CloseFont(font);

    if(entree)
    {
        switch(choix)
        {
            case 0:
                lire_nom(jeu, 1);
                draw_menu_reglages1(jeu);
                break;
            case 1:
                draw_menu_reglages1(jeu);
                break;
            case 2:
                jeu->reglages.nb_manches=NB_MANCHES_DEFAUT;
                strcpy(jeu->reglages.nom1, NOM1_DEFAUT);
                draw_menu_reglages1(jeu);
                break;
            case 3:
                draw_menu_jouer1(jeu);
                break;
            default:
                break;
        }
    }
}

/*Reglages d'une partie à deux joueurs*/
void draw_menu_reglages2(t_jeu *jeu)
{
    int compute=1, choix=4, entree=0;
    char manche[3];
    SDL_Surface *reglage0 = NULL;
    reglage0=IMG_Load("./data/images/image_Reglages0.png");
    SDL_Surface *reglage1 = NULL;
    reglage1=IMG_Load("./data/images/image_Reglages1.png");
    SDL_Surface *reglage2 = NULL;
    reglage2=IMG_Load("./data/images/image_Reglages2.png");
    SDL_Surface *reglage3 = NULL;
    reglage3=IMG_Load("./data/images/image_Reglages3.png");
    SDL_Surface *reglage4 = NULL;
    reglage4=IMG_Load("./data/images/image_Reglages4.png");


    SDL_Color couleur = {0, 0, 0};
    SDL_Surface *texte1 = NULL;
    SDL_Surface *texte2 = NULL;
    SDL_Surface *texte3 = NULL;
    TTF_Font *font = NULL;
    font = TTF_OpenFont("./data/ARJULIAN.ttf", 50);
    texte1=TTF_RenderText_Solid(font, jeu->reglages.nom1, couleur);
    texte2=TTF_RenderText_Solid(font, jeu->reglages.nom2, couleur);
    manche[0]= '0'+(jeu->reglages.nb_manches/10);
    manche[1]='0'+(jeu->reglages.nb_manches%10);
    manche[2]='\0';
    texte3=TTF_RenderText_Solid(font, manche, couleur);

    SDL_Rect position1;
    position1.x=(SCREEN_WIDTH/2)-(strlen(jeu->reglages.nom1)/2)*30;
    position1.y=120;

    SDL_Rect position2;
    position2.x=(SCREEN_WIDTH/2)-(strlen(jeu->reglages.nom2)/2)*30;
    position2.y=210;

    SDL_Rect position3;
    position3.x=(SCREEN_WIDTH/2)-30;
    position3.y=305;

    SDL_BlitSurface(reglage4,NULL,jeu->screen,NULL);
    SDL_Flip(jeu->screen);
    SDL_Event event;

    while(compute)
    {
        SDL_Flip(jeu->screen);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        if(choix==0)
                        {
                            choix=4;
                        }else
                        {
                            choix=(choix-1)%5;
                        }
                        break;
                    case SDLK_DOWN:
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        choix=(choix+1)%5;
                        break;
                    case SDLK_ESCAPE:
                        break;
                    case SDLK_RETURN:
                        Mix_PlayMusic(jeu->reglages.entree, 1);
                        compute=0;
                        entree=1;
                        break;
                    case SDLK_LEFT:
                        if(choix==2)
                        {
                            jeu->reglages.nb_manches--;
                            if(jeu->reglages.nb_manches==0)
                            {
                                jeu->reglages.nb_manches=1;
                            }
                        }
                        break;
                    case SDLK_RIGHT:
                        if(choix==2)
                        {
                            jeu->reglages.nb_manches++;
                            if(jeu->reglages.nb_manches==NB_MANCHES_MAX+1)
                            {
                                jeu->reglages.nb_manches=NB_MANCHES_MAX;
                            }
                        }
                        break;
                    default:
                        break;
                }
                break;

            case SDL_QUIT:
                compute=0;
                break;
            default:
                break;

        }
        switch(choix)
        {
            case 0:
                SDL_BlitSurface(reglage0, NULL, jeu->screen, NULL);
                break;
            case 1:
                SDL_BlitSurface(reglage1, NULL, jeu->screen, NULL);
                break;
            case 2:
                SDL_BlitSurface(reglage2, NULL, jeu->screen, NULL);
                break;
            case 3:
                SDL_BlitSurface(reglage3, NULL, jeu->screen, NULL);
                break;
            case 4:
                SDL_BlitSurface(reglage4, NULL, jeu->screen, NULL);
                break;
            default:
                break;
        }
        SDL_BlitSurface(texte1, NULL, jeu->screen, &position1);
        SDL_BlitSurface(texte2, NULL, jeu->screen, &position2);
        manche[0]= '0'+(jeu->reglages.nb_manches/10);
        manche[1]='0'+(jeu->reglages.nb_manches%10);
        manche[2]='\0';
        texte3=TTF_RenderText_Solid(font, manche, couleur);
        SDL_BlitSurface(texte3, NULL, jeu->screen, &position3);
    }

    SDL_FreeSurface(reglage0);
    SDL_FreeSurface(reglage1);
    SDL_FreeSurface(reglage3);
    SDL_FreeSurface(reglage4);
    SDL_FreeSurface(texte1);
    SDL_FreeSurface(texte2);
    SDL_FreeSurface(texte3);
    TTF_CloseFont(font);


    if(entree)
    {
        switch(choix)
        {
            case 0:
                lire_nom(jeu, 1);
                draw_menu_reglages2(jeu);
                break;
            case 1:
                lire_nom(jeu, 2);
                draw_menu_reglages2(jeu);
                break;
            case 2:
                draw_menu_reglages2(jeu);
                break;
            case 3:
                jeu->reglages.nb_manches=NB_MANCHES_DEFAUT;
                strcpy(jeu->reglages.nom1, NOM1_DEFAUT);
                strcpy(jeu->reglages.nom2, NOM2_DEFAUT);
                draw_menu_reglages2(jeu);
                break;
            case 4:
                draw_menu_jouer2(jeu);
                break;
            default:
                break;
        }
    }
}

void draw_menu_options(t_jeu *jeu)
{
    char volume[3];
    int compute=1, choix=0, entree=0;
    SDL_Surface *options0 = NULL;
    options0=IMG_Load("./data/images/imageOptions0.png");
    SDL_Surface *options1 = NULL;
    options1=IMG_Load("./data/images/imageOptions1.png");

    // Affichage du volume actuel
    SDL_Color couleur = {0, 0, 0};
    SDL_Surface *texte = NULL;
    TTF_Font *font = NULL;
    font = TTF_OpenFont("./data/ARJULIAN.ttf", 50);
    volume[0]= '0'+(jeu->reglages.volume/10);
    volume[1]='0'+(jeu->reglages.volume%10);
    volume[2]='\0';
    texte=TTF_RenderText_Solid(font, volume, couleur);

    SDL_Rect positiontexte;
    positiontexte.x=(SCREEN_WIDTH/2)-30;
    positiontexte.y=215;

    SDL_Rect positionMenu;
    positionMenu.x=0;
    positionMenu.y=0;

    SDL_BlitSurface(options0, NULL, jeu->screen, &positionMenu);
    SDL_Flip(jeu->screen);
    SDL_Event event;

    while(compute)
    {
        SDL_Flip(jeu->screen);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                        if(choix==0)
                        {
                            choix=1;
                        }else
                        {
                            choix=(choix-1)%2;
                        }
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        break;
                    case SDLK_DOWN:
                        Mix_PlayMusic(jeu->reglages.put, 1);
                        choix=(choix+1)%2;
                        break;
                    case SDLK_LEFT:
                        if(choix==0 && jeu->reglages.volume!=0)
                        {
                            Mix_PlayMusic(jeu->reglages.put, 1);
                            jeu->reglages.volume--;
                            volume[0]= '0'+(jeu->reglages.volume/10);
                            volume[1]='0'+(jeu->reglages.volume%10);
                            texte=TTF_RenderText_Solid(font, volume, couleur);
                            Mix_VolumeMusic(jeu->reglages.volume);
                        }
                        break;
                    case SDLK_RIGHT:
                        if(choix==0 && jeu->reglages.volume<VOLUME_MAX)
                        {
                            Mix_PlayMusic(jeu->reglages.put, 1);
                            jeu->reglages.volume++;
                            volume[0]= '0'+(jeu->reglages.volume/10);
                            volume[1]='0'+(jeu->reglages.volume%10);
                            texte=TTF_RenderText_Solid(font, volume, couleur);
                            Mix_VolumeMusic(jeu->reglages.volume);
                        }
                        break;
                    case SDLK_ESCAPE:
                        break;
                    case SDLK_RETURN:
                        Mix_PlayMusic(jeu->reglages.entree, 1);
                        compute=0;
                        entree=1;
                        break;
                    default:
                        break;
                }
                break;

            case SDL_QUIT:
                compute=0;
                break;
            default:
                break;

        }
        switch(choix)
        {
            case 0:
                SDL_BlitSurface(options0, NULL, jeu->screen, &positionMenu);
                break;
            case 1:
                SDL_BlitSurface(options1, NULL, jeu->screen, &positionMenu);
                break;
            default:
                break;
        }
        SDL_BlitSurface(texte, NULL, jeu->screen, &positiontexte);
    }
    SDL_FreeSurface(options0);
    SDL_FreeSurface(options1);
    SDL_FreeSurface(texte);
    TTF_CloseFont(font);

    if(entree)
    {
        switch(choix)
        {
            case 0:
                draw_menu_options(jeu);
                break;
            case 1:
                draw_menu(jeu);
                break;
            default:
                break;
        }
    }
}

/*Lit la saisie au clavier d'un nom*/
void lire_nom(t_jeu *jeu, int i_joueur)
{
    int i, compute=1;
    SDL_Surface *texte = NULL;
    TTF_Font *font = NULL;
    font = TTF_OpenFont("./data/ARJULIAN.ttf", 65);
    SDL_Surface *image_fond = IMG_Load("./data/images/gagnantmanche.png");

    char nom[NB_CARAC_MAX];
    if(i_joueur==1)
    {
        strcpy(nom, jeu->reglages.nom1);
    }else
    {
        strcpy(nom, jeu->reglages.nom2);
    }
    i=strlen(nom);

    char temp[2]="";

    SDL_Rect positionTexte;
    positionTexte.x=150;
    positionTexte.y=(SCREEN_HEIGHT/2)-40;

    SDL_FillRect(jeu->screen, NULL, SDL_MapRGB(jeu->screen->format, 255, 255, 255));
    SDL_Color couleur = {0, 0, 0};

    texte=TTF_RenderText_Solid(font, nom,  couleur);
    SDL_Event event;
    while(compute)
    {
        SDL_WaitEvent(&event);
        if(event.type==SDL_KEYDOWN)
        {
            Mix_PlayMusic(jeu->reglages.entree, 1);
            if(event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z)
            {
                if(i<NB_CARAC_MAX-1) // Bloque la saisie au nombre maximal de caractères
                {
                    i++;
                    SDL_FillRect(jeu->screen, NULL, SDL_MapRGB(jeu->screen->format, 255, 255, 255));
                    temp[0]=(char)event.key.keysym.unicode; // Conversion de la touche en caractère
                    temp[1]='\0';
                    strcat(nom, temp);
                }
            }
            if(event.key.keysym.sym == SDLK_BACKSPACE) // On efface
            {
                SDL_FillRect(jeu->screen, NULL, SDL_MapRGB(jeu->screen->format, 255, 255, 255));
                if(i!=0) // Pour ne pas sortir du tableau
                {
                    i--;
                    nom[i]='\0';
                }
            }
            if(event.key.keysym.sym == SDLK_RETURN)
            {
                compute=0;
            }
        }

        texte=TTF_RenderText_Solid(font, nom, couleur);
        SDL_BlitSurface(image_fond, NULL, jeu->screen, NULL);
        SDL_BlitSurface(texte, NULL, jeu->screen, &positionTexte);
        SDL_Flip(jeu->screen);
    }
    if(i_joueur==1 && i>1) // Deux caractères minimum
    {
        strcpy(jeu->reglages.nom1, nom);
    }else if(i>1)
    {
        strcpy(jeu->reglages.nom2, nom);
    }

    TTF_CloseFont(font);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(image_fond);
}
