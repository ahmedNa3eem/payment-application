#include<stdio.h>
#include<time.h>
#include"app.h"
#include"../server/server.h"
int transNum, postion;
ST_accountsDB_t* transAccount;
ST_accountsDB_t accounts[255];
ST_transaction_t transactions[255];



appstart() {
	ST_cardData_t cardData;
	ST_terminalData_t termData;
	ST_transaction_t transData;
	for (int i = 0; i < 255; i++) {
		if (transactions[i].terminalData.transAmount == 0 && i != 0) {
			transNum = (transactions[i - 1].transactionSequenceNumber) + 1;
			postion = i;
			break;
		}
		else if (transactions[i].terminalData.transAmount == 0 && i == 0) {
			transNum = 1;
			postion = i;
			break;
		}
	}

	getCardHolderName(&cardData);
	getCardPAN(&cardData);
	getCardExpiryDate(&cardData);
	getTransactionDate(&termData);
	if (isCardExpired(&cardData, &termData) == EXPIRED_CARD) {
		return 0;
	}
	else {
		setMaxAmount(&termData);
	}
	getTransactionAmount(&termData);
	if (isBelowMaxAmount(&termData) == EXCEED_MAX_AMOUNT) {
		printf("exceed maximum amount\n");
		return 0 ;
	}else{
		transData.cardHolderData = cardData;
		transData.terminalData = termData;
	  recieveTransactionData(&transData);
	}
	return 0;
}

void loop(void) {
	int sequence = 0;
	ST_transaction_t get;
	char ch[2];
	printf("if you want to make another transaction press 'a' or 't' to search for a transaction:");
	scanf_s("%s", ch, 2);
	if (ch[0] == 'a') {
		appstart();
		loop();
	}
	else if (ch[0] == 't') {
		printf("\nplease enter the sequence number:");
		scanf_s("%d", &sequence);
		getTransaction(sequence, &get);
		loop();

	}
	else {
		loop();
	}
}

int main() {
	appstart();
	loop();


	

	
	
	 


}

