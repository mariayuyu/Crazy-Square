#include "graphicSDL.h"

int init(SDL_Window** gWindow,SDL_Renderer** gRenderer, char* nameWindow)
{
	//Initialization flag
	int success = TRUE;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = FALSE;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		*gWindow = SDL_CreateWindow(nameWindow, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( *gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = FALSE;
		}
		else
		{
			//Create vsynced renderer for window
			*gRenderer = SDL_CreateRenderer( *gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( *gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = FALSE;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( *gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			}
		}
	}

	return success;
}


void close(SDL_Window** gWindow,SDL_Renderer** gRenderer)
{


	//Destroy window
	SDL_DestroyRenderer( *gRenderer );
	SDL_DestroyWindow(* gWindow );
	*gWindow = NULL;
	*gRenderer = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}


// fcts de dessin

void dessine_point(SDL_Renderer *gRenderer,int x1, int y1,long c)
{
   int r = ( uint8_t )(c >> 16);
   int g = ( uint8_t )(c >>  8);
   int b = (  uint8_t  )c;
   SDL_SetRenderDrawColor( gRenderer,r,g,b,255); // selection de la couleur du pinceau
   SDL_RenderDrawPoint(gRenderer,x1,y1); // dessin du point à la couleur précédemment déterminée
}


void dessine_ligne(SDL_Renderer *gRenderer,int x1, int y1, int x2, int y2,long c)
{
   int r = ( uint8_t )(c >> 16);
   int g = ( uint8_t )(c >>  8);
   int b = (  uint8_t  )c;
   SDL_SetRenderDrawColor( gRenderer,r,g,b,255);
   SDL_RenderDrawLine(gRenderer,x1,y1,x2,y2);
}


void dessine_rect_vide(SDL_Renderer *gRenderer,SDL_Rect *rect,long c)
{
   int r = ( uint8_t )(c >> 16);
   int g = ( uint8_t )(c >>  8);
   int b = (  uint8_t  )c;
   SDL_SetRenderDrawColor( gRenderer,r,g,b,255);
   SDL_RenderDrawRect( gRenderer,rect);
}

void dessine_rect_plein(SDL_Renderer *gRenderer,SDL_Rect *rect,long c)
{
   int r = ( uint8_t )(c >> 16);
   int g = ( uint8_t )(c >>  8);
   int b = (  uint8_t  )c;
   SDL_SetRenderDrawColor( gRenderer,r,g,b,255);
   SDL_RenderFillRect( gRenderer,rect);
}


void dessine_cercle(SDL_Renderer *gRenderer,int centreX,int centreY,int radius, long c)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   int r = ( uint8_t )(c >> 16);
   int g = ( uint8_t )(c >>  8);
   int b = (  uint8_t  )c;
   SDL_SetRenderDrawColor( gRenderer,r,g,b,255);

   while (x >= y)
   {
      // Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(gRenderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(gRenderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(gRenderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(gRenderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(gRenderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(gRenderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(gRenderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(gRenderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

void ligneHorizontale(SDL_Renderer *gRenderer,int x, int y, int w, long c)
{
  SDL_Rect rect;
   int r = ( uint8_t )(c >> 16);
   int g = ( uint8_t )(c >>  8);
   int b = (  uint8_t  )c;

  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = 1;

   SDL_SetRenderDrawColor( gRenderer,r,g,b,255);
   SDL_RenderFillRect( gRenderer,&rect);
}

void dessine_disque(SDL_Renderer *gRenderer,int cx, int cy, int rayon, long coul)
{
  int d, y, x;

  d = 3 - (2 * rayon);
  x = 0;
  y = rayon;

  while (y >= x) {
    ligneHorizontale(gRenderer,cx - x, cy - y, 2 * x + 1, coul);
    ligneHorizontale(gRenderer,cx - x, cy + y, 2 * x + 1, coul);
    ligneHorizontale(gRenderer,cx - y, cy - x, 2 * y + 1, coul);
    ligneHorizontale(gRenderer,cx - y, cy + x, 2 * y + 1, coul);

    if (d < 0)
      d = d + (4 * x) + 6;
    else {
      d = d + 4 * (x - y) + 10;
      y--;
    }

    x++;
  }
}

double getCurrentTimeMS()
{
    double clo=clock();
    return 1000.*clo/CLOCKS_PER_SEC;
}

void dessine_fondtransparent(SDL_Renderer *gRenderer, int x, int y, int w, int h)
{
   SDL_Rect rect;
   rect.x=0;
   rect.y=0;
   rect.w=w;
   rect.h=h;
   SDL_SetRenderDrawColor( gRenderer,0,0,0,0);
   SDL_RenderFillRect( gRenderer,&rect);
}




void dessine_ellipse_vide(SDL_Renderer *gRenderer,int cx, int cy, int rx, int ry, int coul, double largeur_trait)
{
    int x,y;
    double dist_ellipse, dist_ellipse_trait_limit;
    int minrxry=rx>ry?ry:rx;

    // couleur
    int r = ( uint8_t )(coul >> 16);
    int g = ( uint8_t )(coul >>  8);
    int b = (  uint8_t  )coul;
    SDL_SetRenderDrawColor( gRenderer,r,g,b,255);

    // traitement largeur trait
    if (largeur_trait<=1) largeur_trait=1;
    if (largeur_trait>=rx) largeur_trait=rx;
    // calcul de la dist /centre ellipse correspondant a la largeur du trait
    dist_ellipse_trait_limit=((minrxry-largeur_trait)*(minrxry-largeur_trait))/(double)(minrxry*minrxry);


    //  methode brute force (parcours de l'ens des pts et dessin des points qui sont sur le perimetre...
    // methode à n'appliquer que si usage de textures....
    for (x=cx-rx;x<=cx+rx;x++)
        for (y=cy-ry;y<=cy+ry;y++)
        {
            dist_ellipse=((x-cx)*(x-cx))/(double)(rx*rx)+((y-cy)*(y-cy))/(double)(ry*ry);
            if(dist_ellipse>=dist_ellipse_trait_limit && dist_ellipse<=1.)
            {
                SDL_RenderDrawPoint(gRenderer,x,y);
            }
        }
}


void dessine_ellipse_plein(SDL_Renderer *gRenderer,int cx, int cy, int rx, int ry, int coul)
{
    int x,y;
    double dist_ellipse;

    // couleur
    int r = ( uint8_t )(coul >> 16);
    int g = ( uint8_t )(coul >>  8);
    int b = (  uint8_t  )coul;
    SDL_SetRenderDrawColor( gRenderer,r,g,b,255);


    //  methode brute force (parcours de l'ens des pts et dessin des points qui sont sur la surface...
    // methode à n'appliquer que si usage de textures....
    for (x=cx-rx;x<=cx+rx;x++)
        for (y=cy-ry;y<=cy+ry;y++)
        {
            dist_ellipse=((x-cx)*(x-cx))/(double)(rx*rx)+((y-cy)*(y-cy))/(double)(ry*ry);
            if(dist_ellipse<=1.)
                SDL_RenderDrawPoint(gRenderer,x,y);
        }
}





