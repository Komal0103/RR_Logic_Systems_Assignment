#include "solution4.hpp"

#define CHUNK_SIZE sizeof(Chunk)

template <typename T>
void checkPointerType() {
    if constexpr (std::is_pointer<T>::value) {
        std::cout << "The pointed-to type is: " << typeid(std::remove_pointer_t<T>).name() << "\n";
    } else {
        std::cout << "T is not a pointer type.\n";
    }
}

Block :: Block(size_t numChunks, int id, Block* blockBegin) : num_chunks(numChunks), block_id(id) {
    // allocates individual blocks or "pools": this is sometime after PoolAllocator initialisation for the first time
    // individual allocation of blocks will create unwanted empty spaces
    block_size = num_chunks * CHUNK_SIZE;
    // create chunks within the block
    void *temp_void = static_cast<void*>(blockBegin);
    Chunk *chunk = static_cast<Chunk*>(temp_void);
    block_begin = chunk;
    for (int i=0; i<num_chunks; i++){
        chunk->previous = chunk;
        chunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + CHUNK_SIZE);
        chunk->chunk_id = i;
        chunk = chunk->next;
    }
    cout<<"Block allocation complete! Yayy!"<<endl;
}

Block :: ~Block() {
    // destroys the individual pools created
    int success = munmap(reinterpret_cast<void*>(block_begin), block_size);
    if (success != 0){
        perror("object destruction unsuccesful");
    }
    // destroy the pool allocator object from the main program
    // call delete <block-name-pointer>
}

// I don't know how correct this is, but I have allocated both the blocks and chunks in this for the initialisation
PoolAllocator :: PoolAllocator(size_t blocks, size_t numChunks) : num_blocks(blocks) {
    // PoolAllocator object is created only once, and is sort of like an interface to the memory to the user
    size_t block_size = numChunks*CHUNK_SIZE;
    total_size = blocks*block_size;
    size_t adjusted_size = ceil(total_size / ((float)PAGE_SIZE)) * PAGE_SIZE;
    cout<<adjusted_size<<endl;
    // currently supports allocating blocks of equal sizes during initialisation. Below the total memory has been allocated. Divide it into blocks and chunks later
    pool_begin = reinterpret_cast<Block*>(mmap(NULL, adjusted_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)); // pointer to the first block
    if (pool_begin == MAP_FAILED) {
        perror("mmap mapping failed");
        exit(1);
    }
    cout<<pool_begin<<endl;
    // construct the first block
    // allocate the blocks and chunks in the same way in the same section
    block_begin_pointers.resize(num_blocks);
    for (int i=0; i<num_blocks; i++){
        Block *memory = reinterpret_cast<Block*>(reinterpret_cast<char*>(pool_begin) + i*block_size);
        cout<<memory;
        Block *temp = new (memory) Block(numChunks, i, memory);
        block_begin_pointers[i] = temp;
    }
    block_begin_pointers[blocks-1]->next = nullptr; // next pointer of the last block
    cout<<"Pool initialisation and allocation complete!"<<endl;
}

PoolAllocator :: ~PoolAllocator(){
    // for (int i=0; i<num_blocks; i++){
    //     block_begin_pointers[i]->~Block();
    // }
    int success = munmap(reinterpret_cast<void*>(pool_begin), total_size);
    if (success != 0){
        perror("pool allocator destruction unsuccesful");
    }
    // destroy the pool allocator object from the main program
    // call delete <pool-allocator-name-pointer>
}

Chunk* Block :: pool_get(){
    Chunk* current = alloc_ptr;
    // give current to the application
    current->next->previous = current->previous->next;
    return current;
}

void Block :: pool_free(Chunk *ret){
    Chunk *last = alloc_ptr;
    last->next = ret;
    ret->next = nullptr;
    ret->previous = last;
}