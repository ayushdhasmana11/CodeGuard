#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iomanip>
#include <cstdint>
#include <stdexcept>

#include "Reader/CodeReader.h"
#include "Preprocessing/Preprocessor.h"
#include "Tokenizer.h"
#include "FingerprintGenerator.h"
#include "HashTable/hash_table.h"

using namespace std;

struct FileData
{
    string filename;
    vector<uint64_t> fingerprints;
};

struct SimilarityResult
{
    string fileA;
    string fileB;
    size_t uniqueA;
    size_t uniqueB;
    size_t shared;
    double similarity;
};

int main()
{
    CodeReader reader;
    Preprocessor preprocessor;
    Tokenizer tokenizer;
    FingerprintGenerator fingerprintGenerator;

    HashTable table{};

    try
    {
        vector<fs::path> files = reader.getCppFiles("data");

        if (files.empty())
        {
            cout << "No source files found in data folder.\n";
            return 0;
        }

        if (!hash_table_init(&table, 1009))
        {
            cerr << "Failed to initialize hash table.\n";
            return 1;
        }

        vector<FileData> allFiles;

        // Stage 1: Read, clean, tokenize and fingerprint each file.
        for (const auto& filePath : files)
        {
            string originalCode =
                reader.readFile(filePath.string());

            string cleanedCode =
                preprocessor.cleanCode(originalCode);

            vector<string> tokens =
                tokenizer.tokenize(cleanedCode);

            vector<uint64_t> fingerprints =
                fingerprintGenerator.generate(tokens);

            FileData data;
            data.filename = filePath.filename().string();
            data.fingerprints = fingerprints;

            for (uint64_t fingerprint : fingerprints)
            {
                if (!hash_table_insert(
                        &table,
                        fingerprint,
                        data.filename.c_str()))
                {
                    throw runtime_error(
                        "Failed to insert fingerprint into hash table."
                    );
                }
            }

            cout << "\nFile: " << data.filename << '\n';
            cout << "Tokens: " << tokens.size() << '\n';
            cout << "Fingerprints: "
                 << fingerprints.size() << '\n';

            allFiles.push_back(data);
        }

        // Stage 2: Calculate similarity for every file pair.
        vector<SimilarityResult> results;

        for (size_t i = 0; i < allFiles.size(); ++i)
        {
            for (size_t j = i + 1; j < allFiles.size(); ++j)
            {
                const auto& a = allFiles[i];
                const auto& b = allFiles[j];

                unordered_set<uint64_t> uniqueA(
                    a.fingerprints.begin(),
                    a.fingerprints.end());

                unordered_set<uint64_t> uniqueB(
                    b.fingerprints.begin(),
                    b.fingerprints.end());

                size_t shared = 0;

                for (uint64_t fingerprint : uniqueA)
                {
                    if (uniqueB.count(fingerprint) > 0)
                    {
                        ++shared;
                    }
                }

                size_t totalA = uniqueA.size();
                size_t totalB = uniqueB.size();

                double similarity = 0.0;

                if (totalA + totalB > 0)
                {
                    similarity =
                        (2.0 * shared /
                         (totalA + totalB)) * 100.0;
                }

                results.push_back({
                    a.filename,
                    b.filename,
                    totalA,
                    totalB,
                    shared,
                    similarity
                });
            }
        }

        // Stage 3: Sort highest similarity first.
        sort(results.begin(), results.end(),
             [](const SimilarityResult& a,
                const SimilarityResult& b)
             {
                 if (a.similarity != b.similarity)
                 {
                     return a.similarity > b.similarity;
                 }

                 if (a.fileA != b.fileA)
                 {
                     return a.fileA < b.fileA;
                 }

                 return a.fileB < b.fileB;
             });

        // Stage 4: Print the ranked report.
        cout << "\n========== RANKED SIMILARITY REPORT ==========\n";

        cout << fixed << setprecision(2);

        for (size_t i = 0; i < results.size(); ++i)
        {
            const auto& result = results[i];

            cout << "\nRank #" << i + 1 << '\n';

            cout << result.fileA
                 << " <-> " << result.fileB << '\n';

            cout << "Unique fingerprints in A: "
                 << result.uniqueA << '\n';

            cout << "Unique fingerprints in B: "
                 << result.uniqueB << '\n';

            cout << "Shared unique fingerprints: "
                 << result.shared << '\n';

            cout << "Similarity: "
                 << result.similarity << "%\n";
        }

        hash_table_destroy(&table);
    }
    catch (const exception& e)
    {
        hash_table_destroy(&table);
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}