
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iomanip>
#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <fstream>

#include "Reader/CodeReader.h"
#include "Preprocessing/Preprocessor.h"
#include "Tokenizer.h"
#include "FingerprintGenerator.h"
#include "HashTable/hash_table.h"

using namespace std;
namespace fs = std::filesystem;

struct FileData {
    string filename;
    vector<uint64_t> fingerprints;
};

struct SimilarityResult {
    string fileA;
    string fileB;
    size_t uniqueA;
    size_t uniqueB;
    size_t shared;
    double similarity;
};

int main(int argc, char* argv[]) {
    CodeReader reader;
    Preprocessor preprocessor;
    Tokenizer tokenizer;
    FingerprintGenerator fingerprintGenerator;
    HashTable table{};

    bool tableInitialized = false;

    try {
        string inputDirectory = "data";
        if (argc > 1) {
            inputDirectory = argv[1];
        }

        vector<fs::path> files = reader.getCppFiles(inputDirectory);
        cout << "\nC++ files found: " << files.size() << '\n';

        if (files.empty()) {
            cout << "No C++ files found in " << inputDirectory << ".\n";
            return 0;
        }

        if (!hash_table_init(&table, 1009)) {
            throw runtime_error("Failed to initialize hash table.");
        }
        tableInitialized = true;

        vector<FileData> allFiles;

        // Stage 1: Read, clean, tokenize, and fingerprint each file.
        for (const auto& filePath : files) {
            string originalCode = reader.readFile(filePath.string());
            string cleanedCode = preprocessor.cleanCode(originalCode);
            vector<string> tokens = tokenizer.tokenize(cleanedCode);
            vector<uint64_t> fingerprints =
                fingerprintGenerator.generate(tokens);

            FileData currentFile;
            currentFile.filename = filePath.filename().string();
            currentFile.fingerprints = fingerprints;

            for (uint64_t fingerprint : fingerprints) {
                if (!hash_table_insert(
                        &table, fingerprint,
                        currentFile.filename.c_str())) {
                    throw runtime_error(
                        "Failed to insert fingerprint into hash table.");
                }
            }

            cout << "Processed: " << currentFile.filename
                 << " | Tokens: " << tokens.size()
                 << " | Fingerprints: " << fingerprints.size() << '\n';

            allFiles.push_back(currentFile);
        }

        // Stage 2: Compare every distinct pair using unique fingerprints.
        vector<SimilarityResult> results;

        for (size_t i = 0; i < allFiles.size(); ++i) {
            for (size_t j = i + 1; j < allFiles.size(); ++j) {
                const auto& a = allFiles[i];
                const auto& b = allFiles[j];

                unordered_set<uint64_t> uniqueA(
                    a.fingerprints.begin(), a.fingerprints.end());
                unordered_set<uint64_t> uniqueB(
                    b.fingerprints.begin(), b.fingerprints.end());

                size_t shared = 0;
                for (uint64_t fingerprint : uniqueA) {
                    if (uniqueB.count(fingerprint) > 0) {
                        ++shared;
                    }
                }

                size_t totalA = uniqueA.size();
                size_t totalB = uniqueB.size();
                double similarity = 0.0;

                if (totalA + totalB > 0) {
                    similarity =
                        (2.0 * shared / (totalA + totalB)) * 100.0;
                }

                results.push_back({
                    a.filename, b.filename,
                    totalA, totalB, shared, similarity
                });
            }
        }

        // Stage 3: Rank highest similarity first.
        sort(results.begin(), results.end(),
            [](const SimilarityResult& a, const SimilarityResult& b) {
                if (a.similarity != b.similarity) {
                    return a.similarity > b.similarity;
                }
                if (a.fileA != b.fileA) {
                    return a.fileA < b.fileA;
                }
                return a.fileB < b.fileB;
            });

        // Stage 4: Write the ranked report to disk and console.
        fs::create_directories("reports");
        ofstream report("reports/similarity_report.txt");

        if (!report.is_open()) {
            throw runtime_error("Could not create similarity report file.");
        }

        cout << fixed << setprecision(2);
        report << fixed << setprecision(2);

        const string heading = "========== RANKED SIMILARITY REPORT ==========\n";
        cout << '\n' << heading;
        report << heading;

        for (size_t i = 0; i < results.size(); ++i) {
            const auto& result = results[i];
            ostringstream line;
            line << "\nRank #" << i + 1 << '\n'
                 << result.fileA << " <-> " << result.fileB << '\n'
                 << "Unique fingerprints in A: " << result.uniqueA << '\n'
                 << "Unique fingerprints in B: " << result.uniqueB << '\n'
                 << "Shared unique fingerprints: " << result.shared << '\n'
                 << "Similarity: " << result.similarity << "%\n";

            cout << line.str();
            report << line.str();
        }

        cout << "\nFiles processed: " << allFiles.size() << '\n';
        cout << "Comparisons performed: " << results.size() << '\n';
        cout << "Report saved to reports/similarity_report.txt\n";

        report.close();
        hash_table_destroy(&table);
        return 0;
    }
    catch (const exception& e) {
        if (tableInitialized) {
            hash_table_destroy(&table);
        }
        cerr << "\nCodeGuard error: " << e.what() << '\n';
        return 1;
    }
}
