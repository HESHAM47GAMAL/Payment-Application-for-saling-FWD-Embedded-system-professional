#include "terminal.h"
#include<time.h>
//This function will ask for the transaction data and store it in terminal data.
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
	time_t t;
	t = time(NULL);//function return calendar time
	struct tm tm = *localtime(&t); //localtime :function take pionter to calendar time and set value for my time zone
//	printf("Current Date: %d/%d/%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
//	fflush(stdout);
	uint8_t day = tm.tm_mday;
	uint8_t month = tm.tm_mon+1;
	uint16_t year = tm.tm_year+1900;

		//operation to store day as character
		if(tm.tm_mday >9)
		{
			termData->transactionDate[1]= (day%10) + '0';
			day/=10;
			termData->transactionDate[0]= (day%10) + '0';
		}
		else
		{
			termData->transactionDate[0]=0 + '0';
			termData->transactionDate[1]= (day%10) + '0';
		}
		termData->transactionDate[2]='/';
		//operation to store month as character
		if(tm.tm_mon >9)
		{
			termData->transactionDate[4]= (month%10) + '0';
			month/=10;
			termData->transactionDate[3]= (month%10) + '0';
		}
		else
		{
			termData->transactionDate[3]=0 + '0';
			termData->transactionDate[4]= (month%10) + '0';
		}
		termData->transactionDate[5]='/';
		//operation to store year as character
		for(uint8_t index=9; index>=6 ;index--)
		{
			termData->transactionDate[index]= (year%10) + '0';
			year/=10;
		}
		termData->transactionDate[10]='\0';
//		printf("\ndate in character form = %s\n",termData->transactionDate);
		uint8_t count = getCharacterEntered(termData->transactionDate);
		if( (count<10) || (termData->transactionDate[2] !='/') || (termData->transactionDate[5] !='/'))
		{
			return WRONG_DATE;
		}
		else
			return OK_TERMINAL;
}

//This function compares the card expiry date with the transaction date.
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
	// 21/7/2022
	// 08/22
	uint8_t expDate=0;
	uint8_t transDate=0;

	// convert year characters to numeric value for expire date & transition date

	expDate = cardData->cardExpirationDate[3]-'0';
	expDate *=10;
	expDate += cardData->cardExpirationDate[4]-'0';

	transDate = termData->transactionDate[8]-'0';
	transDate *=10;
	transDate += termData->transactionDate[9]-'0';


//	printf("expire date = %d , transition data = %d\n",expDate,transDate);
//		fflush(stdout);
	if(transDate > expDate)
	{
		return EXPIRED_CARD;
	}
	else if (transDate < expDate)
	{
		return OK_TERMINAL;
	}
	// convert month characters to numeric value for expire date & transition date
	else if(transDate == expDate)
	{
		expDate=cardData->cardExpirationDate[0]-'0';
		expDate *=10;
		expDate +=cardData->cardExpirationDate[1]-'0';

		transDate=termData->transactionDate[3]-'0';
		transDate *=10;
		transDate +=termData->transactionDate[4]-'0';

//		printf("expire date = %d , transition data = %d\n",expDate,transDate);
//		fflush(stdout);
		if(transDate > expDate)
		{
			return EXPIRED_CARD;
		}
//		else if (transDate <= expDate)
//		{
//			return OK_TERMINAL;
//		}

	}
	return OK_TERMINAL;

}

//This function asks for the transaction amount and saves it into terminal data
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
	printf("please enter transaction amount \n-------------------------------\n");
	fflush(stdout);
	scanf("%f",&termData->transAmount);

	if(termData->transAmount <1)
		return INVALID_AMOUNT;
	else
		return OK_TERMINAL;

}

//This function compares the transaction amount with the terminal max amount.
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
//	printf("max trans = %f  ,trans = %f\n",termData->maxTransAmount,termData->transAmount);
	if( (termData->transAmount) > (termData->maxTransAmount))
		return EXCEED_MAX_AMOUNT;
	else
		return OK_TERMINAL;
}

//This function sets the maximum allowed amount into terminal data.
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData)
{
//	printf("please enter new max amount during transition\n");
//	fflush(stdout);
	scanf("%f",&termData->maxTransAmount);
	printf("new max :%f\n",termData->maxTransAmount);
	if(termData->maxTransAmount < 1)
		return INVALID_MAX_AMOUNT;
	else
		return OK_TERMINAL;
}


