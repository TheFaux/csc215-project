#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int accountNumber;
    char ownerName[50];
    double balance;
    
    // Statistics for current session
    int numDeposits;
    int numWithdraws;
    int numTransfers;
    double totalMoneyIn;
    double totalMoneyOut;
    
    // Statistics for all time (for administrator)
    int totalNumDeposits;
    int totalNumWithdraws;
    int totalNumTransfers;
} Account;

// Global variables
Account accounts[3];
double totalProfit = 0.0;
const double WITHDRAW_FEE_RATE = 0.01;  // 1%
const double TRANSFER_FEE_RATE = 0.025; // 2.5%

// Function prototypes
void init();
void display();
void handleAccountOperations(int accountIndex);
void handleAdminOperations();
void viewBalance(int accountIndex);
void depositMoney(int accountIndex);
void withdrawMoney(int accountIndex);
void transferMoney(int accountIndex);
void displaySummary(int accountIndex);
void viewTotalProfit();
void viewMostPopularOperation();
void reset(int accountIndex);

int main() {
    int choice;
    
    // Initialize account data
    init();
    
    while (1) {
        display();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        if (choice == 1 || choice == 2 || choice == 3) {
            // Account operations
            handleAccountOperations(choice - 1);
        } else if (choice == 4) {
            // Admin operations
            handleAdminOperations();
        } else if (choice == 5) {
            // Exit the program
            printf("\nThank you for using Riyadh National Bank ATM. Goodbye!\n");
            break;
        } else {
            printf("\nInvalid choice. Please try again.\n");
        }
    }
    
    return 0;
}

// Initialize account data with predefined values
void init() {
    // Account 1
    accounts[0].accountNumber = 1001;
    strcpy(accounts[0].ownerName, "Ahmed Abdullah");
    accounts[0].balance = 5000.0;
    reset(0);
    accounts[0].totalNumDeposits = 0;
    accounts[0].totalNumWithdraws = 0;
    accounts[0].totalNumTransfers = 0;
    
    // Account 2
    accounts[1].accountNumber = 1002;
    strcpy(accounts[1].ownerName, "Sarah Mohammed");
    accounts[1].balance = 7500.0;
    reset(1);
    accounts[1].totalNumDeposits = 0;
    accounts[1].totalNumWithdraws = 0;
    accounts[1].totalNumTransfers = 0;
    
    // Account 3
    accounts[2].accountNumber = 1003;
    strcpy(accounts[2].ownerName, "Khalid Al-Saud");
    accounts[2].balance = 10000.0;
    reset(2);
    accounts[2].totalNumDeposits = 0;
    accounts[2].totalNumWithdraws = 0;
    accounts[2].totalNumTransfers = 0;
}

// Display the main menu (Account Selection Menu)
void display() {
    printf("\n==================================\n");
    printf("    RIYADH NATIONAL BANK ATM     \n");
    printf("==================================\n");
    printf("Please select an account or login as administrator:\n\n");
    
    for (int i = 0; i < 3; i++) {
        printf("%d. Account #%d (%s)\n", i + 1, accounts[i].accountNumber, accounts[i].ownerName);
    }
    
    printf("4. Administrator\n");
    printf("5. Exit\n");
    printf("==================================\n");
}

// Handle account operations menu and actions
void handleAccountOperations(int accountIndex) {
    int choice;
    
    // Reset session statistics when logging in
    reset(accountIndex);
    
    while (1) {
        // Display Account Operations Menu
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
                // Display transactions summary before returning to main menu
                displaySummary(accountIndex);
                return;
                
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }
}

// Handle administrator operations menu and actions
void handleAdminOperations() {
    int choice;
    
    while (1) {
        // Display Administrator Menu
        printf("\n==================================\n");
        printf("      ADMINISTRATOR MENU        \n");
        printf("==================================\n");
        printf("1. View Total Profit\n");
        printf("2. Most Popular Operation\n");
        printf("3. Exit\n");
        printf("==================================\n");
        
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                viewTotalProfit();
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

// View account balance
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

// Deposit money into account
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
    accounts[accountIndex].totalNumDeposits++;
    accounts[accountIndex].totalMoneyIn += amount;
    
    printf("\nDeposit successful!\n");
    printf("New Balance: %.2f SAR\n", accounts[accountIndex].balance);
    printf("==================================\n");
}

// Withdraw money from account
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
        printf("\nError: Insufficient funds. Total required: %.2f SAR\n", amount + fee);
        return;
    }
    
    accounts[accountIndex].balance -= (amount + fee);
    accounts[accountIndex].numWithdraws++;
    accounts[accountIndex].totalNumWithdraws++;
    accounts[accountIndex].totalMoneyOut += (amount + fee);
    totalProfit += fee;
    
    printf("\nWithdrawal successful!\n");
    printf("Amount withdrawn: %.2f SAR\n", amount);
    printf("Fee charged (1%%): %.2f SAR\n", fee);
    printf("New Balance: %.2f SAR\n", accounts[accountIndex].balance);
    printf("==================================\n");
}

// Transfer money to another account
void transferMoney(int accountIndex) {
    int targetAccountIndex;
    double amount, fee;
    
    printf("\n==================================\n");
    printf("         TRANSFER MONEY          \n");
    printf("==================================\n");
    
    printf("Available accounts for transfer:\n");
    for (int i = 0; i < 3; i++) {
        if (i != accountIndex) {
            printf("%d. Account #%d (%s)\n", i + 1, accounts[i].accountNumber, accounts[i].ownerName);
        }
    }
    
    printf("\nEnter the account number (1-3): ");
    scanf("%d", &targetAccountIndex);
    
    // Adjust for 0-based indexing
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
        printf("\nError: Insufficient funds. Total required: %.2f SAR\n", amount + fee);
        return;
    }
    
    accounts[accountIndex].balance -= (amount + fee);
    accounts[targetAccountIndex].balance += amount;
    accounts[accountIndex].numTransfers++;
    accounts[accountIndex].totalNumTransfers++;
    accounts[accountIndex].totalMoneyOut += (amount + fee);
    accounts[targetAccountIndex].totalMoneyIn += amount;
    totalProfit += fee;
    
    printf("\nTransfer successful!\n");
    printf("Amount transferred: %.2f SAR\n", amount);
    printf("Fee charged (2.5%%): %.2f SAR\n", fee);
    printf("New Balance: %.2f SAR\n", accounts[accountIndex].balance);
    printf("==================================\n");
}

// Display transaction summary for the current session
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
    printf("Total money in: %.2f SAR\n", accounts[accountIndex].totalMoneyIn);
    printf("Total money out: %.2f SAR\n", accounts[accountIndex].totalMoneyOut);
    printf("==================================\n");
}

// View total profit from fees (admin function)
void viewTotalProfit() {
    printf("\n==================================\n");
    printf("          TOTAL PROFIT           \n");
    printf("==================================\n");
    printf("Total profit from all fees: %.2f SAR\n", totalProfit);
    printf("==================================\n");
}

// View most popular operation across all accounts (admin function)
void viewMostPopularOperation() {
    int totalDeposits = 0, totalWithdraws = 0, totalTransfers = 0;
    
    // Count total operations across all accounts
    for (int i = 0; i < 3; i++) {
        totalDeposits += accounts[i].totalNumDeposits;
        totalWithdraws += accounts[i].totalNumWithdraws;
        totalTransfers += accounts[i].totalNumTransfers;
    }
    
    printf("\n==================================\n");
    printf("      MOST POPULAR OPERATION     \n");
    printf("==================================\n");
    
    // Find the maximum operation count
    int maxCount = totalDeposits;
    if (totalWithdraws > maxCount) maxCount = totalWithdraws;
    if (totalTransfers > maxCount) maxCount = totalTransfers;
    
    // Display the most popular operation(s)
    if (maxCount == 0) {
        printf("No operations have been performed yet.\n");
    } else {
        if (totalDeposits == maxCount) {
            printf("Deposit: %d operations\n", totalDeposits);
        }
        if (totalWithdraws == maxCount) {
            printf("Withdraw: %d operations\n", totalWithdraws);
        }
        if (totalTransfers == maxCount) {
            printf("Transfer: %d operations\n", totalTransfers);
        }
    }
    
    printf("==================================\n");
}

// Reset session statistics for an account
void reset(int accountIndex) {
    accounts[accountIndex].numDeposits = 0;
    accounts[accountIndex].numWithdraws = 0;
    accounts[accountIndex].numTransfers = 0;
    accounts[accountIndex].totalMoneyIn = 0.0;
    accounts[accountIndex].totalMoneyOut = 0.0;
}