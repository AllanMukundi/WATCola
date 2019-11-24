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

// Convert a char to a unsigned int.
unsigned unsignedIntConvert(char *arg, char *argv[]) {
    try {
        if (stoi(arg) < 0) {
            // Don't allow negatives.
            throw 1;
        }
        return stoi(arg);
    } catch (...) {
        usage(argv); 
    }
    return 0;
}

int main(int argc, char *argv[]) {
    // Default parameter values.
    string configFile = "soda.config";
    unsigned seed = getpid();

    // Read command line arguments.
    switch (argc) {
        // Seed
		case 3: {
            seed = unsignedIntConvert(argv[2], argv);
            if (seed == 0)
                usage(argv);
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
