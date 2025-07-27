#include "../include/csv_loader.h"
#include <fstream>
#include <cstdio>
using namespace std;

static void parseHechizos(const string& cell, Mage* m) {
    int st = 0;
    while(st < (int)cell.size()) {
        int p = cell.find(';', st);
        if (p == (int)string::npos) p = cell.size();
        string sp = cell.substr(st, p - st);
        if (!sp.empty()) {
            SpellNode* node = new SpellNode(sp);
            node->next = m->spells;
            m->spells = node;
        }
        st = p + 1;
    }
}

void loadCsv(const string& filename, MageTree& tree) {
    ifstream f(filename.c_str());
    if (!f.is_open()) {
        cerr << "No se puede abrir " << filename << "\n";
        return;
    }
    string line;
    if (!getline(f, line) || 
        line != "id,name,last_name,gender,age,id_father,is_dead,type_magic,is_owner,spells") {
        cerr << "Cabecera invalida en " << filename << "\n";
        return;
    }
    while(getline(f, line)) {
        int col = 0, start = 0;
        int id=0,fid=0,age=0;
        bool dead=false,owner=false;
        char gender='H';
        string name,last,type,cell,spells_csv;
        while(col < 10) {
            int comma = line.find(',', start);
            if (comma == (int)string::npos) comma = line.size();
            cell = line.substr(start, comma - start);
            switch(col) {
              case 0: id = stoi(cell); break;
              case 1: name = cell; break;
              case 2: last = cell; break;
              case 3: gender = cell[0]; break;
              case 4: age = stoi(cell); break;
              case 5: fid = stoi(cell); break;
              case 6: dead = (cell=="1"); break;
              case 7: type = cell; break;
              case 8: owner = (cell=="1"); break;
              case 9: spells_csv = cell; break;
            }
            start = comma + 1;
            ++col;
        }
        Mage* m = new Mage(id,fid,name,last,gender,age,dead,type,owner);
        parseHechizos(spells_csv, m);
        tree.addMage(m);
    }
    tree.buildTree();
}

void saveCsv(const string& filename, const MageTree& tree) {

    ofstream f(filename.c_str());
    if (!f.is_open()) {
        cerr << "No se puede abrir " << filename << " para guardar\n";
        return;
    }
    f << "id,name,last_name,gender,age,id_father,is_dead,type_magic,is_owner,spells\n";
    Mage** all = tree.getAllMages();
    int   n   = tree.getMageCount();
    for(int i = 0; i < n; ++i) {
        Mage* m = all[i];
        f << m->id << "," << m->name << "," << m->last_name
          << "," << m->gender << "," << m->age << "," << m->father_id
          << "," << (m->is_dead?"1":"0")
          << "," << m->type_magic
          << "," << (m->is_owner?"1":"0") << ",";
        SpellNode* s = m->spells;
        bool first = true;
        while(s) {
            if (!first) f << ";";
            f << s->spell;
            first = false;
            s = s->next;
        }
        f << "\n";
    }
}
