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

private:
    static vector<string> listFiles(string folderPath);
};

#endif