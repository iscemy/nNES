#include <stdio.h>
#include <emu_defs.h>
#include <linked_list.h>
#include <cpu_addr_space.h>



int regpc_delta = 0;

void dis_imp();
void dis_acc();
void dis_imm();
void dis_zp();
void dis_zpx();
void dis_zpy();
void dis_rel();
void dis_abso();
void dis_absx();
void dis_absy();
void dis_ind();
void dis_indx();
void dis_indy();


static void (*addrtable[256]) = {
		/*          |0|   |1|     |2|   |3|     |4|    |5|	  |6|	|7|     |8|	 |9|     |A|   |B|    |C|    |D|   |E|	  |F|         */
		/* 0 */     &dis_imp, &dis_indx,  &dis_imp, &dis_indx,  &dis_zp,   &dis_zp,   &dis_zp,  &dis_zp,  &dis_imp,  &dis_imm,  &dis_acc,  &dis_imm, &dis_abso, &dis_abso, &dis_abso, &dis_abso, /* 0 */
		/* 1 */     &dis_rel, &dis_indy,  &dis_imp, &dis_indy,  &dis_zpx,  &dis_zpx,  &dis_zpx, &dis_zpx, &dis_imp, &dis_absy,  &dis_imp, &dis_absy, &dis_absx, &dis_absx, &dis_absx, &dis_absx, /* 1 */
		/* 2 */    &dis_abso, &dis_indx,  &dis_imp, &dis_indx,  &dis_zp,   &dis_zp,   &dis_zp,  &dis_zp,  &dis_imp,  &dis_imm,  &dis_acc,  &dis_imm, &dis_abso, &dis_abso, &dis_abso, &dis_abso, /* 2 */
		/* 3 */     &dis_rel, &dis_indy,  &dis_imp, &dis_indy,  &dis_zpx,  &dis_zpx,  &dis_zpx, &dis_zpx, &dis_imp, &dis_absy,  &dis_imp, &dis_absy, &dis_absx, &dis_absx, &dis_absx, &dis_absx, /* 3 */
		/* 4 */     &dis_imp, &dis_indx,  &dis_imp, &dis_indx,  &dis_zp,   &dis_zp,   &dis_zp,  &dis_zp,  &dis_imp,  &dis_imm,  &dis_acc,  &dis_imm, &dis_abso, &dis_abso, &dis_abso, &dis_abso, /* 4 */
		/* 5 */     &dis_rel, &dis_indy,  &dis_imp, &dis_indy,  &dis_zpx,  &dis_zpx,  &dis_zpx, &dis_zpx, &dis_imp, &dis_absy,  &dis_imp, &dis_absy, &dis_absx, &dis_absx, &dis_absx, &dis_absx, /* 5 */
		/* 6 */     &dis_imp, &dis_indx,  &dis_imp, &dis_indx,  &dis_zp,   &dis_zp,   &dis_zp,  &dis_zp,  &dis_imp,  &dis_imm,  &dis_acc,  &dis_imm, &dis_ind, &dis_abso, &dis_abso, &dis_abso, /* 6 */
		/* 7 */     &dis_rel, &dis_indy,  &dis_imp, &dis_indy,  &dis_zpx,  &dis_zpx,  &dis_zpx, &dis_zpx, &dis_imp, &dis_absy,  &dis_imp, &dis_absy, &dis_absx, &dis_absx, &dis_absx, &dis_absx, /* 7 */
		/* 8 */     &dis_imm, &dis_indx,  &dis_imm, &dis_indx,  &dis_zp,   &dis_zp,   &dis_zp,  &dis_zp,  &dis_imp,  &dis_imm,  &dis_imp,  &dis_imm, &dis_abso, &dis_abso, &dis_abso, &dis_abso, /* 8 */
		/* 9 */     &dis_rel, &dis_indy,  &dis_imp, &dis_indy,  &dis_zpx,  &dis_zpx,  &dis_zpy, &dis_zpy, &dis_imp, &dis_absy,  &dis_imp, &dis_absy, &dis_absx, &dis_absx, &dis_absy, &dis_absy, /* 9 */
		/* A */     &dis_imm, &dis_indx,  &dis_imm, &dis_indx,  &dis_zp,   &dis_zp,   &dis_zp,  &dis_zp,  &dis_imp,  &dis_imm,  &dis_imp,  &dis_imm, &dis_abso, &dis_abso, &dis_abso, &dis_abso, /* A */
		/* B */     &dis_rel, &dis_indy,  &dis_imp, &dis_indy,  &dis_zpx,  &dis_zpx,  &dis_zpy, &dis_zpy, &dis_imp, &dis_absy,  &dis_imp, &dis_absy, &dis_absx, &dis_absx, &dis_absy, &dis_absy, /* B */
		/* C */     &dis_imm, &dis_indx,  &dis_imm, &dis_indx,  &dis_zp,   &dis_zp,   &dis_zp,  &dis_zp,  &dis_imp,  &dis_imm,  &dis_imp,  &dis_imm, &dis_abso, &dis_abso, &dis_abso, &dis_abso, /* C */
		/* D */     &dis_rel, &dis_indy,  &dis_imp, &dis_indy,  &dis_zpx,  &dis_zpx,  &dis_zpx, &dis_zpx, &dis_imp, &dis_absy,  &dis_imp, &dis_absy, &dis_absx, &dis_absx, &dis_absx, &dis_absx, /* D */
		/* E */     &dis_imm, &dis_indx,  &dis_imm, &dis_indx,  &dis_zp,   &dis_zp,   &dis_zp,  &dis_zp,  &dis_imp,  &dis_imm,  &dis_imp,  &dis_imm, &dis_abso, &dis_abso, &dis_abso, &dis_abso, /* E */
		/* F */     &dis_rel, &dis_indy,  &dis_imp, &dis_indy,  &dis_zpx,  &dis_zpx,  &dis_zpx, &dis_zpx, &dis_imp, &dis_absy,  &dis_imp, &dis_absy, &dis_absx, &dis_absx, &dis_absx, &dis_absx  /* F */
};																																											   



static	char* optable[256] = {
		/*			 |0|   |1|   |2|   |3|      |4|  	|5|	   |6|	 |7|  |8|	 |9|     |A|   |B|    |C|    |D|   |E|	  |F|         */
		/* 0 */      "brk", "ora", "illi", "illi",  "illi",  "ora", "asl", "illi", "php",  "ora",  "asl",  "illi", "illi",  "ora", "asl",  "illi",  /* 0 */
		/* 1 */      "bpl", "ora", "illi", "illi",  "illi",  "ora", "asl", "illi", "clc",  "ora",  "illi",  "illi", "illi",  "ora", "asl",  "illi",  /* 1 */
		/* 2 */      "jsr", "and_","illi", "illi",  "bit",  "and_","rol", "illi", "plp",  "and_", "rol",  "illi", "bit",  "and_", "rol",  "illi",  /* 2 */
		/* 3 */      "bmi", "and_","illi", "illi",  "illi",  "and_","rol", "illi", "sec",  "and_", "illi",  "illi", "illi",  "and_", "rol",  "illi",  /* 3 */
		/* 4 */      "rti", "eor", "illi", "illi",  "illi",  "eor", "lsr", "illi", "pha",  "eor",  "lsr",  "illi", "jmp",  "eor", "lsr",  "illi",  /* 4 */
		/* 5 */      "bvc", "eor", "illi", "illi",  "illi",  "eor", "lsr", "illi", "cli",  "eor",  "illi",  "illi", "illi",  "eor", "lsr",  "illi",  /* 5 */
		/* 6 */      "rts", "adc", "illi", "illi",  "illi",  "adc", "ror", "illi", "pla",  "adc",  "ror",  "illi", "jmp",  "adc", "ror",  "illi",  /* 6 */
		/* 7 */      "bvs", "adc", "illi", "illi",  "illi",  "adc", "ror", "illi", "sei",  "adc",  "illi",  "illi", "illi",  "adc", "ror",  "illi",  /* 7 */
		/* 8 */      "illi", "sta", "illi", "illi",  "sty",  "sta", "stx", "illi", "dey",  "illi",  "txa",  "illi", "sty",  "sta", "stx",  "illi",  /* 8 */
		/* 9 */      "bcc", "sta", "illi", "illi",  "sty",  "sta", "stx", "illi", "tya",  "sta",  "txs",  "illi", "illi",  "sta", "illi",  "illi",  /* 9 */
		/* A */      "ldy", "lda", "ldx", "illi",  "ldy",  "lda", "ldx", "illi", "tay",  "lda",  "tax",  "illi", "ldy",  "lda", "ldx",  "illi",  /* A */
		/* B */      "bcs", "lda", "illi", "illi",  "ldy",  "lda", "ldx", "illi", "clv",  "lda",  "tsx",  "illi", "ldy",  "lda", "ldx",  "illi",  /* B */
		/* C */      "cpy", "cmp", "illi", "illi",  "cpy",  "cmp", "dec", "illi", "iny",  "cmp",  "dex",  "illi", "cpy",  "cmp", "dec",  "illi",  /* C */
		/* D */      "bne", "cmp", "illi", "illi",  "illi",  "cmp", "dec", "illi", "cld",  "cmp",  "illi",  "illi", "illi",  "cmp", "dec",  "illi",  /* D */
		/* E */      "cpx", "sbc", "illi", "illi",  "cpx",  "sbc", "inc", "illi", "inx",  "sbc",  "nop",  "illi", "cpx",  "sbc", "inc",  "illi",  /* E */
		/* F */      "beq", "sbc", "illi", "illi",  "illi",  "sbc", "inc", "illi", "sed",  "sbc",  "illi",  "illi", "illi",  "sbc", "inc",  "illi"   /* F */
};	



//((void(*)(void))addrtable[*(regpc_ptr)])();
int disasm_llist_id;

int start_disassembler(uint16_t rom_addr, int size){ //sign is important remember
    disasm_llist_id = create_llist();    
    uint8_t opcode;
    char buffer[50];
    int index = 0, str_len;
    while(index < size){

        opcode =  *cas_mem_read(rom_addr + index);
        ((void (*) (void))addrtable[opcode])();

        if(regpc_delta == 2){
            str_len = snprintf(buffer, 20, "%04hx %s %04hx\n",rom_addr+index, optable[opcode], ((*cas_mem_read(rom_addr + index + 2)) << 8) + (*cas_mem_read(rom_addr + index + 1)));
        }else if(regpc_delta == 1){
            str_len = snprintf(buffer, 20, "%04hx %s %02hx\n",rom_addr+index, optable[opcode], *cas_mem_read(rom_addr + index + 1));
        }else if(regpc_delta == 0){
            str_len =  snprintf(buffer, 20, "%04hx %s\n",rom_addr+index, optable[opcode]);
        }
        
        add_item_to_llist(disasm_llist_id, buffer, str_len, rom_addr + index);
        //printf("%s\n", optable[opcode]);
        index += 1 + regpc_delta; 
    }
    return 0; 
}

char *get_disasm_at_addr(uint16_t addr){
    int size = 0;

    char *disasm_str = get_from_key(disasm_llist_id, addr, &size);

    if((disasm_str == NULL)||(size == 0)){
        printf("no diasm at %hx", addr);
        return NULL;
    }
    
    return disasm_str;
}

int dissassemble_at_addr(uint16_t rom_addr){ //sign is important remember  
    uint8_t opcode;

    opcode =  *cas_mem_read(rom_addr);
    ((void (*) (void))addrtable[opcode])();

    if(regpc_delta == 2){
        printf("%04hx %s %hx %04hx\n", rom_addr, optable[opcode], opcode, ((*cas_mem_read(rom_addr + 2)) << 8) + (*cas_mem_read(rom_addr + 1)));
    }else if(regpc_delta == 1){
        printf("%04hx %s %hx %02hx\n", rom_addr, optable[opcode], opcode, *cas_mem_read(rom_addr + 1));
    }else if(regpc_delta == 0){
        printf("%04hx %s\n", rom_addr, optable[opcode]);
    }
    return 0;         
}

void dis_abso() { //dis_absolute
	regpc_delta = 2;
}
void dis_absx() { //dis_absolute,X
	regpc_delta = 2;
}
void dis_absy() { //dis_absolute,Y
	regpc_delta = 2;
}
void dis_acc() { //dis_accumulator
    regpc_delta = 0;
}
void dis_imm() { //dis_immediate
	regpc_delta = 1;
}
void dis_zp() { //zero-page
	regpc_delta = 1;
}
void dis_zpx() { //zero-page,X
	regpc_delta = 1;
}
void dis_zpy() { //zero-page,Y
	regpc_delta = 1;
}
void dis_rel() { //dis_relative for branch ops (8-bit dis_immediate value, sign-extended)

    regpc_delta = 1;
}
void dis_ind() { //indirect only for jmp
	regpc_delta = 2;
}
void dis_indx() { // (indirect,X) aka pre index

	regpc_delta = 1;
}
void dis_indy() { // (indirect),Y aka post index
	regpc_delta = 1;
}

void dis_imp(){
    regpc_delta = 0;
}


//void main(){
//    printf("%s",*optable);

//}
