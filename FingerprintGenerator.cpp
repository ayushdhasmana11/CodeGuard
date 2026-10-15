#include "FingerprintGenerator.h"

// Part 1: Initialize the fingerprint generator.
FingerprintGenerator::FingerprintGenerator(size_t windowSize)
    : windowSize_(windowSize == 0 ? 1 : windowSize) {}

// Part 2: Convert each token into a numeric hash.
// This helper will be used by generate() in the next stage.
static uint64_t hashToken(const string& token) {
    uint64_t hash = 0;

    for (unsigned char ch : token) {
        hash = (hash * 131 + ch) % 1000000007ULL;
    }

    return hash;
}
// Part 3: Hash the input tokens.
// This is an intermediate implementation; the next part will combine
// consecutive token hashes into rolling-window fingerprints.
vector<uint64_t> FingerprintGenerator::generate(
    const vector<string>& tokens) const {

    vector<uint64_t> fingerprints;

    if (tokens.empty()) {
        return fingerprints;
    }

    fingerprints.reserve(tokens.size());

    for (const string& token : tokens) {
        fingerprints.push_back(hashToken(token));
    }

    return fingerprints;
}
