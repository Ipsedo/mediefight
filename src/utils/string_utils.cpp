//
// Created by samuel on 03/02/18.
//

#include <sstream>

#include "string_utils.h"

std::vector<std::string> split(const std::string &s, char delim) {
    using namespace std;

    stringstream ss(s);
    string item;
    vector<string> elems;
    while (getline(ss, item, delim)) {
        //elems.push_back(item);
        elems.push_back(move(item)); // if C++11 (based on comment from @mchiasson)
    }
    return elems;
}