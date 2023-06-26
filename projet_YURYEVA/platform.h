
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "graphicSDL.h"

#define VMIN 1
#define VMAX 3
#define WIDTH 80
#define PROBABILITE 80


typedef struct s_plat Platform;
struct s_plat{
    int x;  // position x
    int y;  // position y
    int v;  // vitesse
    Platform* suivant;
    Platform* precedent;
};

SDL_Texture *creerTexturePlatform(SDL_Renderer*ecran, Platform *p);
void dessine_platform(SDL_Renderer *ecran, SDL_Texture *sceneFixe, Platform *p);
Platform* first_plat();
Platform* init_plat();
void insert_platform(Platform *p, Platform* newp);
void remove_platform(Platform **head, Platform** element);
void descend(Platform *p);
int proba_new_platform (int* proba);
void free_platforms(Platform** p);
void platform_final(SDL_Renderer* ecran, SDL_Texture* plat, Platform** p, int* proba);
