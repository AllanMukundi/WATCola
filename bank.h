#pragma once

_Monitor Bank {
	unsigned numStudents;
    unsigned *accountBalances;
  public:
	Bank(unsigned int numStudents);
	~Bank();
	void deposit(unsigned int id, unsigned int amount);
	void withdraw(unsigned int id, unsigned int amount);
};
