#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Split string by delimiter
vector<string> split(const string &str, char delimiter);

// Print vector
template<typename T>
void printVector(const vector<T> &vec, const string &name = "") {
    if (!name.empty()) {
        cout << name << ": ";
    }
    for (const auto &item : vec) {
        cout << item << " ";
    }
    cout << endl;
}

#endif