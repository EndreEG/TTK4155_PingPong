#include "sram_test.h"

    void SRAM_test(void)
    {
        volatile char *SRAM = (char *) 0x1800;
        volatile char *ADC = (char *) 0x1400;
        while(1) {
            uint16_t i = 0;
            while (i < 100000)
            {
                i++;
            }
            i = 0;
            SRAM[0] = 15;
            printf("Sram høy nå\n\r");
            while (i < 100000) {
                i++;
            }  
            i = 0;      
            ADC[0] = 15;        
            printf("ADC høy nå\n\r");
        }
}