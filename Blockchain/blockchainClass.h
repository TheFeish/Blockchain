#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "hashFunc.h"

using namespace std;

class User {
public:
    User();
    User(string name, string publicKey, int balance);

    string getName();
    string getPublicKey();
    int getBalance();

    void setName(string name);
    void setPublicKey(string publicKey);
    void setBalance(int balance);

    void addBalance(int balance);
    void print();

    bool operator== (User& rhs);
    bool operator== (const User& rhs) const;

private:
    string _name, _publicKey;
    int _balance;
};

class Transaction {
public:
    Transaction();
    Transaction(string sender, string recipient, int amount);
    string getId();
    string getSender();
    string getRecipient();
    int getAmount();
    unsigned int getTimestamp();

    void setId(string id);
    void setSender(string sender);
    void setRecipient(string recipient);
    void setAmount(int amount);
    void setTimestamp(unsigned int timestamp);

    void print();

    bool operator== (Transaction& rhs);
    bool operator== (const Transaction& rhs) const;


private:
    string _id, _sender, _recipient;
    int _amount;
    unsigned int _timestamp;
};

class Block {
public:
    Block();
    Block(string prevHash, unsigned int version, unsigned int diffTarget, vector<Transaction> transactions, unsigned int timestamp = 0, unsigned int nonce = 0);

    string getPrevHash();
    string getRootHash();
    unsigned int getTimestamp();
    unsigned int getVersion();
    unsigned int getNonce();
    unsigned int getDiffTarget();
    vector<Transaction> getTransactions();

    void setPrevHash(string prevHash);
    void setRootHash(string rootHash);
    void setTimestamp(unsigned int timestamp);
    void setVersion(unsigned int version);
    void setNonce(unsigned int nonce);
    void setDiffTarget(unsigned int diffTarget);
    void setTransactions(vector<Transaction> transactions);

    void print(bool printTransactions);

private:
    string _prevHash, _rootHash;
    unsigned int _timestamp, _version, _nonce, _diffTarget;
    vector<Transaction> _transactions;

    vector<string> transactionsToHashVec();
    vector<string> getMerkle(vector<string> vec);
};