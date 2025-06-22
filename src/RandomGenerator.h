#include <random>
#include <list>
#include <unordered_map>
#include <iostream>
#include <vector>

class RandomGenerator {
public:
    RandomGenerator(size_t capacity): cap(capacity), rd(), gen(rd()) {}
    size_t next(size_t n);
private:
    size_t cap;  // LRU 最大容量
    std::list<size_t> cache;     // LRU cache，前面是最舊、後面是最新
    std::unordered_map<size_t, decltype(cache.begin())> mp;
    std::random_device rd;
    std::mt19937 gen;
    bool inCache(size_t x) { return mp.find(x) != mp.end(); }
    void touch(size_t x);
};
