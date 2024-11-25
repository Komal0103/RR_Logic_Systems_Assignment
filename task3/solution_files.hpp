#include <iostream>
#include <string>
#include <map>
#include <set>
#include <algorithm>

#include "../task1/utilities.hpp"

using namespace std;

void create_hash_table(map<char, set<int>>& map, string master);
bool find_pattern(map<char, set<int>>& hash_bucket, string input);