#include "emu_defs.h"


#define uint16_t unsigned short

//TODO: mapper should be exculuded from this source
extern uint8_t submapper, *file_ptr, trainer, *prg_rom_ptr, *chr_rom_ptr, *misc_rom_ptr, *prg_ram, *prg_nvram;
extern int prg_rom_size,chr_rom_size, prg_ram_size, prg_nvram_size, trainer_offset, misc_rom_size;

//TODO:this registers would be better if constructed as structs
uint8_t cpu_internal_ram[0x7FF];
uint8_t ppu_registers[8];
uint8_t apu_registers[16];



char dummy_mem[0xFFFF];

uint8_t* mapper0(uint16_t addr){
    if((addr>=0x6000)&&(addr < 0x8000)){
        if(prg_ram_size > 0){
            return &prg_ram[(addr&0x1FFF)&((uint16_t)(prg_ram_size-1))];
        }else{
            //prg_ram not not defined by cartridge yet loaded program asked for prg_ram address
            return 0;
        }
    }else if((addr>=0x8000)&&(addr < 0xC000)){
        return &prg_rom_ptr[addr&0x3FFF];
    }else if((addr>=0xC000)&&(addr <= 0xFFFF)){
        return &prg_rom_ptr[0x4000 + (addr&0x3FFF)];
    }else{
        return 0;
    }

    return 0;
}


/*

int mem_map(char *name,int name_size, int size){
    if(name_size < 5){

    }else{
        return -1;
    }
}

*/

int cas_mem_read(uint16_t addr, uint8_t *return_val){
    uint8_t *mapper_ret;
    if(addr < 0x2000){//cpu internal ram
        *return_val = cpu_internal_ram[addr&0x7FF];
    }else if((addr>=0x2000)&&(addr<0x4000)){//ppu registers
        *return_val = ppu_registers[addr&0x0007];
    }else if((addr>=0x4000)&&(addr<0x4018)){//apu registers
        *return_val = apu_registers[addr&0x000F];
    }else if((addr>=0x4018)&&(addr<0x4020)){//test mode registers

    }else if((addr>=0x4020)&&(addr<0xFFFF)){//to mapper
        //only mapper0 for now
        mapper_ret = mapper0(addr);
        if(mapper_ret != NULL){
            *return_val = *mapper0(addr);
        }
    }else{
        return -1;
    }

    return 0;
}


int cas_mem_write(uint16_t addr, uint8_t val){
    //only mapper0 for now
   if(addr < 0x2000){
        cpu_internal_ram[addr&0x7FF] = val;
    }else if((addr>=0x2000)&&(addr<0x4000)){//ppu registers
        ppu_registers[addr&0x0007] = val;
    }else if((addr>=0x4000)&&(addr<0x4018)){//apu registers
        apu_registers[addr&0x000F] = val;
    }else if((addr>=0x4018)&&(addr<0x4020)){//test mode registers

    }else if((addr>=0x4020)&&(addr<0xFFFF)){//to mapper
        //only mapper0 for now
        *mapper0(addr) = val;

    }else{
        return -1;
    }

    return 0;
}

