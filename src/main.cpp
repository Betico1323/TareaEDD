#include "mage_tree.h"
using namespace std;

// Leer la opcion del menu 1-5
int get_menu_option() {
    int opt;
    while (true) {
        cout << "Opcion: ";
        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(256, '\n');
            cout << "Entrada invalida, ingresa un numero.\n";
            continue;
        }
        cin.ignore(256, '\n');
        if (opt < 1 || opt > 5) {
            cout << "Opcion invalida, elige entre 1 y 5.\n";
            continue;
        }
        return opt;
    }
}

int get_int(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (!(cin >> x)) {
            cin.clear();
            cin.ignore(256, '\n');
            cout << "Entrada invalida, ingresa un numero.\n";
            continue;
        }
        cin.ignore(256, '\n');
        return x;
    }
}

int main() {
    MageTree tree;
    tree.load_from_csv("data.csv");

    int opt;
    do {
        cout << "\n--- Menu Hechizo ---\n"
             << "1) Mostrar sucesion\n"
             << "2) Registrar muerte\n"
             << "3) Editar mago\n"
             << "4) Mostrar hechizos\n"
             << "5) Salir\n";
        opt = get_menu_option();

        switch (opt) {
            case 1:
                tree.show_succession();
                break;
            case 2: {
                int id = get_int("ID fallecido: ");
                tree.handle_death(id);
                break;
            }
            case 3: {
                int id = get_int("ID a editar: ");
                tree.edit_mage(id);
                break;
            }
            case 4: {
                int id = get_int("ID mago: ");
                tree.show_spells(id);
                break;
            }
            case 5:
                cout << "Adios!\n";
                break;
        }
    } while (opt != 5);

    return 0;
}
