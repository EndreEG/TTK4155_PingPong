#include "utilities.h"


void XMEM_init() 
{
	//enables external memory
	set_bit(MCUCR,SRE);

	//releases PA4 - PA7
	set_bit(SFIOR, XMM2);
	clear_bit(SFIOR,XMM1);
	clear_bit(SFIOR,XMM0);
	
}

void ADC_init()
{
	// writes the desired top value

	//configures PD5 as output and PD4 as input
	set_bit(DDRD,DDD5);
	clear_bit(DDRD,DDD4);
	clear_bit(DDRB, DDB0);
	clear_bit(DDRB, DDB1);
	
	//set fast PWM mode, choose ICR1 as TOP
	set_bit(TCCR1B,WGM13);
	set_bit(TCCR1B,WGM12);
	set_bit(TCCR1A,WGM11);
	clear_bit(TCCR1A,WGM10);
		
	// sets TOP value
	ICR1 = 3;

	// sets compare match value
	OCR1A = 1;

	// clear on compare match, set on TOP reached
	set_bit(TCCR1A,COM1A1);
	clear_bit(TCCR1A,COM1A0);

	// sets I/O clock as frequency, with no prescaler
	set_bit(TCCR1B,CS10);
	clear_bit(TCCR1B,CS11);
	clear_bit(TCCR1B,CS12);
}

void ADC_get(uint8_t* ADC_values) {
	volatile char *ADC_BASE = (char *) 0x1400; // Start address for the ADC
	ADC_BASE[0] = 0;
	
	loop_until_bit_is_set(PIND, PIND4);
	
	//gets values
	//consecutive rd pulses reads new registers
	for (uint8_t i = 0; i < 4; i++)
	{
		uint8_t value = ADC_BASE[0];
		ADC_values[i] = value;
	}
	
	ADC_values[4] = test_bit(PINB, PINB0);
	ADC_values[5] = test_bit(PINB, PINB1);
}

// void sram_test(void)
//     {
// 		volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
//         uint16_t ext_ram_size = 0x800;
//         uint16_t write_errors = 0;
//         uint16_t retrieval_errors = 0;
//         printf("Starting SRAM test...\n\r");
//         // rand() stores some internal state, so calling this function in a loop will
//         // yield different seeds each time (unless srand() is called before this function)
//         uint16_t seed = rand();
//         // Write phase: Immediately check that the correct value was stored
//         srand(seed);
//         for (uint16_t i = 0; i < ext_ram_size; i++) {
//             uint8_t some_value = rand();
//             ext_ram[i] = some_value;
//             uint8_t retreived_value = ext_ram[i];
// 			//printf("%4u \r\n",some_value);
//             if (retreived_value != some_value) {
//                 //printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
//                 write_errors++;
//             }
//         }
//         // Retrieval phase: Check that no values were changed during or after the write phase
//         srand(seed);
//         // reset the PRNG to the state it had before the write phase
//         for (uint16_t i = 0; i < ext_ram_size; i++) {
//             uint8_t some_value = rand();
//             uint8_t retreived_value = ext_ram[i];
//             if (retreived_value != some_value) {
//                 //printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
//                 retrieval_errors++;
//             }
//         }
//         printf("SRAM test completed with \r\n%4d errors in write phase and \r\n%4d errors in retrieval phase\n\r\n", write_errors, retrieval_errors);
// }
