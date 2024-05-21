/*--------------------------------------------------------*/
// stm32gestSpiLIS3DH.c
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

#include "stm32gestSpiLIS3DH.h"
#include "gpio.h"
#include "spi.h" //nécessaire à l'utilisation des fonctions SPI


#define DUMMY 0x81

//Adresses des registres
#define ADDR_STATUS_REG_AUX	0x07
#define ADDR_OUT_ADC1_L		0x08
#define ADDR_OUT_ADC1_H		0x09
#define ADDR_OUT_ADC2_L		0x0A
#define ADDR_OUT_ADC2_H		0x0B
#define ADDR_OUT_ADC3_L		0x0C
#define ADDR_OUT_ADC3_H		0x0D
#define ADDR_WHO_AM_I		0x0F
#define ADDR_CTRL_REG0		0x1E
#define ADDR_TEMP_CFG_REG	0x1F
#define ADDR_CTRL_REG1		0x20
#define ADDR_CTRL_REG2		0x21
#define ADDR_CTRL_REG3		0x22
#define ADDR_CTRL_REG4		0x23
#define ADDR_CTRL_REG5		0x24
#define ADDR_CTRL_REG6		0x25
#define ADDR_REFERENCE		0x26
#define ADDR_STATUS_REG2	0x27
#define ADDR_OUT_X_L		0x28
#define ADDR_OUT_X_H		0x29
#define ADDR_OUT_Y_L		0x2A
#define ADDR_OUT_Y_H		0x2B
#define ADDR_OUT_Z_L		0x2C
#define ADDR_OUT_Z_H		0x2D
#define ADDR_FIFO_CTRL_REG	0x2E
#define ADDR_FIFO_SRC_REG	0x2F
#define ADDR_INT1_CFG		0x30
#define ADDR_INT1_SRC       0x31
#define ADDR_INT1_THS		0x32
#define ADDR_INT1_DURATION	0x33
#define ADDR_INT2_CFG		0x34
#define ADDR_INT2_SRC       0x35
#define ADDR_INT2_THS		0x36
#define ADDR_INT2_DURATION	0x37
#define ADDR_CLICK_CFG		0x38
#define ADDR_CLICK_SRC		0x39
#define ADDR_CLICK_THS		0x3A
#define ADDR_TIME_LIMIT		0x3B
#define ADDR_TIME_LATENCY	0x3C
#define ADDR_TIME_WINDOW	0x3D
#define ADDR_ACT_THS     	0x3C
#define ADDR_ACT_DUR    	0x3D


//Valeur lue par défaut
#define VALDEFAULT_WHO_AM_I 0x33


//Valeurs d'initialisation des registres
#define VALINIT_CTRL_REG1   0x47    // Signification ?
#define VALINIT_CTRL_REG4   0x88    // Signification ?
tramespi trame1;
// write register
void writeRegister8 (uint8_t address, uint8_t data)
{
	static uint8_t value; 
	
	trame1.RW=1;
	trame1.adresse = address;
	trame1.data=data;
	trame1.MS=0;
	value= ((trame1.RW)<<7)|trame1.adresse;
	HAL_SPI_Transmit(&hspi2,&value,8,15);
	HAL_SPI_Transmit(&hspi2,&data,8,15);
}


// ----------------------------------------------------------------
// Initialisation du LIS3DH
// Le SPI2 doit avoir été configuré graphiquement au préalable
// Paramètre(s) : -
// Valeur de retour : -
void LIS3DH_Init(void)  
{

	writeRegister8(ADDR_CTRL_REG1,VALINIT_CTRL_REG1);
	writeRegister8(ADDR_CTRL_REG4,VALINIT_CTRL_REG4);
	
}

// ----------------------------------------------------------------
// Ecriture 1 registre du LIS3DH
// Paramètre(s) : regAddr=adresse du registre / data=donnée à écrire
// Valeur de retour : True si opération ok, false sinon
bool LIS3DH_Write(uint8_t regAddr, uint8_t data)
{

	writeRegister8(regAddr,data);
	
}

// ----------------------------------------------------------------
// Lecture 1 registre du LIS3DH
// Paramètre(s) : regAddr=adresse du registre / *data=donnée lue
// Valeur de retour : True si opération ok, false sinon
bool LIS3DH_Read(uint8_t regAddr, uint8_t* data)
{
	trame1.RW=0;
	trame1.adresse = regAddr;
	static uint8_t value;
	value= ((trame1.RW)<<7)|trame1.adresse;
	HAL_SPI_Transmit(&hspi2,&value,8,15);
	HAL_SPI_Receive(&hspi2,&value,8,15);
	trame1.data=value;
} 

// ----------------------------------------------------------------
// Lecture accélérations x-y-z
// Paramètre(s) : *pValAcc=tableau de 3 int16_t pour valeurs lues
// Valeur de retour : True si opération ok, false sinon
bool LIS3DH_ReadAcc(int16_t* pValAcc)
{
	static uint8_t value;
	// A compléter...
	
	LIS3DH_Read(ADDR_OUT_X_L,&value);
	pValAcc[XVALUE] = value;
	LIS3DH_Read(ADDR_OUT_X_H,&value);
	pValAcc[XVALUE]=(pValAcc[XVALUE] | (value<<8)); 
	
	LIS3DH_Read(ADDR_OUT_Y_L,&value);
	pValAcc[YVALUE] = value;
	LIS3DH_Read(ADDR_OUT_Y_H,&value);
	pValAcc[YVALUE]=(pValAcc[YVALUE] | (value<<8)); 
	
	LIS3DH_Read(ADDR_OUT_Z_L,&value);
	pValAcc[ZVALUE] = value;
	LIS3DH_Read(ADDR_OUT_Z_H,&value);
	pValAcc[ZVALUE]=(pValAcc[ZVALUE] | (value<<8)); 
	
}
