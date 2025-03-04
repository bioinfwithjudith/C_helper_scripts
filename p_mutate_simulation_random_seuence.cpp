#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>  // for std::rand and std::srand
#include <ctime>    // for std::time
#include <fstream>  // for file output
#include <sstream>  // for string conversion



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

    // Check if the user asks for help
    if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        std::cerr << "Usage: " << argv[0] << " <sequence_length> <mutation_rate> <interations>" << std::endl;
        std::cerr << "Generates a random DNA sequence of given length with mutations at the specified rate." << std::endl;
        return 0;
    }

    // Check if correct number of arguments are provided
    if (argc != 4) {
        return 1;
    }

    // Convert command-line arguments
    int len = std::stoi(argv[1]);
    double p = std::stod(argv[2]);
    int iter = std::stoi(argv[3]);
    
    std::string random_sequence = create_random_sequence(len);

    std::ofstream output_file("random_sequence.fna");
    if (output_file.is_open()) {
        for (int i = 0; i < iter; i++) {
            if (i == 0) {
                output_file << ">random_sequence_ref\n";
                output_file << random_sequence + "\n";
            }
            else {
                output_file << ">random_sequence_"+ std::to_string(i) + "\n";
                output_file << introduce_mutations(random_sequence, p) + "\n";
            }     
        }
        output_file.close();
        std::cout << "Random sequence written to random_sequence.txt" << std::endl;
    } else {
        std::cerr << "Error: Could not open file for writing." << std::endl;
    }
    return 0;
}
