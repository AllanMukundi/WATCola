#pragma once

_Monitor Printer {
    const unsigned numStudents, numVendingMachines, numCouriers, Undefined;
    const unsigned INVALID_VALUE = UINT_MAX;
    unsigned totalCols;
    unsigned cnt = 0;                // number of filled buffer elements
    unsigned prev = UINT_MAX;        // previous inserter into the buffer

    // Defines information that needs to be tracked
    struct Info {
        unsigned id;
        unsigned lid;
        char state;
        unsigned value1;
        unsigned value2;
    } curr, *columns;

    bool details(Info &column);
    void flush();
  public:
	enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
	Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
	void print( Kind kind, char state );
	void print( Kind kind, char state, unsigned int value1 );
	void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
	void print( Kind kind, unsigned int lid, char state );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );
  private: 
    unsigned int translate(Kind kind, unsigned int lid = 0);
};
