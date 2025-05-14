#include <iostream>
#include "NameSearch.hpp"
#include <limits>
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

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again." << endl;
            continue;
        }

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
                        cout << "Invalid input. Provide a year after '-e'.\n" << endl;
                        break;
                    }
                    string yearStr = mode.substr(yearPos + 1);
                    try {
                        int year = stoi(yearStr);
                        NameSearch::searchByNameAndYear(folderPath, name, year, 2022);
                    } catch (invalid_argument&) {
                        cout << "Invalid year. Provide a valid year." << endl;
                    }
                } else if (mode.find("-s") == 0) {
                    size_t statePos = mode.find(' ');
                    if (statePos == string::npos) {
                        cout << "Invalid input. Provide a state after '-s'.\n" << endl;
                        break;
                    }
                    string state = mode.substr(statePos + 1);
                    if (state.length() != 2) {
                        cout << "Invalid state. Provide state abbriviation." << endl;
                        break;
                    }
                    state[0] = toupper(state[0]);
                    state[1] = toupper(state[1]);
                    NameSearch::searchByName(folderPath, name, "-s", state);
                } else {
                    cout << "Invalid mode. Use '-e' for year or '-s' for state." << endl;
                }
                break;
            }
            case 2: {
                cout << "Enter a valid year between 1880 and 2022: ";
                int year;
                cin >> year;

                if (year < 1880 || year > 2022) {
                    cout << "Invalid date entered. To search by year, enter a valid year between 1880 and 2022." << endl;
                } else {
                    NameSearch::searchByYear(folderPath, year);
                }
                break;
            }
            case 3: {
                cout << "Enter a state abbreviation (e.g. NY): ";
                string state;
                cin >> state;
                if (state.length() != 2) {
                    cout << "Invalid state. Provide a state abbriviation" << endl;
                    break;
                }
                state[0] = toupper(state[0]);
                state[1] = toupper(state[1]);
                NameSearch::searchByState("namesbystate", state);
                break;
            }
            case 4:
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 4);

    return 0;
}