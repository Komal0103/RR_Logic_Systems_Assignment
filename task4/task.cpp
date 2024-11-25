#include "solution4.hpp"

using namespace std;
/*
Sequence of arguments while compiling:]
size of chunks is constant in this - unsigned long int - area of improvement: use templates to change the datatype
user defined limits on the number of chunks and blocks that can be allocated
number of chunks in each block
number of blocks (or pools) during initialisation
*/

int main(int argc, char* argv[]){
    // size of chunk: 2*8 + 4 + 8 = 28 bytes
    int num_chunks_in_each_block = *argv[1] - '0';
    int num_blocks = *argv[2] - '0';
    int max_chunks = *argv[3] - '0';
    int max_blocks = *argv[4] - '0';
    PoolAllocator *pl = new PoolAllocator(num_blocks, num_chunks_in_each_block);
    pl->~PoolAllocator();
    return 0;
}