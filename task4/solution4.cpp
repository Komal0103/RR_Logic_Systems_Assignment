#include "solution4.hpp"

#define CHUNK_SIZE sizeof(Chunk)
#define BLOCK_SIZE sizeof(Block)
#define ALLOCATOR_SIZE sizeof(PoolAllocator)

template <typename T>
void checkPointerType() {
    if constexpr (std::is_pointer<T>::value) {
        std::cout << "The pointed-to type is: " << typeid(std::remove_pointer_t<T>).name() << "\n";
    } else {
        std::cout << "T is not a pointer type.\n";
    }
}

// allocates individual blocks or "pools": this is sometime after PoolAllocator initialisation for the first time
Block :: Block(size_t numChunks, int id, Block* blockBegin, bool flag) : num_chunks(numChunks), block_id(id) {
    Chunk *chunk;
    block_size = num_chunks * CHUNK_SIZE + BLOCK_SIZE;
    // flag will be true when the Block constructor is called directly. This is in case of allocations after the PoolAllocator object is declared.
    if (flag) {
        // size of block is a member function
        size_t adjusted_size = ceil(block_size / ((float)PAGE_SIZE)) * PAGE_SIZE;
        block_begin = reinterpret_cast<Chunk*>(mmap(NULL, adjusted_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
        if (block_begin == MAP_FAILED) {
            perror("mmap mapping failed");
            exit(1);
        }
        cout<<block_begin<<endl;
        chunk = block_begin;
    }
    else {
        // typecast input blockBegin to Chunk* for allocation
        void *temp_void = static_cast<void*>(blockBegin);
        chunk = static_cast<Chunk*>(temp_void);
        block_begin = chunk;
        cout<<block_begin<<endl;
    }
    // create chunks within the block
    alloc_ptr = chunk;
    for (int j=0; j<numChunks; ++j) {
        chunk->previous = chunk;
        chunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + CHUNK_SIZE);
        chunk->chunk_id = j;
        chunk = chunk->next;
    }
    cout<<"Block allocation complete! Yayy!"<<endl;
}

// only called when flag is true, i.e., the block constructor is called independently.
Block :: ~Block() {
    // if the current_block is on this block, change to the next block. If this is the last block use nullptr
    // destroys the individual pools created
    int success = munmap(reinterpret_cast<void*>(block_begin), block_size);
    if (success != 0){
        perror("object destruction unsuccesful");
    }
    cout<<"object destruction successful"<<endl;
    // destroy the pool allocator object from the main program
}

// I don't know how correct this is, but I have allocated both the blocks and chunks in this for the initialisation
PoolAllocator :: PoolAllocator(size_t blocks, size_t numChunks) : num_blocks(blocks) {
    // PoolAllocator object is created only once, and is sort of like an interface to the memory to the user
    size_t block_size = static_cast<size_t>(numChunks*CHUNK_SIZE) + BLOCK_SIZE;
    total_size = static_cast<size_t>(blocks*block_size) + ALLOCATOR_SIZE;
    size_t adjusted_size = ceil(total_size / ((float)PAGE_SIZE)) * PAGE_SIZE;
    // currently supports allocating blocks of equal sizes during initialisation. Below the total memory has been allocated. Divide it into blocks and chunks later
    pool_begin = reinterpret_cast<Block*>(mmap(NULL, adjusted_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)); // pointer to the first block
    if (pool_begin == MAP_FAILED) {
        perror("mmap mapping failed");
        exit(1);
    }
    cout<<pool_begin<<endl;
    current_block = pool_begin;
    // construct the first block
    // allocate the blocks and chunks in the same way in the same section
    block_begin_pointers.resize(num_blocks);
    for (int i=0; i<num_blocks; i++){
        Block *memory = reinterpret_cast<Block*>(reinterpret_cast<char*>(pool_begin) + i*block_size);
        // cout<<memory<<endl;
        Block *temp = new (memory) Block(numChunks, i, memory, false);
        block_begin_pointers[i] = temp;
    }
    block_begin_pointers[blocks-1]->next = nullptr; // next pointer of the last block
    cout<<"Pool initialisation and allocation complete!"<<endl;
}

PoolAllocator :: ~PoolAllocator(){
    int success = munmap(reinterpret_cast<void*>(pool_begin), total_size);
    if (success != 0){
        perror("pool allocator destruction unsuccesful");
    }
    // destroy the pool allocator object from the main program
    // call delete <pool-allocator-name-pointer>
    cout<<"PoolAllocator destruction successful"<<endl;
}

Chunk* PoolAllocator :: pool_get(){
    // current_block is a member
    Block *block = current_block;
    Chunk* current = block->alloc_ptr;
    Chunk *to_return = current;
    current->previous->next = current->next;
    if (current->next != nullptr){
        current->next->previous = current->previous;
        block->alloc_ptr = current->next;
    }
    else {
        // if the chunk is the last in the block
        // If we have reached the end of the current block, move the block pointer to the next block
        auto it = find(block_begin_pointers.begin(), block_begin_pointers.end(), block);
        // if the block is the last one, just put the pointers as nullptr, and give warning
        if (it == block_begin_pointers.end() - 1){
            current_block = nullptr;
            current->previous->next = nullptr;
            cout<<"The current block is the last one. Cannot move to the next block. Do not ask for any more chunks please, unless you allocate new ones"<<endl;
        }
        else {
            block = block_begin_pointers[distance(block_begin_pointers.begin(), it) + 1];
            block->alloc_ptr = block->block_begin;
            cout<<"Block shifted to the next one"<<endl;
        }
    }
    cout<<"Given chunk: "<<to_return->chunk_id<<" from block: "<<block->block_id<<endl;
    // give current to the application
    return to_return;
}

// if (current_block == ret) {
//         auto it = find(block_begin_pointers.begin(), block_begin_pointers.end(), ret);
//         current_block = block_begin_pointers[distance(block_begin_pointers.begin(), it) - 1];
//         current_block->next = nullptr;
//     }

void PoolAllocator :: pool_free(Chunk *ret){
    // find the first block that has space for additional chunks
    for (auto block : block_begin_pointers){
        if (block->num_chunks < block->max_chunks){
            // add the returning chunk to the end of the block
            Chunk *last = block->alloc_ptr;
            while (last->next != nullptr){
                last = last->next;
            }
            last->next = ret;
            ret->next = nullptr;
            ret->previous = last;
            cout<<"Returned chunk to pool at block: "<<block->block_id<<" at position: "<<last->chunk_id<<"!"<<endl;
            break;
        }
    }
}

Block* allocate_additional_pools(PoolAllocator* pl, int num_chunks_in_each_block, int num_blocks) {
    Block *bl;
    if (pl->num_blocks < pl->max_num_blocks){
        int last_block_id_in_pool = pl->num_blocks - 1;
        bl = new Block(num_chunks_in_each_block, last_block_id_in_pool + 1, nullptr, true);
        pl->block_begin_pointers.push_back(bl);
        pl->num_blocks++;
        pl->total_size += sizeof(Block);
    }
    cout<<"Allocation successful"<<endl;
    return bl;
}