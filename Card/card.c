#include "card.h"

//This function will ask for the cardholder's name and store it into card data.
EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
	printf("please enter Holder Name\n------------------------- \n");
	fflush(stdout);
//	as gets prototype char *gets(char *str)
	gets((char *)cardData->cardHolderName);
	uint8_t count= getCharacterEntered(cardData->cardHolderName);
	if( (count <20) || (count>24))
		return WRONG_NAME;
	else
		return OK_CARD;
}


//This function will ask for the card expiry date and store it in card data
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
	printf("please enter Card Expire Date \n------------------------------\n");
	fflush(stdout);
	gets((char *)cardData->cardExpirationDate);

	uint8_t count= getCharacterEntered(cardData->cardExpirationDate);
	//will check that seperator between MM and YY is '/'
	// MM value between 00 and 12
	// check length of array = 5
	uint8_t month=cardData->cardExpirationDate[0] - '0';//for following three line try to get month  entered as value
	month *= 10;
	month += cardData->cardExpirationDate[1] - '0';
//	printf("month value : %d \n",month);
//		fflush(stdout);
	if( (cardData->cardExpirationDate[2]!= '/') || (count !=5)  || (month >12) || (month <1))
		return WRONG_EXP_DATE;
	else
		return OK_CARD;
}


//This function will ask for the card's Primary Account Number and store it in card data.
EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
	printf("please enter Card PAN \n-----------------------\n");
	fflush(stdout);
	gets((char *)cardData->primaryAccountNumber);
//	printf("PAN : %s \n",cardData->primaryAccountNumber);
//	fflush(stdout);
	uint8_t count= getCharacterEntered(cardData->primaryAccountNumber);
//	printf("length of string entered :%d\n",count);
//	fflush(stdout);
	if( (count<16) || (count>19))
		return WRONG_PAN;
	else
		return OK_CARD;
}

//function to get number of character entered by user
uint8_t getCharacterEntered(uint8_t *noChracter)
{
	uint8_t count=0;
		while(noChracter[count] != '\0')
		{
			count++;
		}
		return count;
}
