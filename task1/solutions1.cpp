#include "solutions1.hpp"

void brute_force(int n, std::vector<std::vector<std::string>>& table){
    for (int i = 0; i<=n; i++){
        for (int j = 0; j<=n; j++){
            if (i == 0){
                if (j == 0)
                    continue;
                else{
                    table[i][j] = "c" + std::to_string(j);
                }
            }
            else{
                if (j == 0){
                    table[i][j] = "c" + std::to_string(i);
                }
                else{
                    if (i == j)
                        continue;
                    std::string start = table[i][0];
                    std::string end = table[0][j];
                    std::string distance = calc_distance(start, end);
                    table[i][j] = distance;
                }
            }
        }
    }

    print_table(table, n);
}

void better_soln(int n, std::vector<std::vector<std::string>>& table){
    // allocate only the upper triangular matrix here, since the matrices are symmetric
    for (int i = 0; i<=n; i++){
        for (int j = i; j<=n; j++){
            if (i == 0){
                if (j == 0)
                    continue;
                else{
                    table[i][j] = "c" + std::to_string(j);
                }
            }
            else{
                if (i == j)
                    continue;
                std::string start = table[i][0];
                std::string end = table[0][j];
                std::string distance = calc_distance(start, end);
                table[i][j] = distance;
            }
        }
    }

    // here the lower triangular matrix is allocated
    for (int i=n; i>=0; i--){
        for (int j=0; j<=i; j++){
            table[i][j] = table[j][i];
        }
    }

    print_table(table, n);
}

void flattened_matrix_soln(int n, std::vector<std::vector<std::string>>& table){
    print_table(table, n);
}