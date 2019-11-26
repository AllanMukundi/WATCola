#include <iostream>
#include "MPRNG.h"
#include "config.h"

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
unsigned get_cmd_line_arg(char *arg, char *argv) {
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
}
