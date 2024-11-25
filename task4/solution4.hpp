#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>
#include <cmath>
#include <cstdio>

using namespace std;

#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)

/*
 memory divided into blocks and chunks
reference: http://dmitrysoshnikov.com/compilers/writing-a-pool-allocator/#more-3891
writing this in C++ is so much more convenient
pool_id of the question varies according to the context here
pool_element_size: number of chunks in the Blocks(mostly)
TODO: Multiple thread functionality - can I implement pthread with this?, intrinsically find the maximum number of blocks that can be handled
*/

// chunk is the only "physical" entity - the rest are logical abstractions]
struct Chunk {
    // keeping the data type of data constant here
    __uint64_t data;
    // size of the chunk is constant: 2*sizeof(Chunk*) + sizeof(int) + sizeof(unsigned long int)
    int chunk_id;
    Chunk *next;
    Chunk *previous;
};

// Blocks here are the "pools" of the question
class Block {
    public:
        // requests for a new chunk when needed
        size_t num_chunks;
        size_t block_size;
        Chunk* alloc_ptr = nullptr;
        Chunk* block_begin;
        int block_id;
        // the block list is a normal list and not a free-list
        Block* next;

        // allocates a new block from OS
        Block(size_t numChunks, int id, Block* block_begin); // create individual pools - sometime after initialisation
        ~Block(); // destroy individual pools
        Chunk* pool_get();
        void pool_free(Chunk* ret);
};

// created only once during the program
class PoolAllocator {
    // allocates blocks
    size_t num_blocks;
    size_t max_num_blocks = 4; // max_num_pools
    Block* pool_begin;
    size_t total_size;
    vector<Block*> block_begin_pointers;

    public:
        PoolAllocator(size_t blocks, size_t numChunks); // alias for create_pool of the question
        ~PoolAllocator(); // alias for destroy_pools
};

template <typename T>
void checkPointerType();