#ifndef NAMESEARCH_HPP
#define NAMESEARCH_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class NameSearch {
public:
    static void searchByName(string folderPath, string name);
    static void searchByName(string folderPath, string name, string mode, string param); 
    static void searchByName(string folderPath, string name, string mode, int year);
    static void searchByYear(string folderPath, int year);
    static void searchByNameAndYear(string folderPath, string name, int year); 
    static void searchByNameAndYear(string folderPath, string name, int startYear, int endYear);
    static void searchByState(string folderPath, string state);

private:
    static vector<string> listFiles(string folderPath);
};

#endif