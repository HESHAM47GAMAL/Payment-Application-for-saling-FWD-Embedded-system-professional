
#ifndef TERMINAL_TERMINAL_H_
#define TERMINAL_TERMINAL_H_
#include "stdio.h"
#include "../Card/card.h"
/**************************************
 *         typedefs
**************************************/
typedef unsigned char uint8_t;

typedef struct ST_terminalData_t
{
float transAmount;
float maxTransAmount;
uint8_t transactionDate[11];
}ST_terminalData_t;
typedef struct ST_terminalDataText_t
{
uint8_t transAmount[7];
uint8_t maxTransAmount[7];
uint8_t transactionDate[11];
}ST_terminalDataText_t;

typedef enum EN_terminalError_t
{
OK_TERMINAL, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}EN_terminalError_t ;



/**************************************
 *         prototypes
**************************************/
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData);
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData);
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData);

#endif /* TERMINAL_TERMINAL_H_ */
