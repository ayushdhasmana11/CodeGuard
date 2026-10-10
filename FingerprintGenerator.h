#ifndef CODEGUARD_FINGERPRINT_GENERATOR_H
#define CODEGUARD_FINGERPRINT_GENERATOR_H
#include <cstdint>
#include <string>
#include <vector>
using namespace std;
class FingerprintGenerator {
public:
    explicit FingerprintGenerator(size_t windowSize = 5);
    vector<uint64_t> generate(const vector<string>& tokens) const;
private:
    size_t windowSize_;
    static constexpr uint64_t BASE = 257;
    static constexpr uint64_t MOD = 1000000007ULL;
};
#endif
