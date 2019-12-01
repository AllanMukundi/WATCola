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
            // Before each gift is transferred, the parent yields for 
            // `parentalDelay` times (not random).
            yield(parentalDelay);

            // Get the random student.
            unsigned studentID = mprng(numStudents - 1);

            // Get the amount of money.
            unsigned moneyAmount = mprng(1, 3);

            // Print.
            printer.print(Printer::Parent, 'D', studentID, moneyAmount);

            // Deposit!
            bank.deposit(studentID, moneyAmount);
        }
    }
    printer.print(Printer::Parent, 'F');
}
