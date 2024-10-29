#include "catalog.hpp"
#include <iostream>
using namespace std;

// TODO Function definitions go here

void displayWizardInfo(const wizard* loggedInWizard) {
    // Display wizard details including name, ID, position, and beard length
    cout << "Wizard Information:" << endl;
    cout << "Name: " << loggedInWizard->name << endl;
    cout << "ID: " << loggedInWizard->id << endl;
    cout << "Position: " << loggedInWizard->position_title << endl;
    cout << "Beard Length: " << loggedInWizard->beard_length << " inches" << endl;
}

string getValidatedSpellEffect(const wizard* loggedInWizard) {
    string effect;
    do {
        // Prompt user to enter a spell effect
        cout << "Enter spell effect to search: ";
        cin >> effect;  

        // Check if the effect is restricted for the wizard
        if (isRestrictedEffect(loggedInWizard, effect)) {
            cout << "Students are not allowed to search for spells with poison or death effects. Please enter a different effect." << endl;
        } else {
            break;
        }
    } while (true);
    return effect;
}


bool isRestrictedEffect(const wizard* loggedInWizard, const string& effect) {
    // Restrict students from accessing poison or death spells
    if (loggedInWizard->position_title == "Student" && 
        (effect == "poison" || effect == "death")) {
        return true;
    }
    return false;
}

void handleMenuSelection(int choice, spellbook* spellbooks, int numSpellbooks, const wizard* loggedInWizard) {
    string input;

    switch (choice) {
        case 1:
            // Show all spellbooks and spells, filtered if the user is a student
            if (loggedInWizard->position_title == "Student") {
                showFilteredSpellbooks(spellbooks, numSpellbooks, loggedInWizard->position_title);
            } else {
                showAllSpellbooks(spellbooks, numSpellbooks);
            }
            break;
        case 2:
            // Search for a spellbook by title
            cout << "Enter spellbook title to search: ";
            cin.ignore();  // To ignore the newline character after the previous input
            getline(cin, input);
            searchSpellbookByTitle(spellbooks, numSpellbooks, input);
            break;
        case 3:
            // Search spells by effect
            input = getValidatedSpellEffect(loggedInWizard);
            searchSpellByEffect(spellbooks, numSpellbooks, input);
            break;
        case 4:
            // Quit the program
            cout << "Quitting program." << endl;
            break;
        default:
            // Handle invalid menu choice
            cout << "Invalid choice. Please try again." << endl;
    }
}

void showFilteredSpellbooks(spellbook* spellbooks, int size, const string& position_title) {
    // Loop through each spellbook to display information
    for (int i = 0; i < size; ++i) {
        cout << "Spellbook: " << spellbooks[i].title 
             << " by " << spellbooks[i].author 
             << ", Pages: " << spellbooks[i].num_pages 
             << ", Edition: " << spellbooks[i].edition 
             << ", Average Success Rate: " << spellbooks[i].avg_success_rate << "%" << endl;
        
        // Loop through each spell in the spellbook
        for (int j = 0; j < spellbooks[i].num_spells; ++j) {
            if (position_title == "Student" && 
                (spellbooks[i].spells[j].effect == "poison" || spellbooks[i].spells[j].effect == "death")) {
                // Skip spells with "poison" or "death" effects for students
                continue;
            }
            // Display spell details
            cout << "  Spell: " << spellbooks[i].spells[j].name 
                 << " - Effect: " << spellbooks[i].spells[j].effect 
                 << ", Success Rate: " << spellbooks[i].spells[j].success_rate << "%" << endl;
        }
    }
}


// Function: showAllSpellbooks (Unfiltered version)
void showAllSpellbooks(spellbook* spellbooks, int size) {
    // Simple bubble sort to sort spellbooks by average success rate in descending order
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (spellbooks[j].avg_success_rate < spellbooks[j + 1].avg_success_rate) {
                // Swap spellbooks[j] and spellbooks[j + 1]
                spellbook temp = spellbooks[j];
                spellbooks[j] = spellbooks[j + 1];
                spellbooks[j + 1] = temp;
            }
        }
    }

    // Loop through each spellbook to display information
    for (int i = 0; i < size; ++i) {
        cout << "Spellbook: " << spellbooks[i].title 
             << " by " << spellbooks[i].author 
             << ", Pages: " << spellbooks[i].num_pages 
             << ", Edition: " << spellbooks[i].edition 
             << ", Average Success Rate: " << spellbooks[i].avg_success_rate << "%" << endl;

        // Loop through each spell in the spellbook
        for (int j = 0; j < spellbooks[i].num_spells; ++j) {
                 // Display spell details
            cout << "  Spell: " << spellbooks[i].spells[j].name 
                 << " - Effect: " << spellbooks[i].spells[j].effect 
                 << ", Success Rate: " << spellbooks[i].spells[j].success_rate << "%" << endl;
        }
    }
}

spell* create_spells(int size) {
    // Allocate a dynamic array of spells of the given size
    spell* spells = new spell[size];
    
    // Check if allocation was successful
    if (!spells){
        cout << "Error: Memory allocation for spells array failed." << endl;
        return nullptr;
    }

    return spells;
}

spellbook* create_spellbooks(int size){
    // Allocate a dynamic array of spellbooks of the given size
    spellbook* spellbooks = new spellbook[size];

    // Check if allocation was successful
    if (!spellbooks){
        cout << "Error: Memory allocation for spells array failed." << endl;
        return nullptr;
    }

    return spellbooks;
}

spell read_spell_data(ifstream& file) {
    spell sp;
    // Read spell details from the file
    file >> sp.name >> sp.success_rate >> sp.effect;
    return sp;
} 

spellbook read_spellbook_data(ifstream& file) {
    spellbook sb;

    // Read spellbook details
    file >> sb.title; 
    file >> sb.author; 
    file >> sb.num_pages; 
    file >> sb.edition;
    file >> sb.num_spells;

    // Allocate memory for spells in the spellbook
    sb.spells = create_spells(sb.num_spells);
    if (sb.spells == nullptr) {
        cout << "Error: Unable to allocate memory for spells in spellbook." << endl;
        return sb;
    }

    // Read spells and calculate the average success rate
    float total_success_rate = 0.0;
    for (int i = 0; i < sb.num_spells; ++i) {
        file >> sb.spells[i].name >> sb.spells[i].success_rate >> sb.spells[i].effect;
        total_success_rate += sb.spells[i].success_rate;
    }
    sb.avg_success_rate = total_success_rate / sb.num_spells;

    return sb;
}

void delete_spells(spell*& spells) {
    // Deallocate memory and set pointer to nullptr
    delete[] spells;
    spells = nullptr;
}

void delete_spellbooks(spellbook*& spellbooks, int size){
    for (int i = 0; i < size; i++) {
        delete_spells(spellbooks[i].spells);
    }

    // Deallocate memory for spellbooks and set pointer to nullptr
    delete[] spellbooks;
    spellbooks = nullptr;
}

wizard* initializeWizards(const string& wizardFile, int& numWizards) {
    // Open the wizard data file
    ifstream file(wizardFile);
    if (!file) {
        cout << "Error: Unable to open wizard file." << endl;
        return nullptr;
    }

    // Read number of wizards and allocate memory
    file >> numWizards;
    wizard* wizards = new wizard[numWizards];

    // Read each wizard's details from the file
    for (int i = 0; i < numWizards; ++i) {
        file >> wizards[i].name;
        file >> wizards[i].id;
        file >> wizards[i].password;
        file >> wizards[i].position_title;
        file >> wizards[i].beard_length;
    }

    return wizards;
}

bool validateWizardID(const wizard* wizards, int numWizards, int id) {
    for (int i = 0; i < numWizards; ++i) {
        if (wizards[i].id == id) {
            return true;
        }
    }
    return false;
}

wizard* authenticateWizard(wizard* wizards, int numWizards) {
    int attempts = 0;
    while (attempts < 3) {
        int id;
        string password;

        // Prompt user for ID
        cout << "Enter your wizard ID: ";
        cin >> id;

        // Validate ID
        if (!validateWizardID(wizards, numWizards, id)) {
            cout << "Error: Invalid wizard ID. Please try again." << endl;
            attempts++;
            continue;
        }

        // Prompt user for password
        cout << "Enter your password: ";
        cin >> password;

        // Check credentials against wizard data
        for (int i = 0; i < numWizards; ++i) {
            if (wizards[i].id == id && wizards[i].password == password) {
                cout << "Welcome, " << wizards[i].name << "!" << endl;
                return &wizards[i];
            }
        }

        // Inform user of incorrect credentials and increment attempts
        cout << "Incorrect password. Please try again." << endl;
        attempts++;
    }

    cout << "Error: Maximum login attempts exceeded." << endl;
    return nullptr;
}

void searchSpellbooks(spellbook* spellbooks, int size) {
    string searchTitle;
    // Prompt user for spellbook title
    cout << "Enter the title of the spellbook you want to search for: ";
    cin >> searchTitle;

    // Loop through spellbooks to find a match
    for (int i = 0; i < size; ++i) {
        if (spellbooks[i].title == searchTitle) {
            // Display details of the found spellbook
            cout << "Spellbook found: " << spellbooks[i].title << " by " << spellbooks[i].author << endl;
            cout << "Number of pages: " << spellbooks[i].num_pages << ", Edition: " << spellbooks[i].edition << endl;
            cout << "Average success rate: " << spellbooks[i].avg_success_rate << "%" << endl;
            return;
        }
    }

    // Inform user if spellbook was not found
    cout << "Spellbook not found." << endl;
}

spellbook* initializeSpellbooks(const string& spellbookFile, int& numSpellbooks) {
    // Open the spellbook data file
    ifstream file(spellbookFile);
    if (!file) {
        cout << "Error: Unable to open spellbook file." << endl;
        return nullptr;
    }

    // Read number of spellbooks and allocate memory
    file >> numSpellbooks;
    spellbook* spellbooks = new spellbook[numSpellbooks];

    // Read each spellbook's details from the file
    for (int i = 0; i < numSpellbooks; ++i) {
        spellbooks[i] = read_spellbook_data(file);
    }
    return spellbooks;
}

void searchSpellbookByTitle(spellbook* spellbooks, int size, const string& title) {
    bool found = false;
    // Loop through spellbooks to find a match by title
    for (int i = 0; i < size; ++i) {
        if (spellbooks[i].title == title) {
                // Display details of the found spellbook
            cout << "Found Spellbook: " << spellbooks[i].title 
                 << " by " << spellbooks[i].author 
                 << ", Pages: " << spellbooks[i].num_pages 
                 << ", Edition: " << spellbooks[i].edition 
                 << ", Average Success Rate: " << spellbooks[i].avg_success_rate << "%" << endl;
            found = true;
            break;
        }
    }
    // Inform user if spellbook was not found
    if (!found) {
        cout << "No spellbook found with the title: " << title << endl;
    }
}

void searchSpellByEffect(spellbook* spellbooks, int size, const string& effect) {
    bool found = false;
    // Loop through each spellbook and spell to find a match by effect
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < spellbooks[i].num_spells; ++j) {
            if (spellbooks[i].spells[j].effect == effect) {
                // Display details of the found spell
                cout << "Found Spell: " << spellbooks[i].spells[j].name 
                     << " in Spellbook: " << spellbooks[i].title 
                     << " - Effect: " << spellbooks[i].spells[j].effect 
                     << ", Success Rate: " << spellbooks[i].spells[j].success_rate << "%" << endl;
                found = true;
            }
        }
    }
    // Inform user if no spells were found with the given effect
    if (!found) {
        cout << "No spells found with the effect: " << effect << endl;
    }
}

int displayMenu() {
    // Display the menu options
    int choice;
    cout << "\nMenu:\n";
    cout << "1. Show All Spellbooks and Spells\n";
    cout << "2. Search Spellbooks by Title\n";
    cout << "3. Search Spells by Effect\n";
    cout << "4. Quit\n";
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}
