#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <iostream>

#include "Vector3.h"
#include "Quaternion.h"

using namespace std;
using namespace NCL::Maths;

vector<string> split_string(const string &s, char delimiter = ' ');
string ToString(Vector3 v);
string ToString(Quaternion q);
