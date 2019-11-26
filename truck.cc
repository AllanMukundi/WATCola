#include "MPRNG.h"
#include "truck.h"
#include "vending_machine.h"
#include <limits.h>

extern MPRNG mprng;

Truck::Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
             unsigned int numVendingMachines, unsigned int maxStockPerFlavour):
                printer(prt), nameServer(nameServer), plant(plant),
                numVendingMachines(numVendingMachines), 
                maxStockPerFlavour(maxStockPerFlavour) {}

void Truck::main() {
    printer.print(Printer::Truck, 'S');

    VendingMachine **vendingMachines = nameServer.getMachineList();
    
    // Keep track of the last machine we served. Necessary because of the
    // following in the assignment:
    //      "To ensure fairness, the vending machines are restocked in cyclic 
    //       order starting at the vending machine after the last machine the
    //       truck restocked"
    // Start at -1 (with underflow) so that the first machine will be indexed
    // to 0.
    unsigned nextMachine = -1;
    unsigned truckStorage[NUM_FLAVORS]; // Has the bottles that we will ship.

    while (true) {
        // the truck yields a random number of times [1, 10] 
        // to get a coffee from Tom Hortons.
        yield(mprng(1, 10));

        // The truck then obtains a new shipment of soda; any soda still
        // on the truck is thrown away as it is past its due date.
        // If the bottling plant is closing down, the truck terminates 
        // (error thrown).
        try {
            plant.getShipment(truckStorage);
            printer.print(Printer::Truck, 'P');
        } catch (BottlingPlant::Shutdown &) {
            break;
        }

        // Keep track of total since machines are restocked
        // "until there is no more soda on the truck".
        unsigned totalBottles = 0;
        for (unsigned i = 0; i < NUM_FLAVORS; i++) {
            totalBottles += truckStorage[i];
        }


        // Start restocking. Note that "there is no guarantee each 
        // vending machine is completely restocked or the entire complement 
        // of vending machines is restocked or all the soda on the truck is used"
        // As we only loop around once through the machines.
        for (unsigned i = 0; i < numVendingMachines; i++) {
            if (totalBottles == 0) 
                break;

            nextMachine = (nextMachine + 1) % numVendingMachines;
            unsigned *inventory = vendingMachines[nextMachine]->inventory();
            
            // d v,r
            // begin delivery to vending machine vending machine v, 
            // total amount remaining r in the shipment.
            printer.print(Printer::Truck, 'd', nextMachine, totalBottles);

            // Restock the machine.
            unsigned missing = 0; // Keep track of left overs for print statement.
            for (unsigned j = 0; j < NUM_FLAVORS; j++) {
                // Figure out how many bottles to add, and update the right bins.
                unsigned bottlesAdded = std::min(truckStorage[j], maxStockPerFlavour - inventory[j]);
                inventory[j] += bottlesAdded;
                truckStorage[j] -= bottlesAdded;

                totalBottles -= bottlesAdded;

                // See if there is still some room in the vending machine. 
                // Necessary for print statement.
                missing += maxStockPerFlavour - inventory[j];
            }

            // "the truck calls restocked to indicate the operation is complete."
            vendingMachines[nextMachine]->restocked();

            if (missing > 0) {
                // U v,b 
                // unsuccessfully filled vending machine vending machine v, 
                // total number of bottles b not replenished.
                printer.print(Printer::Truck, 'U', nextMachine, missing);
            }

            // D v,r 
            // end delivery to vending machine vending machine v, 
            // total amount remaining r in the shipment.
            printer.print(Printer::Truck, 'D', nextMachine, totalBottles);
        }

        // F 
        // finished.
        printer.print(Printer::Truck, 'F');
    }

}
