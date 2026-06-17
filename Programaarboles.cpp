#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 1. ESTRUCTURA DEL NODO (Tal cual tu especificación)
struct Nodo {
    int id;
    string nombre;
    string rol;
    Nodo* izquierdo;
    Nodo* derecho;

    Nodo(int _id, string _nombre, string _rol) {
        id = _id;
        nombre = _nombre;
        rol = _rol;
        izquierdo = NULL;
        derecho = NULL;
    }
};

// ALGORITMO DE INSERCIÓN (Recursivo, evita duplicados)
Nodo* insertar(Nodo* raiz, int id, string nombre, string rol) {
    if (raiz == NULL) {
        return new Nodo(id, nombre, rol);
    }
    if (id < raiz->id) {
        raiz->izquierdo = insertar(raiz->izquierdo, id, nombre, rol);
    } else if (id > raiz->id) {
        raiz->derecho = insertar(raiz->derecho, id, nombre, rol);
    } else {
        cout << "[Aviso] ID " << id << " ya existe. Insercion rechazada para evitar duplicidad.\n";
    }
    return raiz;
}

// ALGORITMO DE BÚSQUEDA (Devuelve el puntero al nodo si lo encuentra)
Nodo* buscar(Nodo* raiz, int id) {
    if (raiz == NULL || raiz->id == id) {
        return raiz;
    }
    if (id < raiz->id) {
        return buscar(raiz->izquierdo, id);
    }
    return buscar(raiz->derecho, id);
}

// Funciones auxiliares para la Eliminación
Nodo* encontrarMinimo(Nodo* raiz) {
    while (raiz && raiz->izquierdo != NULL) {
        raiz = raiz->izquierdo;
    }
    return raiz;
}

// ALGORITMO DE ELIMINACIÓN (Maneja los 3 escenarios planteados)
Nodo* eliminar(Nodo* raiz, int id) {
    if (raiz == NULL) return raiz;

    if (id < raiz->id) {
        raiz->izquierdo = eliminar(raiz->izquierdo, id);
    } else if (id > raiz->id) {
        raiz->derecho = eliminar(raiz->derecho, id);
    } else {
        // Escenario 1 y 2: Sin hijos o un solo hijo
        if (raiz->izquierdo == NULL) {
            Nodo* temporal = raiz->derecho;
            delete raiz;
            return temporal;
        } else if (raiz->derecho == NULL) {
            Nodo* temporal = raiz->izquierdo;
            delete raiz;
            return temporal;
        }

        // Escenario 3: Dos hijos (Busca el sucesor en Inorden)
        Nodo* temporal = encontrarMinimo(raiz->derecho);
        
        // Copiar los datos del sucesor al nodo actual
        raiz->id = temporal->id;
        raiz->nombre = temporal->nombre;
        raiz->rol = temporal->rol;

        // Eliminar físicamente el sucesor original
        raiz->derecho = eliminar(raiz->derecho, temporal->id);
    }
    return raiz;
}

// 2. DESARROLLO DE LA LÓGICA DE RECORRIDOS (Tus 3 consultas)

// A. Consulta por Generación (Preorden Modificado con contador de nivel)
void consultaGeneracion(Nodo* raiz, int nivelObjetivo, int nivelActual = 0) {
    if (raiz == NULL) return;

    if (nivelActual == nivelObjetivo) {
        cout << "   - ID: " << raiz->id << " | " << raiz->nombre << " (" << raiz->rol << ")\n";
    }

    consultaGeneracion(raiz->izquierdo, nivelObjetivo, nivelActual + 1);
    consultaGeneracion(raiz->derecho, nivelObjetivo, nivelActual + 1);
}

// B. Consulta de Ancestros (Rastreo de Camino de Búsqueda)
bool mostrarAncestros(Nodo* raiz, int idBuscado) {
    if (raiz == NULL) return false;

    // Si llegamos al nodo, terminamos la recursión sin imprimirlo a él mismo (solo sus ancestros)
    if (raiz->id == idBuscado) return true;

    // Si el nodo está en el camino izquierdo o derecho, este nodo actual es un ancestro
    if (mostrarAncestros(raiz->izquierdo, idBuscado) || mostrarAncestros(raiz->derecho, idBuscado)) {
        cout << " -> [" << raiz->id << ": " << raiz->nombre << " (" << raiz->rol << ")]\n";
        return true;
    }
    return false;
}

// Recorrido Inorden estándar para mostrar subárboles ordenados cronológicamente
void recorrerInorden(Nodo* raiz) {
    if (raiz == NULL) return;
    recorrerInorden(raiz->izquierdo);
    cout << "   - ID: " << raiz->id << " | " << raiz->nombre << " (" << raiz->rol << ")\n";
    recorrerInorden(raiz->derecho);
}

// C. Consulta de Descendientes (Inorden desde el Subárbol)
void consultaDescendientes(Nodo* raiz, int idMiembro) {
    Nodo* miembro = buscar(raiz, idMiembro);
    if (miembro == NULL) {
        cout << "Miembro no encontrado en las excavaciones.\n";
        return;
    }
    cout << "Descendientes de " << miembro->nombre << " (excluyendo otras ramas):\n";
    // Iniciamos el inorden únicamente desde sus dos ramas hijas
    if (miembro->izquierdo == NULL && miembro->derecho == NULL) {
        cout << "   (No posee descendientes registrados)\n";
    } else {
        recorrerInorden(miembro->izquierdo);
        recorrerInorden(miembro->derecho);
    }
}

// Función estética para ver el árbol rápidamente tipo carpetas
void mostrarEstructuraVisual(Nodo* raiz, string prefijo = "", bool esIzquierdo = false) {
    if (raiz == NULL) return;
    cout << prefijo << (esIzquierdo ? "+-- " : "+-- ") << raiz->id << " (" << raiz->rol << ")\n";
    string nuevoPrefijo = prefijo + (esIzquierdo ? "¦   " : "    ");
    if (raiz->izquierdo != NULL || raiz->derecho != NULL) {
        if (raiz->izquierdo != NULL) mostrarEstructuraVisual(raiz->izquierdo, nuevoPrefijo, true);
        if (raiz->derecho != NULL) mostrarEstructuraVisual(raiz->derecho, nuevoPrefijo, false);
    }
}

int main() {
    Nodo* raiz = NULL;

    // Simulando el ingreso de datos de la excavación (Añadí nombres inventados para cumplir tu estructura)
    raiz = insertar(raiz, 50, "Yax K'uk' Mo'", "ancestro fundador");
    raiz = insertar(raiz, 30, "Ah Kin", "sacerdote mayor");
    raiz = insertar(raiz, 20, "Ixchel", "princesa");
    raiz = insertar(raiz, 40, "Balam", "noble");
    raiz = insertar(raiz, 70, "K'an Tok", "general");
    raiz = insertar(raiz, 80, "Chilan", "astronomo");

    cout << "\n============================================\n";
    cout << "  ESTRUCTURA DE LA CIVILIZACION REGISTRADA  \n";
    cout << "============================================\n";
    mostrarEstructuraVisual(raiz);

    cout << "\n============================================\n";
    cout << "  PRUEBA DE RECORRIDOS (TUS CONSULTAS)       \n";
    cout << "============================================\n";

    // 1. Prueba de Generación (Nivel 2)
    cout << "\n1. Miembros de la Generacion 2 (Nivel 2):\n";
    consultaGeneracion(raiz, 2);

    // 2. Prueba de Ancestros
    cout << "\n2. Linea de ascendencia de la princesa Ixchel (ID: 20):\n";
    cout << "[Inicio: Persona consultada]\n";
    mostrarAncestros(raiz, 20);
    cout << " -> [Fin: Raiz principal del arbol]\n";

    // 3. Prueba de Descendientes
    cout << "\n3. Consulta de descendientes de Sacerdote Mayor (ID: 30):\n";
    consultaDescendientes(raiz, 30);

    return 0;
}
