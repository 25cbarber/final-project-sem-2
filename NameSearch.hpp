#ifndef NAMESEARCH_HPP
#define NAMESEARCH_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class NameSearch {
public:
    static void searchByName(string folderPath, string name); // Overload 1
    static void searchByName(string folderPath, string name, string mode, string param); // Overload 2
    static void searchByName(string folderPath, string name, string mode, int year); // Overload 3
    static void searchByYear(string folderPath, int year); // New function for year search
    static void searchByNameAndYear(string folderPath, string name, int year); // New function for name search by year

private:
    static vector<string> listFiles(string folderPath);
};

#endif