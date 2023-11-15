#include "blockchainFunc.h"

#define THREAD_COUNT 5
#define DIFFICULTY 4
#define VERSION 2

int main() {
    
    // Initialising values and generating users and transactions, transaction validation
    vector<User> users;
    vector<Transaction> transactions;
    vector<Block> blocks;
    string prevHash = "0000000000000000000000000000000000000000000000000000000000000000";
    generateUsers(users);
    generateTransactions(transactions, users);
    validateTransactions(transactions, users);

    // Creating genesis block
    blocks.push_back(Block(prevHash, VERSION, DIFFICULTY, vector<Transaction>()));
    unsigned int nonce = 0, timestamp = 0;
    prevHash = "1111111111111111111111111111111111111111111111111111111111111111";
    getNonceTimestamp(nonce, timestamp, prevHash, blocks[0].getPrevHash(), blocks[0].getRootHash(), DIFFICULTY, VERSION);
    blocks[0].setNonce(nonce);
    blocks[0].setTimestamp(timestamp);
    cout << "------------------------Genesis block------------------------" << endl;
    blocks[0].print(false);
    

    // Mining and explorer
    cout << endl << "----------Begining mining. Press Escape key to stop----------" << endl << endl;
    mining(blocks, transactions, users, prevHash, VERSION, DIFFICULTY, THREAD_COUNT);
    explorer(blocks, users);
    return 0;
}