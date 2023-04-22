#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iomanip>


// Define a table that maps mnemonics to their corresponding numeric values
const std::unordered_map<std::string, std::string> MnemonicToNumericTable = {
        {"MOVEI", "E"}, {"ADD", "D"}, {"BNE", "FD"}, {"RTS", "FFF0"}
};

// Function to process a single line of input and write the result to the output file
bool ProcessLine(const std::string& line, const int lineNumber, std::ofstream& outputFile) {
    std::istringstream iss(line);
    std::string mnemonic, operand;
    iss >> mnemonic >> operand;

    auto it = MnemonicToNumericTable.find(mnemonic);
    if (it == MnemonicToNumericTable.end()) {
        std::cerr << "Invalid mnemonic on line " << lineNumber << ": " << mnemonic << '\n';
        return false;
    }

    std::string numeric = it->second;
    if (numeric == "E" || numeric == "D" || numeric == "FD") {
        if (operand.empty()) {
            std::cerr << "Missing operand on line " << lineNumber << ": " << mnemonic << '\n';
            return false;
        }

        unsigned int operandValue;
        std::stringstream ss(operand);
        ss >> std::hex >> operandValue;

        if (operandValue < 0 || operandValue > 0xFFF) {
            std::cerr << "Invalid operand on line " << lineNumber << ": " << operand << '\n';
            return false;
        }

        ss << std::uppercase << std::hex << operandValue;
        numeric = it->second + ss.str();
    }

    outputFile << lineNumber << " : " << numeric << '\n';
    return true;
}

int main(int argc, char* argv[]) {
    // Check if correct number of arguments provided
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " input_file.asm output_file.cdm\n";
        return 1;
    }

    // Open input and output files
    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);
    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening files\n";
        return 1;
    }

    // Process each line of the input file
    int lineNumber = 0;
    for (std::string line; std::getline(inputFile, line); ++lineNumber) {
        if (!ProcessLine(line, lineNumber, outputFile)) {
            return 1;
        }
    }

    // Close input and output files
    inputFile.close();
    outputFile.close();

    return 0;
}