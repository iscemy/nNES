#include <2c02.h>
#include <emu_defs.h>
uint8_t int_ram[0x2000]; //this can be remapped with mapper implement with mapper

uint8_t *pattern_table_0, *pattern_table_1, *name_table_0, *name_table_1, *name_table_2, *name_table_3, *palette_ram_ind;


void mapper0_ppu(){

}

void ppu_powerup_state(){
    *(uint8_t*)&ppu_registers.PPUSTATUS = 0xA0;
}


int pixel_counter = 0;



int render_scanline(){
    
    return 0;
}

int cc_cnt = 341;
int scanline_counter = 0;

void ppu_tick(){
    
    if(cc_cnt > 0){
        if(scanline_counter < 20){
            
        }else if(scanline_counter == 21){
            //dummy scanline does not renders a thing
            
        }else if(scanline_counter < 260){

        }else if(scanline_counter == 261){
            ppu_registers.PPUSTATUS.vblanks  = true;
        }else{
            //things went so wrong
        }

       

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
    }
    
    return ret;    
}

//int mem_fetch_8(){

//}

uint16_t address_latch = 0x0000;
uint8_t address_latch_index = 1;

uint16_t scroll_latch = 0x0000;
uint8_t scroll_latch_index = 1;

void generate_cpu_nmi(){

}

int write_ppu(uint16_t addr, uint8_t data){
    switch (addr){
        case 0x2000: //ppuctrl wo
            printf("write ppuctrl");
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
            address_latch &= 0xFF<<(address_latch_index*0x08);
            address_latch |= data<<(address_latch_index*0x08);
            address_latch_index--;
            if(address_latch_index < 0) address_latch_index = 1;   
        break;

        case 0x2007: //data reg
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

        break;

        case 0x2004:

        break;

        case 0x2005:

        break;

        case 0x2006:

        break;

        case 0x2007:
            address_latch += ((ppu_registers.PPUCTRL.vram_inc_size*31) + 1);            
            return get_addr(address_latch); 
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
