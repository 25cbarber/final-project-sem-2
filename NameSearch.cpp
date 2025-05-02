#include "NameSearch.hpp"
#include <dirent.h>
#include <cctype>
#include <iomanip>
#include <map>

void NameSearch::searchByName(string folderPath, string name, string mode, string param) {
    if (mode == "-e") {
        int startYear = stoi(param);
        int endYear = 2022;
        if (startYear < 1880 || startYear > endYear) {
            cout << "Invalid year. Please provide a year between 1880 and " << endYear << "." << endl;
            return;
        }

        int totalOccurrences = 0;
        vector<string> files = listFiles(folderPath);

        for (string fileName : files) {
            if (fileName.substr(0, 3) == "yob" && fileName.substr(fileName.size() - 4) == ".txt") {
                string year = fileName.substr(3, 4);
                int yearInt = stoi(year);
                if (yearInt >= startYear) {
                    ifstream file(folderPath + "/" + fileName);
                    string line;

                    while (getline(file, line)) {
                        size_t comma1 = line.find(',');
                        size_t comma2 = line.rfind(',');

                        string currentName = line.substr(0, comma1);
                        int occurrences = stoi(line.substr(comma2 + 1));

                        if (currentName == name) {
                            totalOccurrences += occurrences;
                        }
                    }
                }
            }
        }

        cout << "\nTotal occurrences for " << name << " from " << startYear << " to " << endYear << ": " << totalOccurrences << "\n" << endl;
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
        cout << "Invalid mode. Please use '-e' for year or '-s' for state." << endl;
    }
}

void NameSearch::searchByName(string folderPath, string name) {
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
    cout << "(Search again with -s state to show data for a specific state)" << endl;
}

void NameSearch::searchByName(string folderPath, string name, string mode, int year) {
    if (mode == "-e") {
        int startYear = year;
        int endYear = 2022;
        if (startYear < 1880 || startYear > endYear) {
            cout << "Invalid year. Please provide a year between 1880 and " << endYear << "." << endl;
            return;
        }

        vector<string> files = listFiles(folderPath);
        map<int, int> yearOccurrences;

        for (string fileName : files) {
            if (fileName.substr(0, 3) == "yob" && fileName.substr(fileName.size() - 4) == ".txt") {
                string yearStr = fileName.substr(3, 4);
                int fileYear = stoi(yearStr);
                if (fileYear >= startYear) {
                    ifstream file(folderPath + "/" + fileName);
                    string line;

                    while (getline(file, line)) {
                        size_t comma1 = line.find(',');
                        size_t comma2 = line.rfind(',');

                        string currentName = line.substr(0, comma1);
                        int occurrences = stoi(line.substr(comma2 + 1));

                        if (currentName == name) {
                            yearOccurrences[fileYear] += occurrences;
                        }
                    }
                }
            }
        }

        cout << "\nCount by Year" << endl;
        for (int y = startYear; y <= endYear; ++y) {
            if (yearOccurrences.find(y) != yearOccurrences.end()) {
                cout << y << " : " << yearOccurrences[y] << endl;
            } else {
                cout << y << " : 0" << endl;
            }
        }
        cout << endl;
    } else {
        cout << "Invalid mode for this overload. Please use '-e' for year." << endl;
    }
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
        cerr << "Error: Unable to open directory " << folderPath << endl;
    }
    return files;
}