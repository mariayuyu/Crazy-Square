#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "graphicSDL.h"
#include "platform.h"
#include "bunny.h"

/*Scene Fixe */
// cree la texture de la scene fixe
SDL_Texture *creerTextureSceneFixe(SDL_Renderer*ecran);
// dessine la texture de la scene fixe
void dessine_sceneFixe(SDL_Renderer *ecran, SDL_Texture *sceneFixe);


int main(int argc, char** argv)
{
    /// MAIN MENU ///
    printf("\t\t CRAZY SQURE ! \n\n\n");
    printf("Voulez-vous jouer ?\n\n");
    printf("\tOui !       :       n'importe quelle touche\n");
    printf("\tNon >:(     :       ESPACE\n");
    printf("\tpuis ENTER\n");
    int c = getchar();
    printf("\n");
    if (c == ' ') return 0;

    /// GAME ///
    else {

        int score = 0;
        int success = FALSE;

        SDL_Window* fenetre = NULL;
        SDL_Renderer* ecran = NULL;
        // les différentes textures des objets
        SDL_Texture* sceneFixeTexture = NULL;
        SDL_Texture* platformTexture = NULL;
        SDL_Texture* bunnyTexture = NULL;
        int proba = PROBABILITE;
        success = init(&fenetre, &ecran, "Bunny!");



        if (!success)
        {
            fprintf(stderr, "Erreur de création de la fenêtre: %s\n", SDL_GetError());
        }
        else
        {
            srand(time(NULL));

            /* INIT */

            SDL_Event e;

            Bunny* bunny = init_bunny();
            Platform* p = first_plat();

            int quit = FALSE;   //test de sortie du jeu (de la boucle principale)
            int jumping = 0;    // compteur quelle hauteur le lapin a sauté
            int going_where = 1;    // indication si le lapin est en train de sauter ou tomber, 1 : sauter, 0 : tomber
            int nb_frames = 0;  // compteur de frames (d'affichage)

            srand(time(NULL));  // init tirage aleatoire

            // Initialisation des différents objets
            sceneFixeTexture = creerTextureSceneFixe(ecran);

            platformTexture = creerTexturePlatform(ecran, p);   // init plateau

            bunnyTexture = creerTextureBunny(ecran, bunny); //init lapin



            dessine_sceneFixe(ecran, sceneFixeTexture);
            SDL_Delay(3000);

                            /* BOUCLE DU JEU / GAME LOOP */

            while (!(quit))
            {
                /* ENTREES/INPUT */
                //lit les evenements (appuie sur ue touche...)
                while (SDL_PollEvent(&e) != 0)
                {
                    move_bunny(e, bunny, &quit);
                }


                /* RENDU/RENDER */

                //nettoie écran
                SDL_SetRenderDrawColor(ecran, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(ecran);

                // dessine la scène/décor de fond
                dessine_sceneFixe(ecran, sceneFixeTexture);

                // dessine plateau
                platform_final(ecran, platformTexture, &p, &proba);

                // dessine lapin
                bunny_final(ecran, bunnyTexture, bunny, p, &quit, &going_where, &jumping, &score);

                //Mise à jour de l'écran
                SDL_RenderPresent(ecran);

                nb_frames = nb_frames + 1;
            }

            // frees
            free_platforms(&p);
            free(bunny);
            bunny = NULL;

            // destruction des textures
            if (sceneFixeTexture) { SDL_DestroyTexture(sceneFixeTexture); sceneFixeTexture = NULL; }
            if (platformTexture) { SDL_DestroyTexture(platformTexture); platformTexture = NULL; }
            if (bunnyTexture) { SDL_DestroyTexture(bunnyTexture); bunnyTexture = NULL; }

        }

        // ferme ecran et fenetre
        close(&fenetre, &ecran);
        // Score
        printf("Score : %d\nGood job !", score);
        SDL_Delay(5000);

        return 0;
    }
}





/* Scene Fixe */
SDL_Texture *creerTextureSceneFixe(SDL_Renderer*ecran)
{
    SDL_Texture* sceneFixe=NULL;

    sceneFixe=SDL_CreateTexture(ecran,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH,SCREEN_HEIGHT);
    SDL_SetRenderTarget(ecran, sceneFixe); //on modifie la texture

    // dessine ciel bleu
    SDL_Rect ciel_p1= {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT/5};
    dessine_rect_plein(ecran,&ciel_p1,COLOR1);
    SDL_Rect ciel_p2 = { 0, SCREEN_HEIGHT / 5, SCREEN_WIDTH,SCREEN_HEIGHT / 5 };
    dessine_rect_plein(ecran, &ciel_p2, COLOR2);
    SDL_Rect ciel_p3 = { 0, 2 * SCREEN_HEIGHT / 5, SCREEN_WIDTH,SCREEN_HEIGHT / 5 };
    dessine_rect_plein(ecran, &ciel_p3, COLOR3);
    SDL_Rect ciel_p4 = { 0, 3 * SCREEN_HEIGHT / 5, SCREEN_WIDTH,SCREEN_HEIGHT / 5 };
    dessine_rect_plein(ecran, &ciel_p4, COLOR4);
    SDL_Rect ciel_p5 = { 0, 4 * SCREEN_HEIGHT / 5, SCREEN_WIDTH,SCREEN_HEIGHT / 5 };
    dessine_rect_plein(ecran, &ciel_p5, COLOR5);

    dessine_ellipse_plein(ecran, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 30, SCREEN_WIDTH / 2 + 50, 60, COLOR6);

    SDL_SetRenderTarget(ecran, NULL);

    return sceneFixe;// Dorénavant, on modifie à nouveau le renderer
}


void dessine_sceneFixe(SDL_Renderer *ecran, SDL_Texture *sceneFixe)
{
    SDL_Rect position;
    position.x = 0;
    position.y = 0;
    SDL_QueryTexture(sceneFixe, NULL, NULL, &position.w, &position.h);
    SDL_RenderCopy(ecran, sceneFixe, NULL, &position);
}

