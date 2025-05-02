#include <iostream>
#include "NameSearch.hpp"
using namespace std;

void displayMenu() {
    cout << "Menu Options:" << endl;
    cout << "1. Search by Name" << endl;
    cout << "2. Search by Year" << endl;
    cout << "3. Search by State" << endl;
    cout << "4. Quit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    int choice;
    string folderPath = "./names"; 

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter the name to search: ";
                string name;
                cin >> name;
                NameSearch::searchByName(folderPath, name);
                break;
            }
            case 2:
                cout << "You selected: Search by Year" << endl;
              
                break;
            case 3:
                cout << "You selected: Search by State" << endl;
              
                break;
            case 4:
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 4);

    return 0;
}