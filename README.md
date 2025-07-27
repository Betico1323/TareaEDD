## 📁 Estructura del proyecto

```
hechizo_magos/
├── README.md
├── bin/
│   └── data.csv
├── include/
│   ├── csv_loader.h
│   ├── magic_system.h
│   ├── menu.h
│   └── tree_structure.h
└── src/
    ├── csv_loader.cpp
    ├── magic_system.cpp
    ├── menu.cpp
    ├── tree_structure.cpp
    └── main.cpp
```

---

## 📚 Módulos

### 1. tree_structure  
- **Archivos**: `include/tree_structure.h` + `src/tree_structure.cpp`  
- **Responsabilidad**: Define `Mage`, `SpellNode` y `MageTree`.  
  - Almacena todos los nodos en un arreglo dinámico.  
  - Construye la jerarquía hijo‑hermano (primer hijo / siguiente hermano).  
  - Recorridos pre‑order para imprimir sucesión.

### 2. csv_loader  
- **Archivos**: `include/csv_loader.h` + `src/csv_loader.cpp`  
- **Responsabilidad**: I/O de `data.csv` con backup (`data.bak`).  
  - Parser robusto: valida cabecera y número de columnas.  
  - Serializa y deserializa la lista de hechizos (`spell1;spell2;…`).

### 3. magic_system  
- **Archivos**: `include/magic_system.h` + `src/magic_system.cpp`  
- **Responsabilidad**: Lógica completa de herencia mágica.  
  - `chooseSuccessor(tree, dead)`: aplica las reglas en orden (edad, magia, hermanos, tíos, fallback mujeres).  
  - `showSuccession(tree)`: imprime solo vivos, marcando dueño.

### 4. menu  
- **Archivos**: `include/menu.h` + `src/menu.cpp`  
- **Responsabilidad**: Interfaz de usuario y validaciones.  
  - Lectura segura de opciones y de IDs.  
  - Validaciones de género, magia, edad y hechizos (sin duplicados).

### 5. main  
- **Archivo**: `src/main.cpp`  
- **Responsabilidad**: Punto de entrada:  
  1. Carga CSV.  
  2. Inicia bucle de menú.  

---

## ⚙️ Compilación

Desde la raíz del proyecto:

```bash
g++ -std=c++17 src/*.cpp -Iinclude -o bin/hechizo_magos
```

---

## 🚀 Uso

```bash
./bin/hechizo_magos
```

Sigue el menú con opciones para mostrar sucesión, registrar muertes, editar magos y listar hechizos.

---

## ✅ Validaciones

- **Género**: solo `H` o `M`.  
- **Magia**: `elemental`, `unique`, `mixed`, `no_magic` (se normaliza e insiste hasta correcto).  
- **Edad**: rango `0–200`.  
- **Hechizos**: no vacíos, máximo razonable (100 chars) y sin duplicados.  
- **IDs**: no modificables en edición; validación de existencia y unicidad.

---

## 🧙‍♂️ Reglas de sucesión

1. **Edad > 70** + tiene hijos:  
   - Busca en todo su subárbol un hijo vivo con **misma magia**.  
   - Si no hay, elige el hijo vivo **más viejo**.  

2. **Tiene hijos** (edad ≤ 70):  
   - Primero un hijo vivo con magia **elemental** o **unique**.  
   - Si no, un hijo vivo con magia **mixed**.  
   - Si no, un hijo vivo de género **‘H’**.  

3. **Sin hijos**:  
   - Busca entre **hermanos** (mismo padre) un vivo con **misma magia**.  
   - Si no, recorre el subárbol de cada hermano (magia / género).  
   - Si no, asigna a un **tío** vivo.  
   - Si el padre está muerto, aplica recursivamente estas reglas al padre.  

4. **Fallback femenil**:  
   - Mujer viva con hijos y magia **mixed** más joven.  
   - Si no, mujer viva más joven.

---