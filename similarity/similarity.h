#ifndef SIMILARITY_CALCULATOR_H
#define SIMILARITY_CALCULATOR_H

#include <cstdint>
#include <vector>

class SimilarityCalculator {
public:
    // Returns similarity as a percentage between 0 and 100.
    double calculateSimilarity(
        const std::vector<std::uint64_t>& fingerprintsA,
        const std::vector<std::uint64_t>& fingerprintsB
    ) const;
};

#endif