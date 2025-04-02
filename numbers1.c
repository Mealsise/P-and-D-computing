
#include <stdbool.h>

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

    // Input

    // Check
    bool correct = false;
    if (correct)
    {
        printf("Process <rank> has at least one number out of order.");
    }
    printf("Process <rank> has now finished.");

    // Finalize

    return 0;
}