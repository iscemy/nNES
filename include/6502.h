#pragma once
#include <stdbool.h>

//move those to a common header
#define GET_BIT(num, c)     ((num&(0x01<<c))>>c)
#define SET_BIT(num, c)     (num=((num)|(0x01<<c)))
#define CLEAR_BIT(num, c)   (num=(num)&(~(0x01<<c)))


/*addr modes*/
void imp();
void acc();
void imm();
void zp();
void zpx();
void zpy();
void rel();
void abso();
void absx();
void absy();
void ind();
void indx();
void indy();

/*instructions*/
void adc();
void and_();
void asl();
void bcc();
void bcs();
void beq();
void bne();
void bmi();
void bpl();
void bvc();
void bvs();
void clc();
void cld();
void cli();
void clv();
void bit();
void cmp();
void cpx();
void cpy();
void dec();
void dex();
void dey();
void eor();
void inc();
void inx();
void iny();
void jmp();
void lda();
void ldx();
void ldy();
void lsr();
void nop();
void ora();
void pha();
void php();
void pla();
void plp();
void rol();
void ror();
void sbc();
void sec();
void sed();
void sei();
void sta();
void stx();
void sty();
void tax();
void tay();
void tsx();
void txa();
void txs();
void tya();
void jsr();
void irq();
void nmi();
void reset();
void rts();
void rti();
void brk();

void illi();






enum{
	addr_mode_imp,
	addr_mode_acc,  //ok
	addr_mode_imm,  //ok
	addr_mode_zp,   //ok
	addr_mode_zpx,  //ok
	addr_mode_zpy,  //ok
	addr_mode_rel,  //ok
	addr_mode_abso, //ok
	addr_mode_absx, //ok
	addr_mode_absy, //ok
	addr_mode_ind,  //ok
	addr_mode_indx, //ok
	addr_mode_indy, //ok
}addr_modes;


//static unsigned char address_space[0x10000];
	void (*addrtable[256]) = {
		/*          |0|   |1|     |2|   |3|     |4|    |5|	  |6|	|7|     |8|	 |9|     |A|   |B|    |C|    |D|   |E|	  |F|         */
		/* 0 */     &imp, &indx,  &imp, &indx,  &zp,   &zp,   &zp,  &zp,  &imp,  &imm,  &acc,  &imm, &abso, &abso, &abso, &abso, /* 0 */
		/* 1 */     &rel, &indy,  &imp, &indy,  &zpx,  &zpx,  &zpx, &zpx, &imp, &absy,  &imp, &absy, &absx, &absx, &absx, &absx, /* 1 */
		/* 2 */    &abso, &indx,  &imp, &indx,  &zp,   &zp,   &zp,  &zp,  &imp,  &imm,  &acc,  &imm, &abso, &abso, &abso, &abso, /* 2 */
		/* 3 */     &rel, &indy,  &imp, &indy,  &zpx,  &zpx,  &zpx, &zpx, &imp, &absy,  &imp, &absy, &absx, &absx, &absx, &absx, /* 3 */
		/* 4 */     &imp, &indx,  &imp, &indx,  &zp,   &zp,   &zp,  &zp,  &imp,  &imm,  &acc,  &imm, &abso, &abso, &abso, &abso, /* 4 */
		/* 5 */     &rel, &indy,  &imp, &indy,  &zpx,  &zpx,  &zpx, &zpx, &imp, &absy,  &imp, &absy, &absx, &absx, &absx, &absx, /* 5 */
		/* 6 */     &imp, &indx,  &imp, &indx,  &zp,   &zp,   &zp,  &zp,  &imp,  &imm,  &acc,  &imm, &ind, &abso, &abso, &abso, /* 6 */
		/* 7 */     &rel, &indy,  &imp, &indy,  &zpx,  &zpx,  &zpx, &zpx, &imp, &absy,  &imp, &absy, &absx, &absx, &absx, &absx, /* 7 */
		/* 8 */     &imm, &indx,  &imm, &indx,  &zp,   &zp,   &zp,  &zp,  &imp,  &imm,  &imp,  &imm, &abso, &abso, &abso, &abso, /* 8 */
		/* 9 */     &rel, &indy,  &imp, &indy,  &zpx,  &zpx,  &zpy, &zpy, &imp, &absy,  &imp, &absy, &absx, &absx, &absy, &absy, /* 9 */
		/* A */     &imm, &indx,  &imm, &indx,  &zp,   &zp,   &zp,  &zp,  &imp,  &imm,  &imp,  &imm, &abso, &abso, &abso, &abso, /* A */
		/* B */     &rel, &indy,  &imp, &indy,  &zpx,  &zpx,  &zpy, &zpy, &imp, &absy,  &imp, &absy, &absx, &absx, &absy, &absy, /* B */
		/* C */     &imm, &indx,  &imm, &indx,  &zp,   &zp,   &zp,  &zp,  &imp,  &imm,  &imp,  &imm, &abso, &abso, &abso, &abso, /* C */
		/* D */     &rel, &indy,  &imp, &indy,  &zpx,  &zpx,  &zpx, &zpx, &imp, &absy,  &imp, &absy, &absx, &absx, &absx, &absx, /* D */
		/* E */     &imm, &indx,  &imm, &indx,  &zp,   &zp,   &zp,  &zp,  &imp,  &imm,  &imp,  &imm, &abso, &abso, &abso, &abso, /* E */
		/* F */     &rel, &indy,  &imp, &indy,  &zpx,  &zpx,  &zpx, &zpx, &imp, &absy,  &imp, &absy, &absx, &absx, &absx, &absx  /* F */
	};																																											   



	void(*optable[256]) = {
		/*			 |0|   |1|   |2|   |3|      |4|  	|5|	   |6|	 |7|  |8|	 |9|     |A|   |B|    |C|    |D|   |E|	  |F|         */
		/* 0 */      &brk, &ora, &illi, &illi,  &illi,  &ora, &asl, &illi, &php,  &ora,  &asl,  &illi, &illi,  &ora, &asl,  &illi,  /* 0 */
		/* 1 */      &bpl, &ora, &illi, &illi,  &illi,  &ora, &asl, &illi, &clc,  &ora,  &illi,  &illi, &illi,  &ora, &asl,  &illi,  /* 1 */
		/* 2 */      &jsr, &and_,&illi, &illi,  &bit,  &and_,&rol, &illi, &plp,  &and_, &rol,  &illi, &bit,  &and_, &rol,  &illi,  /* 2 */
		/* 3 */      &bmi, &and_,&illi, &illi,  &illi,  &and_,&rol, &illi, &sec,  &and_, &illi,  &illi, &illi,  &and_, &rol,  &illi,  /* 3 */
		/* 4 */      &rti, &eor, &illi, &illi,  &illi,  &eor, &lsr, &illi, &pha,  &eor,  &lsr,  &illi, &jmp,  &eor, &lsr,  &illi,  /* 4 */
		/* 5 */      &bvc, &eor, &illi, &illi,  &illi,  &eor, &lsr, &illi, &cli,  &eor,  &illi,  &illi, &illi,  &eor, &lsr,  &illi,  /* 5 */
		/* 6 */      &rts, &adc, &illi, &illi,  &illi,  &adc, &ror, &illi, &pla,  &adc,  &ror,  &illi, &jmp,  &adc, &ror,  &illi,  /* 6 */
		/* 7 */      &bvs, &adc, &illi, &illi,  &illi,  &adc, &ror, &illi, &sei,  &adc,  &illi,  &illi, &illi,  &adc, &ror,  &illi,  /* 7 */
		/* 8 */      &illi, &sta, &illi, &illi,  &sty,  &sta, &stx, &illi, &dey,  &illi,  &txa,  &illi, &sty,  &sta, &stx,  &illi,  /* 8 */
		/* 9 */      &bcc, &sta, &illi, &illi,  &sty,  &sta, &stx, &illi, &tya,  &sta,  &txs,  &illi, &illi,  &sta, &illi,  &illi,  /* 9 */
		/* A */      &ldy, &lda, &ldx, &illi,  &ldy,  &lda, &ldx, &illi, &tay,  &lda,  &tax,  &illi, &ldy,  &lda, &ldx,  &illi,  /* A */
		/* B */      &bcs, &lda, &illi, &illi,  &ldy,  &lda, &ldx, &illi, &clv,  &lda,  &tsx,  &illi, &ldy,  &lda, &ldx,  &illi,  /* B */
		/* C */      &cpy, &cmp, &illi, &illi,  &cpy,  &cmp, &dec, &illi, &iny,  &cmp,  &dex,  &illi, &cpy,  &cmp, &dec,  &illi,  /* C */
		/* D */      &bne, &cmp, &illi, &illi,  &illi,  &cmp, &dec, &illi, &cld,  &cmp,  &illi,  &illi, &illi,  &cmp, &dec,  &illi,  /* D */
		/* E */      &cpx, &sbc, &illi, &illi,  &cpx,  &sbc, &inc, &illi, &inx,  &sbc,  &nop,  &illi, &cpx,  &sbc, &inc,  &illi,  /* E */
		/* F */      &beq, &sbc, &illi, &illi,  &illi,  &sbc, &inc, &illi, &sed,  &sbc,  &illi,  &illi, &illi,  &sbc, &inc,  &illi   /* F */
	};															 

	const unsigned ticktable[256] = {
		/*        |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  A  |  B  |  C  |  D  |  E  |  F  |     */
		/* 0 */      7,    6,    2,    8,    3,    3,    5,    5,    3,    2,    2,    2,    4,    4,    6,    6,  /* 0 */
		/* 1 */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 1 */
		/* 2 */      6,    6,    2,    8,    3,    3,    5,    5,    4,    2,    2,    2,    4,    4,    6,    6,  /* 2 */
		/* 3 */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 3 */
		/* 4 */      6,    6,    2,    8,    3,    3,    5,    5,    3,    2,    2,    2,    3,    4,    6,    6,  /* 4 */
		/* 5 */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 5 */
		/* 6 */      6,    6,    2,    8,    3,    3,    5,    5,    4,    2,    2,    2,    5,    4,    6,    6,  /* 6 */
		/* 7 */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 7 */
		/* 8 */      2,    6,    2,    6,    3,    3,    3,    3,    2,    2,    2,    2,    4,    4,    4,    4,  /* 8 */
		/* 9 */      2,    6,    2,    6,    4,    4,    4,    4,    2,    5,    2,    5,    5,    5,    5,    5,  /* 9 */
		/* A */      2,    6,    2,    6,    3,    3,    3,    3,    2,    2,    2,    2,    4,    4,    4,    4,  /* A */
		/* B */      2,    5,    2,    5,    4,    4,    4,    4,    2,    4,    2,    4,    4,    4,    4,    4,  /* B */
		/* C */      2,    6,    2,    8,    3,    3,    5,    5,    2,    2,    2,    2,    4,    4,    6,    6,  /* C */
		/* D */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* D */
		/* E */      2,    6,    2,    8,    3,    3,    5,    5,    2,    2,    2,    2,    4,    4,    6,    6,  /* E */
		/* F */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7   /* F */
	};
