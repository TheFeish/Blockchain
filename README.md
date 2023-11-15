# Blockchain v0.2

## Usage and features
### User and transactions generation
Upon launching the program 1000 users, 10000 transactions and genesis block are generated.
All transaction are verified by recalculating its hash and checking if sender has enough balance.

### Mining
After generation mining process with selected difficulty is simulated with every new block mined and its transactions printed to the console.
Several miners are simulated by mining in parallel using multithreading (openMP).
This mining process can be stopped at any time by pressing button.

### Explorer
Information about any block, transaction or user can be printed by using block hash, transaction id or user public key respectively.
