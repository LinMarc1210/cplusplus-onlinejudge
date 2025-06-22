#include "RandomGenerator.h"

size_t RandomGenerator::next(size_t n) {
    if (n == 0) throw std::runtime_error("No elements to choose from");

    std::uniform_int_distribution<size_t> dist(0, n-1);
    size_t idx;

    // 重抽直到不在 LRU 裡
    do {
        idx = dist(gen);
    } while (inCache(idx) && cache.size() < n);

    touch(idx);
    return idx;
}

void RandomGenerator::touch(size_t x) {
    // if already in cache then erase
    if (inCache(x)) {
        cache.erase(mp[x]);
        mp.erase(x);
    }
    // re-insert to newest
    cache.push_back(x);
    mp[x] = std::prev(cache.end());
    // exceed then remove oldest
    if (cache.size() > cap) {
        size_t old = cache.front();
        cache.pop_front();
        mp.erase(old);
    }
}