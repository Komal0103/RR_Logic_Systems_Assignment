#include "solution4.hpp"

using namespace std;

/*
Sequence of arguments while compiling:]
size of chunks is constant in this - unsigned long int - area of improvement: use templates to change the datatype
number of chunks in each block
number of blocks (or pools) during initialisation
*/

int main(int argc, char* argv[]){
    // size of chunk: 2*8 + 4 + 8 = 28 bytes
    int num_chunks_in_each_block = *argv[1] - '0';
    int num_blocks = *argv[2] - '0';
    PoolAllocator *pl = new PoolAllocator(num_blocks, num_chunks_in_each_block);

    Chunk *get_chunk = pl->pool_get();
    Chunk *get_second_chunk = pl->pool_get();
    pl->pool_free(get_chunk);

    // allocate an additional pool
    Block *bl = allocate_additional_pools(pl, num_chunks_in_each_block, num_blocks);
    // delete this block
    bl->~Block();

    pl->~PoolAllocator();
    return 0;
}