#include <stdio.h>

void memdump(char *mem, int from, int to){
    printf("from %04x to %04x \n",from,to);
    for(int index = from; index < to; index++){
        if(index%36==0){
            printf("\n");
        }
        printf(" %02x ", mem[index]&0xff);
    }    

}
