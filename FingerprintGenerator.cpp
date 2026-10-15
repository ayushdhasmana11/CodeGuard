#include "FingerprintGenerator.h"

FingerprintGenerator::FingerprintGenerator(size_t windowSize)
    : windowSize_(windowSize == 0 ? 1 : windowSize) {}

static uint64_t hashToken(const string& token) {
    uint64_t hash = 0;

    for (unsigned char ch : token) {
        hash = (hash * 131 + ch) % 1000000007ULL;
    }

    return hash;
}

vector<uint64_t> FingerprintGenerator::generate(
    const vector<string>& tokens) const {

    vector<uint64_t> fingerprints;

    if (tokens.empty() || tokens.size() < windowSize_) {
        return fingerprints;
    }

    vector<uint64_t> tokenHashes;
    tokenHashes.reserve(tokens.size());

    for (const string& token : tokens) {
        tokenHashes.push_back(hashToken(token));
    }

    uint64_t power = 1;
    for (size_t i = 1; i < windowSize_; ++i) {
        power = (power * BASE) % MOD;
    }

    uint64_t rollingHash = 0;
    for (size_t i = 0; i < windowSize_; ++i) {
        rollingHash = (rollingHash * BASE + tokenHashes[i]) % MOD;
    }
    fingerprints.push_back(rollingHash);

    for (size_t i = windowSize_; i < tokenHashes.size(); ++i) {
        uint64_t outgoing =
            (tokenHashes[i - windowSize_] * power) % MOD;

        rollingHash = (rollingHash + MOD - outgoing) % MOD;

        rollingHash = (rollingHash * BASE + tokenHashes[i]) % MOD;
        fingerprints.push_back(rollingHash);
    }

    return fingerprints;
}
