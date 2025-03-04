# Helper scripts using C++

## Synthetic simulations

### Produce a fasta file of random sequences with mutation introductions

    g++ p_mutate_simulation_random_sequence.cpp -o p_mutate_simulation_random_sequence
    ./p_mutate_simulation_random_sequence 15 0.5 10

### Take a fasta file with a real sequence and introduce mutations

    g++ p_mutate_simulation_real_sequence.cpp -o p_mutate_simulation_real_sequence
    ./p_mutate_simulation_real_sequence -input tal1.fna -p 0.5 -iterations 10

## Estimate dN/dS Estimations

Take a fasta file and estimate dN/dS using KaKs_Calculator:

    g++ kaks.cpp -o kaks
    ./kaks -input mutated_real_sequence.fna
