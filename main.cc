#include <iostream>
#include <sstream>

#include "bank.h"
#include "bottling_plant.h"
#include "config.h"
#include "groupoff.h"
#include "name_server.h"
#include "parent.h"
#include "printer.h"
#include "student.h"
#include "wat_card_office.h"
#include "vending_machine.h"
#include "MPRNG.h"

using namespace std;

MPRNG mprng;

// Helper function in case user put in the wrong argument type.
void usage(char *argv[]) {
	cerr << "Usage: " 
         << argv[0]
		 << " [ config-file [ random-seed (> 0) ] ]"
         << endl;
	exit(EXIT_FAILURE);
}

/**********************
 * get_cmd_line_arg - tries to interpret cmd line arg as an integer > 0
 *   
 * Inputs:          
 *  arg     - cmd line arg              
 *  storage - place to put converted cmd line arg 
 * Output: the read in value from argv
 *********************/
unsigned get_cmd_line_arg(char *arg, char *argv[]) {
    unsigned storage;
    istringstream iss(arg);
    if (iss >> storage) {
        if (storage <= 0 || storage > INT_MAX) {
            usage(argv);
        }                
    } else {                
        usage(argv);
    }
    return storage;                   
}

int main(int argc, char *argv[]) {
    // Default parameter values.
    string configFile = "soda.config";
    unsigned seed = getpid();

    // Read command line arguments.
    switch (argc) {
        // Seed
		case 3: {
            seed = get_cmd_line_arg(argv[2], argv);
        }
		case 2: {
            configFile = argv[1];
        }
		case 1:
			break;
		default:
			usage(argv);
	}

    // Set seed.
    mprng.set_seed(seed);

    // Read config file.
    ConfigParms config;
    processConfigFile(configFile.c_str(), config);

    // Let's go.
    Printer printer(config.numStudents, config.numVendingMachines, config.numCouriers);
    Bank bank(config.numStudents); 
    Parent parent(printer, bank, config.numStudents, config.parentalDelay);
    WATCardOffice office(printer, bank, config.numCouriers);
    Groupoff groupoff(printer, config.numStudents, config.sodaCost, config.groupoffDelay);
    NameServer nameserver(printer, config.numVendingMachines, config.numStudents);

    VendingMachine *machines[config.numVendingMachines];
    for(unsigned i = 0; i < config.numVendingMachines; ++i) {
        machines[i] = new VendingMachine(printer, nameserver, i, config.sodaCost);
    }

    BottlingPlant *plant = new BottlingPlant(printer, nameserver, config.numVendingMachines, config.maxShippedPerFlavour,
            config.maxStockPerFlavour, config.timeBetweenShipments);

    Student *students[config.numStudents];
    for(unsigned i = 0; i < config.numStudents; ++i) {
        students[i] = new Student(printer, nameserver, office, groupoff, i, config.maxPurchases);
    }

    for(unsigned i = 0; i < config.numStudents; ++i) {
        delete students[i];
    }

    delete plant;

    for(unsigned i = 0; i < config.numVendingMachines; ++i) {
        delete machines[i];
    }
}
