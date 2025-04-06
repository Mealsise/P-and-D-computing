#include <stdbool.h>
#include <stdio.h>
#include <mpi.h>

int MASTER_RANK = 0;

enum Direction {
    LEFT = -1,
    RIGHT = 1
};

static inline int get_neighbour_rank(int my_rank, int num_ranks, enum Direction direction)
{
    return (my_rank + direction + num_ranks) % num_ranks;
}

static inline void send(int my_value, int my_rank, int num_ranks, enum Direction direction)
{
    int address_rank = get_neighbour_rank(my_rank, num_ranks, direction);
    MPI_Send(&my_value, 1, MPI_INT, address_rank, 0, MPI_COMM_WORLD);
}


static inline int receive(int my_rank, int num_ranks, enum Direction direction)
{
    int received_value;
    int address_rank = get_neighbour_rank(my_rank, num_ranks, direction);
    MPI_Recv(&received_value, 1, MPI_INT, address_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return received_value;
}

int relay_message(
    int my_rank,
    int source_rank,
    int destination_rank,
    int num_ranks,
    int message
    )
{
    // Normalize direction so we always send LEFT in decreasing rank order
    if (source_rank < destination_rank) {
        int temp = source_rank;
        source_rank = destination_rank;
        destination_rank = temp;
    }

    bool is_receiver = my_rank >= destination_rank && my_rank <  source_rank;
    bool is_sender   = my_rank >  destination_rank && my_rank <= source_rank;

    // printf("%d to %d rank %d is receiving ", source_rank, destination_rank, my_rank);
    // if (is_receiver) printf("True\n"); else printf("False\n");

    // printf("%d to %d rank %d is sending ", source_rank, destination_rank, my_rank);
    // if (is_sender) printf("True\n"); else printf("False\n");
    
    if (is_receiver) message = receive(my_rank, num_ranks, RIGHT);
    if (is_sender) send(message, my_rank, num_ranks, LEFT);
    
    return message;
}







int main(void)
{
    // Initialize MPI environment and get process details.
    int my_rank, num_ranks;
    MPI_Init(NULL, NULL);                              // Start the MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);           // Get the rank of this process
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);         // Get the total number of processes




    
    for (int rank = 0; rank < num_ranks; rank++)
    {
        int data = my_rank;
        data = relay_message(my_rank, rank, MASTER_RANK, num_ranks, data);
        
        if (my_rank == MASTER_RANK)
        {
            printf("%d\n", data);
        }
    }

    MPI_Finalize();

    return 0;
}
