# Hechizo de Magos

Una pequeña **herramienta en C++** que simula la línea de sucesión de un “hechizo” familiar de magos, con:

- Estructura de árbol dinámico **hijo–hermano**.  
- Lectura/escritura de `data.csv` (incluye hechizos).  
- Reasignación automática de “dueño” según:
  - Hijos con magia **elemental/unique/mixed**  
  - Edad **> 70** (busca mismo tipo o el más viejo)  
  - Hermanos, tíos, fallback a **mujeres mixed**, luego la mujer viva más joven.  
- Menú interactivo con validaciones de entrada.

---

## Estructura del proyecto

```
hechizo_magos/
├── README.md
├── bin/
│   ├── data.csv         # Datos de prueba (ID, magia, hechizos…)
│   └── hechizo_magos    # Ejecutable tras compilar
└── src/
    ├── main.cpp         # Lógica de menú y validaciones
    ├── mage_tree.h      # Declaraciones de `Mage` y `MageTree`
    └── mage_tree.cpp    # Implementación del árbol, sucesión y CSV I/O
```

---

## Compilación

Desde la raíz del proyecto:

```bash
g++ -std=c++17 -Wall -Wextra -g3 src/main.cpp src/mage_tree.cpp -o bin/main.exe
```

---

## Uso

```bash
cd hechizo_magos/bin
./main.exe
```

Verás el menú:

```
--- Menu Hechizo ---
1) Mostrar sucesion
2) Registrar muerte
3) Editar mago
4) Mostrar hechizos
5) Salir
Opcion: 
```

- **1) Mostrar sucesión**: muestra todos los magos vivos, marcando con `*` al dueño.  
- **2) Registrar muerte**: ingresa el `ID` de un mago; el programa reasigna dueño y actualiza `data.csv`.  
- **3) Editar mago**: modifica nombre, edad, estado, magia, dueño y hechizos (validado).  
- **4) Mostrar hechizos**: lista los hechizos de un mago por `ID`.  
- **5) Salir**: cierra la aplicación.

---

## Licencia

Proyecto libre, ¡que la magia te acompañe! ✨