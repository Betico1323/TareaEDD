#ifndef MAGE_TREE_H
#define MAGE_TREE_H

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

struct SpellNode
{
    string spell;
    SpellNode *next;
    SpellNode(const string &s) : spell(s), next(nullptr) {}
};

struct Mage
{
    int id;
    int father_id;
    string name, last_name;
    char gender;
    int age;
    bool is_dead;
    bool is_owner;
    string type_magic;
    Mage *first_child;
    Mage *next_sibling;
    SpellNode *spells;

    Mage(int _id, int _fid,
         const string &_name, const string &_last_name,
         char _gender, int _age, bool _is_dead,
         const string &_type_magic, bool _is_owner);
};

class MageTree
{
public:
    MageTree();
    ~MageTree();

    void load_from_csv(const string &filename);
    void save_to_csv(const string &filename) const;
    void show_succession() const;
    void handle_death(int id_dead);
    void edit_mage(int id);
    void show_spells(int id) const;

private:
    Mage *root;
    Mage **all_mages;
    int mage_count, mage_capacity;

    void append_mage(Mage *m);
    Mage *find_by_id(int id) const;
    void insert_node(Mage *m);
    void delete_subtree(Mage *node);
    void print_succession(Mage *node) const;

    Mage *choose_new_owner(Mage *dead);
    Mage *find_in_subtree(Mage *node, const string types[], int tc) const;
    Mage *find_first_male(Mage *node) const;
    void find_oldest(Mage *node, Mage *&best) const;
};

#endif 