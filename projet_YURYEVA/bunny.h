
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "graphicSDL.h"

#define SIZEB 50
#define SIZE_JUMP 80
#define JUMP_D 11.5   // vitesse du saut initiale

typedef struct {
    int x;
    int y;
    int dx;
    int dyu;
    int dyd;
}Bunny;


Bunny* init_bunny();
SDL_Texture* creerTextureBunny(SDL_Renderer* ecran, Bunny* b);
void dessine_bunny(SDL_Renderer* ecran, SDL_Texture* buntex, Bunny* b);
void move_bunny(SDL_Event e, Bunny* bunny, int* quit);
void bunny_jump(SDL_Renderer* ecran, SDL_Texture* buntex, Bunny* bunny, int* going_where, int* jump);
int bunny_collision(Bunny* b, Platform* p, int* score);
void bunny_final(SDL_Renderer* ecran, SDL_Texture* buntex, Bunny* b, Platform* p, int* quit, int* going_where, int* jump, int* score);
