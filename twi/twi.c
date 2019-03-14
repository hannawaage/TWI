#include <stdint.h>
#include "twi.h"
#include "gpio.h"



void twi_init(){
	GPIO->PIN_CNF[SCL_PIN] = (1 << 2| 6 << 8);
    GPIO->PIN_CNF[SDA_PIN] = (1 << 2| 6 << 8);
	TWI0->PSELSCL = SCL_PIN; 
    TWI0->PSELSDA = SDA_PIN; 
    TWI0->FREQUENCY = 0x01980000;
    TWI0->ENABLE = 5; 

}

void twi_multi_read(
		uint8_t slave_address,
		uint8_t start_register,
		int registers_to_read,
		uint8_t * data_buffer
		)
	{
	
	TWI0->ADDRESS = slave_address;
    TWI0->STARTTX = 1;
    TWI0->TXDSENT = 0;
    TWI0->TXD = start_register;

    while(!TWI0->TXDSENT);

	int i = 0;
    for(i; i < 10; i++){
        __asm("nop");
    }

    TWI0->RXDREADY = 0;
	TWI0->STARTRX = 1;

	for(i = 0; i < registers_to_read-1; i++)
	{
		while(!TWI0->RXDREADY);

		TWI0->RXDREADY = 0;

		data_buffer[i]=TWI0->RXD;
	}

    TWI0->STOP = 1; 
	while(!TWI0->RXDREADY);
    data_buffer[registers_to_read-1] = TWI0->RXD;


}

void twi_multi_write(
		uint8_t slave_address,
		uint8_t start_register,
		int registers_to_write,
		uint8_t * data_buffer
		){
	TWI0->ADDRESS = slave_address;
	TWI0->STARTTX = 1;

	TWI0->TXDSENT = 0;
	TWI0->TXD = start_register;
	while(!TWI0->TXDSENT);
	int i = 0;
	for(i; i < registers_to_write; i++){
		TWI0->TXDSENT = 0;
		TWI0->TXD = data_buffer[i];
		while(!TWI0->TXDSENT);
	}

	TWI0->STOP = 0;
}
