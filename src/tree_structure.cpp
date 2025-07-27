#include "../include/tree_structure.h"
#include <cstring> 
using namespace std;

// Constructor de Mage
Mage::Mage(int _id,int _fid,const string& _name,const string& _last_name,
           char _gender,int _age,bool _is_dead,
           const string& _type_magic,bool _is_owner)
  : id(_id)
  , father_id(_fid)
  , name(_name)
  , last_name(_last_name)
  , gender(_gender)
  , age(_age)
  , is_dead(_is_dead)
  , is_owner(_is_owner)
  , type_magic(_type_magic)
  , first_child(nullptr)
  , next_sibling(nullptr)
  , spells(nullptr)
{}

// MageTree

MageTree::MageTree()
  : root(nullptr), all_mages(nullptr), mage_count(0), mage_capacity(0)
{}

MageTree::~MageTree() {
    clear();
}

void MageTree::clear() {
    deleteSubtree(root);
    root = nullptr;
    // liberar arreglo
    delete [] all_mages;
    all_mages = nullptr;
    mage_count = mage_capacity = 0;
}

void MageTree::appendMage(Mage* m) {
    if (mage_count == mage_capacity) {
        int new_cap = (mage_capacity == 0 ? 8 : mage_capacity * 2);
        Mage** tmp = new Mage*[new_cap];
        for(int i = 0; i < mage_count; ++i)
            tmp[i] = all_mages[i];
        delete [] all_mages;
        all_mages = tmp;
        mage_capacity = new_cap;
    }
    all_mages[mage_count++] = m;
}

void MageTree::addMage(Mage* m) {
    appendMage(m);
}

Mage** MageTree::getAllMages() const { return all_mages; }
int    MageTree::getMageCount() const { return mage_count; }
Mage*  MageTree::getRoot()      const { return root; }

Mage* MageTree::findById(int id) const {
    for(int i = 0; i < mage_count; ++i)
        if (all_mages[i]->id == id)
            return all_mages[i];
    return nullptr;
}

void MageTree::buildTree() {
    // primero localiza la raíz
    for(int i = 0; i < mage_count; ++i) {
        Mage* m = all_mages[i];
        if (m->father_id == 0) {
            root = m;
            break;
        }
    }
    // luego inserta cada mago bajo su padre
    for(int i = 0; i < mage_count; ++i) {
        Mage* m = all_mages[i];
        if (m == root) continue;
        Mage* p = findById(m->father_id);
        if (!p) continue;
        if (!p->first_child)
            p->first_child = m;
        else {
            Mage* s = p->first_child;
            while(s->next_sibling) s = s->next_sibling;
            s->next_sibling = m;
        }
    }
}

void MageTree::deleteSubtree(Mage* node) {
    if (!node) return;
    deleteSubtree(node->first_child);
    deleteSubtree(node->next_sibling);
    // liberar hechizos
    SpellNode* sp = node->spells;
    while(sp) {
        SpellNode* nx = sp->next;
        delete sp;
        sp = nx;
    }
    delete node;
}

void MageTree::printSubtreeRec(Mage* node) const {
    if (!node) return;
    if (!node->is_dead) {
        cout << (node->is_owner ? "* " : "  ")
             << node->name << " " << node->last_name << "\n";
    }
    printSubtreeRec(node->first_child);
    printSubtreeRec(node->next_sibling);
}

void MageTree::printSubtree() const {
    cout << "\nLinea de sucesion (vivos):\n";
    printSubtreeRec(root);
}
