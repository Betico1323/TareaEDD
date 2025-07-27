#ifndef MAGIC_SYSTEM_H
#define MAGIC_SYSTEM_H

#include "tree_structure.h"
using namespace std;

// Devuelve el sucesor según todas las reglas
Mage* chooseSuccessor(const MageTree& tree, Mage* dead);

// Imprime la sucesión actual de vivos (marca dueño con *)
void showSuccession(const MageTree& tree);

#endif
