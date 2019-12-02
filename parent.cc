#include "MPRNG.h"
#include "parent.h"

extern MPRNG mprng;

Parent::Parent(Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay):
    bank(bank), printer(prt), numStudents(numStudents), parentalDelay(parentalDelay) {}

void Parent::main() {
    printer.print(Printer::Parent, 'S');
    while (true) {
        _Accept(Parent::~Parent) {
            break;
        } _Else {
            // Get the amount of money.
            unsigned moneyAmount = mprng(1, 3);

            // Get the random student.
            unsigned studentID = mprng(numStudents - 1);

            // Before each gift is transferred, the parent yields for 
            // `parentalDelay` times (not random).
            yield(parentalDelay);

            // Deposit!
            bank.deposit(studentID, moneyAmount);

            // Print.
            printer.print(Printer::Parent, 'D', studentID, moneyAmount);
        }
    }
    printer.print(Printer::Parent, 'F');
}
