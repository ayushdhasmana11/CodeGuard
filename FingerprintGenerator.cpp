#include "FingerprintGenerator.h"
#include <stdexcept>
using namespace std;

FingerprintGenerator::FingerprintGenerator(size_t windowSize)
    : windowSize_(windowSize) {
    if (windowSize_ == 0)
        throw invalid_argument("Fingerprint window size must be greater than zero.");
}

vector<uint64_t> FingerprintGenerator::generate(const vector<string>& tokens) const {
    vector<uint64_t> fingerprints;
    if (tokens.size() < windowSize_) return fingerprints;

    auto tokenHash = [](const string& token) -> uint64_t {
        uint64_t value = 0;
        for (unsigned char ch : token)
            value = (value * 131ULL + ch) % MOD;
        return value;
    };

    uint64_t highestPower = 1;
    for (size_t i = 1; i < windowSize_; ++i)
        highestPower = (highestPower * BASE) % MOD;

    uint64_t rolling = 0;
    for (size_t i = 0; i < windowSize_; ++i)
        rolling = (rolling * BASE + tokenHash(tokens[i])) % MOD;
    fingerprints.push_back(rolling);

    for (size_t i = windowSize_; i < tokens.size(); ++i) {
        uint64_t outgoing = tokenHash(tokens[i-windowSize_]);
        uint64_t incoming = tokenHash(tokens[i]);
        rolling = (rolling + MOD - (outgoing * highestPower) % MOD) % MOD;
        rolling = (rolling * BASE + incoming) % MOD;
        fingerprints.push_back(rolling);
    }
    return fingerprints;
}
