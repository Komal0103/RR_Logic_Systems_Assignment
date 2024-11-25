#include <iostream>
#include "utilities.hpp"
#include "solutions1.hpp"

int main(){
    int n;
    // what data structure will minimise storage and function complexity
    std::cout<<"Enter the number of cities: ";
    std::cin>>n;
    std::vector<std::vector<std::string>> table(n+1, std::vector<std::string>(n+1, "0"));
    
    brute_force(n, table);
    better_soln(n, table);

    return 0;
}