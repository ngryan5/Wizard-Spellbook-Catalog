/*
 * Author Ryan Nguyen
 * 
 * Assignment 2: Wizard Spellbook Catalog
 * 
 * This program is designed to manage a catalog of spellbooks and spells for a wizarding school.
 * It reads data from two input files containing wizard and spellbook information, and allows 
 * users to log in using their wizard ID and password. Once logged in, users can search for spellbooks 
 * by name, spells by effect, or display all spellbook information. The program restricts access 
 * to certain dangerous spells for students, and offers options to output data to the screen or a file.
 * 
 * The program uses dynamic memory allocation for storing spellbook and spell data, and ensures
 * that file I/O is properly handled. It also emphasizes good software design, including modular
 * functions, file separation, and memory management.
 */

#include "catalog.hpp"
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    string wizardFile = "wizards.txt";
    string spellbookFile = "spellbooks.txt";

    // Login wizard
    int numWizards = 0;
    wizard* wizards = initializeWizards(wizardFile, numWizards);
    if (wizards == nullptr) {
        return 1;
    }

    wizard* loggedInWizard = authenticateWizard(wizards, numWizards);
    if (loggedInWizard == nullptr) {
        delete[] wizards; // Clean up wizard memory before exiting
        return 1;
    }

    // Display wizard information after login
    displayWizardInfo(loggedInWizard);

    // Initialize spellbooks
    int numSpellbooks;
    spellbook* spellbooks = initializeSpellbooks(spellbookFile, numSpellbooks);
    if (spellbooks == nullptr) {
        delete[] wizards; // Clean up wizard memory
        return 1;
    }

    // Main menu loop
    int choice;
    do {
        choice = displayMenu();
        handleMenuSelection(choice, spellbooks, numSpellbooks, loggedInWizard);
    } while (choice != 4);

    // Clean up memory
    delete_spellbooks(spellbooks, numSpellbooks);
    delete[] wizards;

    return 0;
}
