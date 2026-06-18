#include <iostream>
#include <string>
#include <cstdlib> // Para system("cls") y system("pause")
#include <queue>   // Estructura necesaria para el recorrido por niveles (Normal)
#include <cmath>   // Para calcular los espacios matematicos con pow()

using namespace std;

struct NodoMiembro {
    int id;                 
    string nombre;          
    string rol;             
    NodoMiembro* izquierdo; 
    NodoMiembro* derecho;   
};


//GESTIÓN DE MEMORIA Y CREACIÓN DE NODOS
NodoMiembro* crearNodo(int id, string nombre, string rol) {
    NodoMiembro* nuevo = new NodoMiembro();
    nuevo->id = id;
    nuevo->nombre = nombre;
    nuevo->rol = rol;
    nuevo->izquierdo = NULL;
    nuevo->derecho = NULL;
    return nuevo;
}

// OPERACIONES FUNDAMENTALES DEL ABB

bool insertarNodo(NodoMiembro*& raiz, int id, string nombre, string rol) {
    if (raiz == NULL) {
        raiz = crearNodo(id, nombre, rol);
        return true;
    }
    if (id < raiz->id) {
        return insertarNodo(raiz->izquierdo, id, nombre, rol);
    } else if (id > raiz->id) {
        return insertarNodo(raiz->derecho, id, nombre, rol);
    } else {
        return false; 
    }
}

NodoMiembro* buscarNodo(NodoMiembro* raiz, int id) {
    if (raiz == NULL || raiz->id == id) {
        return raiz;
    }
    if (id < raiz->id) {
        return buscarNodo(raiz->izquierdo, id);
    }
    return buscarNodo(raiz->derecho, id);
}

NodoMiembro* encontrarMinimo(NodoMiembro* raiz) {
    NodoMiembro* actual = raiz;
    while (actual && actual->izquierdo != NULL) {
        actual = actual->izquierdo;
    }
    return actual;
}

NodoMiembro* eliminarNodo(NodoMiembro* raiz, int id, bool& encontrado) {
    if (raiz == NULL) {
        encontrado = false;
        return raiz;
    }

    if (id < raiz->id) {
        raiz->izquierdo = eliminarNodo(raiz->izquierdo, id, encontrado);
    } else if (id > raiz->id) {
        raiz->derecho = eliminarNodo(raiz->derecho, id, encontrado);
    } else {
        encontrado = true; 
        
        if (raiz->izquierdo == NULL && raiz->derecho == NULL) {
            delete raiz;
            return NULL;
        }
        else if (raiz->izquierdo == NULL) {
            NodoMiembro* temp = raiz->derecho;
            delete raiz;
            return temp;
        } else if (raiz->derecho == NULL) {
            NodoMiembro* temp = raiz->izquierdo;
            delete raiz;
            return temp;
        }
        else {
            NodoMiembro* temp = encontrarMinimo(raiz->derecho);
            raiz->id = temp->id;
            raiz->nombre = temp->nombre;
            raiz->rol = temp->rol;
            raiz->derecho = eliminarNodo(raiz->derecho, temp->id, encontrado);
        }
    }
    return raiz;
}

// RECORRIDOS Y CONSULTAS ESPECIALIZADAS
void recorridoInorden(NodoMiembro* raiz) {
    if (raiz == NULL) return;
    recorridoInorden(raiz->izquierdo);
    cout << "[" << raiz->id << " - " << raiz->nombre << " (" << raiz->rol << ")]\n";
    recorridoInorden(raiz->derecho);
}

void consultaPorGeneracion(NodoMiembro* raiz, int nivelObjetivo, int nivelActual, bool& huboDatos) {
    if (raiz == NULL) return;
    if (nivelActual == nivelObjetivo) {
        cout << " -> ID: " << raiz->id << " | Nombre: " << raiz->nombre << " | Rol: " << raiz->rol << "\n";
        huboDatos = true;
    }
    consultaPorGeneracion(raiz->izquierdo, nivelObjetivo, nivelActual + 1, huboDatos);
    consultaPorGeneracion(raiz->derecho, nivelObjetivo, nivelActual + 1, huboDatos);
}

bool rastrearAncestros(NodoMiembro* raiz, int idObjetivo) {
    if (raiz == NULL) return false;
    if (raiz->id == idObjetivo) return true;
    
    if (rastrearAncestros(raiz->izquierdo, idObjetivo) || rastrearAncestros(raiz->derecho, idObjetivo)) {
        cout << " -> ID: " << raiz->id << " | Nombre: " << raiz->nombre << " | Rol: " << raiz->rol << "\n";
        return true;
    }
    return false;
}

void consultaDescendientes(NodoMiembro* raiz, int idAncestro) {
    NodoMiembro* subRaiz = buscarNodo(raiz, idAncestro);
    if (subRaiz == NULL) {
        cout << "--> Miembro no encontrado en el sistema.\n";
        return;
    }
    cout << "Descendientes de " << subRaiz->nombre << " (ID: " << idAncestro << "):\n";
    if (subRaiz->izquierdo == NULL && subRaiz->derecho == NULL) {
        cout << " -> No tiene descendencia registrada.\n";
    } else {
        recorridoInorden(subRaiz->izquierdo); 
        recorridoInorden(subRaiz->derecho);
    }
}

int obtenerAltura(NodoMiembro* raiz) {
    if (raiz == NULL) return 0;
    return 1 + max(obtenerAltura(raiz->izquierdo), obtenerAltura(raiz->derecho));
}

void mostrarArbolNormalPorNiveles(NodoMiembro* raiz) {
    if (raiz == NULL) {
        cout << "\nEl arbol esta vacio actualmente.\n";
        return;
    }

    int altura = obtenerAltura(raiz);
    queue<NodoMiembro*> colaActual;
    colaActual.push(raiz);

    int maxNodosNivel = 1; 
    
    for (int nivel = 0; nivel < altura; nivel++) {
        int espaciosAntes = pow(2, altura - nivel) - 2;
        int espaciosEntre = pow(2, altura - nivel + 1) - 3;

        for (int i = 0; i < espaciosAntes; i++) cout << " ";

        queue<NodoMiembro*> colaSiguiente;

        for (int i = 0; i < maxNodosNivel; i++) {
            NodoMiembro* actual = colaActual.front();
            colaActual.pop();

            if (actual != NULL) {
                if(actual->id < 10) cout << "0" << actual->id;
                else cout << actual->id;
                
                colaSiguiente.push(actual->izquierdo);
                colaSiguiente.push(actual->derecho);
            } else {
                cout << "--"; 
                colaSiguiente.push(NULL);
                colaSiguiente.push(NULL);
            }

            for (int j = 0; j < espaciosEntre; j++) cout << " ";
        }
        
        cout << "\n\n"; 
        colaActual = colaSiguiente;
        maxNodosNivel *= 2;
    }
    cout << "* Nota: '--' representa una rama vacia (puntero NULL) para mantener la simetria.\n";
}

int main() {
    NodoMiembro* raiz = NULL;
    int opcion, id, nivel;
    string nombre, rol;
    bool exito;

    do {
        system("cls"); 
        cout << "=======================================================\n";
        cout << "     SISTEMA DINAMICO DE LINAJES ARQUEOLOGICOS (ABB)   \n";
        cout << "=======================================================\n";
        cout << " 1. Insertar nuevo miembro de la civilizacion\n";
        cout << " 2. Buscar miembro por ID\n";
        cout << " 3. Eliminar miembro (Reestructuracion automatica)\n";
        cout << " 4. Mostrar estructura NORMAL del Arbol (De arriba a abajo)\n";
        cout << " 5. Listar Cronologia Dinastica (Inorden)\n";
        cout << " 6. Consultar miembros por Generacion (Nivel)\n";
        cout << " 7. Rastrear Ancestros de un miembro\n";
        cout << " 8. Consultar Descendientes de una sublinea\n";
        cout << " 9. Salir del programa\n";
        cout << "=======================================================\n";
        cout << " Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                system("cls");
                cout << "--- REGISTRO DE NUEVO MIEMBRO ---\n\n";
                cout << "Ingrese ID cronologico (Clave numerica): ";
                cin >> id;
                cin.ignore(); 
                cout << "Ingrese Nombre historico/runico: ";
                getline(cin, nombre);
                cout << "Ingrese Rol/Estamento social: ";
                getline(cin, rol);
                
                if (insertarNodo(raiz, id, nombre, rol)) {
                    cout << "\n[Exito] Miembro registrado correctamente.\n";
                } else {
                    cout << "\n[Error] El ID " << id << " ya existe.\n";
                }
                break;

            case 2:
                system("cls");
                cout << "--- BUSQUEDA DE REGISTROS ARQUEOLOGICOS ---\n\n";
                cout << "Ingrese el ID a buscar: ";
                cin >> id;
                {
                    NodoMiembro* encontrado = buscarNodo(raiz, id);
                    if (encontrado != NULL) {
                        cout << "\n[Registro Localizado]\n";
                        cout << " -> ID: " << encontrado->id << "\n";
                        cout << " -> Nombre: " << encontrado->nombre << "\n";
                        cout << " -> Rol: " << encontrado->rol << "\n";
                    } else {
                        cout << "\n[Alerta] No se encontro el ID " << id << ".\n";
                    }
                }
                break;

            case 3:
                system("cls");
                cout << "--- ELIMINACION Y REESTRUCTURACION ---\n\n";
                cout << "Ingrese el ID del miembro a eliminar: ";
                cin >> id;
                exito = false;
                raiz = eliminarNodo(raiz, id, exito);
                if (exito) {
                    cout << "\n[Exito] Miembro eliminado. El arbol se ha reestructurado.\n";
                } else {
                    cout << "\n[Error] El ID " << id << " no existe.\n";
                }
                break;

            case 4:
                system("cls");
                cout << "--- REPRESENTACION DEL ARBOL BINARIO (VISTA TRADICIONAL) ---\n\n";
                mostrarArbolNormalPorNiveles(raiz);
                break;

            case 5:
                system("cls");
                cout << "--- CRONOLOGIA DINASTICA GENERADA (INORDEN) ---\n\n";
                if (raiz == NULL) {
                    cout << "El arbol esta vacio.\n";
                } else {
                    recorridoInorden(raiz);
                }
                break;

            case 6:
                system("cls");
                cout << "--- CONSULTA FILTRADA POR GENERACION ---\n\n";
                cout << "Ingrese el nivel de generacion a consultar (Raiz = 0): ";
                cin >> nivel;
                cout << "\nMiembros pertenecientes a la generacion " << nivel << ":\n";
                exito = false;
                consultaPorGeneracion(raiz, nivel, 0, exito);
                if (!exito) {
                    cout << " -> No se encontraron miembros en este nivel.\n";
                }
                break;

            case 7:
                system("cls");
                cout << "--- RASTREO DE ANCESTROS DIRECTOS ---\n\n";
                cout << "Ingrese el ID del miembro objetivo: ";
                cin >> id;
                cout << "\nCamino ascendente de ancestros:\n";
                if (!rastrearAncestros(raiz, id)) {
                    cout << " -> El ID no existe o es la propia raiz.\n";
                }
                break;

            case 8:
                system("cls");
                cout << "--- CONSULTA DE DESCENDENCIA DE SUBARBOL ---\n\n";
                cin >> id;
                cout << "\n";
                consultaDescendientes(raiz, id);
                break;

            case 9:
                system("cls");
                cout << "\nCerrando el Sistema Arqueologico. ¡Hasta pronto!\n\n";
                break;

            default:
                cout << "\n[Opcion Invalida] Seleccione un numero del 1 al 9.\n";
                break;
        }
        cout << "\n";
        if (opcion != 9) {
            system("pause"); 
        }
    } while (opcion != 9);

    return 0;
}
