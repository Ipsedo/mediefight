//
// Created by samuel on 02/02/18.
//

#include <utils/res.h>
#include <vector>
#include "string_utils.h"

std::string getResFolder() {
    std::string this_file = __FILE__;
    std::vector<std::string> splitted_path = split(this_file, '/');

    while (splitted_path.back() != "mediefight") {
        splitted_path.pop_back();
    }

    std::string res_path;
    for (auto& elt : splitted_path) {
        res_path += "/" + elt;
    }

    res_path += "/res";

    return res_path;
}
