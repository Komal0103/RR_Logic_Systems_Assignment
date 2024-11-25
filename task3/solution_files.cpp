#include "solution_files.hpp"

void create_hash_table(map<char, set<int>>& hash_bucket, string master){
    int size = master.length();

    for (int i=0; i<size; i++){
        char c = master[i];
        if (hash_bucket.find(c) == hash_bucket.end()){  // add new key
            set<int> indices;
            indices.insert(i);
            hash_bucket[c] = indices;
        }
        else{
            set<int> temp = hash_bucket[c];
            temp.insert(i);
            hash_bucket[c] = temp;
        }
    }
    print_hash_table(hash_bucket);
}

bool find_pattern(map<char, set<int>>& hash_bucket, string input){
    // search for a pattern in a given search string
    int len = input.length();
    int current = -1;
    for (int i=0; i<len; i++){
        char c = input[i];
        if (hash_bucket.find(c) == hash_bucket.end())
            return false;
        else{
            set<int> indices = hash_bucket[c];
            auto it = lower_bound(indices.begin(), indices.end(), current+1);
            if (it == indices.end())
                return false;
            current = *it;
        }
    }
    return true;
}