/*--------------------------------------------------------*/
// stm32gestSpiLIS3DH.h
/*--------------------------------------------------------*/
//	Description :	Gestion par SPI de l'accéléro LIS3DH
//                  sur carte 17400C kit ARM M0
//									Repris de librairie pic32 via i2c
//
//	Auteur 		: 	SCA
//	Version		:	V1.0    30.03.2021
//	Compilateur/environnement	:	uVision 5.33
//
//  Modifications (nom, date, description) :
//
/*--------------------------------------------------------*/

#ifndef stm32gestSpiLIS3DH_H
#define stm32gestSpiLIS3DH_H

#pragma anon_unions

#include <stdint.h>
#include <stdbool.h>

typedef union{
	struct{
		uint8_t RW:1;
		uint8_t MS:1;
		uint8_t adresse:6;
		uint8_t data;
	};
	short tb_trame[8];
}tramespi;

// prototypes des fonctions

void LIS3DH_Init(void);

bool LIS3DH_Write(uint8_t regAddr, uint8_t data);

bool LIS3DH_Read(uint8_t regAddr, uint8_t* data);

bool LIS3DH_ReadAcc(int16_t* pValAcc);

#endif // stm32gestSpiLIS3DH
