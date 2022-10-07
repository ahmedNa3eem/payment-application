#include<stdio.h>
#include"card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	enum WR{RIGHT,WRONG};
	int y = 0;
	int x = 0;
	printf("please enter your name (min 20 letter, max 24 letter):");
	scanf_s(" %[^\n]%*c",cardData->cardHolderName,25); 

	for (int i = 0; cardData->cardHolderName[i] !=0; i++) {
		x++;
	}

	printf("%d\n", x);
	for (int i = 0; i < x; i++) {
		if ( cardData->cardHolderName[i] > 64 && cardData->cardHolderName[i]<91) {
			y = RIGHT;
		}
		else if (cardData->cardHolderName[i] > 96 && cardData->cardHolderName[i] < 123) {
			y = RIGHT;
		}
		else if (cardData->cardHolderName[i] == 32&&i!=0&&!(i==x-1) ) {
			if (cardData->cardHolderName[i-1] != 32 && cardData->cardHolderName[i+1] != 32) {
				y = RIGHT;

			}
			else {
				y = WRONG;
				break;
			}
		}
		else {
			y = WRONG;
			break;
		}
	}
	if (x<20||y==WRONG) {
		printf("wrong name\nplease try again\n");
		getCardHolderName(cardData);
		return WRONG_NAME;
	}
	else {
		printf("%s\n", cardData->cardHolderName);
		return CARD_OK;
	}
}
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	enum RW{RIGHT,WRONG};
	int x = 0;
	int y;
	int cardMonth;
	printf("please enter the expiry date (mm/yy):");
	scanf_s(" %[^\n]%*c", cardData->cardExpirationDate, 6);

	cardMonth = (cardData->cardExpirationDate[0] * 100) + cardData->cardExpirationDate[1];
	for (int i = 0; i < 5; i++) {
			if (cardData->cardExpirationDate[i] > 47 && cardData->cardExpirationDate[i] < 58) {
				x++;
			}
	} 

	if (cardData->cardExpirationDate[0] == '0' && cardData->cardExpirationDate[1] == '0') {
		y = WRONG;
	}
	else {
		y = RIGHT;
	}
	if (x == 4 && cardMonth<=4950 && cardData->cardExpirationDate[2] == '/' && cardData->cardExpirationDate[5] == 0&&y==RIGHT) {
	 printf("%s\n", cardData->cardExpirationDate);
		return CARD_OK;
 }
 else {
	 printf("wrong exipry date\nplease try again\n");
	 getCardExpiryDate(cardData);
	 return WRONG_EXP_DATE;
 }

}
EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	enum RW{RIGHT,WRONG};
	int x = 0;
	int y = 0;
	printf("please enter your PAN number:");
	scanf_s("%s", cardData->primaryAccountNumber, 20);
	
	for (int i = 0; cardData->primaryAccountNumber[i] != 0; i++) {
		x++;
	}
	for (int i = 0; i < x; i++) {
			if (cardData->primaryAccountNumber[i] > 47 && cardData->primaryAccountNumber[i] < 58) {
				y=RIGHT;
			}
			else {
				y = WRONG;
				break;
			}
	}
	printf("%d\n",x);
	if (x >= 16 && y == RIGHT) {
		printf("%s\n", cardData->primaryAccountNumber);
		return CARD_OK;

	}
	else {
		printf("wrong number\nplease try again\n");
		getCardPAN(cardData);
		return WRONG_PAN;
	}

}
 


