#include <iomanip>
#include <iostream>
#include <string>

#include "printer.h"

using namespace std;

/**********************
 * details - determines if printer column contains valid information
 *
 * Inputs:
 *  column - printer column
 *
 * Output: true if valid column, false otherwise
 *********************/
bool Printer::details(Info &column) {
    return !(column.id == Undefined);
}

/**********************
 * translate - gets the appropriate index in columns for an entity
 *
 * Inputs:
 *  kind - type of entity
 *  lid  - identifier for a type with (potentially) multiple instances
 *
 * Output: column index
 *
 * Note: A lot of the cases below could be eliminated by casting kind to an int, but we opted to
 *       make the printer more robust by separating out each case because that solution would rely
 *       on the order of elements in enum Kind to never change
 *********************/
unsigned Printer::translate(Kind kind, unsigned lid) {
    switch (kind) {
        case Parent:
            return 0;
        case Groupoff:
            return 1;
        case WATCardOffice:
            return 2;
        case NameServer:
            return 3;
        case Truck:
            return 4;
        case BottlingPlant:
            return 5;
        case Student:
            return 6+lid;
        case Vending:
            return 6+numStudents+lid;
        default:
            return 6+numStudents+numVendingMachines+lid;
    }
}

/**********************
 * flush - print stored information in buffer and clear all current information.
 *********************/
void Printer::flush() {
    int tmp_cnt = cnt;
    cnt = 0;
    for(unsigned i = 0; i < totalCols; ++i) {
        if (details(columns[i])) {
            cout << columns[i].state;
            if (columns[i].value1 != INVALID_VALUE) {
                cout << columns[i].value1;
                if (columns[i].value2 != INVALID_VALUE) {
                    cout << "," << columns[i].value2;
                }
            }
            tmp_cnt--;
        }

        columns[i].id = Undefined;

        if (tmp_cnt == 0) {
            break;
        }

        if (i < totalCols-1) {
            cout << "\t";
        }
    }
    cout << endl;
}

void Printer::print( Kind kind, char state ) {
    unsigned idx = translate(kind);
    if (prev == idx || columns[idx].id != Undefined) {
        flush();
    }
    curr.id = idx;
    curr.lid = INVALID_VALUE;
    curr.state = state;
    curr.value1 = INVALID_VALUE;
    curr.value2 = INVALID_VALUE;
    columns[idx] = curr;
    prev = idx;
    cnt++;
}

void Printer::print( Kind kind, char state, unsigned value1 ) {
    unsigned idx = translate(kind);
    if (prev == idx || columns[idx].id != Undefined) {
        flush();
    }
    curr.id = idx;
    curr.lid = INVALID_VALUE;
    curr.state = state;
    curr.value1 = value1;
    curr.value2 = INVALID_VALUE;
    columns[idx] = curr;
    prev = idx;
    cnt++;
}

void Printer::print( Kind kind, char state, unsigned value1, unsigned value2 ) {
    unsigned idx = translate(kind);
    if (prev == idx || columns[idx].id != Undefined) {
        flush();
    }
    curr.id = idx;
    curr.lid = INVALID_VALUE;
    curr.state = state;
    curr.value1 = value1;
    curr.value2 = value2;
    columns[idx] = curr;
    prev = idx;
    cnt++;
}

void Printer::print( Kind kind, unsigned lid, char state ) {
    unsigned idx = translate(kind, lid);
    if (prev == idx || columns[idx].id != Undefined) {
        flush();
    }
    curr.id = idx;
    curr.lid = lid;
    curr.state = state;
    curr.value1 = INVALID_VALUE;
    curr.value2 = INVALID_VALUE;
    columns[idx] = curr;
    prev = idx;
    cnt++;
}

void Printer::print( Kind kind, unsigned lid, char state, unsigned value1 ) {
    unsigned idx = translate(kind, lid);
    if (prev == idx || columns[idx].id != Undefined) {
        flush();
    }
    curr.id = idx;
    curr.lid = lid;
    curr.state = state;
    curr.value1 = value1;
    curr.value2 = INVALID_VALUE;
    columns[idx] = curr;
    prev = idx;
    cnt++;
}

void Printer::print( Kind kind, unsigned lid, char state, unsigned value1, unsigned value2 ) {
    unsigned idx = translate(kind, lid);
    if (prev == idx || columns[idx].id != Undefined) {
        flush();
    }
    curr.id = idx;
    curr.lid = lid;
    curr.state = state;
    curr.value1 = value1;
    curr.value2 = value2;
    columns[idx] = curr;
    prev = idx;
    cnt++;
}

Printer::Printer( unsigned numStudents, unsigned numVendingMachines, unsigned numCouriers ):
    numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers),
    Undefined(numStudents + numVendingMachines + numCouriers + 6) // 6 other entities besides those added
{
    totalCols = Undefined;
    columns = new Info[totalCols];

    // Initialize undefined state 
    for (unsigned i = 0; i < totalCols; ++i) {
        columns[i].id = Undefined;
    }
    // Print header
    cout << "Parent\t"
         << "Gropoff\t"
         << "WATOff\t"
         << "Names\t"
         << "Truck\t"
         << "Plant\t";
    for (unsigned i = 0; i < numStudents; ++i) {
        cout << "Stud" << i << "\t";
    }
    for (unsigned i = 0; i < numVendingMachines; ++i) {
        cout << "Mach" << i << "\t";
    }
    for (unsigned i = 0; i < numCouriers; ++i) {
        cout << "Cour" << i;
        if (i < numCouriers-1) {
            cout << "\t";
        }
    }
    cout << endl;
    // Print asterisks
    for (unsigned i = 0; i < totalCols; ++i) {
        cout << "*******";
        if (i < totalCols-1) {
            cout << "\t";
        }
    }
    cout << endl;
}

Printer::~Printer() {
    flush(); // there could still be an unprinted row that was never prompted b/c no more input
    delete[] columns;
    cout << "***********************" << endl;
}
