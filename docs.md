# Coding Test 1: Komal Sambhus

Submission date: 25/11/2024

## Task 1
### Aim
To calculate a table that displays the cost of travel between two cities.

### Approach
* Using a two dimensional vector for the table
* Two solutions: Brute force and Better
* Brute Force: calculates the entire matrix as is
* Better Approach: Since the final matrix is symmetric, we can calculate only half of the matrix, and the other half is assigned from the triangle that is computed first.

### Calculation
* Time Complexity: O(n^2)
* Space Complexity: O(n^2)
* Concatenate the distance string from the lower city number to greater city - 1

### Improvement
* Use a flattened array structure instead of a 2D vector - this allows us to fetch the data in a single cache line.

### Compile and Run
`cd task1`\
`g++ -o task1 task.cpp solutions1.cpp utilities.cpp`\
`./task1`

## Task 2
### Aim
Implement a binary search tree and give functions for adding, deleting, searching a node, and printing the tree.

### Approach
* The BST is built using linked lists.
* Iterative adding, and search algorithms.
* Recursive delete function.
* Printing using Inorder traversal

### Complexity
The functions are of the order of the height of the tree

### Note
While giving the "print" and "exit" command, provide an integer after the main command. For example, \
`print 0` \
`exit 0`

### Compile and Run
`cd task2`\
`g++ -o task2 task.cpp solution.cpp`\
`./task2`

## Task 3
### Aim
Pattern Search within the main master string.

### Approach
* User entered strings for both the master and pattern to search
* Create a hash bucket first containing each letter in an alphabetical order. The key is mapped to the indices at which the letter appears
* Pattern search using this hash bucket
* To find consecutive indices,\
    `auto it = lower_bound(indices.begin(), indices.end(), current+1);
    `
* Gives boolean output

### Compile and Run
`cd task3`\
`g++ -o task3 task.cpp solution_files.cpp ../task1/utilities.cpp`\
`./task3 "Hello World!" "rld"`

## Task 4
### Aim
Implementing a pool allocator that gives the following functionality:
* add and destroy pools as per requirement
* allocate individual chunks of pools to applications
* return chunks to the pool once the application is complete

### Approach
* Used C++ instead of C to give object oriented functionality, and allow extension to templates (so that the data type of the stored data can be decided at runtime)
* Some of the names have been changed from the original question. Code is commented wherever the names are modified.
* The entire area mapped by `mmap` function is divided into blocks and chunks. The 'pools' of the question can be considered as 'blocks' here.
* The `PoolAllocator` object is like the management entity that provides the API to create the memory pool.
* Current functionality supports mapping memory once (during initialisation).
* The functions `pool_get` and `pool_free` are implemented, but haven't been tested.
* Pre-defined maximum numbers of chunks and blocks in the pool.

### Improvements/Future scope
* As mentioned above, the pools are allocated once during the lifetime of the program, i.e., during initialisation. Provide functionality for allocating `blocks` at other places in the main function. Requires minor changes in the code.
* Test `pool_get` and `pool_free` function.
* Use template metaprogramming to determine the datatype of the stored data during runtime. Currently `__uint64_t data` is being stored.
* `current_block` points to the last block instead of the first one - problem here.
* The blocks are getting allocated and deallocated correctly, but there are some problems with the block and chunk placements - debug that.

### Compile and Run
`cd task4`\
`g++ -std=c++17 -o task4 task.cpp solution4.cpp` \
`./task4 2 2 4 4`

## Overall Improvements to the Repository
* Convert compilation to CMake based.