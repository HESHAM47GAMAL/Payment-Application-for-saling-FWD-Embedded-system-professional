
#ifndef SERVER_SERVER_H_
#define SERVER_SERVER_H_
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include "../Card/card.h"
#include "../Terminal/terminal.h"
/**************************************
 *         typedefs
**************************************/
typedef unsigned char uint8_t;
typedef unsigned long int uint32_t;

typedef enum EN_transState_t
{
APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR
}EN_transState_t;

typedef struct ST_transaction_t
{
ST_cardData_t cardHolderData;
ST_terminalData_t terminalData;
EN_transState_t transState;
uint32_t transactionSequenceNumber;
}ST_transaction_t;



typedef enum EN_serverError_t
{
OK_SERVER, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE
}EN_serverError_t ;

typedef struct ST_accountsDB_t
{
float balance;
uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

///struct by me to make control throw text file easy is like "ST_accountsDB_t"  but will help me add new account

typedef struct ST_accountsDBText_t
{
uint8_t balance[11];
uint8_t primaryAccountNumber[20];
char x;
}ST_accountsDBText_t;

///struct by me to make control throw text file that it store transaction in text file but it store only character in correct way
typedef struct ST_transactionText_t
{
ST_cardData_t cardHolderData;
ST_terminalDataText_t terminalData;
uint8_t transState[2];
uint8_t transactionSequenceNumber[10];
uint8_t endline;
}ST_transactionText_t;

/**************************************
 *         prototypes
**************************************/
EN_transState_t recieveTransactionData(ST_transaction_t *transData);
EN_serverError_t isValidAccount(ST_cardData_t *cardData);
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData);
EN_serverError_t saveTransaction(ST_transaction_t *transData);
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData);
void  server_side_accounts_database(void);
void  server_side_transactions_database(void);
void intTochar(char *x , int y);


char stringCMP(char *x,char *y);
#endif /* SERVER_SERVER_H_ */
