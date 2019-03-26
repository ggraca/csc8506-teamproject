#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <iostream>

#include "Vector3.h"
#include "Quaternion.h"

using namespace std;

vector<string> split_string(const string &s, char delimiter = ' ');
string ToString(NCL::Maths::Vec3 v);
string ToString(NCL::Maths::Quaternion q);
