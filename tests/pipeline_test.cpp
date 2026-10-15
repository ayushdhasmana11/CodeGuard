#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <unordered_set>
#include <cstdint>

#include "CodeReader.h"
#include "Preprocessor.h"
#include "Tokenizer.h"
#include "FingerprintGenerator.h"
#include "../HashTable/hash_table.h"

using namespace std;

struct FileData
{
    string filename;
    vector<uint64_t> fingerprints;
};

int main()
{
    CodeReader reader;
    Preprocessor processor;
    Tokenizer tokenizer;
    FingerprintGenerator fingerprintGenerator;

    HashTable table{};

    if (!hash_table_init(&table, 10007))
    {
        cerr << "Failed to initialize hash table.\n";
        return 1;
    }

    try
    {
        vector<fs::path> files = reader.getCppFiles("data");
        vector<FileData> allFiles;

        if (files.empty())
        {
            cout << "No source files found in data folder.\n";
            hash_table_destroy(&table);
            return 0;
        }

        // Stage 1: Read, preprocess, tokenize, and fingerprint.
        // Stage 2: Store each fingerprint with its filename.
        for (const auto& filePath : files)
        {
            string originalCode =
                reader.readFile(filePath.string());

            string cleanedCode =
                processor.cleanCode(originalCode);

            vector<string> tokens =
                tokenizer.tokenize(cleanedCode);

            vector<uint64_t> fingerprints =
                fingerprintGenerator.generate(tokens);

            string filename = filePath.filename().string();

            for (uint64_t fingerprint : fingerprints)
            {
                if (!hash_table_insert(
                        &table, fingerprint, filename.c_str()))
                {
                    cerr << "Failed to insert fingerprint for "
                         << filename << '\n';

                    hash_table_destroy(&table);
                    return 1;
                }
            }

            cout << "\nFile: " << filename << '\n';
            cout << "Tokens: " << tokens.size() << '\n';
            cout << "Fingerprints: "
                 << fingerprints.size() << '\n';

            allFiles.push_back({filename, fingerprints});
        }

        // Stage 3: Count unique fingerprints shared
        // with at least one other source file.
        cout << "\n========== SHARED FINGERPRINTS ==========\n";

        for (const auto& file : allFiles)
        {
            unordered_set<uint64_t> uniqueFingerprints(
                file.fingerprints.begin(),
                file.fingerprints.end());

            size_t sharedCount = 0;

            for (uint64_t fingerprint : uniqueFingerprints)
            {
                const SourceFile* matches =
                    hash_table_lookup(&table, fingerprint);

                bool shared = false;

                for (const SourceFile* match = matches;
                     match != nullptr;
                     match = match->next)
                {
                    if (file.filename != match->filename)
                    {
                        shared = true;
                        break;
                    }
                }

                if (shared)
                    ++sharedCount;
            }

            cout << file.filename
                 << ": " << sharedCount
                 << " unique fingerprints also found in other files\n";
        }

        cout << "\nUnique fingerprints stored: "
             << table.entry_count << '\n';
    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << '\n';
        hash_table_destroy(&table);
        return 1;
    }

    hash_table_destroy(&table);
    return 0;
}