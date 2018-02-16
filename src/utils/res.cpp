//
// Created by samuel on 02/02/18.
//

#include <utils/res.h>
#include <vector>
#include "string_utils.h"

std::string getResFolder() {
    using namespace std;
    string this_file = __FILE__;
    vector<string> splittedPath = split(this_file, '/');

    while (splittedPath.back() != "mediefight") {
        splittedPath.pop_back();
    }

    string res_path;
    for (auto& elt : splittedPath) {
        res_path += "/" + elt;
    }

    res_path += "/res";

    return res_path;
}
