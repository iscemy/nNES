#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "6502.c"
#include "disassembler.h"
#include "string.h"
#include "screen.h"
#include <pthread.h> 
#include <time.h>

void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 900 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds); 
} 

int main(int argc, char **argv) { 
    int bytes_read = 0,index = 0, total_readed_byte = 0,lim = 92322648;
    if(argc < 2){
        return -1;
    }
    printf("loading %s file\n", argv[1]);
    FILE *stream = fopen(argv[1],"rb");

    bytes_read = fread(address_space + index*1024,1, 1024, stream);
    while (bytes_read != 0){
        index++;
    //    printf("bytes:%d\n",bytes_read);
        total_readed_byte += bytes_read;
        bytes_read = fread(address_space + index*1024,1, 1024, stream);
    }

    fclose(stream);
    
    //start_disassemble(address_space, 0x400, 0x10);

    printf("total_readed_byte: %d\n",total_readed_byte);
    for(index = 0x400; index < 0x4000; index++){
        if(index%16==0){
        //    printf("\n");
        }
        //printf(" %02x ", address_space[index]);
    }    
    pthread_t thread_id;  
    pthread_create(&thread_id, NULL, screen_thread, NULL); 
    //pthread_join(thread_id, NULL);



    printf("\n");
    status_r.unused = 1;
    regpc = 0x400;
	for(int index = 0; index < lim; index++){
		if (tick() == 1){
			//printf("A:%x X:%x Y:%x PC:%x SP: %x, SR: %x clock:%d\n",regac, regx, regy, regpc, regsp, status_r, index);
            update_cpu_status_texts(regac, regx, regy, regpc, regsp);
            delay(1);
		}
        

	}
    printf("\n %hx Ended\n", regpc);
    return 0;

}

