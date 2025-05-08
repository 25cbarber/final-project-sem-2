#include "NameSearch.hpp"
#include <dirent.h>
#include <cctype>
#include <iomanip>
#include <map>
#include <algorithm>

void NameSearch::searchByName(string folderPath, string name, string mode, string param) {
    if (mode == "-s") {
        string state = param;
        map<string, int> nameOccurrences;

        vector<string> files = listFiles(folderPath);

        // Filter files by state acronym in the file name
        for (const string& fileName : files) {
            // Check if the file name matches the format ".STATE." + state + ".TXT"
            if (fileName.find(".STATE." + state + ".TXT") != string::npos) {
                ifstream file(folderPath + "/" + fileName);
                if (!file.is_open()) {
                    cerr << "Error: Unable to open file " << fileName << endl;
                    continue;
                }

                string line;
                while (getline(file, line)) {
                    size_t comma1 = line.find(',');
                    size_t comma2 = line.rfind(',');

                    if (comma1 == string::npos || comma2 == string::npos) {
                        cerr << "Error: Malformed line in file " << fileName << ": " << line << endl;
                        continue;
                    }

                    string currentName = line.substr(0, comma1);
                    int occurrences = stoi(line.substr(comma2 + 1));

                    // Aggregate occurrences for each name
                    nameOccurrences[currentName] += occurrences;
                }
            }
        }

        if (nameOccurrences.empty()) {
            cout << "No data found for state: " << state << ". Please check the input or data files." << endl;
            return;
        }

        // Sort and display the top 5 most popular names
        vector<pair<string, int>> sortedNames(nameOccurrences.begin(), nameOccurrences.end());
        sort(sortedNames.begin(), sortedNames.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
            return b.second > a.second;
        });

        cout << "\nThe top 5 most popular names in state " << state << " are:\n";
        for (size_t i = 0; i < min(sortedNames.size(), size_t(5)); ++i) {
            cout << i + 1 << ". " << sortedNames[i].first << " | " << sortedNames[i].second << " occurrences" << endl;
        }
        cout << endl;
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
    cout << "(Search again with -s state to show data for a specific state)\n" << endl;
}

void NameSearch::searchByName(string folderPath, string name, string mode, int year) {
    if (mode == "-e") {
        int startYear = year;
        int endYear = year; 
        if (startYear < 1880 || startYear > 2022) {
            cout << "Invalid year. Please provide a year between 1880 and 2022." << endl;
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
            return b.second < a.second;
        });

        cout << "\nThe most popular names that year were:\n";
        for (size_t i = 0; i < min(sortedNames.size(), size_t(5)); ++i) {
            cout << i + 1 << ". " << sortedNames[i].first << " | " << sortedNames[i].second << endl;
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