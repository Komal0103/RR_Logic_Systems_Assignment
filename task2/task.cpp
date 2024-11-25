#include "solution.hpp"

int main() {
    BST bs;
    bs.iInsert(5);
    bs.iInsert(2);
    bs.iInsert(8);
    bs.iInsert(7);
    bs.iInsert(9);
    bs.iInsert(1);
 
    // Delete
    bs.Delete(bs.getRoot(), 7);
    bs.Inorder(bs.getRoot());
    cout<<endl;

    // create a map of command arguments
    map<string, int> command_mapping;
    command_mapping["add"] = 0;
    command_mapping["del"] = 1;
    command_mapping["search"] = 2;
    command_mapping["print"] = 3;
    command_mapping["exit"] = 4;
    bool flag = true;

    while (flag){
        string command;
        int arg;
        cout<<"Your wish is my command! ";
        cin>>command>>arg;
        Node *temp;
        switch (command_mapping[command])
        {
        case 0:
            bs.iInsert(arg);
            bs.Inorder(bs.getRoot());
            cout<<endl;
            continue;
        case 1:
            bs.Delete(bs.getRoot(), arg);
            bs.Inorder(bs.getRoot());
            cout<<endl;
            continue;
        case 2:
            temp = bs.iSearch(arg);
            if (temp) cout<<"Element found!"<<endl;
            else cout<<"Better luck next time :)"<<endl;
            continue;
        case 3:
            bs.Inorder(bs.getRoot());
            cout<<endl;
            continue;
        case 4:
            flag = false;        
        default:
            continue;
        }
    }
    
    return 0;
}