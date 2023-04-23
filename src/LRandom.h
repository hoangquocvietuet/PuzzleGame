#ifndef LRANDOM_H
#define LRANDOM_H
#include <random>
#include <chrono>
#include <algorithm>
class LRandom
{
public:
    LRandom();
    ~LRandom();

    // return pseudo random number
    template <typename T>
    T randnum(T l, T r)
    {
        return std::uniform_int_distribution<T>(l, r)(rng);
    }

    template <typename T>
    void shuffleVector(std::vector<T> &vt)
    {
        shuffle(vt.begin(), vt.end(), rng);
    }

    template <typename T>
    T getOneRandom(std::vector<T> vt)
    {
        assert(vt.size() >= 1);
        int u = randnum(0, (int)vt.size() - 1);
        return vt[u];
    }

private:
    std::mt19937_64 rng;
};
#endif