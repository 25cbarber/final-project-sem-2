#include <iostream>
#include "NameSearch.hpp"
using namespace std;

void displayMenu() {
    cout << "1. Search by Name" << endl;
    cout << "2. Search by Year" << endl;
    cout << "3. Search by State" << endl;
    cout << "4. Quit" << endl;
    cout << "Select an Option from the Menu Above: ";
}

int main() {
    int choice;
    string folderPath = "./names"; 
    string input, name, mode;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter a name: ";
                cin.ignore();
                getline(cin, input);

                size_t spacePos = input.find(' ');
                if (spacePos == string::npos) {
                    NameSearch::searchByName(folderPath, input);
                    break;
                }

                name = input.substr(0, spacePos);
                mode = input.substr(spacePos + 1);

                if (mode.find("-e") == 0) {
                    size_t yearPos = mode.find(' ');
                    if (yearPos == string::npos) {
                        cout << "Invalid input. Please provide a year after '-e'." << endl;
                        break;
                    }
                    string yearStr = mode.substr(yearPos + 1);
                    try {
                        int year = stoi(yearStr);
                        NameSearch::searchByNameAndYear(folderPath, name, year); // Call the new function
                    } catch (invalid_argument&) {
                        cout << "Invalid year. Please provide a valid year." << endl;
                    }
                } else if (mode.find("-s") == 0) {
                    size_t statePos = mode.find(' ');
                    if (statePos == string::npos) {
                        cout << "Invalid input. Please provide a state after '-s'." << endl;
                        break;
                    }
                    string state = mode.substr(statePos + 1);
                    if (state.length() != 2) {
                        cout << "Invalid state. Please provide a 2-character state ID." << endl;
                        break;
                    }
                    state[0] = toupper(state[0]);
                    state[1] = toupper(state[1]);
                    NameSearch::searchByName(folderPath, name, "-s", state);
                } else {
                    cout << "Invalid mode. Please use '-e' for year or '-s' for state." << endl;
                }
                break;
            }
            case 2: {
                cout << "Enter a valid year between 1880 and 2022: ";
                int year;
                cin >> year;

                if (year < 1880 || year > 2022) {
                    cout << "Invalid date entered. To search by year, please enter a valid year between 1880 and 2022." << endl;
                } else {
                    NameSearch::searchByYear(folderPath, year); // Call the new function
                }
                break;
            }
            case 3:
                cout << "You selected: Search by State" << endl;
                cout << "This functionality is not implemented yet." << endl;
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