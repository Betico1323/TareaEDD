#include "../include/tree_structure.h"
#include "../include/csv_loader.h"
#include "../include/menu.h"
using namespace std;

int main(){
    MageTree tree;
    loadCsv("data.csv", tree);
    runMenu(tree);
    return 0;
}
