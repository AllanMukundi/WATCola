#include "name_server.h"

void NameServer::VMregister( VendingMachine *vendingMachine) {
    unsigned id = vendingMachine->getId();
    machines[id] = vendingMachine;
    printer.print(Printer::NameServer, 'R', id);
}

VendingMachine * NameServer::getMachine(unsigned id) {
    unsigned currentMachine = assignment[id];
    VendingMachine *requestedMachine = machines[currentMachine];
    currentMachine = (currentMachine + 1) % numVendingMachines;
    assignment[id] = currentMachine;
    printer.print(Printer::NameServer, 'N', id, currentMachine);
    return requestedMachine;
}

VendingMachine ** NameServer::getMachineList() {
    return machines;
}

void NameServer::main() {
    printer.print(Printer::NameServer, 'S');

    // Register all machines first
    for (unsigned i = 0; i < numVendingMachines; ++i) {
        _Accept( VMregister );
    }
    
    while (true) {
        _Accept( ~NameServer ) {
            break;
        } or _Accept( getMachine, getMachineList ) {}
    }

    printer.print(Printer::NameServer, 'F');
}

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ):
    printer(prt), numVendingMachines(numVendingMachines), numStudents(numStudents) {
        machines = new VendingMachine*[numVendingMachines];
        assignment = new unsigned[numStudents];
        // Initial assignment
        unsigned machine = 0;
        for (unsigned i = 0; i < numStudents; ++i) {
            assignment[i] = machine;
            machine = (machine + 1) % numVendingMachines;
        }
    }

NameServer::~NameServer() {
    delete[] machines;
    delete[] assignment;
}
