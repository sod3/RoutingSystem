#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

vector<string> split(const string &str, char delimiter);

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