
#include <stdbool.h>
#include <stdio.h>
#include <mpi.h>

// inline function to get the rank of a ranks left neighbour
static inline int get_left_rank(int my_rank, int num_ranks) {
    return (my_rank - 1) % num_ranks;
}

// inline function to get the rank of a ranks right neighbour
static inline int get_right_rank(int my_rank, int num_ranks) {
    return (my_rank + 1) % num_ranks;
}

static inline void send_right(int my_value, int my_rank, int num_ranks)
{
    int right_rank = get_right_rank(my_rank, num_ranks);
    MPI_Send(&my_value, 1, MPI_INT, right_rank, 0, MPI_COMM_WORLD);
}

static inline int receive_left(int my_rank, int num_ranks)
{
    int left_value;
    int left_rank = get_left_rank(my_rank, num_ranks);
    MPI_Recv(&left_value, 1, MPI_INT, left_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return left_value;
}


bool is_in_order(int my_value, int my_rank, int num_ranks) {
    // send right unless last
    if (my_rank != num_ranks - 1)
    {
        send_right(my_value, my_rank, num_ranks);
    }

    // Rank 0 has no left neighbour and thus has no need to check
    //   Thus defaults to passing
    if (my_rank == 0)
    {
        return true;
    }

    // grab from the left and compare
    int left_value = receive_left(my_rank, num_ranks);
    return left_value < my_value; // true = in order
}
    

int main(void) {
    // Initialize
    int my_rank, num_ranks;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    // Input
//TODO Change over from FAKE INPUT
    // Hard coded to only be 6 as my pc can only run upto 6 processes `mpirun -np 6 ./numbers1`
    // 5 would be ample given assignment constraints but I wanted for testing
    int values[6] = {5, 7, 9, 1, 13, 16}; 
    int my_value = values[my_rank];
    printf("Rank %d: My number is %d\n", my_rank, my_value);    //! Temporary

    // Check
    if (!is_in_order(my_value, my_rank, num_ranks))
    {
        printf("Process %d has at least one number out of order.\n", my_rank);
    }
    printf("Process %d has now finished.\n", my_rank);

    // Finalize
    MPI_Finalize();

    return 0;
}