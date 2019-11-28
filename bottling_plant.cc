#include "bottling_plant.h"
#include "truck.h"
#include "MPRNG.h"

extern MPRNG mprng;

void BottlingPlant::main() {
    printer.print(Printer::BottlingPlant, 'S');
    Truck truck(printer, nameServer, *this, numVendingMachines, maxStockPerFlavour);
    while(true) {
        _Accept( ~BottlingPlant ) {
            is_closing = true;
            break;
        } _Else {
            yield(timeBetweenShipments);
            bottle_count = 0;
            _Accept( getShipment );
            printer.print(Printer::BottlingPlant, 'G', bottle_count);
            printer.print(Printer::BottlingPlant, 'P');
        }
    }

    try {
        _Accept( getShipment )
    } catch (uMutexFailure::RendezvousFailure &e) {}

    printer.print(Printer::BottlingPlant, 'F');
}

void BottlingPlant::getShipment(unsigned cargo[]) {
    if (is_closing) {
        _Throw Shutdown{};
    }
    for(unsigned i = 0; i < NUM_FLAVOURS; ++i) {
        unsigned amount = mprng(maxShippedPerFlavour);
        bottle_count += amount;
        cargo[i] = amount;
    }
}

BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
        unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
        unsigned int timeBetweenShipments ): printer(prt), nameServer(nameServer),
    numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour),
    maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments) {}
