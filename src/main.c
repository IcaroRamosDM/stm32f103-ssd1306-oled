#include "bsp/clock.h"
#include "bsp/systick.h"

#define MAIN_LOOP_DELAY_MS      1000U

int main(void) {
    clock_setup();
    systick_setup();

    while (1) {
        
      delay_ms(MAIN_LOOP_DELAY_MS);
    }

    return 0;
}
