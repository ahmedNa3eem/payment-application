#include"terminal.h"
#include<stdio.h>
#include<time.h>

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	enum RW{RIGHT,WRONG};
	int x = 0, y = 0, termMonth;
	printf("please enter the transaction date (dd/mm/yyyy):");
	scanf_s("%s", termData->transactionDate, 11);
	 termMonth = (termData->transactionDate[3] * 100) + termData->transactionDate[4];
	for (int i = 0; i < 10; i++) {
			if (termData->transactionDate[i] > 47 && termData->transactionDate[i] < 58) {
				x++;
			
		}
	}

	if (termData->transactionDate[0] == '0' && termData->transactionDate[1] == '0') {
		y = WRONG;
	}
	else if (termData->transactionDate[3] == '0' && termData->transactionDate[4] == '0') {
		y = WRONG;
	}
	else {
		y = RIGHT;
	}
	if (x == 8 && termMonth<=4950 &&
		termData->transactionDate[2] == '/' && termData->transactionDate[10] == 0 &&
		termData->transactionDate[5] == '/' && y ==RIGHT) {
		printf("%s\n", termData->transactionDate);
		return CARD_OK;
	}
	else {
		printf("wrong exipry date\nplease try again\n");
		getTransactionDate(termData);
		return WRONG_DATE;
	}

}
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {
	int cardYear, cardMonth, termYear, termMonth;
	cardYear = (cardData->cardExpirationDate[3] * 100) + cardData->cardExpirationDate[4];
	cardMonth= (cardData->cardExpirationDate[0] * 100) + cardData->cardExpirationDate[1];
	termYear = (termData->transactionDate[8] * 100) + termData->transactionDate[9];
	termMonth= (termData->transactionDate[3] * 100) + termData->transactionDate[4];
	if (cardYear < termYear) {
		printf("card is expired\n");
		return EXPIRED_CARD;
	}
	else if (cardYear == termYear && cardMonth < termMonth) {
			printf("card is expired\n");
			return EXPIRED_CARD;
	}
	else {
		return TERMINAL_OK;
	}
}
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	printf("please enter the transaction amount:");
	scanf_s("%f",&termData->transAmount);
	if (termData->transAmount <= 0.00) {
		printf("invalid amount\n");
		getTransactionAmount(termData);
		return INVALID_AMOUNT;
	}
	else {
		printf("your transaction is in process\n");
		return TERMINAL_OK;
	}
}
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {
	srand((unsigned int)time(NULL));;
	float z = ((float)rand() / (float)(10)) * 1;
	termData->maxTransAmount = (z + 1) * 1;
	if (termData->maxTransAmount <= 0) {
		return INVALID_MAX_AMOUNT;
	}
	else {
		printf("maximum transaction amount for today is %f\n", termData->maxTransAmount);
		return TERMINAL_OK;
	}
}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->transAmount > termData->maxTransAmount) {
		printf("Exceed the max amount");

		return EXCEED_MAX_AMOUNT;
	}
	else {

		return TERMINAL_OK;
	}

}

