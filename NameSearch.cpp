#include "NameSearch.hpp"
#include <dirent.h>
#include <cctype>
#include <iomanip>
#include <map>
#include <algorithm>

void NameSearch::searchByName(string folderPath, string name, string mode, string param) {
    transform(name.begin(), name.end(), name.begin(), ::tolower);

    if (mode == "-e") {
        int searchYear = stoi(param);
        if (searchYear < 1880 || searchYear > 2022) {
            cout << "Invalid year. Provide a year between 1880 and 2022." << endl;
            return;
        }

        int totalOccurrences = 0;
        string fileName = "yob" + to_string(searchYear) + ".txt";
        ifstream file(folderPath + "/" + fileName);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file " << fileName << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            size_t comma1 = line.find(',');
            size_t comma2 = line.rfind(',');

            string currentName = line.substr(0, comma1);
            transform(currentName.begin(), currentName.end(), currentName.begin(), ::tolower);

            int occurrences = stoi(line.substr(comma2 + 1));

            if (currentName == name) {
                totalOccurrences += occurrences;
            }
        }

        if (totalOccurrences > 0) {
            cout << "\nTotal occurrences for " << name << " in " << searchYear << ": " << totalOccurrences << "\n" << endl;
        } else {
            cout << "\nNo occurrences found for " << name << " in " << searchYear << ".\n" << endl;
        }
    } else if (mode == "-s") {
        string state = param;
        int totalOccurrences = 0;

        folderPath = "./namesbystate";

        vector<string> files = listFiles(folderPath);

        for (string fileName : files) {
            ifstream file(folderPath + "/" + fileName);
            if (!file.is_open()) {
                continue;
            }

            string line;
            while (getline(file, line)) {
                if (line.substr(0, 2) == state) {
                    size_t comma1 = line.find(',');
                    size_t comma2 = line.find(',', comma1 + 1);
                    size_t comma3 = line.find(',', comma2 + 1);
                    size_t comma4 = line.find(',', comma3 + 1);

                    string currentName = line.substr(comma3 + 1, comma4 - comma3 - 1);
                    transform(currentName.begin(), currentName.end(), currentName.begin(), ::tolower);

                    int occurrences = stoi(line.substr(comma4 + 1));

                    if (currentName == name) {
                        totalOccurrences += occurrences;
                    }
                }
            }
        }

        if (totalOccurrences > 0) {
            cout << "\nTotal occurrences for " << name << " in state " << state << ": " << totalOccurrences << "\n" << endl;
        } else {
            cout << "\nNo occurrences found for " << name << " in state " << state << ".\n" << endl;
        }
    } else {
        cout << "Invalid mode. Use '-e' for year or '-s' for state.\n" << endl;
    }
}

void NameSearch::searchByName(string folderPath, string name) {
    
    transform(name.begin(), name.end(), name.begin(), ::tolower);

    int totalOccurrences = 0;
    int totalYears = 0;
    int totalOccurrencesSince2010 = 0;
    int yearsSince2010 = 0;
    int mostPopularYear = 0;
    int mostPopularYearOccurrences = 0;

    vector<string> files = listFiles(folderPath);

    for (string fileName : files) {
        if (fileName.substr(0, 3) == "yob" && fileName.substr(fileName.size() - 4) == ".txt") {
            string yearStr = fileName.substr(3, 4);
            int year = stoi(yearStr);

            ifstream file(folderPath + "/" + fileName);
            if (!file.is_open()) {
                continue;
            }

            string line;
            while (getline(file, line)) {
                size_t comma1 = line.find(',');
                size_t comma2 = line.rfind(',');

                string currentName = line.substr(0, comma1);
                transform(currentName.begin(), currentName.end(), currentName.begin(), ::tolower);

                int occurrences = stoi(line.substr(comma2 + 1));

                if (currentName == name) {
                    totalOccurrences += occurrences;
                    totalYears++;

                    if (occurrences > mostPopularYearOccurrences) {
                        mostPopularYear = year;
                        mostPopularYearOccurrences = occurrences;
                    }

                    if (year >= 2010) {
                        totalOccurrencesSince2010 += occurrences;
                        yearsSince2010++;
                    }
                }
            }
        }
    }

    double averagePerYear = totalYears > 0 ? static_cast<double>(totalOccurrences) / totalYears : 0;
    double averageSince2010 = yearsSince2010 > 0 ? static_cast<double>(totalOccurrencesSince2010) / yearsSince2010 : 0;

    string popularity = (totalOccurrences > 100000) ? "Popular" : "Not Popular";

    cout << "\nMost popular year: " << mostPopularYear << ", " << mostPopularYearOccurrences << " babies" << endl;
    cout << "Average babies per year since 1880: " << static_cast<int>(averagePerYear) << endl;
    cout << "Average babies per year since 2010: " << static_cast<int>(averageSince2010) << endl;
    cout << "This name is " << popularity << endl;

    cout << "\n(Search again with -e year for an expanded view)" << endl;
    cout << "(Search again with -s state to show data for a specific state)\n" << endl;
}

void NameSearch::searchByName(string folderPath, string name, string mode, int year) {
    if (mode == "-e") {
        int startYear = year;
        int endYear = year; 
        if (startYear < 1880 || startYear > 2022) {
            cout << "Invalid year. Provide a year between 1880 and 2022." << endl;
            return;
        }

        vector<string> files = listFiles(folderPath);
        map<string, int> nameOccurrences;

        for (string fileName : files) {
            if (fileName.substr(0, 3) == "yob" && fileName.substr(fileName.size() - 4) == ".txt") {
                string yearStr = fileName.substr(3, 4);
                int fileYear = stoi(yearStr);
                if (fileYear == startYear) {
                    ifstream file(folderPath + "/" + fileName);
                    string line;

                    while (getline(file, line)) {
                        size_t comma1 = line.find(',');
                        size_t comma2 = line.rfind(',');

                        string currentName = line.substr(0, comma1);
                        int occurrences = stoi(line.substr(comma2 + 1));

                        nameOccurrences[currentName] += occurrences;
                    }
                }
            }
        }

        vector<pair<string, int>> sortedNames(nameOccurrences.begin(), nameOccurrences.end());
        sort(sortedNames.begin(), sortedNames.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
            return b.second > a.second;
        });

        cout << "\nThe most popular names that year were:\n";
        for (size_t i = 0; i < min(sortedNames.size(), size_t(5)); ++i) {
            cout << i + 1 << ". " << sortedNames[i].first << " | " << sortedNames[i].second << endl;
        }
        cout << endl;
    } else {
        cout << "Invalid mode for this overload. Use '-e' for year." << endl;
    }
}

void NameSearch::searchByYear(string folderPath, int year) {
    if (year < 1880 || year > 2022) {
        cout << "Invalid year. Provide a year between 1880 and 2022." << endl;
        return;
    }

    string fileName = "yob" + to_string(year) + ".txt";
    string filePath = folderPath + "/" + fileName;

    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filePath << endl;
        return;
    }

    map<string, int> nameOccurrences;
    string line;

    while (getline(file, line)) {
        size_t comma1 = line.find(',');
        size_t comma2 = line.rfind(',');

        string name = line.substr(0, comma1);
        string gender = line.substr(comma1 + 1, comma2 - comma1 - 1);
        int occurrences = stoi(line.substr(comma2 + 1));

        nameOccurrences[name] += occurrences;
    }

    file.close();

    vector<pair<string, int>> sortedNames(nameOccurrences.begin(), nameOccurrences.end());
    sort(sortedNames.begin(), sortedNames.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    cout << "\nThe most popular names in " << year << " were:\n";
    for (size_t i = 0; i < min(sortedNames.size(), size_t(5)); ++i) {
        cout << i + 1 << ". " << sortedNames[i].first << " | " << sortedNames[i].second << endl;
    }
    cout << endl;
}

void NameSearch::searchByNameAndYear(string folderPath, string name, int year) {
    transform(name.begin(), name.end(), name.begin(), ::tolower);

    if (year < 1880 || year > 2022) {
        cout << "Invalid year. Provide a year between 1880 and 2022." << endl;
        return;
    }

    vector<string> files = listFiles(folderPath);
    int totalOccurrences = 0;

    for (string fileName : files) {
        if (fileName.substr(0, 3) == "yob" && fileName.substr(fileName.size() - 4) == ".txt") {
            string yearStr = fileName.substr(3, 4);
            int fileYear = stoi(yearStr);
            if (fileYear == year) {
                ifstream file(folderPath + "/" + fileName);
                string line;

                while (getline(file, line)) {
                    size_t comma1 = line.find(',');
                    size_t comma2 = line.rfind(',');

                    string currentName = line.substr(0, comma1);
                    transform(currentName.begin(), currentName.end(), currentName.begin(), ::tolower);

                    int occurrences = stoi(line.substr(comma2 + 1));

                    if (currentName == name) {
                        totalOccurrences += occurrences;
                    }
                }
            }
        }
    }

    if (totalOccurrences > 0) {
        cout << "\nTotal occurrences for " << name << " in " << year << ": " << totalOccurrences << "\n" << endl;
    } else {
        cout << "\nNo occurrences found for " << name << " in " << year << ".\n" << endl;
    }
}

void NameSearch::searchByNameAndYear(string folderPath, string name, int startYear, int endYear) {
    transform(name.begin(), name.end(), name.begin(), ::tolower);

    if (startYear < 1880 || endYear > 2022 || startYear > endYear) {
        cout << "Invalid year range. Provide years between 1880 and 2022." << endl;
        return;
    }

    vector<string> files = listFiles(folderPath);

    cout << "\nCount by Year" << endl;
    for (int year = startYear; year <= endYear; ++year) {
        int totalOccurrences = 0;
        string fileName = "yob" + to_string(year) + ".txt";
        ifstream file(folderPath + "/" + fileName);
        if (!file.is_open()) continue;

        string line;
        while (getline(file, line)) {
            size_t comma1 = line.find(',');
            size_t comma2 = line.rfind(',');

            string currentName = line.substr(0, comma1);
            transform(currentName.begin(), currentName.end(), currentName.begin(), ::tolower);

            int occurrences = stoi(line.substr(comma2 + 1));

            if (currentName == name) {
                totalOccurrences += occurrences;
            }
        }
        cout << year << " : " << totalOccurrences << endl;
    }
    cout << endl;
}

void NameSearch::searchByState(string folderPath, string state) {
    map<string, int> nameOccurrences;
    vector<string> files = listFiles(folderPath);

    for (const string& fileName : files) {
        ifstream file(folderPath + "/" + fileName);
        if (!file.is_open()) continue;

        string line;
        while (getline(file, line)) {
            if (line.substr(0, 2) == state) {
                size_t comma1 = line.find(',');
                size_t comma2 = line.find(',', comma1 + 1);
                size_t comma3 = line.find(',', comma2 + 1);
                size_t comma4 = line.find(',', comma3 + 1);

                string name = line.substr(comma3 + 1, comma4 - comma3 - 1);
                int occurrences = stoi(line.substr(comma4 + 1));
                nameOccurrences[name] += occurrences;
            }
        }
    }

    vector<pair<string, int>> sortedNames(nameOccurrences.begin(), nameOccurrences.end());
    sort(sortedNames.begin(), sortedNames.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    cout << "\nThe most popular names in " << state << " are:\n";
    for (size_t i = 0; i < min(sortedNames.size(), size_t(5)); ++i) {
        cout << i + 1 << ". " << sortedNames[i].first << " | " << sortedNames[i].second << endl;
    }
    cout << endl;
}

vector<string> NameSearch::listFiles(string folderPath) {
    vector<string> files;
    DIR* dir = opendir(folderPath.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            string fileName = entry->d_name;
            if (fileName != "." && fileName != "..") {
                files.push_back(fileName);
            }
        }
        closedir(dir);
    } else {
        cout << "Error: Unable to open directory " << folderPath << endl;
    }
    return files;
}