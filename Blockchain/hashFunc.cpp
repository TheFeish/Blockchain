#include <sstream>
#include <random>
#include "hashFunc.h"

using namespace std;

unsigned int hashDec(int p, int m, string s) {
	unsigned int hash = 0;
	int pow = p;
	for (auto c : s) {
		hash = (hash + c * pow) % m;
		pow = (pow * p) % m;

	}
	return hash;
}

string hashHex(unsigned int h1, unsigned int h2) {
	stringstream hash;

	default_random_engine rng1{ h1 };
	default_random_engine rng2{ h2 };

	uniform_int_distribution<> dist{ 0, 15 };

	for (int i = 0; i < 32; i++) {
		hash << hex << dist(rng1);
		hash << hex << dist(rng2);
	}

	return hash.str();
}

string myHash(string s) {
	return hashHex(hashDec(257, (int)(1e9 + 7), s), hashDec(263, (int)(1e9 + 9), s));
}