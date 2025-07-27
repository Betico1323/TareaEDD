#ifndef CSV_LOADER_H
#define CSV_LOADER_H

#include <string>
#include "tree_structure.h"
using namespace std;

// Carga todos los magos desde CSV en el tree (sin armar la jerarquía)
void loadCsv(const string& filename, MageTree& tree);


void saveCsv(const string& filename, const MageTree& tree);

#endif 
