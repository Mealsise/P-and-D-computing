#include <stdbool.h>

int MASTER_RANK = 0;


/*
Need to redo

each will have to send to left until at master?

1 -> 0
2 -> 1 -> 0
3 -> 2 -> 1 -> 0

Might add a thing where it'll send backward

n -> 0
n-1 -> n -> 0

where n is the number of ranks
*/




void print_message(int rank, bool any_out_of_order, int n_out_of_order)
{
    if (any_out_of_order)
    {
        printf("Process %d has number %d out of order.", rank, n_out_of_order);
    }
    else
    {
        // nothing?
    }
}



int main(void)
{
    int world_size;
    int my_rank;

    // initialize MPI




    // check self
    int self_n_out_of_order;
    bool self_any_out_of_order;





    if (my_rank == MASTER_RANK)
    {
        // print own
        print_message(my_rank, self_any_out_of_order, self_any_out_of_order);
        
        for (int sender_rank = 1; sender_rank <= world_size; sender_rank++) {
            int n_out_of_order;
            bool any_out_of_order;

            // receive message from sender_rank

            // print message on behalf of sender
            print_message(sender_rank, any_out_of_order, n_out_of_order);
        }
    }
    else if (my_rank != MASTER_RANK)
    {
        // send self_n_out_of_order
        // receive self_any_out_of_order
    }


    // Finalize MPI



    return 0;
}