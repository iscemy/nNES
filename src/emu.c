#include <nes2_loader.h>
#include <cpu.h>

//#include <6502.h>


int main(){
    load_nes2("donkey.nes");
    power_up_state();
    while(1){
        tick();
    }
}
