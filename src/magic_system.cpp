#include "../include/magic_system.h"
using namespace std;

// Auxiliares privados:

static Mage* findInSubtree(Mage* node, const string types[], int tc) {
    if (!node) return nullptr;
    if (!node->is_dead) {
        for(int i=0;i<tc;i++)
            if (node->type_magic == types[i])
                return node;
    }
    Mage* r = findInSubtree(node->first_child, types, tc);
    if (r) return r;
    return findInSubtree(node->next_sibling, types, tc);
}

static Mage* findOldestInSubtree(Mage* node) {
    Mage* best = nullptr;
    if (!node) return nullptr;
    if (!node->is_dead) best = node;
    Mage* c = node->first_child;
    while(c) {
        Mage* sub = findOldestInSubtree(c);
        if (sub && (!best || sub->age > best->age))
            best = sub;
        c = c->next_sibling;
    }
    return best;
}

static Mage* findChildByMagic(Mage* dead, const string types[], int tc) {
    Mage* c = dead->first_child;
    while(c) {
        if (!c->is_dead) {
            for(int i=0;i<tc;i++)
                if (c->type_magic == types[i])
                    return c;
        }
        c = c->next_sibling;
    }
    return nullptr;
}

static Mage* findChildByGender(Mage* dead, char gender) {
    Mage* c = dead->first_child;
    while(c) {
        if (!c->is_dead && c->gender == gender)
            return c;
        c = c->next_sibling;
    }
    return nullptr;
}

static Mage* findSiblingByMagic(Mage* dead) {
    Mage* p = nullptr;
    // buscamo padre: recorre all_mages en tree
    // pero aquí se asume que dead->father_id es correcto
    // se deja a chooseSuccessor
    return nullptr; // lo manejamos arriba recursivamente
}

static Mage* findUncle(const MageTree& tree, Mage* dead) {
    Mage* p = tree.findById(dead->father_id);
    if (!p || p->father_id == 0) return nullptr;
    Mage* gp = tree.findById(p->father_id);
    if (!gp) return nullptr;
    Mage* u = gp->first_child;
    while(u) {
        if (u != p && !u->is_dead) return u;
        u = u->next_sibling;
    }
    return nullptr;
}

static Mage* findYoungestWomanWithChildrenMixed(const MageTree& tree) {
    Mage* cand = nullptr;
    Mage** all = tree.getAllMages();
    int   n   = tree.getMageCount();
    for(int i=0;i<n;i++) {
        Mage* m = all[i];
        if (!m->is_dead && m->gender=='M' &&
            m->first_child && m->type_magic=="mixed") {
            if (!cand || m->age < cand->age) cand = m;
        }
    }
    return cand;
}

static Mage* findYoungestWoman(const MageTree& tree) {
    Mage* cand = nullptr;
    Mage** all = tree.getAllMages();
    int   n   = tree.getMageCount();
    for(int i=0;i<n;i++){
        Mage* m = all[i];
        if (!m->is_dead && m->gender=='M'){
            if (!cand || m->age < cand->age) cand = m;
        }
    }
    return cand;
}

// Implementación pública:

Mage* chooseSuccessor(const MageTree& tree, Mage* dead) {
    if (!dead) return nullptr;

    // 1) edad > 70
    if (dead->age > 70 && dead->first_child) {
        string t = dead->type_magic;
        Mage* m = findInSubtree(dead->first_child, &t, 1);
        if (m) return m;
        return findOldestInSubtree(dead->first_child);
    }
    // 2) hijos
    if (dead->first_child) {
        string p1[2] = {"elemental","unique"};
        Mage* m = findChildByMagic(dead, p1, 2);
        if (m) return m;
        string p2[1] = {"mixed"};
        m = findChildByMagic(dead, p2, 1);
        if (m) return m;
        m = findChildByGender(dead, 'H');
        if (m) return m;
    }
    // 3) sin hijos -> tíos vía recursión
    Mage* uncle = findUncle(tree, dead);
    if (uncle) return uncle;
    //  padre muerto?
    Mage* parent = tree.findById(dead->father_id);
    if (parent && parent->is_dead) {
        return chooseSuccessor(tree, parent);
    }
    // 4) fallback mujeres
    Mage* w = findYoungestWomanWithChildrenMixed(tree);
    if (w) return w;
    return findYoungestWoman(tree);
    
    return nullptr; // no hay sucesor
}

void showSuccession(const MageTree& tree) {
    tree.printSubtree();
}
