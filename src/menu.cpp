#include "../include/menu.h"
#include "../include/magic_system.h"
#include "../include/csv_loader.h"
#include <iostream>
using namespace std;

int getMenuOption() {
    int opt;
    while (true) {
        cout << "Opcion: ";
        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(256,'\n');
            cout << "Entrada invalida, ingresa un numero.\n";
            continue;
        }
        cin.ignore(256,'\n');
        if (opt < 1 || opt > 5) {
            cout << "Opcion invalida, elige entre 1 y 5.\n";
            continue;
        }
        return opt;
    }
}

int getInt(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (!(cin >> x)) {
            cin.clear();
            cin.ignore(256,'\n');
            cout << "Entrada invalida, ingresa un numero.\n";
            continue;
        }
        cin.ignore(256,'\n');
        return x;
    }
}

void runMenu(MageTree& tree) {
    const string csv_path = "data.csv";
    int opt;
    do {
        cout << "\n--- Menu Hechizo ---\n"
             << "1) Mostrar sucesion\n"
             << "2) Registrar muerte\n"
             << "3) Editar mago\n"
             << "4) Mostrar hechizos\n"
             << "5) Salir\n";
        opt = getMenuOption();
        switch(opt) {
            case 1:
                showSuccession(tree);
                break;
            case 2: {
                int id = getInt("ID fallecido: ");
                Mage* dead = tree.findById(id);
                if (dead && !dead->is_dead && dead->is_owner) {
                    dead->is_dead = true;
                    // limpiar owner
                    Mage** all = tree.getAllMages();
                    for(int i=0;i<tree.getMageCount();++i)
                        all[i]->is_owner = false;
                    Mage* nxt = chooseSuccessor(tree, dead);
                    if (nxt) {
                        nxt->is_owner = true;
                        cout<<"Nuevo duenio: "<<nxt->name<<" "<<nxt->last_name<<"\n";
                    }
                    saveCsv(csv_path, tree);
                } else {
                    cout<<"ID invalido o no es dueño vivo\n";
                }
                break;
            }
            case 3: {
                int id = getInt("ID a editar: ");
                Mage* m = tree.findById(id);
                if (!m) { cout<<"Mago no encontrado\n"; break; }
                cout<<"Editando "<<m->name<<"\n";
                cout<<"Nombre? "; getline(cin, m->name);
                cout<<"Apellido? "; getline(cin, m->last_name);
                m->age = getInt("Edad? ");
                // genero
                char g;
                do {
                  cout<<"Genero (H/M)? "; cin>>g; cin.ignore(256,'\n');
                } while(g!='H'&&g!='M');
                m->gender = g;
                // muerto
                int d = getInt("Muerto? (1/0): ");
                m->is_dead = (d==1);
                // magia
                string tm;
                do {
                  cout<<"Tipo magia (elemental, unique, mixed, no_magic): ";
                  getline(cin, tm);
                } while(tm!="elemental" && tm!="unique" && tm!="mixed" && tm!="no_magic");
                m->type_magic = tm;
                // owner
                int o = getInt("Duenio? (1/0): ");
                if (o==1) {
                    Mage** all = tree.getAllMages();
                    for(int i=0;i<tree.getMageCount();++i)
                        all[i]->is_owner = false;
                    m->is_owner = true;
                } else m->is_owner = false;
                // hechizos
                char opt2;
                do {
                  cout<<"Agregar hechizo? (s/n): "; cin>>opt2; cin.ignore(256,'\n');
                } while(opt2!='s'&&opt2!='n');
                while(opt2=='s') {
                  cout<<"Hechizo: "; 
                  string sp; getline(cin,sp);
                  if (sp.empty()) {
                    cout<<"Hechizo invalido\n";
                  } else {
                    // evitar duplicados
                    SpellNode* cur = m->spells;
                    bool dup = false;
                    while(cur){ if(cur->spell==sp){ dup=true; break;} cur=cur->next; }
                    if(!dup){
                      SpellNode* node=new SpellNode(sp);
                      node->next = m->spells;
                      m->spells = node;
                    }
                  }
                  do {
                    cout<<"Otro hechizo? (s/n): "; cin>>opt2; cin.ignore(256,'\n');
                  } while(opt2!='s'&&opt2!='n');
                }
                saveCsv(csv_path, tree);
                break;
            }
            case 4: {
                int id = getInt("ID mago: ");
                Mage* m = tree.findById(id);
                if (!m) { cout<<"No encontrado\n"; break; }
                cout<<"Hechizos de "<<m->name<<":\n";
                SpellNode* s = m->spells;
                while(s){ cout<<" - "<<s->spell<<"\n"; s=s->next; }
                break;
            }
            case 5:
                cout<<"Adios!\n";
                break;
        }
    } while(opt!=5);
}
