#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir _mkdir
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

class FileEncryptor {
private:
    int shift;
    mutable std::string lastError;  // Made mutable to allow modification in const methods
    static constexpr int MIN_SHIFT = 1;
    static constexpr int MAX_SHIFT = 25;

    // Check if file exists
    bool fileExists(const std::string& path) const {
        struct stat buffer;
        return (stat(path.c_str(), &buffer) == 0);
    }

    // Validate shift value
    bool validateShift(int value) const {
        return value >= MIN_SHIFT && value <= MAX_SHIFT;
    }

    // Validate file paths
    bool validatePaths(const std::string& inputPath, const std::string& outputPath) const {
        if (!fileExists(inputPath)) {
            lastError = "Input file does not exist: " + inputPath;
            return false;
        }
        
        if (fileExists(outputPath)) {
            lastError = "Output file already exists: " + outputPath;
            return false;
        }
        
        return true;
    }

    // Process a single character
    char processChar(char ch, bool encryptMode) const {
        if (!std::isalpha(ch)) {
            return ch;
        }

        char base = std::isupper(ch) ? 'A' : 'a';
        int offset = encryptMode ? shift : (26 - shift);
        return ((ch - base + offset) % 26) + base;
    }

    // Get file size
    size_t getFileSize(const std::string& filePath) const {
        struct stat buffer;
        if (stat(filePath.c_str(), &buffer) == 0) {
            return buffer.st_size;
        }
        return 0;
    }

public:
    // Constructor with validation
    FileEncryptor(int shiftValue) {
        if (!validateShift(shiftValue)) {
            throw std::invalid_argument("Shift value must be between " + 
                std::to_string(MIN_SHIFT) + " and " + std::to_string(MAX_SHIFT));
        }
        shift = shiftValue;
    }

    // Get last error message
    std::string getLastError() const {
        return lastError;
    }

    // Process file with progress reporting
    bool processFile(const std::string& inputPath, const std::string& outputPath, bool encryptMode) {
        try {
            // Validate paths
            if (!validatePaths(inputPath, outputPath)) {
                return false;
            }

            // Open files
            std::ifstream inFile(inputPath, std::ios::binary);
            std::ofstream outFile(outputPath, std::ios::binary);

            if (!inFile || !outFile) {
                lastError = "Failed to open file(s)";
                return false;
            }

            // Get file size for progress reporting
            size_t fileSize = getFileSize(inputPath);
            size_t processedBytes = 0;
            const size_t progressInterval = std::max(fileSize / 10, size_t(1)); // Report every 10%

            // Process file
            char ch;
            while (inFile.get(ch)) {
                outFile.put(processChar(ch, encryptMode));
                processedBytes++;

                // Report progress
                if (processedBytes % progressInterval == 0) {
                    int progress = (processedBytes * 100) / fileSize;
                    std::cout << "\rProgress: " << progress << "%" << std::flush;
                }
            }

            std::cout << "\rProgress: 100%" << std::endl;
            return true;
        }
        catch (const std::exception& e) {
            lastError = "Error: " + std::string(e.what());
            return false;
        }
    }
};

// Input validation function
int getValidatedInput(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min && value <= max) {
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number between " 
                  << min << " and " << max << ".\n";
    }
}

// Get file path with validation
std::string getFilePath(const std::string& prompt) {
    std::string path;
    while (true) {
        std::cout << prompt;
        std::cin >> path;
        if (!path.empty()) {
            return path;
        }
        std::cout << "Invalid path. Please try again.\n";
    }
}

int main() {
    try {
        std::cout << "╔════════════════════════════════════════╗\n"
                  << "║        FILE ENCRYPTOR/DECRYPTOR        ║\n"
                  << "╚════════════════════════════════════════╝\n\n";

        // Get operation choice
        int choice = getValidatedInput(
            "Choose operation:\n"
            "1. Encrypt File\n"
            "2. Decrypt File\n"
            "Enter choice (1-2): ", 1, 2);

        // Get shift value
        int shiftValue = getValidatedInput(
            "Enter Caesar cipher shift value (1-25): ", 1, 25);

        // Create encryptor instance
        FileEncryptor encryptor(shiftValue);

        // Single file processing
        std::string inputFile = getFilePath("Enter input file path: ");
        std::string outputFile = getFilePath("Enter output file path: ");
        
        bool success = encryptor.processFile(inputFile, outputFile, choice == 1);

        if (success) {
            std::cout << (choice == 1 ? "🔐 Encryption" : "🔓 Decryption") 
                      << " completed successfully.\n";
        } else {
            std::cerr << "❌ Error: " << encryptor.getLastError() << "\n";
            return 1;
        }

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "❌ Fatal error: " << e.what() << "\n";
        return 1;
    }
}
