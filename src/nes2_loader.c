#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emu_defs.h"

#define GET_BIT(num, c)     ((num&(0x01<<c))>>c)
#define SET_BIT(num, c)     (num=((num)|(0x01<<c)))
#define CLEAR_BIT(num, c)   (num=(num)&(~(0x01<<c)))

//following defines depends on architecture
//

typedef struct __attribute__((packed)){
    char magic[4];
    char prg_rom_size_l;
    char chr_rom_size_l;    
    char flags_6;
    char flags_7;
    char mapper;
    char prg_chr_rom_size_h; // 4 bit msb
    char prg_ram_prg_nvram; //shift count 
    char chr_ram_size;
    char timing; //ppu cpu/ ntsc or pal
    char sys_type;
    char misc_roms;
    char default_exp_dev;
    uint8_t payload[0];
}nes2_file_header_t;

uint8_t submapper, *file_ptr, trainer, *prg_rom_ptr, *chr_rom_ptr, *misc_rom_ptr, *prg_ram, *prg_nvram;
int prg_rom_size,chr_rom_size, prg_ram_size, prg_nvram_size, trainer_offset, misc_rom_size;

int load_nes2(char *filepath){
    int bytes_read = 0,index = 0, total_readed_byte = 0, size;

    uint16_t mapper;
    

    nes2_file_header_t *file_header;
    printf("loading %s file\n", filepath);
    FILE *stream = fopen(filepath,"rb");

    fseek(stream, 0, SEEK_END);
    size = ftell(stream);
    rewind(stream);

    file_ptr = malloc(size);
    if(file_ptr == NULL){
        return -1;
    }

    printf("size %d\n",size);

    bytes_read = fread(file_ptr + index*1024,1, 1024, stream);
    while (bytes_read != 0){
        index++;
        total_readed_byte += bytes_read;
        bytes_read = fread(file_ptr + index*1024,1, 1024, stream);
    }

    fclose(stream);
    printf("total_readed_byte: %d\n",total_readed_byte);
    if((file_ptr[0] == 'N')&&(file_ptr[1] == 'N')&&(file_ptr[2] == 'S')&&(file_ptr[0] == 0x1A) == 0){
        return -1;
    }

    file_header = (nes2_file_header_t*)file_ptr;
    prg_rom_size = (file_header->prg_rom_size_l + ((file_header->prg_chr_rom_size_h&0x0F)<<8))*16384;
    chr_rom_size = (file_header->chr_rom_size_l + ((file_header->prg_chr_rom_size_h&0xF0)<<4))*8192;

    prg_ram_size = 1<<(file_header->prg_ram_prg_nvram&0x0F);
    prg_nvram_size = 1<<((file_header->prg_ram_prg_nvram&0xF0)>>4);
    misc_rom_size = size-(chr_rom_size + prg_rom_size + sizeof(nes2_file_header_t));


    mapper = ((file_header->flags_6&0xF0)>>4) + (file_header->flags_7&0xF0) + ((file_header->mapper&0x0F)<<8);
    submapper = file_header->mapper&0xF0>>4;
    trainer = GET_BIT(file_header->flags_6,2);

    if(prg_ram_size > 1){
        prg_ram = malloc(prg_ram_size);
    }else{
        prg_ram_size = 0;
    }

    if(prg_nvram_size > 1){
        prg_nvram = malloc(prg_nvram_size);
    }else{
        prg_nvram_size = 0;
    }

    if((prg_nvram == NULL)||(prg_ram == NULL)){
        free(file_ptr);
        free(prg_nvram);
        free(prg_ram);
        //things gone wrong
        //couldnt able to allocate mem 
    }

    printf("prg_rom_size:%d, chr_rom_size:%d, prg_ram_size:%d, prg_ram_size:%d, misc_rom_size:%d\n",prg_rom_size, chr_rom_size, prg_ram_size, prg_ram_size, misc_rom_size);
    printf("mapper:%d ,submapper:%d, trainer:%d\n", mapper, submapper, trainer);

    if(trainer == 1){
        trainer_offset = 512;
    }else{
        trainer_offset = 0;
    }
    
    prg_rom_ptr = file_header->payload + trainer_offset;
    chr_rom_ptr = prg_rom_ptr + prg_rom_size;
    misc_rom_ptr = chr_rom_ptr + chr_rom_size;
    
    return 0;
}

/*
int main(){
        
    load_nes2("official_only.nes");
    return 0;
}
*/
