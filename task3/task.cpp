#include "solution_files.hpp"

using namespace std;

int main(int argc, char *argv[]){
    string master = argv[1];
    map<char, set<int>> hash_bucket;
    
    create_hash_table(hash_bucket, master);

    string input = argv[2];
    bool result = find_pattern(hash_bucket, input);
    cout<<result<<endl;

    return 0;
}