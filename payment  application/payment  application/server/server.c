#include"server.h"
#include<stdio.h>
int transNum,postion;
ST_accountsDB_t* transAccount;
ST_accountsDB_t accounts[255] = { {(float)200, RUNNING, "8989374615436851"},{(float)100000.0, BLOCKED, "5807007076043875" },
	{(float)12032.1,RUNNING,"23461506177787025"},{(float)14029.4,RUNNING,"366377996489064427"},{(float)53040.3,BLOCKED,"767277924884699753"} };
ST_transaction_t transactions[255] = { 0,0,0,0,0};
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence) {
	int found=0;
	int z = 0;
	for (int i = 0; cardData->primaryAccountNumber[i] != 0; i++) {
		z++;
	}
	for (int i = 0; i < 255; i++) {
		if (found == 1) {
			int x = i - 1;
			transAccount = &accounts[x];
			return SERVER_OK;
		}
		else {
			for (int n = 0; n < z; n++) {
				if (cardData->primaryAccountNumber[n] == accounts[i].primaryAccountNumber[n]) {
					found = 1;

				}
				else {
					found = 0;
					break;

				}
			}
		}
	}
	if (found == 1) {
		transAccount = &accounts[254];
		return SERVER_OK;

	}
	else {
		return ACCOUNT_NOT_FOUND;
	}
}
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) {
	if (accountRefrence->state == BLOCKED){

		return BLOCKED_ACCOUNT;
	}
	else {
		return SERVER_OK;
	}
}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData) {
	if (termData->transAmount > transAccount->balance) {
		return LOW_BALANCE;
	}
	else {
		return SERVER_OK;
	}
}
EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	transactions[postion].cardHolderData = transData->cardHolderData;
	transactions[postion].terminalData=transData->terminalData  ;
	transactions[postion].transactionSequenceNumber = transNum;
	if (transactions[postion].transactionSequenceNumber == transNum) {
		return  SERVER_OK;
	}
	else {
		transactions[postion].terminalData.transAmount = 0;
		return SAVING_FAILED;
	}

}
EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
	int isValid=isValidAccount(&transData->cardHolderData,transAccount);
	if (isValid == SERVER_OK) {
		int isBlocked = isBlockedAccount(transAccount);
		if (isBlocked != BLOCKED_ACCOUNT) {
			int isAvaible = isAmountAvailable(&transData->terminalData);
			if (isAvaible == SERVER_OK) {
				transactions[postion].transState = APPROVED;
				int save = saveTransaction(transData);
				if (save == SERVER_OK) {
					transAccount->balance -= transData->terminalData.transAmount;
					printf("\nAPPROVED\n");
					printf("your new balance is %f\n", transAccount->balance);
					return APPROVED;
				}
				else {
					printf("\nInternal Server Error\n");

					return INTERNAL_SERVER_ERROR;
				}
			}
			else {
				transactions[postion].transState = DECLINED_INSUFFECIENT_FUND;
				int save = saveTransaction(transData);
				if (save == SERVER_OK) {
					printf("\n\n");
					printf("\nDeclined Insuffeient Fund\n");

					return DECLINED_INSUFFECIENT_FUND;

				}
				else {
					return INTERNAL_SERVER_ERROR;
				}
			}
		}
		else {
			transactions[postion].transState = DECLINED_STOLEN_CARD;
			int save = saveTransaction(transData);
			if (save == SERVER_OK) {
				printf("\nDeclined Stolen Card\n");

				return DECLINED_STOLEN_CARD;
			}
			else {
				return INTERNAL_SERVER_ERROR;
			}
		}
	}
	else {
		transactions[postion].transState = FRAUD_CARD;
		int save = saveTransaction(transData);
		if (save ==SERVER_OK) {
			printf("\nFraud card\n");

			return FRAUD_CARD;
		}
		else {
			return INTERNAL_SERVER_ERROR;
		}
	}



}
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData) {
	enum RW{RIGHT,WRONG};
	int x;
	for (int i = 0; i < 255; i++) {
		if (transactions[i].transactionSequenceNumber == transactionSequenceNumber) {
			printf("Name:%s		PAN:%s		ExipiryDate%s\n", transactions[i].cardHolderData.cardHolderName,
				transactions[i].cardHolderData.primaryAccountNumber, transactions[i].cardHolderData.cardExpirationDate);
			printf("transAmount:%f		MaxAmount:%f		Date:%s\n", transactions[i].terminalData.transAmount,
				transactions[i].terminalData.maxTransAmount, transactions[i].terminalData.transactionDate);
			printf("State:%d\n", transactions[i].transState);
			printf("Sequence:%d", transactions[i].transactionSequenceNumber);
			x = RIGHT;
			break;
		}
		else {
			x = WRONG;
		}
	}
	if (x == RIGHT) {
		return SERVER_OK;

	}
	else {
		return TRANSACTION_NOT_FOUND;
	}
	


}


