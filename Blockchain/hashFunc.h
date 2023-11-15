#pragma once
#include <sstream>
#include <random>

using namespace std;

unsigned int hashDec(int p, int m, string s);
string hashHex(unsigned int h1, unsigned int h2);
string myHash(string s);