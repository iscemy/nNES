#include <stdbool.h>
#include <emu_defs.h>
#include <stdio.h>

typedef struct{
	bool base_namet_addr_0:1; // bit 0
    bool base_namet_addr_1:1; // bit 0
	bool vram_inc_size:1;
	bool sprite_patternt_addr:1;
	bool backg_patternt_addr:1;
	bool sprite_size:1;
	bool ppu_ms:1;
	bool nmi_eovbi:1;
}PPUCTRL_t;

typedef struct{
	bool grey_scale:1; // bit 0
    bool shwblep:1; // Show background in leftmost 8 pixels of screen, 0: Hide
	bool shwsilep:1; //Show sprites in leftmost 8 pixels of screen, 0: Hide
	bool shwbg:1;   //Show background
	bool shwsp:1;   //Show sprites
	bool emphR:1; 
	bool emphG:1;
	bool emphB:1;
}PPUMASK_t;

typedef struct {
    bool res0:1;
    bool res1:1;
    bool res2:1;
    bool res3:1;
    bool res4:1;
    bool sprite_ov:1;   //overflow
    bool sprite_zh:1;   //zero hit
    bool vblanks:1;     //vertical blank started
}PPUSTATUS_t;


struct{
    PPUCTRL_t   PPUCTRL;
    PPUMASK_t   PPUMASK;
    PPUSTATUS_t PPUSTATUS;
    uint8_t     OAMADDR;
    uint8_t     OAMDATA;
    uint8_t     PPUSCROLL;
    uint8_t     PPUADDR;
    uint8_t     PPUDATA;
}__attribute__((packed))ppu_registers;



uint8_t OAMDMA;
void ppu_powerup_state();
int write_ppu(uint16_t addr, uint8_t data);
uint8_t *read_ppu(uint16_t addr, int status);



