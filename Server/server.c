#include "server.h"
extern long int transactionNum;
FILE *textFileAccessAccounts=NULL;//contain address of file that have account data
FILE *textFileAccesstransactions=NULL;
//#define MAX_LINE 200
//char Line[MAX_LINE];
ST_accountsDBText_t accessTextFile; //data type defined by me to helpmein accessing text file for accounts data base
ST_transactionText_t accessTextFileTransaction;
unsigned short int lineOfAccount;
void  server_side_accounts_database(void)
{
	FILE * textFile;
	textFile = fopen("E:/3rd/fwd/eclipse/Payment_Application/Server/Accounts_DB.txt","r+");//read this text file
	 if (textFile == NULL)
	  {
		printf("Error opening file.\n");
		fflush(stdout);
	  }
	 else
		 textFileAccessAccounts= textFile;
}

void server_side_transactions_database(void)
{
	FILE * textFile;
	textFile = fopen("E:/3rd/fwd/eclipse/Payment_Application/Server/Transactions_DB.txt","a");//read this text file
	 if (textFile == NULL)
	  {
		printf("Error opening file.\n");
		fflush(stdout);

	  }
	 else
		 textFileAccesstransactions = textFile;
}


//This function will take all transaction data and validate its data.
EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
	server_side_accounts_database();
	server_side_transactions_database();
	uint8_t returnedState =APPROVED;
	uint8_t serverMessage;
	serverMessage = isValidAccount(&transData->cardHolderData);
	if(serverMessage == DECLINED_STOLEN_CARD)
		returnedState  = DECLINED_STOLEN_CARD;
	if(returnedState != DECLINED_STOLEN_CARD ) //should end process and not need to do last function
	{
		serverMessage = isAmountAvailable(&transData->terminalData);

		if(serverMessage == LOW_BALANCE)
		{
			returnedState = DECLINED_INSUFFECIENT_FUND;
		}


		else //at this state user has  balance make him/her approve operation so should update balance
		{

			fseek(textFileAccessAccounts,0,SEEK_SET);//to go to begin of file
			FILE*temp; //temp to help update account_DB.txt (update balance)
			temp = fopen("E:/3rd/fwd/eclipse/Payment_Application/Server/Accounts_DBTemp.txt", "r+");
			if (temp == NULL) {
				printf("Error opening file.\n");
						fflush(stdout);
			}
			char lineChange=0;
//			printf("i will update balance line %d\n",lineOfAccount);
//			fflush(stdout);
		//	ST_accountsDBText_t updateAccountData;//
			while(fread(&accessTextFile, sizeof(ST_accountsDBText_t),
									1, textFileAccessAccounts))
			{
				lineChange++;
				if(lineChange == lineOfAccount)
				{
					int value=0;
					char iterate=0;
					while( (accessTextFile.balance[iterate]) && (accessTextFile.balance[iterate] != 32 ))
					{
//						printf("value for each char %d\n",accessTextFile.balance[iterate]);
						value += accessTextFile.balance[iterate]-'0';
//						printf("value at each iterate = %d\n",value);
						value *=10;
						iterate++;
					}
					value /= 10;
//					printf("value = %d\n",value);
//					printf("transamount =%f\n",transData->terminalData.transAmount);
					value =value- transData->terminalData.transAmount;
//					printf("new value of balance %d\n",value);
//					fflush(stdout);
					strcpy(&accessTextFile.balance,"          ");//as during update balance can't clear array of character then put new value
					intTochar(&accessTextFile.balance,value);

					fwrite(&accessTextFile, sizeof(ST_accountsDBText_t),
											1, temp);
				}
				else
				fwrite(&accessTextFile, sizeof(ST_accountsDBText_t),
									1, temp);

			}

			fseek(textFileAccessAccounts,0,SEEK_SET);//to go to begin of file
			fseek(temp,0,SEEK_SET);//to go to begin of file
			while(fread(&accessTextFile, sizeof(ST_accountsDBText_t),
										1, temp))
			{
				fwrite(&accessTextFile, sizeof(ST_accountsDBText_t),
											1, textFileAccessAccounts);
			}

			fclose(textFileAccessAccounts);
			fclose(temp);
		}
	}


	transData->transState = returnedState;
	transData->transactionSequenceNumber=transactionNum;///->>>>update transaction date
	serverMessage = saveTransaction(transData);
	fclose(textFileAccesstransactions);
	if(serverMessage == SAVING_FAILED)
			returnedState = INTERNAL_SERVER_ERROR;

	return returnedState;
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData)
{
	lineOfAccount =0;
	uint8_t returned = DECLINED_STOLEN_CARD ;

		while(fread(&accessTextFile, sizeof(ST_accountsDBText_t), 1, textFileAccessAccounts))
		{
			lineOfAccount++;
			fflush(stdout);
			if ( (stringCMP((char *)cardData->primaryAccountNumber,(char *)accessTextFile.primaryAccountNumber) ) == 0)
			{
				returned = OK_SERVER;
				break;
			}
		}

	return returned;
}

char stringCMP(char *x,char *y)
{

			while( *x)
			{
				if(*x !=*y)
				{
					break;
				}
				x++;
				y++;

			}

			if( (*y == 32) && (*x ==0))  // as text file store "space" in array of character
			{
				x--;
				y--;
			}
			return *x -*y;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData)
{
	float balanceValue=0;

//	printf("balance %s\n",accessTextFile.balance);
//	fflush(stdout);
	uint8_t count=0;
//	printf("transaction amount %f\n",termData->transAmount);
//	fflush(stdout);
	while( (accessTextFile.balance[count]) && (accessTextFile.balance[count] != 32 ))
	{
//		printf("%c\n",accessTextFile.balance[count]);
//		fflush(stdout);
		balanceValue += accessTextFile.balance[count] -'0';
		balanceValue *= 10;
		count++;
	}
	balanceValue /= 10;

	if(termData->transAmount > balanceValue)
	{
		return LOW_BALANCE;
		printf("declind\n");
		fflush(stdout);
	}

	else
		return OK_SERVER;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{

//	long int transAmount = (long int )transData->terminalData->transAmount;
	accessTextFileTransaction.cardHolderData = transData->cardHolderData;
	accessTextFileTransaction.cardHolderData.cardHolderName[24]='\0';
	strcpy(&accessTextFileTransaction.terminalData.transactionDate,&transData->terminalData.transactionDate);
//	accessTextFileTransaction.terminalData.transactionDate = transData->terminalData.transactionDate;
	intTochar(&accessTextFileTransaction.terminalData.transAmount,(int)transData->terminalData.transAmount);
	intTochar(&accessTextFileTransaction.terminalData.maxTransAmount,transData->terminalData.maxTransAmount);
	accessTextFileTransaction.transState[0] = transData->transState +'0';
	intTochar(&accessTextFileTransaction.transactionSequenceNumber,transData->transactionSequenceNumber);
	accessTextFileTransaction.endline='\n';

	fwrite(&accessTextFileTransaction, sizeof(ST_transactionText_t),
	           1, textFileAccesstransactions);

	fflush(stdout);
	 if (fwrite != 0)
	 {
//		 printf("OK_SERVER ,Data saved\n");
		return OK_SERVER;
	 }
	else
	{
		printf("SAVING_FAILED\n");
		return SAVING_FAILED;
	}
}

void intTochar(char *x , int y)
{
	int n = log10(y) + 1;
		int i;
		for (i = n-1; i >= 0; --i, y /= 10)
		{
			x[n-1] = (y % 10) + '0';
			n--;

		}
}
