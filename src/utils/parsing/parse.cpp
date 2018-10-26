//
// Created by samuel on 17/02/18.
//

#include <cstring>
#include "parse.h"

float parseFloat(ifstream *file) {
	char tmp[4];
	(*file).read(tmp, 4);
	float res;
	memcpy(&res, tmp, sizeof(char) * 4);
	return res;
}
