#pragma once

_Task Truck {
	// Blues Black-Cherry, Classic Cream-Soda, Rock Root-Beer, and Jazz Lime.
	const unsigned NUM_FLAVORS = 4;

	Printer &printer;
	NameServer &nameServer;
	BottlingPlant &plant
	const unsigned numVendingMachines, maxStockPerFlavour;
	
	void main();
  public:
	Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
		  unsigned int numVendingMachines, unsigned int maxStockPerFlavour);
};
