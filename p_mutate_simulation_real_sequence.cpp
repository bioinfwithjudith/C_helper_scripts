#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>  // for std::rand and std::srand
#include <ctime>    // for std::time
#include <fstream>  // for file output
#include <sstream>  // for string conversion
#include <map>      // argument for parsing

// Declar and define function to choose a random nucleotide base
char random_base() {
    std::vector<char> bases = {'G', 'A', 'T', 'C'};
    int randomize = std::rand() % bases.size();
    char base = bases[randomize];
    //std::cout << "Random base chosen: " << base << std::endl; 
    return base;
}

// Declare and define function to create a random DNA sequence
std::string create_random_sequence(int n) {
    std::string random;
    random.reserve(n); //improve performance by reserving space
    for (int i = 0; i < n; i++) {
        random += random_base();
    }
    return random;
}

// Function to introduce mutations into a DNA sequence with a mutation rate of 0.01
std::string introduce_mutations(const std::string& sequence, double mutation_rate = 0.01) {
    std::string mutated_sequence = sequence;
    std::vector<char> bases = {'G', 'A', 'T', 'C'};
    for (size_t i = 0; i < mutated_sequence.size(); i++) {
        double random_chance = static_cast<double>(std::rand()) / RAND_MAX;
        if (random_chance <= mutation_rate) {
            char original_base = mutated_sequence[i];
            char new_base;
            do {
                new_base = random_base();
            } while (new_base == original_base); // Ensure mutation changes the base
            mutated_sequence[i] = new_base;
        }
    }
    return mutated_sequence;
}

int main(int argc, char* argv[]) {
    std::srand(std::time(nullptr));  // Seed the random number generator

    std::map<std::string, std::string> args;
    for (int i = 1; i < argc - 1; i += 2) {
        args[argv[i]] = argv[i + 1];
    }

    // Check if the user asks for help
    if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        std::cerr << "Usage: " << argv[0] << " <sequence_file> <mutation_rate> <iterations>" << std::endl;
        std::cerr << "Introduces a specified mutation rate in a DNA sequence from an input FASTA file." << std::endl;
        return 0;
    }

    // Check if the correct number of arguments are provided
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <sequence_file> <mutation_rate> <iterations>" << std::endl;
        return 1;
    }

    // Validate required arguments
    if (!args.count("-input") || !args.count("-p") || !args.count("-iter")) {
        std::cerr << "Usage: " << argv[0] << " -input <sequence_file> -p <mutation_rate> -iter <iterations>" << std::endl;
        return 1;
    }

    // Convert command-line arguments
    std::string input_file_name = args["-input"];
    double mutation_rate = std::stod(args["-p"]);
    int iterations = std::stoi(args["-iter"]);

    std::ifstream input_file(input_file_name); // Open file for reading
    std::ofstream output_file("mutated_real_sequence.fna");

    if (!input_file) { // Check if the file opened successfully
        std::cerr << "Error: Could not open file for reading." << std::endl;
        return 1;
    }

    if (!output_file) { // Check if the output file opened successfully
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }

    std::string line;
    for (int i = 0; i < iterations; i++) {
        while (std::getline(input_file, line)) { // Read file line by line
            if (!line.empty() && line[0] == '>' && i == 0) {
                output_file << ">reference" << "\n";  // Preserve header lines
            }
            else if (!line.empty() && line[0] != '>' && i == 1) {  // Mutation occurs only in sequence lines
                output_file << line << "\n";
            }
            else if (!line.empty() && line[0] == '>' && i > 1) {
                output_file << ">mutation_" << i << "\n";  // Preserve header lines
            } 
            else if (!line.empty() && line[0] != '>' && i > 1) {  // Mutation occurs only in sequence lines
                output_file << introduce_mutations(line, mutation_rate) << "\n";
            }
        }
    input_file.clear();  // Clear EOF flag to allow re-reading the file in the next iteration
    input_file.seekg(0, std::ios::beg);  // Move file pointer back to the beginning
    }

    input_file.close();
    output_file.close();

    std::cout << "Mutated sequence written to mutated_sequence.fna" << std::endl;

    return 0;
}
