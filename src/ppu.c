#include <2c02.h>
#include <emu_defs.h>
#include <nes2_loader.h>
#include <stdlib.h>
uint8_t int_ram[0x2000]; //this can be remapped with mapper implement with mapper

uint8_t *pattern_table_0, *pattern_table_1, *name_table_0, *name_table_1, *name_table_2, *name_table_3;
uint8_t palette_ram_ind[0x20];


extern bool NMI_SIG;

void mapper0_ppu(){
    pattern_table_0 = chr_rom_ptr; 
    pattern_table_1 = chr_rom_ptr + 0x0FFF;
    name_table_0 = malloc(0x0400);
    name_table_1 = name_table_0;
    name_table_2 = name_table_0;
    name_table_3 = name_table_0;
    
}

void ppu_powerup_state(){
    *(uint8_t*)&ppu_registers.PPUSTATUS = 0xA0;
    mapper0_ppu();
}

void generate_cpu_nmi(){
    NMI_SIG = true;
}


int pixel_counter = 0;



int render_scanline(){
    
    return 0;
}

int cc_cnt = 341;
int scanline_counter = 0;

void ppu_tick(){
    
    if(cc_cnt > 0){
        if(scanline_counter < 240){
            //Visible scanlines
        }else if(scanline_counter == 240){
            //post render scanline
        }else if(scanline_counter == 241){

            ppu_registers.PPUSTATUS.vblanks  = true;
            if((ppu_registers.PPUCTRL.nmi_eovbi == 1)) //this must be happen without being at scanline 241
                generate_cpu_nmi();                

        }else if(scanline_counter <= 260){
            
        }else if(scanline_counter == 261){
            ppu_registers.PPUSTATUS.vblanks  = false;
            scanline_counter = 0;
        }else{
            //things went so wrong
        }
        cc_cnt--;
       

    }else{
         cc_cnt = 341;  
         scanline_counter++;
    }

    


}

uint8_t *get_addr(uint16_t addr){
    uint8_t *ret = NULL;
    if((addr>=0x0000)&&(addr<=0x0FFF)){

        ret = pattern_table_0 + (addr&0x0FFF);

    }else if((addr>=0x1000)&&(addr<=0x1FFF)){

        ret = pattern_table_1 + (addr&0x0FFF);

    }else if(((addr>=0x2000)&&(addr<=0x23FF))||((addr>=0x3000)&&(addr<=0x33FF))){

        ret = name_table_0 + (addr&0x03FF);

    }else if(((addr>=0x2400)&&(addr<=0x27FF))||((addr>=0x3400)&&(addr<=0x37FF))){

        ret = name_table_1 + (addr&0x03FF);

    }else if(((addr>=0x2800)&&(addr<=0x2BFF))||((addr>=0x3800)&&(addr<=0x3BFF))){

        ret = name_table_2 + (addr&0x03FF);

    }else if(((addr>=0x2C00)&&(addr<=0x2FFF))||((addr>=0x3C00)&&(addr<=0x3EFF))){

        ret = name_table_3 + (addr&0x03FF);

    }else if((addr>=0x3F00)&&(addr<=0x3FFF)){

        ret = palette_ram_ind + (addr&0x1F);

    }

    if(ret == NULL){
        //buss error
        printf("ppu addr space bus error\n");
    }
    
    return ret;    
}

//int mem_fetch_8(){

//}

uint16_t address_latch = 0x0000;
uint8_t address_latch_index = 1;

uint16_t scroll_latch = 0x0000;
uint8_t scroll_latch_index = 1;


int write_ppu(uint16_t addr, uint8_t data){
    switch (addr){
        case 0x2000: //ppuctrl wo

            *(uint8_t*)&ppu_registers.PPUCTRL = data; //nss
        break;

        case 0x2001: //ppumask wo
            *(uint8_t*)&ppu_registers.PPUMASK = data; //nss

        break;

        case 0x2002: //ppu status ro
                      

        break;

        case 0x2003:
            ppu_registers.OAMADDR = data;
        break;
           
        case 0x2004:
            ppu_registers.OAMDATA = data;
        break;

        case 0x2005:
            
            scroll_latch &= 0xFF<<(scroll_latch_index*0x08);
            scroll_latch |= data<<(scroll_latch_index*0x08);
            scroll_latch_index--;
            if(scroll_latch_index < 0) scroll_latch_index = 1;
            
        break;

        case 0x2006: //addr reg
            printf("ADDR reg w %hx \n", address_latch);
            address_latch &= ~(0xFF<<(address_latch_index*0x08));
            address_latch |= data<<(address_latch_index*0x08);
            address_latch_index--;
            printf("ADDR reg w %hx %hx \n", address_latch, data);

            if(address_latch_index < 0) address_latch_index = 1;   
        break;

        case 0x2007: //data reg
            printf("DATA reg w, %hx\n", address_latch);
            *get_addr(address_latch) = data;
            address_latch += ((ppu_registers.PPUCTRL.vram_inc_size*31) + 1);
        break;

        case 0x4014:

        break;

        default:
            //bus error
        break;

    }

    return 0;
}


uint8_t status_r_latch = 0;


uint8_t *read_ppu(uint16_t addr, int status){
    switch (addr){
        case 0x2000:
            return (uint8_t*)&ppu_registers.PPUCTRL;
        break;

        case 0x2001:
            return (uint8_t*)&ppu_registers.PPUMASK;
        break;

        case 0x2002: //ppu status ro
            address_latch = 0x0000;

            ppu_registers.PPUSTATUS.vblanks = 1;
            return (uint8_t*)&ppu_registers.PPUSTATUS;
        break;

        case 0x2003:
            return (uint8_t*)&ppu_registers.OAMADDR;
        break;

        case 0x2004:
             return (uint8_t*)&ppu_registers.OAMADDR;
        break;

        case 0x2005:
            return (uint8_t*)&ppu_registers.OAMDATA;
        break;

        case 0x2006:
            //printf("addr reg w %hx \n", addr);
            return (uint8_t*)&ppu_registers.PPUADDR;
        break;

        case 0x2007:
            //printf("data reg r %hx \n", address_latch);
            address_latch += ((ppu_registers.PPUCTRL.vram_inc_size*31) + 1);            
            return (uint8_t*)get_addr(address_latch); 
        break;    

        case 0x4014:

        break;

        default:
            //bus error
        break;
    }
    return 0;
}

/*
Dont think this will be a problem if internal buffer is really contains garbage data, 
At the implementation above first read of 0x0000- 0x3F00 contains real information of memory not an internal buffer

The PPUDATA read buffer (post-fetch)

When reading while the VRAM address is in the range 0-$3EFF (i.e., before the palettes), the read will return the contents of an internal read buffer. This internal buffer is updated only when reading PPUDATA, and so is preserved across frames. After the CPU reads and gets the contents of the internal buffer, the PPU will immediately update the internal buffer with the byte at the current VRAM address. Thus, after setting the VRAM address, one should first read this register and discard the result.

Reading palette data from $3F00-$3FFF works differently. The palette data is placed immediately on the data bus, and hence no dummy read is required. Reading the palettes still updates the internal buffer though, but the data placed in it is the mirrored nametable data that would appear "underneath" the palette. (Checking the PPU memory map should make this clearer.) 

*(
*/
