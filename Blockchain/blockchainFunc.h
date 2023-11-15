#pragma once
#include <random>
#include <string>
#include <Windows.h>
#include <omp.h>
#include "hashFunc.h"
#include "blockchainClass.h"

using namespace std;

void generateUsers(vector<User>& users);
void generateTransactions(vector<Transaction>& transactions, vector<User> users);
void deleteFrom(vector<Transaction>& a, vector<Transaction> b);
bool isHashGood(string hash, int diff);
void getNonceTimestamp(unsigned int& nonce, unsigned int& timestamp, string& hash, string prevHash, string rootHash, int diff, int version);
void processBlock(Block block, vector<User>& users);
void mining(vector<Block>& blocks, vector<Transaction>& transactions, vector<User>& users, string prevHash, int version, int diff, int blockCount);
void getBlock(vector<Block> blocks);
void getTransaction(vector<Block> blocks);
void getUser(vector<User> users);
void explorer(vector<Block> blocks, vector<User> users);
void validateTransactions(vector<Transaction>& transactions, vector<User> users);