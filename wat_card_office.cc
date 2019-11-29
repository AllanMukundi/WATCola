#include "wat_card_office.h"
#include "MPRNG.h"

extern MPRNG mprng;

// ============== WATCardOffice ==============
WATCardOffice::WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers):
    printer(prt), bank(bank), numCouriers(numCouriers) {
    // "Initially, the WATCard office creates a fixed-sized courier pool with 
    // numCouriers courier tasks to communicate with the bank. (Additional 
    // couriers may not be created after the WATCardOffice begins.)"
    couriers = new Courier*[numCouriers];
    for (unsigned i = 0; i < numCouriers; i++) {
     couriers[i] = new Courier(bank, printer, *this, i);
   }
}

// "Each courier task calls requestWork, blocks until a Job request is ready, 
// and then receives the next Job request as the result of the call."
WATCardOffice::Job *WATCardOffice::requestWork() {
    if (jobs.empty())
        return nullptr;

    Job *job = jobs.front();
    jobs.pop();

    // W 
    // request work call complete
    printer.print(Printer::WATCardOffice, 'W');

    return job;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard *card) {
    auto args = Args(sid, amount, card);
    Job *job = new Job(args);
    jobs.push(job);

    // T s,a 
    // transfer call complete 
    // student s, transfer amount a
    printer.print(Printer::WATCardOffice, 'T', sid, amount);

    return job->result;
}

WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
    WATCard *card = new WATCard();
    auto args = WATCardOffice::Args(sid, amount, card);
    Job *job = new Job(args);
    jobs.push(job);

    // C s,a 
    // create call complete 
    // student s, transfer amount a
    printer.print(Printer::WATCardOffice, 'C', sid, amount);

    return job->result;
}

void WATCardOffice::main() {
    // S 
    // starting
    printer.print(Printer::WATCardOffice, 'S');

    while (true) {
        _Accept(~WATCardOffice) {
            // Cleanup existing jobs.
            while (true) {
                if (jobs.empty()) 
                    break;
                delete jobs.front();
                jobs.pop();
            }
            // Wait for all couriers to request for work so they get
            // a nullptr and break out the `main` while loop.
            for (unsigned i = 0; i < numCouriers; i++)
                _Accept(requestWork);
            
            // Clean up (note extra clean up needed due to array
            // of heap objects).
            for (unsigned i = 0; i < numCouriers; i++)
                delete couriers[i];
            delete[] couriers;
            
            break;
        } or _Accept(transfer, create) {
        } or _When(!jobs.empty()) _Accept(requestWork) {} // wait only when there are jobs.
    }

    // F
    // finished
    printer.print(Printer::WATCardOffice, 'F');
}


// ================= Courier =================

WATCardOffice::Courier::Courier(
    Bank &bank, Printer &printer, WATCardOffice &office, unsigned id):
    bank(bank), printer(printer), office(office), id(id) {}

void WATCardOffice::Courier::main() {
    // S 
    // starting
    printer.print(Printer::Courier, id, 'S');

    while (true) {
        _Accept(~Courier) {
            break;
        } _Else {
            Job *job = office.requestWork();
            // If there are no more jobs, that means Courier is about to be 
            // deleted. Thus, break out of the loop.
            if (job == nullptr) 
                break;

            // t s,a 
            // start funds transfer 
            // student s requesting transfer, amount a of transfer
            printer.print(Printer::Courier, id, 't', job->args.studentID, job->args.amount);

            // Take the money out of the bank and add it to the WATCard.
            bank.withdraw(job->args.studentID, job->args.amount);
            job->args.card->deposit(job->args.amount);

            // T s,a 
            // complete funds transfer 
            // student s requesting transfer, amount a of transfer
            printer.print(Printer::Courier, id, 'T', job->args.studentID, job->args.amount);

            // Deliver the WATCard.

            // "There is a 1 in 6 chance a courier loses a student’s WATCard
            // after the update."
            if (mprng(5) == 0) {
                printer.print(Printer::Courier, id, 'L', job->args.studentID);
                // "When the card is lost, the exception WATCardOffice::Lost 
                // is inserted into the future, rather than making the future 
                // available, and the current WATCard is deleted."
                job->result.exception(new WATCardOffice::Lost);
                delete job->args.card;
            } else {
                job->result.delivery(job->args.card);
            }

            delete job; // No need for the job anymore.
        }
    }

    // F
    // finished
    printer.print(Printer::Courier, id, 'F');
}
