#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

// 1. ESTRUCTURA DEL NODO (Con nombre y rol)
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

// ALGORITMO DE INSERCIÓN
Nodo* insertar(Nodo* raiz, int id, string nombre, string rol) {
    if (raiz == NULL) return new Nodo(id, nombre, rol);
    if (id < raiz->id) raiz->izquierdo = insertar(raiz->izquierdo, id, nombre, rol);
    else if (id > raiz->id) raiz->derecho = insertar(raiz->derecho, id, nombre, rol);
    return raiz;
}

// ALGORITMO DE BÚSQUEDA
Nodo* buscar(Nodo* raiz, int id) {
    if (raiz == NULL || raiz->id == id) return raiz;
    if (id < raiz->id) return buscar(raiz->izquierdo, id);
    return buscar(raiz->derecho, id);
}

// --- NUEVA VISTA VISUAL PIRAMIDAL ---
int obtenerAltura(Nodo* raiz) {
    if (raiz == NULL) return 0;
    return 1 + max(obtenerAltura(raiz->izquierdo), obtenerAltura(raiz->derecho));
}

void imprimirArbolPiramidal(Nodo* raiz) {
    if (raiz == NULL) return;

    int altura = obtenerAltura(raiz);
    queue<Nodo*> colaNodos;
    colaNodos.push(raiz);
    int nodosEnNivel = 1;
    
    for (int i = 0; i < altura; i++) {
        int espaciosIniciales = pow(2, altura - i + 1) - 2;
        int espaciosEntre = pow(2, altura - i + 2) - 3;

        for (int k = 0; k < espaciosIniciales; k++) cout << " ";

        int nivelActualNodos = nodosEnNivel;
        nodosEnNivel = 0;
        vector<Nodo*> hijosSiguienteNivel;

        for (int j = 0; j < nivelActualNodos; j++) {
            Nodo* actual = colaNodos.front();
            colaNodos.pop();

            if (actual != NULL) {
                cout << "(" << actual->id << ")";
                hijosSiguienteNivel.push_back(actual->izquierdo);
                hijosSiguienteNivel.push_back(actual->derecho);
                if (actual->izquierdo != NULL) nodosEnNivel++;
                if (actual->derecho != NULL) nodosEnNivel++;
            } else {
                cout << "     "; 
                hijosSiguienteNivel.push_back(NULL);
                hijosSiguienteNivel.push_back(NULL);
            }

            for (int k = 0; k < espaciosEntre - 2; k++) cout << " ";
        }
        cout << "\n\n"; 

        for (size_t h = 0; h < hijosSiguienteNivel.size(); h++) {
            colaNodos.push(hijosSiguienteNivel[h]);
        }
        if (nodosEnNivel == 0) break;
    }
}

// 2. TUS ALGORITMOS DE CONSULTA
void consultaGeneracion(Nodo* raiz, int nivelObjetivo, int nivelActual = 0) {
    if (raiz == NULL) return;
    if (nivelActual == nivelObjetivo) {
        cout << "   - ID: " << raiz->id << " | " << raiz->nombre << " (" << raiz->rol << ")\n";
    }
    consultaGeneracion(raiz->izquierdo, nivelObjetivo, nivelActual + 1);
    consultaGeneracion(raiz->derecho, nivelObjetivo, nivelActual + 1);
}

bool mostrarAncestros(Nodo* raiz, int idBuscado) {
    if (raiz == NULL) return false;
    if (raiz->id == idBuscado) return true;

    if (mostrarAncestros(raiz->izquierdo, idBuscado) || mostrarAncestros(raiz->derecho, idBuscado)) {
        cout << " -> [" << raiz->id << ": " << raiz->nombre << " (" << raiz->rol << ")]\n";
        return true;
    }
    return false;
}

void recorrerInorden(Nodo* raiz) {
    if (raiz == NULL) return;
    recorrerInorden(raiz->izquierdo);
    cout << "   - ID: " << raiz->id << " | " << raiz->nombre << " (" << raiz->rol << ")\n";
    recorrerInorden(raiz->derecho);
}

void consultaDescendientes(Nodo* raiz, int idMiembro) {
    Nodo* miembro = buscar(raiz, idMiembro);
    if (miembro == NULL) {
        cout << "Miembro no encontrado.\n";
        return;
    }
    cout << "Descendientes de " << miembro->nombre << ":\n";
    if (miembro->izquierdo == NULL && miembro->derecho == NULL) {
        cout << "   (No posee descendientes registrados)\n";
    } else {
        recorrerInorden(miembro->izquierdo);
        recorrerInorden(miembro->derecho);
    }
}

int main() {
    Nodo* raiz = NULL;

    // Carga de la estructura arqueológica real
    raiz = insertar(raiz, 50, "Yax K'uk' Mo'", "ancestro fundador");
    raiz = insertar(raiz, 30, "Ah Kin", "sacerdote mayor");
    raiz = insertar(raiz, 20, "Ixchel", "princesa");
    raiz = insertar(raiz, 40, "Balam", "noble");
    raiz = insertar(raiz, 70, "K'an Tok", "general");
    raiz = insertar(raiz, 80, "Chilan", "astronomo");

    cout << "========================================================\n";
    cout << "          RECONSTRUCCION DEL ARBOL GENEALOGICO         \n";
    cout << "========================================================\n\n";
    
    // Imprime el dibujo centrado por niveles
    imprimirArbolPiramidal(raiz);

    cout << "========================================================\n";
    cout << "LEYENDA DE MIEMBROS REGISTRADOS:\n";
    cout << "ID 50: Yax K'uk' Mo' (Ancestor Fundador)\n";
    cout << "ID 30: Ah Kin (Sacerdote Mayor) | ID 70: K'an Tok (General)\n";
    cout << "ID 20: Ixchel (Princesa)        | ID 40: Balam (Noble) | ID 80: Chilan (Astronomo)\n";
    cout << "========================================================\n";

    cout << "\n>>> PRUEBA DE TUS CONSULTAS ARQUEOLOGICAS <<<\n";
    cout << "\nConsultando Ancestros de la Princesa Ixchel (ID: 20):\n";
    cout << "[Inicio]";
    mostrarAncestros(raiz, 20);
    cout << " -> [Raiz Principal]\n";

    return 0;
}
    mostrarAncestros(raiz, 20);
    cout << " -> [Fin: Raiz principal del arbol]\n";

    // 3. Prueba de Descendientes
    cout << "\n3. Consulta de descendientes de Sacerdote Mayor (ID: 30):\n";
    consultaDescendientes(raiz, 30);

    return 0;
}
