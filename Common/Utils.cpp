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
