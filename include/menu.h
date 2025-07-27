#ifndef MENU_H
#define MENU_H

#include "tree_structure.h"
#include <string>
using namespace std;

// menú (1–5)
int  getMenuOption();

int  getInt(const string& prompt);

void runMenu(MageTree& tree);

#endif
