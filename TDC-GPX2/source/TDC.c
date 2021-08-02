/*
 * TDC.c
 *
 *  Created on: 24 апр. 2019 г.
 *      Author: zzzro
 */

#include "SPI.h"
#include "TDC.h"
#include "GPIO.h"
#include "macro.h"
#include "stm32f1xx.h"
#include "delay.h"

#define TDC_SPI SPI_2
#define TDC_NSS PA8

// *** Configuration Registers ***
// ---------------------------------------------------------------------------------------------
const uint8_t config_register[17] = { 0x81, 0x81, 0x00, 0xA0, 0x86, 0x01, 0xC0, 0xC3,
								   0xA1, 0x13, 0x00, 0x0A, 0xCC, 0x05, 0x05, 0x8E, 0x04};
// A typical config settings = { config00, config01, … , config16 }


// *** SPI Opcodes ***
// ---------------------------------------------------------------------------------------------
const uint8_t spiopc_power = 0x30; // opcode for "Power on Reset"
const uint8_t spiopc_init = 0x18; // opcode for "Initialize Chip and Start Measurement"
const uint8_t spiopc_write_config = 0x80; // opcode for "Write Configuration"
const uint8_t spiopc_read_config = 0x40; // opcode for "Read Configuration"
const uint8_t spiopc_read_results = 0x60; // opcode for "Read Measurement Results"
// ---------------------------------------------------------------------------------------------
// *** SPI Addresses ***
// ---------------------------------------------------------------------------------------------
const uint8_t reference_index_ch1_byte3 = 8; //
const uint8_t reference_index_ch1_byte2 = 9;
const uint8_t reference_index_ch1_byte1 = 10;
const uint8_t stopresult_ch1_byte3 = 11;
const uint8_t stopresult_ch1_byte2 = 12;
const uint8_t stopresult_ch1_byte1 = 13;
// . . . .
const uint8_t stopresult_ch4_byte3 = 29;
const uint8_t stopresult_ch4_byte2 = 30;
const uint8_t stopresult_ch4_byte1 = 31;


const uint8_t BytesPerMeasure = 6;
const uint8_t AverageMeasure = 130;

uint64_t stopresult = 0;
uint32_t reference_index = 0;
uint8_t Buffer = 0;


void SSN_Pulse(){
	gpio_state(TDC_NSS, HIGH);
	gpio_state(TDC_NSS, LOW);
}
void Get_Results_Aver(uint8_t *resultArray){
	uint8_t indexArray[780] = {0};
	uint8_t stopArray[780] = {0};
	uint8_t first[130];
	uint8_t second[130];
	uint8_t third[130];
	uint8_t i = 0;
	uint8_t j = 0;
	for(i = 0; i < AverageMeasure; i++){
		SSN_Pulse();
		spi_send8(TDC_SPI, spiopc_read_results + reference_index_ch1_byte3);
		for(; j < 3; j++){
			indexArray[j] = spi_waiting_read8(TDC_SPI); // read one byte from SPI to Buffer variable
		}
		for(; j < 3; j++){
			stopArray[j] = spi_waiting_read8(TDC_SPI); // read one byte from SPI to Buffer variable
		}
	}
	for(uint8_t i = 0; i < BytesPerMeasure * BytesPerMeasure; i += 6){
		for(uint8_t j = 0; j < 6; j++){

		}
	}
	for(uint16_t i = 0; i < BytesPerMeasure * BytesPerMeasure; i += 3){
		if(indexArray[i + 1] != 0xFF && indexArray[i + 2] != 0xFF){
			reference_index += (indexArray[i] << 16) + (indexArray[i + 1] << 8) + (indexArray[i + 2]);
			stopresult += (stopArray[i] << 16) + (stopArray[i + 1] << 8) + stopArray[i + 2];
		}
	}
	reference_index /= AverageMeasure;
	stopresult /= AverageMeasure;
	//if()
	resultArray[0] = reference_index >> 16;
	resultArray[1] = reference_index >> 8;
	resultArray[2] = reference_index;
	resultArray[3] = stopresult >> 16;
	resultArray[4] = stopresult >> 8;
	resultArray[5] = stopresult;
}
/*void CheckBuffer(uint8_t *arr, uint8_t *buffer){
	uint16 sum1 = 0;
	uint16 sum2 = 0;
	uint16 sum3 = 0;
	uint16 sum4 = 0;
	uint8_t iterator = 0;
	for(uint8_t j = 0; j < 6; j++){
		sum1 += arr[j];
	}
	for(uint8_t j = 7; j < 12; j++){
		sum2 += arr[j];
	}
	for(uint8_t j = 12; j < 18; j++){
		sum3 += arr[j];
	}
	for(uint8_t j = 18; j < 24; j++){
		sum4 += arr[j];
	}

	if(sum1 < 1000){
		for(uint8_t j = 0; j < 6; j++){
			buffer[j] += arr[j];
		}
	}
}
*/
void Get_Results(uint8_t *arr){ //return filled array (1 channel)
	uint8_t buffer[18] = {0};

	SSN_Pulse();
//	Delay(1);
	spi_send8(TDC_SPI, spiopc_read_results + reference_index_ch1_byte3);
	for(uint8_t j = 0; j < 6; j++){
		arr[j] = spi_waiting_read8(TDC_SPI);// read one byte from SPI to Buffer variable
	}

	SSN_Pulse();
//	Delay(1);
	spi_send8(TDC_SPI, spiopc_read_results + reference_index_ch1_byte3);
	for(uint8_t j = 0; j < 6; j++){
		arr[j+6] = spi_waiting_read8(TDC_SPI);// read one byte from SPI to Buffer variable
	}

	SSN_Pulse();
//	Delay(1);
	spi_send8(TDC_SPI, spiopc_read_results + reference_index_ch1_byte3);
	for(uint8_t j = 0; j < 6; j++){
		arr[j+12] = spi_waiting_read8(TDC_SPI); // read one byte from SPI to Buffer variable
	}
	//for(uint8_t j = 0; j < 13; j++){
		//SSN_Pulse();
		//SPI_SEND8( spiopc_read_results + reference_index_ch1_byte3);
	//}

}
void TDC_Results(){ //test function for check workability
	SSN_Pulse();
	spi_send8(TDC_SPI, spiopc_read_results + reference_index_ch1_byte3);
	for (uint8_t i = 0; i < BytesPerMeasure; i++){
		Buffer = spi_waiting_read8(TDC_SPI); // read one byte from SPI to Buffer variable
	}
	/*
	SPI_SEND8( spiopc_read_results + reference_index_ch1_byte3 );
	// Opcode for "Read Result" and data address are sent

	SPI1_Read8(Buffer); // read one byte from SPI to Buffer
	reference_index = reference_index // Data is shifted 16 Bits to the left
						+ ( Buffer << 16 ); // and added to the reference_index

	SPI1_Read8( Buffer ); // read one byte from SPI to Buffer
	reference_index = reference_index // Data is shifted 8 Bits to the left
						 + ( Buffer << 8 ); // and added to the reference_index

	SPI1_Read8( Buffer ); // read one byte from SPI to Buffer
	reference_index = reference_index // Data is directly added to reference_index
						+ Buffer;

	SPI1_Read8( Buffer ); // Same process as reference_index
	stopresult = stopresult // is repeated for stop results
	+ ( Buffer << 16 );

	SPI1_Read8( Buffer );
	stopresult = stopresult + ( Buffer << 8 );

	SPI1_Read8( Buffer );
	stopresult = stopresult + Buffer;
	*/
}
void TDC_Init(){
	SSN_Pulse();
	spi_send8(TDC_SPI, spiopc_power); // Opcode for "Power On Reset" is sent over SPI
	 // -----------------------------------------------------------------------------------------
	 // *** Writing the configuration registers ***
	 // -----------------------------------------------------------------------------------------
	SSN_Pulse(); // Reset the SPI interface and select the slave device

	spi_send8(TDC_SPI, spiopc_write_config); // Opcode for "Write Configuration"
	// and config address (00) are sent over SPI
	for (uint8_t i = 0; i < 17; i++){ // Send all 17 config registers via SPI
		spi_send8(TDC_SPI, config_register[i] );
	}
	// ------

	SSN_Pulse();
	spi_send8(TDC_SPI, spiopc_read_config); // Opcode for "Read Configuration"

	for (uint8_t i = 0; i < 17; i++){
		Buffer = spi_waiting_read8(TDC_SPI); // read one byte from SPI to Buffer variable
	}
	// and config address (00) are sent over SPI

	SSN_Pulse();
	spi_send8(TDC_SPI, spiopc_init);
	Delay(100);

}
