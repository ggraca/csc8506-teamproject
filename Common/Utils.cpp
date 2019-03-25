#include "Utils.h"

vector<string> split_string(const string &s, char delimiter) {
	vector<string> words;

	stringstream ss(s);
	string word;
	while (getline(ss, word, delimiter)) {
		if (word != "") words.push_back(word);
	}

	return words;
}

string ToString(Vector3 v) {
	float data[] = { v.x, v.y, v.z };
	return string((char*)data, sizeof(data));
}

string ToString(Quaternion q) {
	float data[] = { q.x, q.y, q.z, q.w };
	return string((char*)data, sizeof(data));
}