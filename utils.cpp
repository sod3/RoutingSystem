#include "utils.h"

vector<string> split(const string &str, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(str);
    
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}