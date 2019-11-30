#pragma once

#include "printer.h"
#include "bank.h"
#include "wat_card.h"
#include <queue>

using namespace std;

_Task WATCardOffice {
    struct Args {
        unsigned studentID, amount;
        WATCard *card;
        Args(unsigned studentID, unsigned amount, WATCard *card):
            studentID(studentID), amount(amount), card(card) {}
	};
    struct Job {							// marshalled arguments and return future
        Args args;							// call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;			// return future
        Job(Args args): args(args) {}
    };
    // communicates with bank.
    _Task Courier { 
        Bank &bank;
        Printer &printer;
        WATCardOffice &office;
        unsigned id;
        void main();
      public:
        Courier(Bank &bank, Printer &printer, WATCardOffice &office, unsigned id);
	};

    Printer &printer;
    Bank &bank;
    unsigned numCouriers;
    // Need an array of heap allocated `Courier`s as there is no default
    // constructor for `Courier`. 
    Courier **couriers;
    queue<Job*> jobs;

    void main();
  public:
    _Event Lost {};							// lost WATCard
    WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers);
    WATCard::FWATCard create(unsigned int sid, unsigned int amount);
    WATCard::FWATCard transfer(unsigned int sid, unsigned int amount, WATCard *card);
    Job *requestWork();
};
