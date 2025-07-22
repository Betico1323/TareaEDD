#include "mage_tree.h"
#include <sstream>
#include <limits>

using namespace std;

Mage::Mage(int _id,int _fid,const string& _n,const string& _ln,
           char _g,int _a,bool _d,const string& _tm,bool _o)
  : id(_id)
  , father_id(_fid)
  , name(_n)
  , last_name(_ln)
  , gender(_g)
  , age(_a)
  , is_dead(_d)
  , is_owner(_o)
  , type_magic(_tm)
  , first_child(nullptr)
  , next_sibling(nullptr)
  , spells(nullptr)
{}

MageTree::MageTree()
  : root(nullptr), all_mages(nullptr), mage_count(0), mage_capacity(0)
{}

MageTree::~MageTree() {
    delete_subtree(root);
    delete [] all_mages;
}

void MageTree::append_mage(Mage* m) {
    if (mage_count == mage_capacity) {
        int new_cap = mage_capacity==0?8:mage_capacity*2;
        Mage** tmp = new Mage*[new_cap];
        for(int i=0;i<mage_count;i++) tmp[i]=all_mages[i];
        delete [] all_mages;
        all_mages=tmp;
        mage_capacity=new_cap;
    }
    all_mages[mage_count++]=m;
}

Mage* MageTree::find_by_id(int id) const {
    for(int i=0;i<mage_count;i++)
        if(all_mages[i]->id==id) return all_mages[i];
    return nullptr;
}

void MageTree::insert_node(Mage* m) {
    if(m->father_id==0){ root=m; return; }
    Mage* p=find_by_id(m->father_id);
    if(!p) return;
    if(!p->first_child) p->first_child=m;
    else {
        Mage* s=p->first_child;
        while(s->next_sibling) s=s->next_sibling;
        s->next_sibling=m;
    }
}

void MageTree::delete_subtree(Mage* node) {
    if(!node) return;
    delete_subtree(node->first_child);
    delete_subtree(node->next_sibling);
    SpellNode* sp=node->spells;
    while(sp){ SpellNode* nx=sp->next; delete sp; sp=nx; }
    delete node;
}

void MageTree::load_from_csv(const string& filename) {
    ifstream f(filename);
    if(!f.is_open()){ cerr<<"No se puede abrir "<<filename<<"\n"; return; }
    string line;
    getline(f,line);
    while(getline(f,line)){
        int col=0,start=0;
        int id=0,fid=0,age=0;
        bool dead=false,owner=false;
        char gender='H';
        string name,last,type,cell,spells_csv;
        while(col<10){
            int comma=line.find(',',start);
            if(comma==-1) comma=line.size();
            cell=line.substr(start,comma-start);
            switch(col){
                case 0: id=stoi(cell); break;
                case 1: name=cell; break;
                case 2: last=cell; break;
                case 3: gender=cell[0]; break;
                case 4: age=stoi(cell); break;
                case 5: fid=stoi(cell); break;
                case 6: dead=(cell=="1"); break;
                case 7: type=cell; break;
                case 8: owner=(cell=="1"); break;
                case 9: spells_csv=cell; break;
            }
            start=comma+1; col++;
        }
        Mage* m=new Mage(id,fid,name,last,gender,age,dead,type,owner);
        if(!spells_csv.empty()){
            int st=0;
            while(st < (int)spells_csv.size()){
                int p = spells_csv.find(';',st);
                if(p==-1) p=spells_csv.size();
                string sp = spells_csv.substr(st,p-st);
                if(!sp.empty()){
                    SpellNode* node = new SpellNode(sp);
                    node->next = m->spells;
                    m->spells = node;
                }
                st = p+1;
            }
        }
        append_mage(m);
    }
    for(int i=0;i<mage_count;i++) insert_node(all_mages[i]);
}

void MageTree::save_to_csv(const string& filename) const {
    ofstream f(filename);
    if(!f.is_open()){ cerr<<"No se puede abrir "<<filename<<"\n"; return; }
    f<<"id,name,last_name,gender,age,id_father,is_dead,type_magic,is_owner,spells\n";
    for(int i=0;i<mage_count;i++){
        Mage* m=all_mages[i];
        f<<m->id<<","<<m->name<<","<<m->last_name
         <<","<<m->gender<<","<<m->age<<","<<m->father_id
         <<","<<(m->is_dead?"1":"0")
         <<","<<m->type_magic
         <<","<<(m->is_owner?"1":"0")<<",";
        SpellNode* s = m->spells;
        bool first = true;
        while(s){
            if(!first) f<<";";
            f<<s->spell;
            first = false;
            s = s->next;
        }
        f<<"\n";
    }
}

void MageTree::print_succession(Mage* node) const {
    if(!node) return;
    if(!node->is_dead){
        cout<<(node->is_owner?"* ":"  ")
            <<node->name<<" "<<node->last_name<<"\n";
    }
    print_succession(node->first_child);
    print_succession(node->next_sibling);
}

void MageTree::show_succession() const {
    cout<<"\nLinea de sucesion (vivos):\n";
    print_succession(root);
}

Mage* MageTree::find_in_subtree(Mage* node, const string types[], int tc) const {
    if(!node) return nullptr;
    if(!node->is_dead){
        for(int i=0;i<tc;i++) if(node->type_magic==types[i]) return node;
    }
    Mage* r=find_in_subtree(node->first_child,types,tc);
    if(r) return r;
    return find_in_subtree(node->next_sibling,types,tc);
}

Mage* MageTree::find_first_male(Mage* node) const {
    if(!node) return nullptr;
    if(!node->is_dead && node->gender=='H') return node;
    Mage* r=find_first_male(node->first_child);
    if(r) return r;
    return find_first_male(node->next_sibling);
}

void MageTree::find_oldest(Mage* node, Mage*& best) const {
    if(!node) return;
    if(!node->is_dead){
        if(!best || node->age>best->age) best=node;
    }
    find_oldest(node->first_child,best);
    find_oldest(node->next_sibling,best);
}