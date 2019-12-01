#pragma once

#include "printer.h"
#include "name_server.h"
#include "wat_card_office.h"
#include "groupoff.h"


_Task Student {
    const unsigned NUM_FLAVORS = 4;
    Printer &printer;
    NameServer &nameServer;
    WATCardOffice &cardOffice;
    Groupoff &groupoff;
    unsigned id, maxPurchases;

    // "If the vending machine delivers a free bottle of soda
    // ...must watch an advertisement by yielding 4 times."
    void freeDrink(char c, VendingMachine::Flavours favFlavour, 
                   unsigned balance);

    // "If the vending machine indicates insufficient funds, a
    // student transfers the current vending-machine soda-cost
    // plus $5 to their WATCard via the WATCard office and 
    // attempts another purchase.""
    void insufficientFunds(VendingMachine *vendingMachine, WATCard *watCard);

    // "If the vending machine is out of the student’s 
    // favourite flavour, the student must obtain a new
    // vending machine from the name server and attempt
    // another purchase. (Hence, a student may busy wait among
    // vending machines until their specific soda appears from 
    // the bottling plant.)"
    void outOfStock(VendingMachine *&vendingMachine);

    // "create a new WATCard...with a $5 balance, and 
    // re-attempt to buy a soda but without yielding"
    void lostWatCard(WATCard::FWATCard &watCard);

    void main();
  public:
    Student(Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, 
            Groupoff &groupoff, unsigned int id, unsigned int maxPurchases);
};
