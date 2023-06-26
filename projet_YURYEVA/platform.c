#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "graphicSDL.h"
#include "platform.h"




SDL_Texture *creerTexturePlatform(SDL_Renderer*ecran, Platform *p)
{
    SDL_Texture* plat=NULL;

    plat=SDL_CreateTexture(ecran,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,WIDTH ,20);
    SDL_SetRenderTarget(ecran, plat); //on modifie la texture
    SDL_RenderClear(ecran);

    // dessine plateau
    SDL_Rect platform={p->x,p->y,WIDTH,20};
    dessine_rect_plein(ecran,&platform,Black);

    SDL_SetRenderTarget(ecran, NULL);

    return plat;// Dorénavant, on modifie à nouveau le renderer
}


void dessine_platform(SDL_Renderer *ecran, SDL_Texture *plat, Platform *p)
{
    SDL_Rect position;
    position.x = p->x;
    position.y = p->y;
    SDL_QueryTexture(plat, NULL, NULL, &position.w, &position.h);
    SDL_RenderCopy(ecran, plat, NULL, &position);
}



/// //////////////////////////////////////////////////////


Platform* first_plat()
{
    // premier plateau sous le lapin
    Platform* newp = malloc(sizeof(Platform));
    if (newp == NULL) {
        printf("Erreur : allocation de la memoire du plateau.\n");
        return NULL;
    }
    newp->x = SCREEN_WIDTH/2 - WIDTH/2;
    newp->y = SCREEN_HEIGHT/2;
    newp->v = 1;
    newp->suivant = NULL;
    newp->precedent = NULL;
    return newp;
}


Platform* init_plat()
{
    int quel_x = 1 + rand()%(5);
    int v = VMIN + rand()%(VMAX - VMIN);
    Platform* newp = malloc(sizeof(Platform));
    if (newp == NULL) {
        printf("Erreur : allocation de la memoire du plateau.\n");
        return NULL;
    }
    // 5 differents positions en axe x possible
    if (quel_x == 1) newp->x = 0;
    else if (quel_x == 2) newp->x = 80;
    else if (quel_x == 3) newp->x = 160;
    else if (quel_x == 4) newp->x = 240;
    else { newp->x = 320; }
    newp->y = 0;
    newp->v = v;
    newp->suivant = NULL;
    newp->precedent = NULL;
    return newp;
}


void insert_platform(Platform *p, Platform* newp)
{
    // inserer un element
    if (p == NULL) { // si on insere un element dans une liste vide
        p = newp;
        return;
    }
    while (p->suivant != NULL)
        p = p->suivant;
    p->suivant = newp;
    newp->precedent = p;
}


void remove_platform(Platform** head, Platform** element) {
    // Si l'élément à supprimer est la tête de liste
    if ((*element) == (*head)) {
        *head = (*element)->suivant;
        if (*head != NULL) {
            (*head)->precedent = NULL;
        }
        free(*element);
        *element = NULL;
        return;
    }

    // Si l'élément à supprimer est le dernier élément de la liste
    if ((*element)->suivant == NULL) {
        (*element)->precedent->suivant = NULL;
        free(*element);
        *element = NULL;
        return;
    }
    // Si l'élément à supprimer est au milieu de la liste
    (*element)->precedent->suivant = (*element)->suivant;
    (*element)->suivant->precedent = (*element)->precedent;
    free(*element);
    *element = NULL;
}


void descend(Platform *p)
{
    if (p != NULL){
        p->y += p->v;
    }
}


int proba_new_platform(int* proba)
{
    // retourne 1 si il y aura un nouveau plateau, 0 sinon
    int a = rand()%(5000);
    if (a<(*proba))
        return 1;
    return 0;
}


void free_platforms(Platform** p) {
    Platform* currentPlatform = *p;
    while (currentPlatform != NULL) {
        Platform* nextPlatform = currentPlatform->suivant;
        free(currentPlatform);
        currentPlatform = nextPlatform;
    }
    *p = NULL;
}


void platform_final(SDL_Renderer *ecran, SDL_Texture *plat, Platform **p, int* proba)
{
    Platform* tmp = *p;
    // Si il n'y a pas des plateaus on ajoute un
    if (*p == NULL) {
        Platform* newp = init_plat();
        *p = newp;
    }
    // Si proba_new_platform retourne 1 on ajoute un plateau
    if (proba_new_platform(proba)){
        Platform* newp = init_plat();
        insert_platform(*p, newp);
        (*proba)--; // probabilité diminue
    }
    while (tmp != NULL){
        // si un plateau touche le bas de l'ecran cet plateau est supprimé
        if (tmp->y >= SCREEN_HEIGHT){
            Platform* tmp2 = tmp;
            remove_platform(p,&tmp2);
            if (tmp2 == NULL) {
                tmp = *p;
            }
        }
        // plateau descend
        else{
            descend(tmp);
            dessine_platform(ecran, plat, tmp);
            tmp = tmp->suivant;
        }
    }

}
