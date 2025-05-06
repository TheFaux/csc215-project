#include <stdio.h>
#include <string.h>


typedef struct {
    int accountNumber;
    char ownerName[50];
    double balance;
    
    /*current session*/
    int numDeposits;
    int numWithdraws;
    int numTransfers;
    double totalMoneyIn;
    double totalMoneyOut;
} Account;


Account accounts[3];
double totalProfit = 0.0;
const double WITHDRAW_FEE_RATE = 0.01;  /* 1% */
const double TRANSFER_FEE_RATE = 0.025; /* 2.5% */
/*global session*/
int totalNumDeposits = 0;
int totalNumTransfers = 0;
int totalNumWithdraws = 0;


void init();
void display();
void handleAccountOperations(int accountIndex);
void handleAdminOperations();
void viewBalance(int accountIndex);
void depositMoney(int accountIndex);
void withdrawMoney(int accountIndex);
void transferMoney(int accountIndex);
void displaySummary(int accountIndex);
void viewtotalProfit();
void viewMostPopularOperation();
void reset(int accountIndex);

int main() {
    int choice;
    
    init();
    
    while (1) {
        display();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        if (choice == 1 || choice == 2 || choice == 3) {
            handleAccountOperations(choice - 1);
        } else if (choice == 4) {
            handleAdminOperations();
        } else if (choice == 5) {
            printf("\nThank you for using Riyadh National Bank ATM. Goodbye!\n");
            break;
        } else {
            printf("\nInvalid choice. Please try again.\n");
        }
    }
    
    return 0;
}

void init() {
    accounts[0].accountNumber = 1001;
    strcpy(accounts[0].ownerName, "Ahmed Abdullah");
    accounts[0].balance = 5000.0;
    reset(0);
    
    accounts[1].accountNumber = 1002;
    strcpy(accounts[1].ownerName, "Sarah Mohammed");
    accounts[1].balance = 7500.0;
    reset(1);

    accounts[2].accountNumber = 1003;
    strcpy(accounts[2].ownerName, "Khalid Al-Saud");
    accounts[2].balance = 10000.0;
    reset(2);
}

/*main menu*/
void display() {
    printf("\n==================================\n");
    printf("    RIYADH NATIONAL BANK ATM     \n");
    printf("==================================\n");
    printf("Please select an account or login as administrator:\n\n");
    

    int i;
    for(i=0; i < 3; i++) {
        printf("%d. Account #%d (%s)\n", i + 1, accounts[i].accountNumber, accounts[i].ownerName);
    }
    
    printf("4. Administrator\n");
    printf("5. Exit\n");
    printf("==================================\n");
}

void handleAccountOperations(int accountIndex) {
    int choice;
    
    while (1) {
        printf("\n==================================\n");
        printf("    ACCOUNT OPERATIONS MENU     \n");
        printf("==================================\n");
        printf("Welcome, %s (Account #%d)\n\n", 
               accounts[accountIndex].ownerName, 
               accounts[accountIndex].accountNumber);
        
        printf("1. View Balance\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Transfer Money\n");
        printf("5. Exit\n");
        printf("==================================\n");
        
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                viewBalance(accountIndex);
                break;
                
            case 2:
                depositMoney(accountIndex);
                break;
                
            case 3:
                withdrawMoney(accountIndex);
                break;
                
            case 4:
                transferMoney(accountIndex);
                break;
                
            case 5:
                displaySummary(accountIndex);
                reset(accountIndex);/*wipe current session*/
                return;
                
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }
}


void handleAdminOperations() {
    int choice;
    
    while (1) {
        printf("\n==================================\n");
        printf("      ADMINISTRATOR MENU        \n");
        printf("==================================\n");
        printf("1. View total Profit\n");
        printf("2. Most Popular Operation\n");
        printf("3. Exit\n");
        printf("==================================\n");
        
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                viewtotalProfit();
                break;
                
            case 2:
                viewMostPopularOperation();
                break;
                
            case 3:
                return;
                
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }
}


void viewBalance(int accountIndex) {
    printf("\n==================================\n");
    printf("          ACCOUNT BALANCE        \n");
    printf("==================================\n");
    printf("Account #%d (%s)\n", 
           accounts[accountIndex].accountNumber, 
           accounts[accountIndex].ownerName);
    printf("Current Balance: %.2f SAR\n", accounts[accountIndex].balance);
    printf("==================================\n");
}


void depositMoney(int accountIndex) {
    double amount;
    
    printf("\n==================================\n");
    printf("          DEPOSIT MONEY          \n");
    printf("==================================\n");
    
    printf("Enter the amount to deposit: ");
    scanf("%lf", &amount);
    
    if (amount <= 0) {
        printf("\nError: Deposit amount must be positive.\n");
        return;
    }
    
    accounts[accountIndex].balance += amount;
    accounts[accountIndex].numDeposits++;
    totalNumDeposits++;
    accounts[accountIndex].totalMoneyIn += amount;
    
    printf("\nDeposit successful!\n");
    printf("New Balance: %.2f SAR\n", accounts[accountIndex].balance);
    printf("==================================\n");
}


void withdrawMoney(int accountIndex) {
    double amount, fee;
    
    printf("\n==================================\n");
    printf("         WITHDRAW MONEY          \n");
    printf("==================================\n");
    
    printf("Enter the amount to withdraw: ");
    scanf("%lf", &amount);
    
    if (amount <= 0) {
        printf("\nError: Withdrawal amount must be positive.\n");
        return;
    }
    
    fee = amount * WITHDRAW_FEE_RATE;
    
    if (amount + fee > accounts[accountIndex].balance) {
        printf("\nError: Insufficient funds. total required: %.2f SAR\n", amount + fee);
        return;
    }
    
    accounts[accountIndex].balance -= (amount + fee);
    accounts[accountIndex].numWithdraws++;
    totalNumWithdraws++;
    accounts[accountIndex].totalMoneyOut += (amount + fee);
    totalProfit += fee;
    
    printf("\nWithdrawal successful!\n");
    printf("Amount withdrawn: %.2f SAR\n", amount);
    printf("Fee charged (1%%): %.2f SAR\n", fee);
    printf("New Balance: %.2f SAR\n", accounts[accountIndex].balance);
    printf("==================================\n");
}


void transferMoney(int accountIndex) {
    int targetAccountIndex;
    double amount, fee;
    
    printf("\n==================================\n");
    printf("         TRANSFER MONEY          \n");
    printf("==================================\n");
    
    printf("Available accounts for transfer:\n");

    int i;
    for(i=0; i < 3; i++) {
        if (i != accountIndex) {
            printf("%d. Account #%d (%s)\n", i + 1, accounts[i].accountNumber, accounts[i].ownerName);
        }
    }
    
    printf("\nEnter the account number (1-3): ");
    scanf("%d", &targetAccountIndex);
    
    /*index at 1 to index at 0*/
    targetAccountIndex--;
    
    if (targetAccountIndex < 0 || targetAccountIndex >= 3 || targetAccountIndex == accountIndex) {
        printf("\nError: Invalid account selection.\n");
        return;
    }
    
    printf("Enter the amount to transfer: ");
    scanf("%lf", &amount);
    
    if (amount <= 0) {
        printf("\nError: Transfer amount must be positive.\n");
        return;
    }
    
    fee = amount * TRANSFER_FEE_RATE;
    
    if (amount + fee > accounts[accountIndex].balance) {
        printf("\nError: Insufficient funds. total required: %.2f SAR\n", amount + fee);
        return;
    }
    
    accounts[accountIndex].balance -= (amount + fee);
    accounts[targetAccountIndex].balance += amount;
    accounts[accountIndex].numTransfers++;
    totalNumTransfers++;
    accounts[accountIndex].totalMoneyOut += (amount + fee);
    accounts[targetAccountIndex].totalMoneyIn += amount;
    totalProfit += fee;
    
    printf("\nTransfer successful!\n");
    printf("Amount transferred: %.2f SAR\n", amount);
    printf("Fee charged (2.5%%): %.2f SAR\n", fee);
    printf("New Balance: %.2f SAR\n", accounts[accountIndex].balance);
    printf("==================================\n");
}

/*current session*/
void displaySummary(int accountIndex) {
    printf("\n==================================\n");
    printf("      TRANSACTIONS SUMMARY       \n");
    printf("==================================\n");
    printf("Account #%d (%s)\n\n", 
           accounts[accountIndex].accountNumber, 
           accounts[accountIndex].ownerName);
    
    printf("Number of deposits: %d\n", accounts[accountIndex].numDeposits);
    printf("Number of withdrawals: %d\n", accounts[accountIndex].numWithdraws);
    printf("Number of transfers: %d\n", accounts[accountIndex].numTransfers);
    printf("total money in: %.2f SAR\n", accounts[accountIndex].totalMoneyIn);
    printf("total money out: %.2f SAR\n", accounts[accountIndex].totalMoneyOut);
    printf("==================================\n");
}


void viewtotalProfit() {
    printf("\n==================================\n");
    printf("          total PROFIT           \n");
    printf("==================================\n");
    printf("total profit from all fees: %.2f SAR\n", totalProfit);
    printf("==================================\n");
}


void viewMostPopularOperation() {
    printf("\n==================================\n");
    printf("      MOST POPULAR OPERATION     \n");
    printf("==================================\n");
    
    /*max*/
    int maxCount = totalNumDeposits;
    if (totalNumWithdraws > maxCount) maxCount = totalNumWithdraws;
    if (totalNumTransfers > maxCount) maxCount = totalNumTransfers;
    

    if (maxCount == 0) {
        printf("No operations have been performed yet.\n");
    } else {
        if (totalNumDeposits == maxCount) {
            printf("Deposit: %d operations\n", totalNumDeposits);
        }
        if (totalNumWithdraws == maxCount) {
            printf("Withdraw: %d operations\n", totalNumWithdraws);
        }
        if (totalNumTransfers == maxCount) {
            printf("Transfer: %d operations\n", totalNumTransfers);
        }
    }
    
    printf("==================================\n");
}


void reset(int accountIndex) {
    accounts[accountIndex].numDeposits = 0;
    accounts[accountIndex].numWithdraws = 0;
    accounts[accountIndex].numTransfers = 0;
    accounts[accountIndex].totalMoneyIn = 0.0;
    accounts[accountIndex].totalMoneyOut = 0.0;
}