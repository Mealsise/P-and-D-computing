/*
 * Tyler Roe-Smith
 * Student ID: a1899603
 *
 * Assignment 1 Milestone – numbers2.c
 *
 * This MPI program builds on numbers1.c by introducing rank 0 as a master process.
 * When a process identifies its value as out of order, it sends a message to the master
 * by relaying it left through intermediate processes in the ring.
 * 
 * The master receives and prints these messages in order, reporting which processes
 * are out of order and how many total violations were found.
 *
 * This milestone version supports LEFT-only communication and uses hardcoded input
 * for demonstration purposes.
 */


#include <stdbool.h>
#include <stdio.h>
#include <mpi.h>

// The rank of the master process (used in numbers2.c).
// This process coordinates result collection and final output.
int MASTER_RANK = 0;

// Enum to represent communication direction in a ring topology.
// LEFT  = -1 → send to the previous rank
// RIGHT =  1 → send to the next rank
enum Direction {
    LEFT = -1,
    RIGHT = 1
};

// Returns the rank of the neighbour in the given direction (LEFT or RIGHT).
// Uses modulo to wrap around the ring correctly, ensuring circular topology.
// Example: if my_rank = 0, num_ranks = 5, direction = LEFT → returns 4
static inline int get_neighbour_rank(int my_rank, int num_ranks, enum Direction direction)
{
    return (my_rank + direction + num_ranks) % num_ranks;
}

// Sends an integer value to the neighbour in the specified direction.
// Wraps around the ring using get_neighbour_rank() to find the destination.
// Uses point-to-point MPI_Send communication.
static inline void send(int my_value, int my_rank, int num_ranks, enum Direction direction)
{
    int address_rank = get_neighbour_rank(my_rank, num_ranks, direction);
    MPI_Send(&my_value, 1, MPI_INT, address_rank, 0, MPI_COMM_WORLD);
}

// Receives an integer value from the neighbour in the specified direction.
// Wraps around the ring using get_neighbour_rank() to find the source.
// Uses point-to-point MPI_Recv communication.
static inline int receive(int my_rank, int num_ranks, enum Direction direction)
{
    int received_value;
    int address_rank = get_neighbour_rank(my_rank, num_ranks, direction);
    MPI_Recv(&received_value, 1, MPI_INT, address_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return received_value;
}

// Relays an integer message from a source process to a destination process,
// using only LEFT direction (decreasing rank) in a ring topology.
// Each process can only communicate with its immediate neighbours.
// 
// The message is passed along from the source to the destination,
// hopping through intermediate processes. Each process in the path:
// - Receives the message from its right neighbour
// - Then forwards it to its left neighbour (unless it's the destination)
//
// This implementation assumes LEFT-only communication by ensuring that
// source_rank > destination_rank. If not, they are swapped internally.
//
// Example: If source = 3, destination = 1, and num_ranks = 5:
// - Rank 3 sends to 2
// - 2 receives from 3, sends to 1
// - 1 receives from 2 and stores the message
int relay_message(
    int my_rank,
    int source_rank,
    int destination_rank,
    int num_ranks,
    int message
    )
{
    // Normalize direction to enforce LEFT-only communication.
    // Swap if needed so source > destination.
    if (source_rank < destination_rank) {
        int temp = source_rank;
        source_rank = destination_rank;
        destination_rank = temp;
    }

    // Identify if this process is in the receiving path:
    // It lies between destination and source (excluding source).
    bool is_receiver = my_rank >= destination_rank && my_rank <  source_rank;

    // Identify if this process is in the sending path:
    // It lies between destination and source (excluding destination).
    bool is_sender   = my_rank >  destination_rank && my_rank <= source_rank;
    
    // If in the receiving path, receive the message from the right neighbour.
    if (is_receiver) message = receive(my_rank, num_ranks, RIGHT);

    // If in the sending path, send the message to the left neighbour.
    if (is_sender) send(message, my_rank, num_ranks, LEFT);
    
    // Return the final message (used if this process is the destination).
    return message;
}







// Main function not fully implemented but it does have master being a master with ordered outputs, just not output the comparision

int main(void)
{
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
    


    // Testing and this area will be used for actual functionality

    for (int rank = 0; rank < num_ranks; rank++)
    {
        int data = my_rank;
        data = relay_message(my_rank, rank, MASTER_RANK, num_ranks, data);
        
        if (my_rank == MASTER_RANK)
        {
            printf("%d\n", data);
        }
    }





    // Finalize MPI environment.
    MPI_Finalize();

    return 0;
}
