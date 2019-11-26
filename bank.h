#pragma once

_Monitor Bank {
	unsigned numStudents;
    unsigned *accountBalances;
  public:
	Bank(unsigned int numStudents);
	void deposit(unsigned int id, unsigned int amount);
	void withdraw(unsigned int id, unsigned int amount);
	~Bank() {
        delete[] accountBalances;
	}
};
