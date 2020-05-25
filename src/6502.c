#include "6502.h"
#include <stdio.h>
#include <string.h>
#include <cpu_addr_space.h>
#include <cpu.h>

unsigned int operand = 0, outval = 0, cycles, addr_mode = 0, total_cycle = 0, regpc_d;
bool is_page_crossed = 0;
void *operand_addr = NULL;
bool i_mask_pre = 0;

bool NMI_SIG;

/*addressing modes*/
void imp() { //implied
    //no operand implied by instruction
}
void abso() { //absolute
    
	operand = *cas_mem_read(((*cas_mem_read(regpc+2))<<8) + *cas_mem_read(regpc+1));
	operand_addr = cas_mem_read(((*cas_mem_read(regpc+2))<<8) + *cas_mem_read(regpc+1));
	addr_mode = addr_mode_abso;
	regpc_d = 2;
}
void absx() { //absolute,X
	operand = *cas_mem_read(((*cas_mem_read(regpc+2))<<8) + *cas_mem_read(regpc+1) + regx);
	operand_addr = cas_mem_read(((*cas_mem_read(regpc+2))<<8) + *cas_mem_read(regpc+1) + regx);
	addr_mode = addr_mode_absx;
	if((*cas_mem_read(regpc+1) + regx) > 0xFF){
		is_page_crossed = 1;
	}
	regpc_d = 2;
}
void absy() { //absolute,Y
	operand = *cas_mem_read(((*cas_mem_read(regpc+2))<<8) + *cas_mem_read(regpc+1) + regy);
	operand_addr = cas_mem_read(((*cas_mem_read(regpc+2))<<8) + *cas_mem_read(regpc+1) + regy);
	addr_mode = addr_mode_absy;
	if((*cas_mem_read(regpc+1) + regy) > 0xFF){
		is_page_crossed = 1;
	}
	regpc_d = 2;
}
void acc() { //accumulator
	operand = regac; //most likely
	operand_addr = &regac;
	addr_mode = addr_mode_acc;
}
void imm() { //immediate
	operand = *cas_mem_read(regpc+1);
	addr_mode = addr_mode_imm;
	regpc_d = 1;
}
void zp() { //zero-page
	operand = *cas_mem_read(*cas_mem_read(regpc+1));
	operand_addr = cas_mem_read(*cas_mem_read(regpc+1));
	addr_mode = addr_mode_zp;
	regpc_d = 1;
}
void zpx() { //zero-page,X
	operand = *cas_mem_read((*cas_mem_read(regpc+1) + regx)&0xFF);
	operand_addr = cas_mem_read((*cas_mem_read(regpc+1) + regx)&0xFF);
	addr_mode = addr_mode_zpx;
	regpc_d = 1;
}
void zpy() { //zero-page,Y
	operand = *cas_mem_read((*cas_mem_read(regpc+1) + regy)&0xFF);
	operand_addr = cas_mem_read((*cas_mem_read(regpc+1) + regy)&0xFF);
	addr_mode = addr_mode_zpy;
	regpc_d = 1;
}
void rel() { //relative for branch ops (8-bit immediate value, sign-extended)
	operand = *cas_mem_read(regpc+1);
	addr_mode = addr_mode_rel;
    regpc_d = 1;
}
void ind() { //indirect only for jmp
    unsigned short op = (*cas_mem_read(regpc+1)) + ((*cas_mem_read(regpc+2))<<8);
	operand = (*cas_mem_read(op)) + ((*cas_mem_read(op+1))<<8) ;
	addr_mode = addr_mode_ind;
    //printf("ind addressing %x %x \n", op, operand);
	regpc += 2;
}
void indx() { // (indirect,X) aka pre index
	operand = *cas_mem_read(*cas_mem_read((*cas_mem_read(regpc+1) + regx)&0xFF) + (*cas_mem_read((*cas_mem_read(regpc+1) + regx + 1)&0xFF) << 8));
	operand_addr = &*cas_mem_read(*cas_mem_read((*cas_mem_read(regpc+1) + regx)&0xFF) + (*cas_mem_read((*cas_mem_read(regpc+1) + regx + 1)&0xFF) << 8));
	addr_mode = addr_mode_indx;
	regpc_d = 1;
}
void indy() { // (indirect),Y aka post index
	operand = *cas_mem_read(*cas_mem_read((*cas_mem_read(regpc+1))) + (*cas_mem_read((*cas_mem_read(regpc+1) + 1)) << 8) + regy);
	operand_addr = &*cas_mem_read(*cas_mem_read((*cas_mem_read(regpc+1))) + (*cas_mem_read((*cas_mem_read(regpc+1) + 1)&0xFF) << 8) + regy);
	addr_mode = addr_mode_indy;
	if(*cas_mem_read((*cas_mem_read(regpc+1))) + regy > 0xFF){
		is_page_crossed = 1;
	}
	regpc_d = 1;
}
/*addressing modes*/


static inline void N_flag(unsigned char val)
{
    ////printf("\nnflag: %d  val:%hx\n", (val&(0x01<<7))>>7,val);
    ////printf("\nnflag: %d  val:%hx\n", GET_BIT(val,7),val);
	status_r.sign = GET_BIT(val,7);
}

static inline void Z_flag(unsigned char val)
{
	status_r.zero = val == 0;
}



/*instructions*/
//ok
void adc(){ // BCD not implemented and not necessary for nes 
	unsigned int temp = regac + operand + status_r.carry;
	status_r.carry = temp > 0xFF;
	status_r.overflow = ((temp ^ regac) & ((temp ^ operand)&0x80)) != 0;
	regac = temp&0xFF;
	N_flag(regac);
	Z_flag(regac);
}

//ok
void and_(){
	regac &= operand;
	N_flag(regac);
	Z_flag(regac);
}
//ok
void asl(){
	unsigned int temp = 0;
	status_r.carry = GET_BIT(operand,7);
	temp = operand << 1;

	//regac = temp&0xFF;
    *(unsigned char*)operand_addr = temp&0xFF;
	N_flag(temp&0xFF);
	Z_flag(temp&0xFF);	
}

void bcc(){//clock will vary with new/page success
	if(status_r.carry == 0){
        if (operand>=0x80){
		    regpc = (regpc + ((0xFF00) + operand) + 2) & 0xFFFF;
        }else{
            regpc = (regpc + (operand) + 2) & 0xFFFF;
        }
        regpc_d = -1;
	}
}

void bcs(){
	if(status_r.carry == 1){
        if (operand>=0x80){
		    regpc = (regpc + ((0xFF00) + operand) + 2) & 0xFFFF;
        }else{
            regpc = (regpc + (operand) + 2) & 0xFFFF;
        }
        regpc_d = -1;
	}
}

void beq(){
	if(status_r.zero == 1){
        if (operand>=0x80){
		    regpc = (regpc + ((0xFF00) + operand) + 2) & 0xFFFF;
        }else{
            regpc = (regpc + (operand) + 2) & 0xFFFF;
        }
        regpc_d = -1;
	}
}

void bne(){
	if(status_r.zero == 0){
        if (operand>=0x80){
		    regpc = (regpc + ((0xFF00) + operand) + 2) & 0xFFFF;
        }else{
            regpc = (regpc + (operand) + 2) & 0xFFFF;
        }
        regpc_d = -1;
	}
}

void bmi(){
	if(status_r.sign == 1){
        if (operand>=0x80){
		    regpc = (regpc + ((0xFF00) + operand) + 2) & 0xFFFF;
        }else{
            regpc = (regpc + (operand) + 2) & 0xFFFF;
        }
        regpc_d = -1;
	}
}

void bpl(){
	if(status_r.sign == 0){
        if (operand>=0x80){
		    regpc = (regpc + ((0xFF00) + operand) + 2) & 0xFFFF;
        }else{
            regpc = (regpc + (operand) + 2) & 0xFFFF;
        }
        regpc_d = -1;
	}
}

void bvc(){
	if(status_r.overflow == 0){
        if (operand>=0x80){
		    regpc = (regpc + ((0xFF00) + operand) + 2) & 0xFFFF;
        }else{
            regpc = (regpc + (operand) + 2) & 0xFFFF;
        }
        regpc_d = -1;
	}
}

void bvs(){
	if(status_r.overflow == 1){
        if (operand>=0x80){
		    regpc = (regpc + ((0xFF00) + operand) + 2) & 0xFFFF;
        }else{
            regpc = (regpc + (operand) + 2) & 0xFFFF;
        }
        regpc_d = -1;
	}
}

void clc(){
	status_r.carry = 0;
}

void cld(){
	status_r.decimal = 0;
}

void cli(){
	status_r.interrupt = 0;
}

void clv(){
	status_r.overflow = 0;
}

void bit(){
	outval = operand & regac;
    //printf("debug: %hx , out:%hx\n",operand, outval);
	status_r.overflow = GET_BIT(operand,6);
	N_flag(operand);
	Z_flag(outval);
}

void cmp(){
	outval = regac - operand;

	status_r.carry = regac >= operand;
	N_flag(outval);
	Z_flag(outval);
}

void cpx(){
	outval = regx - operand;
    
	status_r.carry = regx >= operand;
	N_flag(outval);
	Z_flag(outval);
}

void cpy(){
	outval = regy - operand;
	status_r.carry = regy >= operand;
	N_flag(outval);
	Z_flag(outval);
}

void dec(){
	*(unsigned char*)operand_addr = operand - 1;
	N_flag(*(unsigned char*)operand_addr);
	Z_flag(*(unsigned char*)operand_addr);	
}
//ok, only implied mem acc mode
void dex(){
	regx = regx - 1;
	N_flag(regx);
	Z_flag(regx);	
}
//ok, only implied mem acc mode
void dey(){
	regy = regy - 1;
	N_flag(regy);
	Z_flag(regy);	
}
//ok, only implied mem acc mode
void eor(){
	regac = regac ^ operand;
	N_flag(regac);
	Z_flag(regac);		
}
//ok
void inc(){
	*(unsigned char*)operand_addr = operand + 1;
	N_flag(*(unsigned char*)operand_addr);
	Z_flag(*(unsigned char*)operand_addr);	
}
//ok, only implied mem acc mode
void inx(){
	regx = regx + 1;
	N_flag(regx);
	Z_flag(regx);	
}

void iny(){
	regy = regy + 1;
	N_flag(regy);
	Z_flag(regy);	
}

void jmp(){ //if indirect vector fall on a page bounday a bug exists and it needs to be implemented 
    if(addr_mode == addr_mode_abso){
        outval = ((*cas_mem_read(regpc+2))<<8) + *cas_mem_read(regpc+1);
    }else{
	    outval = operand;
    }	
    regpc_d = -1;
    regpc = outval;
    //printf("\njmp to %x\n",regpc);
	//needs
}

void lda(){
	regac = operand;
	N_flag(regac);
	Z_flag(regac);		
}

void ldx(){
	regx = operand;
	N_flag(regx);
	Z_flag(regx);		
}

void ldy(){
	regy = operand;
	N_flag(regy);
	Z_flag(regy);		
}

void lsr(){
	status_r.carry = GET_BIT(operand,0);
	outval = operand>>1;
	N_flag(outval);
	Z_flag(outval);		
	*(unsigned char*)operand_addr = outval;
}

void nop() {
	
}

void ora(){
	regac = regac | operand;
	N_flag(regac);
	Z_flag(regac);			
}

void pha(){
    cas_mem_write(0x100 + regsp, regac);
	regsp--;
}

void php(){
    status_r.brk = 1;
    status_r.unused = 1;
    cas_mem_write(0x100 + regsp, *((unsigned char*)&status_r));
	regsp--;
    status_r.brk = 0;
}

void pla(){
	regsp++;
	regac = *cas_mem_read(0x100 + regsp);
	N_flag(regac);
	Z_flag(regac);		
}

void plp(){
	regsp++;
	*((unsigned char*)&status_r) = *cas_mem_read(0x100 + regsp);
    status_r.unused = 1;
    status_r.brk = 0;
}



void rol(){
	outval = ((operand<<1)|status_r.carry)&0xFF;
	status_r.carry = GET_BIT(operand,7);
	N_flag(outval);
	Z_flag(outval);	
	*(unsigned char*)operand_addr = outval;
}

void ror(){
	outval = ((operand>>1)|(status_r.carry<<7))&0xFF;
	status_r.carry = GET_BIT(operand,0);
	N_flag(outval);
	Z_flag(outval);	
	*(unsigned char*)operand_addr = outval;
}

void sbc(){

    
	outval =  regac + ((operand^0xFF) + status_r.carry);
	N_flag(outval);
	Z_flag(outval);	
	status_r.overflow = ((outval ^ regac) & (outval ^ (operand^0xFF))&0x80) != 0;
	status_r.carry = ((outval & 0xFF00) != 0x0000);
	regac = outval;
}

void sec(){
	status_r.carry = 1;
}

void sed(){ //artihmatic functions needs to implemented accordingly for decimal mode
	status_r.decimal = 1;

}

void sei(){
	status_r.interrupt = 1;
}

void sta(){
	*(unsigned char*)operand_addr = regac;
}

void stx(){
	*(unsigned char*)operand_addr = regx;
}

void sty(){
	*(unsigned char*)operand_addr = regy;
}

void tax(){
	regx = regac; 
	N_flag(regx);
	Z_flag(regx);	
}

void tay(){
	regy = regac;
	N_flag(regy);
	Z_flag(regy);	
}

void tsx(){
	regx = regsp;
	N_flag(regx);
	Z_flag(regx);	
}

void txa(){
	regac = regx;
	N_flag(regac);
	Z_flag(regac);		
}
void txs(){
	regsp = regx;
   	//N_flag(regsp);
	//Z_flag(regsp);		
}

void tya(){
	regac = regy;
	N_flag(regac);
	Z_flag(regac);		
}

void jsr(){
    //printf("d jsr sp: %d",regsp);
	cas_mem_write(0x100 + regsp,((regpc + 2) & 0xFF00)>>8);
	regsp--;
	cas_mem_write(0x100 + regsp,((regpc + 2) & 0x00FF));
	regsp--;
	regpc = ((*cas_mem_read(regpc+2))<<8) + *cas_mem_read(regpc+1);
    //printf("d jsr sp: %d, regpc %hx",regsp, regpc);
    regpc_d = -1;
}


void irq(){
	if(status_r.interrupt == 0){
        status_r.brk = 0;
        status_r.unused = 1;
		cas_mem_write(0x100 + regsp, (regpc & 0xFF00)>>8);
		regsp--;
		cas_mem_write(0x100 + regsp, (regpc & 0x00FF));
		regsp--;	
		cas_mem_write(0x100 + regsp, *((unsigned char*)&status_r));
		regsp--;
		regpc = ((*cas_mem_read(0xFFFF))<<8) + *cas_mem_read(0xFFFE);
        i_mask_pre = status_r.interrupt;         
        status_r.interrupt = 1;
        
	}
}

void nmi(){
    status_r.unused = 1;
    status_r.brk = 0;
	cas_mem_write(0x100 + regsp, (regpc & 0xFF00)>>8);
	regsp--;
	cas_mem_write(0x100 + regsp, (regpc & 0x00FF));
	regsp--;	
	cas_mem_write(0x100 + regsp, *((unsigned char*)&status_r));
	regsp--;
	regpc = ((*cas_mem_read(0xFFFB))<<8) + *cas_mem_read(0xFFFA);
    i_mask_pre = status_r.interrupt;
    status_r.interrupt = 1;
}

void reset(){
	regac = 0x00;
	regx = 0x00;
	regsp = 0xFD;
	operand = 0;
	operand_addr = NULL;
	regpc = ((*cas_mem_read(0xFFFD))<<8) + *cas_mem_read(0xFFFC);
	
}

void rts(){
	regpc = *cas_mem_read(0x100 + regsp + 1) + ((*cas_mem_read(0x100 + regsp + 2))<<8);
	regsp += 2;
}

void rti(){
    regsp++;
    *((unsigned char*)&status_r) = *cas_mem_read(0x100 + regsp);
	regpc = *cas_mem_read(0x100 + regsp + 1) + ((*cas_mem_read(0x100 + regsp + 2 ))<<8);
	regsp += 2;
    status_r.interrupt = i_mask_pre;
    regpc_d = -1;
	//*((unsigned char*)&status_r) = address_space[0x100 + regsp];
}

void brk(){

        regpc_d = -1;
        status_r.unused = 1;
        status_r.brk = 1;
    
		cas_mem_write(0x100 + regsp, ((regpc+2) & 0xFF00)>>8);
		regsp--;
		cas_mem_write(0x100 + regsp, ((regpc+2) & 0x00FF));
		regsp--;	
		cas_mem_write(0x100 + regsp, *((unsigned char*)&status_r));
        //printf("BUGBUGBUG pushed to stack %hx to addr %hx \n", address_space[0x100 + regsp], 0x100 + regsp);
		regsp--;
        i_mask_pre = status_r.interrupt; 
        status_r.interrupt = 1;
		regpc = ((*cas_mem_read(0xFFFF))<<8) + *cas_mem_read(0xFFFE);
	
}
//"A:%x X:%x Y:%x PC:%x SP: %x, SR: %x clock:%d\n",regac, regx, regy, regpc, regsp, status_r, index
int cpu_tick(){
	unsigned char opcode;
	if(cycles == 0){
        if(NMI_SIG == true){
            nmi();
            NMI_SIG = false;
        }
		opcode = *cas_mem_read(regpc);
        ////printf("pc:%04x after: opcode: %02x operand %02x A:%02x X:%02x Y:%02x SP: %02x, SR: %02x\n",regpc, opcode, operand,regac, regx, regy, regsp, status_r);
        //printf("pre:   opcode: %x pc:%x operand %x\n",opcode, regpc, operand);
		((void(*)(void))addrtable[opcode])();
		((void(*)(void))optable[opcode])();
		cycles = ticktable[opcode];
        //printf("pc:%04x after: opcode: %02x operand %02x A:%02x X:%04x Y:%02x PC:%04x SP: %02x, SR: %02x\n",regpc, opcode, operand,regac, regx, regy, regpc, regsp, status_r);
		if(is_page_crossed == 1){
			cycles += 1;
		}
		is_page_crossed = 0;
		regpc += 1 + regpc_d;
        regpc_d = 0;
        cycles--;
		return 1;
	}else{
		cycles--;
	}
	return 0;
}

void reset_state(){
    regsp -=3;
    status_r.interrupt = 1;
    regpc = ((*cas_mem_read(0xFFFD))<<8) + *cas_mem_read(0xFFFC);
}

void power_up_state(){
    *((uint8_t*)&status_r) = 0x34;
    regac = 0;
    regx = 0;
    regy = 0;
    regpc = ((*cas_mem_read(0xFFFD))<<8) + *cas_mem_read(0xFFFC);
    printf("\npower up state regpc %x\n",regpc);
}

void illi(){
    printf("\nillegal instruction\n");
    //printf("A:%x X:%x Y:%x PC:%x SP: %x opcode:%x\n",regac, regx, regy, regpc, regsp, address_space[regpc]);
}

/*
int main(){

	memcpy(address_space+0x600, test_rom, sizeof(test_rom));
	address_space[0xFFFD] = 0x06;
	address_space[0xFFFC] = 0x00;
	reset();
	//printf("A:%x X:%x Y:%x PC:%x SP: %x, SR: %x clock:%d\n\n",regac, regx, regy, regpc, regsp, status_r);
	int lim = 99;
	while(lim--){
		if (tick() == 1){
			//printf("A:%x X:%x Y:%x PC:%x SP: %x, SR: %x clock:%d\n\n",regac, regx, regy, regpc, regsp, status_r);
		}
	}
	return 0;
}
*/
