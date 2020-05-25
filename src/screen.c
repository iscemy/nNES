/*
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#define EMU_INFO_YOFFSET 260
#define WINDOW_WIDTH 300+EMU_INFO_YOFFSET
#define WINDOW_HEIGHT (300)
#define FONT_SIZE 20




void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}


char *lines[5] = {0,0,0,0,0};
char line1[30], line2[30];
void update_cpu_status_texts(char regac, char regx, char regy, unsigned short pc, char sp){
    
    snprintf(line1, sizeof(line1), "A:%02x X:%02x Y:%02x", regac,regx,regy);
    snprintf(line2, sizeof(line2), "PC:%04x SP:%02x", pc, sp);
    lines[0] = line1;	
    lines[1] = line2;
}


int screen_thread() {
    SDL_Event event;
    SDL_Rect rect1;
    SDL_Renderer *renderer;
    SDL_Texture *texture1;
    SDL_Window *window;
    char *font_path;
    int quit;


    font_path = "FreeSerif.ttf";

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(font_path, FONT_SIZE);

    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }
   
    quit = 0;



    while (!quit) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        for(int line_index = 0; line_index<sizeof(lines)/sizeof(char*);line_index++){
            if(lines[line_index] != 0){
                get_text_and_rect(renderer, EMU_INFO_YOFFSET, line_index*(FONT_SIZE),lines[line_index] , font, &texture1, &rect1);
                SDL_RenderCopy(renderer, texture1, NULL, &rect1);
                SDL_DestroyTexture(texture1);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }


    SDL_DestroyTexture(texture1);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
*/
