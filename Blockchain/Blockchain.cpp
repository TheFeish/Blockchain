#include <iostream>
#include <string>
#include <Vector>
#include <random>
#include "hashFunc.h"
using std::string;
using std::back_inserter;
using std::sample;

class User {
    public:
        User() : _name(""), _publicKey(myHash("")), _balance(0) {}

        User(string name, string publicKey, int balance) : _name(name), _publicKey(publicKey), _balance(balance) {}

        //string getName() { return _name; }
        string getPublicKey() { return _publicKey; }
        int getBalance() { return _balance; }

        void setBalance(int balance) { _balance = balance; }

        void print() {
            cout << "name: " << _name << endl;
            cout << "publicKey: " << _publicKey << endl;
            cout << "balance: " << _balance << endl;
        }

        bool operator== (User& rhs) {
            const User& lhs = *this;
            return lhs._publicKey == rhs._publicKey;
        }

        bool operator== (const User& rhs) const {
            const User& lhs = *this;
            return lhs._publicKey == rhs._publicKey;
        }

    private:
        string _name, _publicKey;
        int _balance;
};

class Transaction {
    public:
        Transaction() : _sender(""), _recipient(""), _amount(0), _timestamp(0)
        {
            _id = myHash(_sender + _recipient + to_string(_amount) + to_string(_timestamp));
        }

        Transaction(string sender, string recipient, int amount) : _sender(sender), _recipient(recipient), _amount(amount), _timestamp(time(0))
        {
            _id = myHash(_sender + _recipient + to_string(_amount) + to_string(_timestamp));
        }

        string getId() { return _id; }
        string getSender() { return _sender; }
        string getRecipient() { return _recipient; }
        int getAmount() { return _amount; }
        //int getTimestamp() { return _timestamp; }

        void print() {
            cout << "id: " << _id << endl;
            cout << "sender: " << _sender << endl;
            cout << "recipient: " << _recipient << endl;
            cout << "amount: " << _amount << endl;
            cout << "timestamp: " << _timestamp << endl;
            cout << endl;
        }

        bool operator== (Transaction& rhs) {
            const Transaction& lhs = *this;
            return lhs._id == rhs._id;
        }

        bool operator== (const Transaction& rhs) const {
            const Transaction& lhs = *this;
            return lhs._id == rhs._id;
        }


    private:
        string _id, _sender, _recipient;
        int _amount, _timestamp;
};

class Block {
    public:
        Block() : _prevHash(""), _version(0), _diffTarget(0), _timestamp(0), _nonce(0)
        {
            string th;
            for (Transaction t : _transactions) {
                th += t.getId();
            }
            _rootHash = myHash(th);
        }

        Block(string prevHash, unsigned int version, unsigned int diffTarget, vector<Transaction> transactions, unsigned int timestamp = 0, unsigned int nonce = 0) :
            _prevHash(prevHash), _version(version), _diffTarget(diffTarget), _transactions(transactions), _timestamp(timestamp), _nonce(nonce)
        {
            string th = "";
            for (Transaction t : _transactions) {
                th += t.getId();
            }
            _rootHash = myHash(th);
        }

        void setTimestamp(unsigned int i) { _timestamp = i; }
        void setNonce(unsigned int i) { _nonce = i; }

        string getRootHash() { return _rootHash; }
        vector<Transaction> getTransactions() { return _transactions; }

        void print(bool printTransactions) {
            cout << "prevHash: " << _prevHash << endl;
            cout << "rootHash: " << _rootHash << endl;
            cout << "timestamp: " << _timestamp << endl;
            cout << "version: " << _version << endl;
            cout << "nonce: " << _nonce << endl;
            cout << "diffTarget: " << _diffTarget << endl;
            if (printTransactions) {
                cout << endl << "Transactions:" << endl;
                for (Transaction t : _transactions) {
                    t.print();
                }
            }
            cout << endl;
        }

    private:
        string _prevHash, _rootHash;
        unsigned int _timestamp, _version, _nonce, _diffTarget;
        vector<Transaction> _transactions;
};

void generateUsers(vector<User>& users) {
    default_random_engine rng{(unsigned int)time(0)};
    uniform_int_distribution<> dist{ 100, 1000000 };
    for (int i = 0; i < 1000; i++) {
        string name = "user" + to_string(i);
        users.push_back(User(name, myHash(name), dist(rng)));
    }
}

void generateTransactions(vector<Transaction>& transactions, vector<User> users) {
    default_random_engine rng{ (unsigned int)time(0) };
    uniform_int_distribution<> distUser{ 0, 999 };
    uniform_int_distribution<> distAmount{ 100, 1000000 };
    for (int i = 0; i < 10000; i++){
        transactions.push_back(Transaction(users[distUser(rng)].getPublicKey(), users[distUser(rng)].getPublicKey(), distAmount(rng)));
    }
}

void deleteFrom(vector<Transaction>& a, vector<Transaction> b) {
    a.erase(remove_if(begin(a), end(a),
        [&](auto x) {return find(begin(b), end(b), x) != end(b); }), end(a));
}

bool isHashGood(string hash, int diff) {
    string str = hash.substr(0, diff);
    return str.find_first_not_of('0') == string::npos;
}

void processBlock(Block block, vector<User>& users) {
    for (Transaction t : block.getTransactions()) {
        int senderI = distance(users.begin(), find_if(users.begin(), users.end(), [&](User obj) {return obj.getPublicKey() == t.getSender(); }));
        int recipientI = distance(users.begin(), find_if(users.begin(), users.end(), [&](User obj) {return obj.getPublicKey() == t.getRecipient(); }));

        users[senderI].setBalance(users[senderI].getBalance() - t.getAmount());
        users[recipientI].setBalance(users[recipientI].getBalance() + t.getAmount());
    }
}

void mining(vector<Block>& blocks, vector<Transaction>& transactions, vector<User>& users, string prevHash, int version, int diff) {
    default_random_engine rng{ (unsigned int)time(0) };
    unsigned int nonce = 0;
    while (transactions.size() > 0) {
        vector<Transaction> blockTransactions;
        size_t test = 100;
        sample(transactions.begin(), transactions.end(), back_inserter(blockTransactions), test, mt19937(rng));
        Block block(prevHash, version, diff, blockTransactions);
        string rootHash = block.getRootHash();
        string hash = "11111111111111111111111111";
        while (!isHashGood(hash, diff)) {
            hash = myHash(rootHash + to_string(nonce));
            nonce++;
        }
        block.setNonce(nonce);
        block.setTimestamp(time(0));
        prevHash = hash;
        block.print(false);
        processBlock(block, users);
        blocks.push_back(block);
        deleteFrom(transactions, blockTransactions);
    }
}

int main() {
    vector<User> users;
    vector<Transaction> transactions;
    vector<Block> blocks;
    string prevHash = "0000922faa407970e4402aa57eabe2186a5c72b90b70ff4fe52f571e9d46b5f2";
    unsigned int diff = 5, version = 1;
    generateUsers(users);
    generateTransactions(transactions, users);
    blocks.push_back(Block("", version, diff, vector<Transaction>(), time(0), 74569));
    blocks[0].print(false);
    mining(blocks, transactions, users, prevHash, version, diff);
    return 0;
}