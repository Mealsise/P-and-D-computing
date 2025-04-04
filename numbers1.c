
#include <stdbool.h>
#include <stdio.h>
#include <mpi.h>

// inline function to get the rank of a ranks left neighbour
inline int neighbour_left(int my_rank, int num_ranks) {
    return (my_rank - 1) % num_ranks;
}

// inline function to get the rank of a ranks right neighbour
inline int neighbour_right(int my_rank, int num_ranks) {
    return (my_rank + 1) % num_ranks;
}

int main(void) {
    // Initialize
    int my_rank, num_ranks;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    // Input
//TODO Change over from FAKE INPUT
    int values[6] = {5, 7, 9, 1, 13, 14}; // Hard coded to only be 6 as my pc can only run upto 6 processes `mpirun -np 6 ./numbers1`
    int my_value = values[my_rank];
    printf("Rank %d: My number is %d\n", my_rank, my_value);

    // Check
    bool correct = false;
    if (correct)
    {
        printf("Process %d has at least one number out of order.\n", my_rank);
    }
    printf("Process %d has now finished.\n", my_rank);

    // Finalize
    MPI_Finalize();

    return 0;
}