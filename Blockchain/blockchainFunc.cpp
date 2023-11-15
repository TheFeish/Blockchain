#include "blockchainFunc.h"

using std::back_inserter;
using std::sample;

using namespace std;

void generateUsers(vector<User>& users) {
    default_random_engine rng{ (unsigned int)time(0) };
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
    for (int i = 0; i < 10000; i++) {
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

void getNonceTimestamp(unsigned int& nonce, unsigned int& timestamp, string& hash, string prevHash, string rootHash, int diff, int version) {
    while (!isHashGood(hash, diff)) {
        nonce++;
        timestamp = time(0);
        hash = myHash(prevHash + rootHash + to_string(version) + to_string(diff) + to_string(nonce) + to_string(timestamp));
    }
}

void processBlock(Block block, vector<User>& users) {
    for (Transaction t : block.getTransactions()) {
        int senderI = distance(users.begin(), find_if(users.begin(), users.end(), [&](User obj) {return obj.getPublicKey() == t.getSender(); }));
        int recipientI = distance(users.begin(), find_if(users.begin(), users.end(), [&](User obj) {return obj.getPublicKey() == t.getRecipient(); }));

        users[senderI].addBalance(-t.getAmount());
        users[recipientI].addBalance(t.getAmount());
    }
}

void mining(vector<Block>& blocks, vector<Transaction>& transactions, vector<User>& users, string prevHash, int version, int diff, int blockCount) {
    default_random_engine rng{ (unsigned int)time(0) };
    uniform_int_distribution<> dist{ 0, (numeric_limits<int>::max)() };
    vector<unsigned int> nonces;
    for (int i = 0; i < blockCount; i++) {
        nonces.push_back(dist(rng));
    }
    while (transactions.size() > 0) {
        bool halt = false;
        Block currentBlock;
        string hash;
        int size = 128;
        if (transactions.size() < 100) { size = transactions.size(); }
        unsigned int timestamp = 0;
        bool cont = true, gotHash = false;
        int miner = -1;
        vector<Transaction> currentTransactions;
#pragma omp parallel for shared(miner, cont, gotHash, currentBlock, hash, nonces, currentTransactions)
        for (int i = 0; i < blockCount; i++) {
            vector<Transaction> blockTransactions;
            sample(transactions.begin(), transactions.end(), back_inserter(blockTransactions), size, mt19937(rng));
            Block block(prevHash, version, diff, blockTransactions);
            int thread = omp_get_thread_num(), nonce = nonces[thread];
            string newHash = "1111111111111111111111111111111111111111111111111111111111111111", rootHash = block.getRootHash();
            while (!gotHash && !isHashGood(newHash, diff) && cont) {
                nonce++;
                timestamp = time(0);
                newHash = myHash(prevHash + rootHash + to_string(version) + to_string(diff) + to_string(nonce) + to_string(timestamp));
                if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { cont = false; }
            }
            if (gotHash || !cont) {
                break;
            }
            nonces[thread] = nonce;
            hash = newHash;
            miner = thread;
            block.setNonce(nonces[miner]);
            block.setTimestamp(timestamp);
            currentBlock = block;
            currentTransactions = blockTransactions;
            gotHash = true;
        }
        if (!cont) {
            cout << endl << "Mining stopped" << endl << endl;
            return;
        }

        prevHash = hash;
        cout << "Miner " << miner << " mined:" << endl;
        currentBlock.print(false);
        processBlock(currentBlock, users);
        blocks.push_back(currentBlock);
        deleteFrom(transactions, currentTransactions);
        currentTransactions.clear();
        currentTransactions.shrink_to_fit();
    }
}

void getBlock(vector<Block> blocks) {
    string hash;
    cout << "Enter block hash: ";
    cin >> hash;
    cout << endl;
    int i = distance(blocks.begin(), find_if(blocks.begin(), blocks.end(),
        [&](Block b) {return myHash(b.getPrevHash() + b.getRootHash() + to_string(b.getVersion()) + to_string(b.getDiffTarget()) + to_string(b.getNonce()) + to_string(b.getTimestamp())) == hash; }));
    if (i == blocks.size()) {
        cout << "Block not found" << endl;
    }
    else {
        blocks[i].print(true);
    }
}

void getTransaction(vector<Block> blocks) {
    string id;
    cout << "Enter transaction id: ";
    cin >> id;
    cout << endl;
    for (auto b : blocks) {
        vector<Transaction> t = b.getTransactions();
        auto it = find_if(t.begin(), t.end(), [&](Transaction t) {return t.getId() == id; });
        if (it != t.end()) {
            t[distance(t.begin(), it)].print();
            return;
        }
    }
    cout << "Transaction not found" << endl;
}

void getUser(vector<User> users) {
    string key;
    cout << "Enter user public key: ";
    cin >> key;
    cout << endl;
    int i = distance(users.begin(), find_if(users.begin(), users.end(),
        [&](User u) {return u.getPublicKey() == key; }));
    if (i == users.size()) {
        cout << "User not found" << endl;
    }
    else {
        users[i].print();
    }
}

void explorer(vector<Block> blocks, vector<User> users) {
    int temp = 0;
    while (temp != 4) {
        cout << endl << "1 - find block\n2 - find transaction\n3 - find user\n4 - exit" << endl;
        while (temp < 1 || temp > 4) {
            cin >> temp;
            if (temp < 1 || temp > 4 || cin.fail()) {
                cout << "Ivedete neteisinga reiksme" << endl;
                temp = 0;
                cin.clear();
                cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            }
        }
        switch (temp) {
        case 1:
            getBlock(blocks);
            break;
        case 2:
            getTransaction(blocks);
            break;
        case 3:
            getUser(users);
            break;
        }
        if (temp != 4) { temp = 0; }
    }
}

void validateTransactions(vector<Transaction>& transactions, vector<User> users) {
    vector<Transaction> validated;
    for (Transaction t : transactions) {
        if (t.getId() != myHash(t.getSender() + t.getRecipient() + to_string(t.getAmount()) + to_string(t.getTimestamp()))) { continue; }
        auto it = find_if(users.begin(), users.end(), [&](User u) {return u.getPublicKey() == t.getSender() && u.getBalance() >= t.getAmount(); });
        if (it != users.end()) {
            users[distance(users.begin(), it)].addBalance(-t.getAmount());
            validated.push_back(t);
        }
    }
    transactions = validated;
}