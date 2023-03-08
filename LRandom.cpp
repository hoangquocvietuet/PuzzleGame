#include "LRandom.h"

LRandom::LRandom() {
    rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

LRandom::~LRandom() {}