
#include "similarity.h"

#include <unordered_set>

double SimilarityCalculator::calculateSimilarity(
    const std::vector<std::uint64_t>& fingerprintsA,
    const std::vector<std::uint64_t>& fingerprintsB
) const {

    // Store unique fingerprints from both files.
    std::unordered_set<std::uint64_t> setA(
        fingerprintsA.begin(),
        fingerprintsA.end()
    );

    std::unordered_set<std::uint64_t> setB(
        fingerprintsB.begin(),
        fingerprintsB.end()
    );

    // Avoid division by zero.
    const std::size_t unionSize = [&]() {
        std::unordered_set<std::uint64_t> combined = setA;

        combined.insert(setB.begin(), setB.end());

        return combined.size();
    }();

    if (unionSize == 0) {
        return 0.0;
    }

    // Count fingerprints that occur in both files.
    std::size_t intersectionSize = 0;

    for (const auto& fingerprint : setA) {
        if (setB.find(fingerprint) != setB.end()) {
            ++intersectionSize;
        }
    }

    return (static_cast<double>(intersectionSize) /
            static_cast<double>(unionSize)) * 100.0;
}
