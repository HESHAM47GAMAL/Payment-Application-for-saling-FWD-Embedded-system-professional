
#include "app.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"
typedef unsigned char uint8_t;


/**************************************
 *        variable initialized
**************************************/
long int transactionNum=0;
	//variables for card
	ST_cardData_t takenDataCard;
	uint8_t cardMessage ;

	//variables for terminal
	ST_terminalData_t terminalData;

	uint8_t terminalMessage ;

	//variable for server


	ST_transaction_t serverData;
	uint8_t serverMessage;


int main()
{




	printf("Welcome to Payment application \n-------------------------------\n\n");
	fflush(stdout);
	while(1)
	{
		appStart();
	}




}



void appStart(void)
{
	//card operation
	uint8_t try=3;
	uint8_t done=0;
	while(try >0)
	{
		cardMessage = getCardHolderName(&takenDataCard);
		if(cardMessage == OK_CARD)
		{
			printf("OK,holder name taken.\n");
			fflush(stdout);
			done =1;
			if(done)
				break;
		}
		else
		{
			printf("WRONG_NAME ,you have %d tries .\n",try-1);
			fflush(stdout);
			try--;
		}
	}
	if(done)
	{
		try=3;
		done=0;
		while(try >0)
		{
			cardMessage = getCardExpiryDate(&takenDataCard);
			if(cardMessage == OK_CARD)
			{
				printf("OK,expire date taken.\n");
				fflush(stdout);
				done =1;
				if(done)
					break;
			}
			else
			{
				printf("WRONG_EXP_DATE,you have %d tries .\n",try-1);
				fflush(stdout);
				try--;
			}
		}

	}
	if(done)
	{
		try=3;
		done=0;
		while(try >0)
		{
			cardMessage = getCardPAN(&takenDataCard);
			if(cardMessage == OK_CARD)
			{
				printf("OK,PAN taken.\n\n");
				fflush(stdout);
				done =1;
				if(done)
					break;
			}
			else
			{
				printf("WRONG_PAN,you have %d tries .\n",try-1);
				fflush(stdout);
					try--;
			}
		}

	}

	//terminal operation
	terminalData.maxTransAmount =5000;
	if(done)
	{
		terminalMessage = getTransactionDate(&terminalData);
		if(terminalMessage == OK_TERMINAL)
		{
			printf("OK,transaction date taken\n");
			fflush(stdout);
		}
		else
		{
			printf("WRONG_DATE\n");
			fflush(stdout);
			done=0;
		}

		terminalMessage = isCardExpired(&takenDataCard,&terminalData);
		if(terminalMessage == OK_TERMINAL)
		{
			printf("OK,card not expired.\n");
			fflush(stdout);
			terminalMessage = getTransactionAmount(&terminalData);
			if(terminalMessage == OK_TERMINAL)
			{
				printf("OK,transaction amount taken .\n");
				fflush(stdout);
			}
			else
			{
				printf("INVALID_AMOUNT.\n");
				fflush(stdout);
				done=0;
			}

			terminalMessage = isBelowMaxAmount(&terminalData);
			if(terminalMessage == OK_TERMINAL)
			{
				printf("OK,amount not exceed max.\n\n");
				fflush(stdout);
			}
			else
			{
				printf("Declined amount exceeding limit.\n");
				fflush(stdout);
				done=0;
			}
		}
		else
		{
			printf("Declined Expired card.\n");
			fflush(stdout);
			done=0;
		}



	}
	//server operation
	if(done)
	{

		transactionNum++;
		serverData.cardHolderData = takenDataCard;
		serverData.terminalData = terminalData;

		serverMessage = recieveTransactionData(&serverData);
		if( serverMessage == DECLINED_STOLEN_CARD)
		{
			printf("Declined invalid account.\n");
			fflush(stdout);
		}
		else if (serverMessage == DECLINED_INSUFFECIENT_FUND )
		{
			printf("Declined insuffecient fund.\n");
			fflush(stdout);
		}
		else
		{
			printf("ok,transaction completely done.\n");
			fflush(stdout);
		}
	}

printf("\n\n");
fflush(stdout);
	printf("new transaction\n\n");
	fflush(stdout);
}



