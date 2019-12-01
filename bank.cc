#include "bank.h"

Bank::Bank(unsigned int numStudents): numStudents(numStudents) {
    // Allocate mem for array and set all initial balances to 0.
    accountBalances = new unsigned[numStudents];
    for (unsigned i = 0; i < numStudents; i++) {
        accountBalances[i] = 0;
    }
}

Bank::~Bank() {
    delete[] accountBalances;
}

void Bank::deposit(unsigned int id, unsigned int amount) {
    accountBalances[id] += amount;
}

// The courier waits until enough money has been deposited, 
// which may require multiple deposits.
void Bank::withdraw(unsigned int id, unsigned int amount) {
    // NOTE: this is technically not busy waiting.
    while (true) {
        // Stay in loop until there is a sufficient amount.
        if (accountBalances[id] >= amount) {
            break;
        } else {
            _Accept(Bank::deposit);
        }
    }
    accountBalances[id] -= amount;
}
