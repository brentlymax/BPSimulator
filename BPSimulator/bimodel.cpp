#include <array>
#include <iostream>
#include <cmath>

/**
 * Strong not taken: 0
 * Weakly not taken: 1
 * Weakly taken:     2
 * Strong taken:     3
 */


using namespace std;

static const int kMemLimit = 32 * 1024 / 2; // 2 bit counter used
static const int kTableBit = std::log2(kMemLimit);
static const int kTableMask = (1 << kTableBit) - 1; // mask to hash to table
static array<int, kMemLimit> kTable; // table for branch prediction

void init_predictor() {
    /**
     * every entry in table initilized to 00;
     */
    kTable.fill(0);
}

bool make_prediction(uint32_t pc) {
    int index = pc & kTableMask; // found table index
    return kTable[index] >= 2; // 0, 1 for not take; 2, 3 for taken
}

void train_predictor(uint32_t pc, bool outcome) {
    int index = pc & kTableMask;
    // branch taken, but prediction not strong taken
    if(outcome == true && kTable[index] < 3) {
        kTable[index]++;
    }
    // branch not taken, but prediction not strong not taken
    if(outcome == false && kTable[index] > 0) {
        kTable[index]--;
    }
}
