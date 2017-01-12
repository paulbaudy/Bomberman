#include "map.h"

void load_map_1(t_partie *partie)
{
    int x=0, y=1;
    /*Initialisation des contours*/
    while(x<MAP_SIZE_W)
    {
        partie->map[0][x]=1;
        partie->map[MAP_SIZE_H-1][x]=1;
        x++;
    }
    while(y<MAP_SIZE_H-1)
    {
        partie->map[y][0]=1;
        partie->map[y][MAP_SIZE_W-1]=1;
        y++;
    }
    x=1;
    y=1;
    /*Quadrillage des blocs durs*/
    while(y<MAP_SIZE_H-1)
    {
        x=1;
        while(x<MAP_SIZE_W-1)
        {
            if(x%2==0 && y%2==0)
            {
                partie->map[y][x]=1;
            }else
            {
                /*Génération aléatoire des blocs cassables sans obstruer l'emplacement initial des joueurs*/
                if(rand()%100<FREQ_BRICK && (y>2 || x>2) && (y<MAP_SIZE_H-3 || x<MAP_SIZE_W-3) && (y<MAP_SIZE_H-3 || x>2) && (y>2 || x<MAP_SIZE_W-3))
                {
                    partie->map[y][x]=2;
                }else
                {
                    partie->map[y][x]=3;
                }
            }
            x++;
        }
        y++;
    }
}

/*On positionne la tuile sélectionnée*/
void tile(SDL_Surface* image, int dx, int dy, int sx, int sy, t_jeu *jeu)
{
    SDL_Rect d;
    d.x=dx;
    d.y=dy;
    d.h=TILE_SIZE;
    d.w=TILE_SIZE;
    SDL_Rect s;
    s.x=sx;
    s.y=sy;
    s.h=TILE_SIZE;
    s.w=TILE_SIZE;
    SDL_BlitSurface(image, &s, jeu->screen, &d);
}

/*On positionne l'ensemble des cases de la map*/
void map(t_jeu *jeu)
{
    int x;
    int y;
    int a;
    int ys;
    int xs;
    for(x=0; x<MAP_SIZE_W; x++)
    {
        for(y=0;y<MAP_SIZE_H;y++)
        {
            a=jeu->partie.map[y][x];
            ys=(a/NB_TILES)*TILE_SIZE;
            xs=(a%NB_TILES)*TILE_SIZE;
            if(a>=0 && a<64) //Fichier tileset
            {
                tile(jeu->partie.map_image, x*TILE_SIZE, y*TILE_SIZE, xs, ys, jeu);
            }
            else if (a>=64 && a<75) //Fichier bonuset
            {
                a=a-64;
                ys=(a/NB_TILES)*TILE_SIZE;
                xs=(a%NB_TILES)*TILE_SIZE;
                tile(jeu->partie.map_bonus, x*TILE_SIZE, y*TILE_SIZE, xs, ys, jeu);
            }
        }
    }
}
