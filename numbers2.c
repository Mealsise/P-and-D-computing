#include <stdbool.h>

int MASTER_RANK = 0;






// inline function to get the rank of a ranks left neighbour
inline int get_left__neighbour(int my_rank, int num_ranks) {
    return (my_rank - 1) % num_ranks;
}

// inline function to get the rank of a ranks right neighbour
inline int get_right_neighbour(int my_rank, int num_ranks) {
    return (my_rank + 1) % num_ranks;
}








int chinese_whisper(int my_rank, int source_rank, int target_rank, int num_ranks, int data = 0)
{
    // Error catching for invalid path
    if (source_rank < target_rank) 
//! EARLY RETURN
        return chinese_whisper(my_rank, target_rank, source_rank, num_ranks, data);

    // Check and exit if this rank
    bool is_rank__left = my_rank < target_rank;
    bool is_rank_right = my_rank > source_rank;
    
    if (is_rank__left || is_rank_right)
//! EARLY RETURN
        return data;

    // If sending 0 distance
    if (source_rank == target_rank)
//! EARLY RETURN
        return data;

    int new_data = data;
    
    bool is_rank___sender = my_rank > target_rank;
    bool is_rank_receiver = my_rank < source_rank;

    if (is_rank___sender)
    {
        int left__neighbour = get_left__neighbour(my_rank, num_ranks);
        // send data to left__neighbour
    }

    if (is_rank_receiver)
    {
        int right_neighbour = get_right_neighbour(my_rank, num_ranks);
        // receive from right_neighbour
    }

//! ACTUAL RETURN
    return new_data;
}




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


int pack_data(bool any_out_of_order, int n_out_of_order)
{
    int data;
    data = n_out_of_order << 1;
    data += any_out_of_order ? 1 : 0;

    return data;
}

bool unpack_data_any(int data)
{
    return data % 2;
}

int unpack_data_n(int data)
{
    return data >> 1;
}


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



    /* 
    chinese_whisper makes it so that i can tell every rank:
    "Hey a message needs to get from A to B"
    and then if that rank is involved in that chain it will grab from the right and give to left
    else it will just do nothing.
    */
    for (int rank = 0; rank <= world_size; rank++)
    {
        int data = chinese_whisper(my_rank, rank, MASTER_RANK, world_size, my_data);
        
        if (my_rank == MASTER_RANK)
        {
            print_message_from_data(rank, data);
        }
    }



    return 0;
}