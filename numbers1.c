/*
 * Tyler Roe-Smith
 * Student ID: a1899603
 *
 * Assignment 1 Milestone – numbers1.c
 *
 * This program implements a ring-based MPI communication pattern with five processes.
 * Each process holds a number and compares it to its left neighbour's number.
 * If the value is out of order (less than the left neighbour), a message is printed.
 * 
 * Only immediate left and right neighbour communication is allowed.
 * This milestone version uses hardcoded input for testing and demonstrates basic
 * inter-process communication using point-to-point MPI functions.
 */

#include <stdbool.h>
#include <stdio.h>
#include <mpi.h>

// Returns the rank of the left neighbour in the ring topology.
// Uses modulo to wrap around for rank 0, making the topology circular.
// Example: if my_rank = 0 and num_ranks = 5 → left = 4
static inline int get_left_rank(int my_rank, int num_ranks) {
    return (my_rank - 1) % num_ranks;
}

// Returns the rank of the right neighbour in the ring topology.
// Uses modulo to wrap around for the last rank.
// Example: if my_rank = 4 and num_ranks = 5 → right = 0
static inline int get_right_rank(int my_rank, int num_ranks) {
    return (my_rank + 1) % num_ranks;
}

// Sends this process's value to its right neighbour in the ring.
// This function uses point-to-point MPI_Send communication.
static inline void send_right(int my_value, int my_rank, int num_ranks)
{
    int right_rank = get_right_rank(my_rank, num_ranks);
    MPI_Send(&my_value, 1, MPI_INT, right_rank, 0, MPI_COMM_WORLD);
}

// Receives an integer value from the left neighbour in the ring.
// This function uses point-to-point MPI_Recv communication.
static inline int receive_left(int my_rank, int num_ranks)
{
    int left_value;
    int left_rank = get_left_rank(my_rank, num_ranks);
    MPI_Recv(&left_value, 1, MPI_INT, left_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return left_value;
}

// Determines if the current process's value is in correct order
// relative to its left neighbour in a ring topology.
// Returns true if the value is in order (i.e., not less than the left neighbour's value),
// or if this is rank 0, which has no left neighbour.
//
// In this ring communication model:
// - Each process (except the last) sends its value to the right neighbour
// - Each process (except rank 0) receives a value from its left neighbour
// - A value is considered out of order if it is less than its left neighbour's value
bool is_in_order(int my_value, int my_rank, int num_ranks) {
    // Send this process’s value to the right neighbour.
    // Only needed if not the last rank (to avoid wrapping unnecessarily).
    if (my_rank != num_ranks - 1)
    {
        send_right(my_value, my_rank, num_ranks);
    }

    // Rank 0 has no left neighbour to compare against.
    // By definition, it is always considered "in order".
    if (my_rank == 0)
    {
        return true;
    }

    // Receive the left neighbour’s value and compare.
    // If our value is greater than or equal to the left neighbour's, it is in order.
    int left_value = receive_left(my_rank, num_ranks);
    return left_value < my_value; // true = in order
}
    

int main(void) {
    // Initialize MPI environment and get process details.
    int my_rank, num_ranks;
    MPI_Init(NULL, NULL);                              // Start the MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);           // Get the rank of this process
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);         // Get the total number of processes

    // Provide input values for each rank (temporary setup).
    // TODO: Real implementation will include external input
    // For testing, I hardcode an array and each process selects its value by rank.
    // This assumes no more than 6 processes are launched (`mpirun -np 6 ./numbers1`).
    // Running with more may cause out-of-bounds access (undefined behaviour).
    int values[6] = {5, 7, 9, 1, 13, 16}; 
    int my_value = values[my_rank];
    printf("Rank %d: My number is %d\n", my_rank, my_value);  // Temporary debug output

    // Check if this process's value is in correct order relative to the left neighbour.
    // If not, print a message indicating that it's out of order.
    if (!is_in_order(my_value, my_rank, num_ranks))
    {
        printf("Process %d has at least one number out of order.\n", my_rank);
    }

    // All processes print a termination message as required.
    printf("Process %d has now finished.\n", my_rank);

    // Finalize MPI environment.
    MPI_Finalize();

    return 0;
}