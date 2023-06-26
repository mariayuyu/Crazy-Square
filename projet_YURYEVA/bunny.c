#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "graphicSDL.h"
#include "platform.h"
#include "bunny.h"



SDL_Texture* creerTextureBunny(SDL_Renderer* ecran, Bunny* b)
{
    SDL_Texture* bun = NULL;

    bun = SDL_CreateTexture(ecran, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SIZEB, SIZEB);
    SDL_RenderClear(ecran);
    SDL_SetRenderTarget(ecran, bun); //on modifie la texture

    // dessine bunny
    SDL_Rect bunny = { b->x,b->y,SIZEB,SIZEB };
    dessine_rect_plein(ecran, &bunny, White);

    SDL_SetRenderTarget(ecran, NULL);

    return bun;// Dorénavant, on modifie à nouveau le renderer
}


void dessine_bunny(SDL_Renderer* ecran, SDL_Texture* buntex, Bunny* b)
{
    SDL_Rect position;
    position.x = b->x;
    position.y = b->y;
    position.w = SIZEB;
    position.h = SIZEB;
    SDL_QueryTexture(buntex, NULL, NULL, &position.w, &position.h);
    SDL_RenderCopy(ecran, buntex, NULL, &position);
}


////////////////////////////////////////////////////////


Bunny* init_bunny()
{
    // le lapin initialisé au milieu de l'ecran
    Bunny* b = malloc(sizeof(Bunny));
    if (b == NULL) {
        printf("Erreur : allocation de la memoire du lapin\n");
        return NULL;
    }
    b->x = SCREEN_WIDTH / 2 - SIZEB / 2;
    b->y = SCREEN_HEIGHT / 2 - SIZEB;
    b->dx = 10;
    b->dyu = 4;
    b->dyd = 3;
    return b;
}



void move_bunny(SDL_Event e, Bunny *bunny, int* quit)
{
    //quit en fermant la fenetre
    if (e.type == SDL_QUIT)
    {
        *quit = TRUE;
    }
    //si une touche est presée
    if (e.type == SDL_KEYDOWN)
    {

        if (e.key.repeat == 0) // et relachée
        {
            //quel touche ?
            switch (e.key.keysym.sym)
            {

                // bouge bunny
            case SDLK_RIGHT:   //droite
                bunny->x += bunny->dx;
                break;
            case SDLK_LEFT://gauche*
                bunny->x -= bunny->dx;
                break;
                // quitter le jeu en appuyant sur la touche espace
            case SDLK_SPACE:
                *quit = TRUE;
                break;
            }
        }
        else
            // si une touche est pressée sans relache
        {
            //quel touche ?
            switch (e.key.keysym.sym)
            {
                // bouge bunny
            case SDLK_RIGHT://droite
                //pour ne pas depasser l'ecran
                if ((bunny->x + SIZEB) < SCREEN_WIDTH) {
                    bunny->x += bunny->dx;
                }
                break;
            case SDLK_LEFT://gauche
                //pour ne pas depasser l'ecran
                if (bunny->x > 0) {
                    bunny->x -= bunny->dx;
                }
                break;
            }
        }
    }
}
/*

void bunny_jump(SDL_Renderer* ecran, SDL_Texture* buntex, Bunny* bunny, int* going_where, int* jump)
{
    if (*going_where == 0) {
        bunny->y += bunny->dyd;
        (*jump) = 0;
        dessine_bunny(ecran, buntex, bunny);
    }
    else {
        if (*jump <= SIZE_JUMP) {
            bunny->y -= bunny->dyu;
            dessine_bunny(ecran, buntex, bunny);
            (*jump)++;
        }
        else
            *going_where = 0;
    }
}
*/


///*
void bunny_jump(SDL_Renderer* ecran, SDL_Texture* buntex, Bunny* bunny, int* going_where, int* jump)
{
    static float jump_speed = JUMP_D;
    float gravity = 0.3;

    if (*going_where == 0) { // en train de tomber
        bunny->y += bunny->dyd;
        jump_speed = JUMP_D;
    }
    else { // en train de sauter
        if (*jump > SIZE_JUMP) { // atteint la limite du saut
            *going_where = 0;
            *jump = 0;
        }
        float speed_factor = ((float)SIZE_JUMP - (float)(*jump)) / (float)SIZE_JUMP;
        speed_factor = fmaxf(speed_factor, 0.1);
        jump_speed -= gravity * speed_factor;
        bunny->y -= jump_speed;
        (*jump)++;

    }
    dessine_bunny(ecran, buntex, bunny);
}
//*/
int bunny_collision(Bunny* b, Platform* p, int* score)
{
    Platform* tmp = p;
    while (tmp != NULL) {
        // si le lapin touche une platforme retourne 1
        if ((b->x >= tmp->x - SIZEB + 1 && b->x <= tmp->x + WIDTH) && (b->y >= tmp->y - SIZEB - 1  &&  b->y <= tmp->y - SIZEB + 1) ) {
            (*score)++;
            return 1;
        }
        else {
            tmp = tmp->suivant;
        }
    }
    // sinon 0
    return 0;
}

void bunny_final(SDL_Renderer* ecran, SDL_Texture* buntex, Bunny* b, Platform* p, int* quit, int* going_where, int* jump, int* score)
{
    // le lapin touche le bas de l'ecran
    if (b->y > SCREEN_HEIGHT + SIZEB)
        *quit = TRUE;
    // le lapin est en train de sauter ou toucher une platforme
    if (bunny_collision(b, p, score) == 1 || *going_where == 1) {
        *going_where = 1;
        bunny_jump(ecran, buntex, b, going_where, jump);
    }
    //sinon il est en train de tomber
    else {
       *going_where = 0;
        bunny_jump(ecran, buntex, b, going_where, jump);
    }
}


