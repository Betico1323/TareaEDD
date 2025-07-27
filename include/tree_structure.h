#ifndef TREE_STRUCTURE_H
#define TREE_STRUCTURE_H

#include <string>
#include <iostream>
using namespace std;

// Nodo simple para hechizos
struct SpellNode {
    string spell;
    SpellNode* next;
    SpellNode(const string& s): spell(s), next(nullptr) {}
};


struct Mage {
    int id;
    int father_id;
    string name, last_name;
    char gender;      
    int age;
    bool is_dead;
    bool is_owner;
    string type_magic; // elemental, unique, mixed, no_magic

    Mage* first_child;
    Mage* next_sibling;
    SpellNode* spells;

    Mage(int _id,int _fid,
         const string& _name,const string& _last_name,
         char _gender,int _age,bool _is_dead,
         const string& _type_magic,bool _is_owner);
};

class MageTree {
public:
    MageTree();
    ~MageTree();

    
    void addMage(Mage* m);

    
    void buildTree();

    
    void clear();

    
    Mage** getAllMages() const;
    int     getMageCount() const;
    Mage*   getRoot() const;

    
    Mage*   findById(int id) const;

    
    void    printSubtree() const;

private:
    Mage* root;
    Mage** all_mages;
    int    mage_count, mage_capacity;

    void appendMage(Mage* m);
    void deleteSubtree(Mage* node);
    void printSubtreeRec(Mage* node) const;
};

#endif
