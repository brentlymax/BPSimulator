#include <array>
#include <iostream>
#include <cmath>

using namespace std;

static const int kMemLimit = 32 * 1024 / 2; // 2-bit table for saturating counter
static const int kTableBits = std::log2(kMemLimit);
static const int kHistoryBits = kTableBits;
static const int kAddressMask =  (1 << kHistoryBits) - 1;
static const int kGhrMask = (1 << kTableBits) - 1;
static int kGlobalHistoryRegister = 0;
static array<int, kMemLimit> kTable;

void init_predictor ()
{
	kGlobalHistoryRegister = 0;
	kTable.fill(0);
	cout << kHistoryBits << endl;
}

// A very stupid predictor.  It will always predict not taken.
bool make_prediction (unsigned int pc)
{
	int index = (pc & kAddressMask) ^ kGlobalHistoryRegister;
	return kTable[index] >= 2;
}

void train_predictor (unsigned int pc, bool outcome)
{
	int index = (pc & kAddressMask) ^ kGlobalHistoryRegister;
	// cout << pc << ", " << boolalpha << outcome << "," << index << endl;
	if (outcome == true && kTable[index] < 3)	kTable[index]++;
	if (outcome == false && kTable[index] > 0)	kTable[index]--;

	kGlobalHistoryRegister =  (((kGlobalHistoryRegister << 1) | outcome) & kGhrMask);
}