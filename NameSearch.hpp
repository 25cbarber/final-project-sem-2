#ifndef NAMESEARCH_HPP
#define NAMESEARCH_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
using namespace std;

class NameSearch {
public:
    static void searchByName(string folderPath, string name) {
        int totalOccurrences = 0;
        int totalOccurrences2010To2022 = 0;
        int maxOccurrences = 0;
        string maxYear;

        vector<string> files = listFiles(folderPath);

        for (string fileName : files) {
            if (fileName.substr(0, 3) == "yob" &&  fileName.substr(fileName.size() - 4) == ".txt") {
                string year = fileName.substr(3, 4);
                ifstream file(folderPath + "/" + fileName);
                string line;

                while (getline(file, line)) {
                    size_t comma1 = line.find(',');
                    size_t comma2 = line.rfind(',');

                    string currentName = line.substr(0, comma1);
                    int occurrences = stoi(line.substr(comma2 + 1));

                    if (currentName == name) {
                        totalOccurrences += occurrences;

                        if (stoi(year) >= 2010 && stoi(year) <= 2022) {
                            totalOccurrences2010To2022 += occurrences;
                        }

                        if (occurrences > maxOccurrences) {
                            maxOccurrences = occurrences;
                            maxYear = year;
                        }
                    }
                }
            }
        }

        cout << "\nTotal occurrences across all files: " << totalOccurrences << endl;
        cout << "Total occurrences from 2010 to 2022: " << totalOccurrences2010To2022 << endl;
        cout << "Highest occurrence: " << maxOccurrences << " in year " << maxYear << "\n" << endl;
    }

private:
    static vector<string> listFiles(string& folderPath) {
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
};

#endif