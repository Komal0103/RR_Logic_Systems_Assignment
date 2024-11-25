#include "utilities.hpp"

std::string calc_distance(std::string start, std::string end){
    char s = start[1];
    std::string start_i(1, s);
    int start_ind = std::stoi(start_i);
    char e = end[1];
    std::string end_i(1, e);
    int end_ind = std::stoi(end_i);
    int difference = std::abs(end_ind - start_ind);
    std::string distance;
    for (int i=0; i<difference; i++){
        distance += "d" + std::to_string(i+1) + "+";
    }
    // remove the last + character
    distance.erase(distance.length() - 1);
    return distance;
}

void print_table(std::vector<std::vector<std::string>>& table, int n){
    for (int i=0; i<=n; i++){
        for (int j=0; j<=n; j++){
            std::cout<<table[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}

void print_hash_table(std::map<char, std::set<int>>& hash_table){
    for (auto it:hash_table){
        std::cout<<it.first<<": ";
        for (auto ele:it.second){
            std::cout<<ele<<" ";
        }
        std::cout<<std::endl;
    }
}