
int load_nes2(char *filepath);

uint8_t submapper, *file_ptr, trainer, *prg_rom_ptr, *chr_rom_ptr, *misc_rom_ptr, *prg_ram, *prg_nvram;
int prg_rom_size,chr_rom_size, prg_ram_size, prg_nvram_size, trainer_offset, misc_rom_size;
