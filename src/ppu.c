#include <2c02.h>
#include <emu_defs.h>
uint8_t int_ram[0x2000]; //this can be remapped with mapper implement with mapper

uint8_t *pattern_table_0, *pattern_table_1, *name_table_0, *name_table_1, *name_table_2, *name_table_3, *palette_ram_ind;



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

int write_ppu(){


    return 0;
}

uint8_t *read_ppu(int status){
    

    return NULL;
}

