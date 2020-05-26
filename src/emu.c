#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#include <2c02.h>
#include <nes2_loader.h>
#include <cpu.h>
#include <utilities.h>
#include <disassembler.h>

#define EMU_INFO_YOFFSET 260
#define WINDOW_WIDTH 300+EMU_INFO_YOFFSET
#define WINDOW_HEIGHT (300)
#define FONT_SIZE 20

//#include <6502.h>


///&//int main(){

//}



/*
- x, y: upper left corner.
- texture, rect: outputs.
*/

extern bool NMI_SIG;

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
char line1[30], line2[30], line3[30], line4[30];
void update_cpu_status_texts(uint8_t regac, uint8_t regx, uint8_t regy, uint16_t pc, uint8_t sp){
    
    snprintf(line1, sizeof(line1), "A:%02x X:%02x Y:%02x", regac,regx,regy);
    snprintf(line2, sizeof(line2), "PC:%04x SP:%02x", pc, sp);
    snprintf(line3, sizeof(line3), "%02x, %02x, %02x, %02x, %02x, %02x",    ppu_registers.PPUCTRL, ppu_registers.PPUMASK, ppu_registers.PPUSTATUS,
                                                                            ppu_registers.OAMADDR, ppu_registers.OAMDATA, ppu_registers.PPUSCROLL);
    snprintf(line4, sizeof(line4), "%d", NMI_SIG);
    lines[0] = line1;	
    lines[1] = line2;
    lines[2] = line3;
    lines[3] = line4;
}


extern uint8_t *name_table_0;

int main() {
    SDL_Event event;
    SDL_Rect rect1;
    SDL_Renderer *renderer;
    SDL_Texture *texture1;
    SDL_Window *window;
    char *font_path = "FreeSerif.ttf";
    int quit = 0;

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(font_path, FONT_SIZE);

    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }
   
    load_nes2("donkey.nes");
    power_up_state();
    ppu_powerup_state();
    update_cpu_status_texts(regac, regx, regy, regpc,  regsp);
    bool run = false;
    //while(1);
    start_disassembler(0x8000,0x7FFF);
    bool print_evey_inst = false;
    //return 0;
    while (!quit) {

        if(run == true){
            for(int i = 0; i<100; i++){
                        ppu_tick();
                        ppu_tick();
                        ppu_tick();
                        if((cpu_tick() == 1)&&(print_evey_inst == 1)){
                            dissassemble_at_addr(regpc);
                        }

            }
            update_cpu_status_texts(regac, regx, regy, regpc,  regsp);
        }else{
            SDL_Delay(10);
        }  
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }         
            if(event.type == SDL_KEYDOWN){
                
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        memdump(name_table_0, 0x0, 0x400); 
                        //start_disassemble(regpc, regpc, int size);
                        break;
                    case SDLK_RIGHT:
                        print_evey_inst = !print_evey_inst;
                        update_cpu_status_texts(regac, regx, regy, regpc,  regsp);
                        
                        break;
                    case SDLK_UP:
                        run = !run;
                        if(run == true)
                            printf("%x\n",run);

                        break;

                    case SDLK_s:
                        
                        ppu_tick();
                        ppu_tick();
                        ppu_tick();         
                        cpu_tick();               
                        dissassemble_at_addr(regpc);
                        update_cpu_status_texts(regac, regx, regy, regpc,  regsp);
                            
                    break;
                    default:
                        break;
                }               
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
        
    }

    /* Deinit TTF. */
    SDL_DestroyTexture(texture1);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
