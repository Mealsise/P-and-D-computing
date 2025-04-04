
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

    // Check
    bool correct = false;
    if (correct)
    {
        printf("Process <rank> has at least one number out of order.");
    }
    printf("Process <rank> has now finished.");

    // Finalize
    MPI_Finalize();

    return 0;
}