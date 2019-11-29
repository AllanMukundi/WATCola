#include "bottling_plant.h"
#include "truck.h"
#include "MPRNG.h"

extern MPRNG mprng;

void BottlingPlant::main() {
    printer.print(Printer::BottlingPlant, 'S');
    Truck truck(printer, nameServer, *this, numVendingMachines, maxStockPerFlavour);
    while(true) {
        _Accept( ~BottlingPlant ) {
            isClosing = true;
            break;
        } _Else {
            yield(timeBetweenShipments);
            unsigned bottleCount = 0;
            for(unsigned i = 0; i < NUM_FLAVOURS; ++i) {
                shipment[i] = mprng(maxShippedPerFlavour);
                bottleCount += shipment[i];
            }
            printer.print(Printer::BottlingPlant, 'G', bottleCount);
            _Accept( getShipment );
        }
    }

    try {
        _Accept( getShipment )
    } catch (uMutexFailure::RendezvousFailure &e) {}

    printer.print(Printer::BottlingPlant, 'F');
}

void BottlingPlant::getShipment(unsigned cargo[]) {
    if (isClosing) {
        _Throw Shutdown{};
    }
    for(unsigned i = 0; i < NUM_FLAVOURS; ++i) {
        cargo[i] = shipment[i];
    }
    printer.print(Printer::BottlingPlant, 'P');
}

BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
        unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
        unsigned int timeBetweenShipments ): printer(prt), nameServer(nameServer),
    numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour),
    maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments) {}
