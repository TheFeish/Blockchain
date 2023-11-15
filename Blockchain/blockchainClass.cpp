#include "blockchainClass.h"

using namespace std;

User::User() : _name(""), _publicKey(myHash("")), _balance(0) {}

User::User(string name, string publicKey, int balance) : _name(name), _publicKey(publicKey), _balance(balance) {}

string User::getName() { return _name; }
string User::getPublicKey() { return _publicKey; }
int User::getBalance() { return _balance; }

void User::setName(string name) { _name = name; }
void User::setPublicKey(string publicKey) { _publicKey = _publicKey; }
void User::setBalance(int balance) { _balance = balance; }

void User::addBalance(int balance) { _balance += balance; }

void User::print() {
    cout << "name: " << _name << endl;
    cout << "publicKey: " << _publicKey << endl;
    cout << "balance: " << _balance << endl;
}

bool User::operator== (User& rhs) {
    const User& lhs = *this;
    return lhs._publicKey == rhs._publicKey;
}

bool User::operator== (const User& rhs) const {
    const User& lhs = *this;
    return lhs._publicKey == rhs._publicKey;
}


Transaction::Transaction() : _sender(""), _recipient(""), _amount(0), _timestamp(0)
{
    _id = myHash(_sender + _recipient + to_string(_amount) + to_string(_timestamp));
}

Transaction::Transaction(string sender, string recipient, int amount) : _sender(sender), _recipient(recipient), _amount(amount), _timestamp(time(0))
{
    _id = myHash(_sender + _recipient + to_string(_amount) + to_string(_timestamp));
}

string Transaction::getId() { return _id; }
string Transaction::getSender() { return _sender; }
string Transaction::getRecipient() { return _recipient; }
int Transaction::getAmount() { return _amount; }
unsigned int Transaction::getTimestamp() { return _timestamp; }

void Transaction::setId(string id) { _id = id; }
void Transaction::setSender(string sender) { _sender = sender; }
void Transaction::setRecipient(string recipient) { _recipient = recipient; }
void Transaction::setAmount(int amount) { _amount = amount; }
void Transaction::setTimestamp(unsigned int timestamp) { _timestamp = timestamp; }

void Transaction::print() {
    cout << "id: " << _id << endl;
    cout << "sender: " << _sender << endl;
    cout << "recipient: " << _recipient << endl;
    cout << "amount: " << _amount << endl;
    cout << "timestamp: " << _timestamp << endl;
    cout << endl;
}

bool Transaction::operator== (Transaction& rhs) {
    const Transaction& lhs = *this;
    return lhs._id == rhs._id;
}

bool Transaction::operator== (const Transaction& rhs) const {
    const Transaction& lhs = *this;
    return lhs._id == rhs._id;
}


Block::Block() : _prevHash(""), _version(0), _diffTarget(0), _timestamp(0), _nonce(0)
{
    _rootHash = getMerkle(transactionsToHashVec())[0];
}

Block::Block(string prevHash, unsigned int version, unsigned int diffTarget, vector<Transaction> transactions, unsigned int timestamp, unsigned int nonce) :
    _prevHash(prevHash), _version(version), _diffTarget(diffTarget), _transactions(transactions), _timestamp(timestamp), _nonce(nonce)
{
    _rootHash = getMerkle(transactionsToHashVec())[0];
}

string Block::getPrevHash() { return _prevHash; }
string Block::getRootHash() { return _rootHash; }
unsigned int Block::getTimestamp() { return _timestamp; }
unsigned int Block::getVersion() { return _version; }
unsigned int Block::getNonce() { return _nonce; }
unsigned int Block::getDiffTarget() { return _diffTarget; }
vector<Transaction> Block::getTransactions() { return _transactions; }

void Block::setPrevHash(string prevHash) { _prevHash = prevHash; }
void Block::setRootHash(string rootHash) { _rootHash = rootHash; }
void Block::setTimestamp(unsigned int timestamp) { _timestamp = timestamp; }
void Block::setVersion(unsigned int version) { _version = version; }
void Block::setNonce(unsigned int nonce) { _nonce = nonce; }
void Block::setDiffTarget(unsigned int diffTarget) { _diffTarget = diffTarget; }
void Block::setTransactions(vector<Transaction> transactions) { _transactions = transactions; }

void Block::print(bool printTransactions) {
    cout << "prevHash: " << _prevHash << endl;
    cout << "rootHash: " << _rootHash << endl;
    cout << "timestamp: " << _timestamp << endl;
    cout << "version: " << _version << endl;
    cout << "nonce: " << _nonce << endl;
    cout << "diffTarget: " << _diffTarget << endl;
    if (printTransactions) {
        cout << endl << "Transactions:" << endl;
        for (Transaction t : _transactions) {
            cout << t.getId() << endl;
        }
    }
    cout << endl;
}

vector<string> Block::transactionsToHashVec() {
    vector<string> vec;
    for (Transaction t : _transactions) {
        vec.push_back(myHash(t.getId()));
    }
    return vec;
}

vector<string> Block::getMerkle(vector<string> vec) {
    unsigned int size = vec.size();
    vector<string> newVec;
    if (size == 0) {
        newVec.push_back(myHash(""));
        return newVec;
    }
    else if (size == 1) {
        newVec.push_back(vec[0]);
        return newVec;
    }

    if (size % 2 != 0) {
        vec.push_back(vec[size - 1]);
        size++;
    }
    for (int i = 0; i < size; i += 2) {
        newVec.push_back(myHash(vec[i] + vec[i + 1]));
    }
    return getMerkle(newVec);
}