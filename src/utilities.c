#include <stdio.h>

void memdump(char *mem, int from, int to){
    printf("from %04x to %04x \n",from,to);
    for(int index = from; index < to; index++){
        if(index%32==0){
            printf("\n");
        }
        printf(" %02x ", mem[index]&0xff);
    }    

}

void dump_pattern_table(unsigned char *pattern_table_addr){
    for(int pattern_ind = 0x00; pattern_ind < 0x100; pattern_ind++){
        printf("\npattern %d\n", pattern_ind);
        for(int i = 0; i < 8; i++){
            unsigned char ptand = (*(pattern_table_addr + i)) & (*(pattern_table_addr + i + 8));
            unsigned char ptxor  = (*(pattern_table_addr + i)) ^ (*(pattern_table_addr + i + 8));
            for(int j = 7; j >= 0; j--){
                 printf("%d",((ptand&(1<<j))>>j)*2 + ((ptxor&(1<<j))>>j));
            }
            printf(" %02x %02x\n", *(pattern_table_addr + i) ,*(pattern_table_addr + i + 8));
        }
        pattern_table_addr += 0x10;
    }
    
    printf("\n");
}

void dump_name_table(char *mem, int from, int to){
    printf("from %04x to %04x \n",from,to);
    for(int index = from; index < to; index++){
        if(index%32==0){
            printf("\n");
        }
        printf(" %02x ", mem[index]&0xff);
    }    

}

