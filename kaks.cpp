#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>  // for std::rand, std::srand, and system()
#include <ctime>    // for std::time
#include <fstream>  // for file output
#include <sstream>  // for string conversion
#include <map>      // argument for parsing


int main(int argc, char* argv[]) {
    std::srand(std::time(nullptr));  // Seed the random number generator

    std::map<std::string, std::string> args;
    for (int i = 1; i < argc - 1; i += 2) {
        args[argv[i]] = argv[i + 1];
    }

    // Check if the user asks for help
    if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        std::cerr << "Usage: " << argv[0] << " <sequence_file>" << std::endl;
        std::cerr << "Produce an AXT file and Apply the NG86 dN/dS model using KaKs_Calculator from a fasta input file." << std::endl;
        return 0;
    }

    // Validate required arguments
    if (!args.count("-input")) {
        std::cerr << "Usage: " << argv[0] << " -input <sequence_file>" << std::endl;
        return 1;
    }

    // Convert command-line arguments
    std::string input_file_name = args["-input"];

    std::ifstream input_file(input_file_name); // Open file for reading
    std::ofstream output_file("kaks.axt");

    if (!input_file) { // Check if the file opened successfully
        std::cerr << "Error: Could not open file for reading." << std::endl;
        return 1;
    }

    if (!output_file) { // Check if the output file opened successfully
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }

    std::string line;
    std::string reference_sequence;
    std::string query_sequence;
    std::string current_header;
    std::map<std::string, std::string> sequences;
    std::vector<std::string> headers;
    
    while (std::getline(input_file, line)) {
        if (!line.empty() && line[0] == '>') {  // If it's a header
            if (!current_header.empty()) { // Store previous sequence
                sequences[current_header] = query_sequence;
                query_sequence.clear();
            }
            current_header = line.substr(1); // Remove '>'
            headers.push_back(current_header);
        } else {
            query_sequence += line; // Concatenate sequence lines
        }
    }
    
    // Store last sequence
    if (!current_header.empty()) {
        sequences[current_header] = query_sequence;
    }
    
    reference_sequence = sequences[headers[0]]; // First sequence is the reference
    
    for (size_t i = 0; i < headers.size(); i++) {
        output_file << headers[i] << "\n";
        output_file << reference_sequence << "\n";
        output_file << sequences[headers[i]] << "\n\n";
    }

    input_file.close();
    output_file.close();

    std::cout << "AXT file written to output.axt" << std::endl;

    // Run KaKs_Calculator after writing the AXT file
    int ret = system("KaKs_Calculator -i kaks.axt -o results.axt.kaks -m NG");
    if (ret == 0) {
        std::cout << "KaKs_Calculator executed successfully." << std::endl;
    } else {
        std::cerr << "Error executing KaKs_Calculator. Return code: " << ret << std::endl;
    }
    
    return 0;
}