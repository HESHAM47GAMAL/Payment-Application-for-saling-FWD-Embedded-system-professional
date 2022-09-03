
#ifndef CARD_CARD_H_
#define CARD_CARD_H_
#include <stdio.h>

/**************************************
 *         typedefs
**************************************/
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;

typedef struct ST_cardData_t
{
uint8_t cardHolderName[25];// 24 char + NULL
uint8_t primaryAccountNumber[20];
uint8_t cardExpirationDate[6];
}ST_cardData_t;

typedef enum EN_cardError_t
{
OK_CARD, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

/**************************************
 *         prototypes
**************************************/
EN_cardError_t getCardHolderName(ST_cardData_t *cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);
EN_cardError_t getCardPAN(ST_cardData_t *cardData);
uint8_t getCharacterEntered(uint8_t *noChracter);


#endif /* CARD_CARD_H_ */
