#include <iostream>
#include <string>
#include <cstdlib> // Para system("pause") y system("cls")
#include <queue>   // Estructura necesaria para el recorrido por niveles
#include <cmath>   // Para calcular los espacios matemáticos con pow()
#include <limits>  // Necesario para limpiar por completo el buffer de cin
#include <iomanip> // Para formatear la salida con ceros a la izquierda (setw)

using namespace std;

struct NodoMiembro {
    int id;                 
    string nombre;          
    string rol;             
    NodoMiembro* izquierdo; 
    NodoMiembro* derecho;   
};

NodoMiembro* crearNodo(int id, string nombre, string rol) {
    NodoMiembro* nuevo = new NodoMiembro();
    nuevo->id = id;
    nuevo->nombre = nombre;
    nuevo->rol = rol;
    nuevo->izquierdo = NULL;
    nuevo->derecho = NULL;
    return nuevo;
}

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

void recorridoInorden(NodoMiembro* raiz) {
    if (raiz == NULL) return;
    recorridoInorden(raiz->izquierdo);
    cout << " -> ID: " << raiz->id << " - " << raiz->nombre << " (" << raiz->rol << ")\n";
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

// VISUALIZACIÓN VERTICAL CORREGIDA PARA MOSTRAR IDs COMPLETOS (HASTA 3 DÍGITOS)
void mostrarArbolNormalPorNiveles(NodoMiembro* r) {
    if (r == NULL) {
        cout << "\nEl arbol esta vacio actualmente.\n";
        return;
    }

    int altura = obtenerAltura(r);
    queue<NodoMiembro*> q;
    q.push(r);

    for (int nivel = 0; nivel < altura; nivel++) {
        // 1. Imprimir las líneas de conexión vertical
        if (nivel > 0) {
            int espAntesCon = pow(2, altura - nivel + 1) - 1;
            int espEntreCon = pow(2, altura - nivel + 2) - 3;
            
            for (int i = 0; i < espAntesCon; i++) cout << " ";
            
            for (int i = 0; i < pow(2, nivel - 1); i++) {
                cout << "/   \\";
                for (int j = 0; j < espEntreCon - 2; j++) cout << " ";
            }
            cout << "\n";
        }

        // 2. Imprimir los nodos con formato fijo de 3 espacios
        int espAntes = pow(2, altura - nivel + 1) - 2;
        int espEntre = pow(2, altura - nivel + 2) - 3;

        for (int i = 0; i < espAntes; i++) cout << " ";

        int nodosEnNivel = pow(2, nivel);
        queue<NodoMiembro*> siguienteQ;

        for (int i = 0; i < nodosEnNivel; i++) {
            NodoMiembro* actual = q.front();
            q.pop();

            if (actual != NULL) {
                // Rellena con ceros a la izquierda si el número es menor a 100 (Ej: 005, 080, 125)
                cout << setfill('0') << setw(3) << actual->id;

                siguienteQ.push(actual->izquierdo);
                siguienteQ.push(actual->derecho);
            } else {
                cout << "---"; // Mantiene simetría de 3 caracteres para nodos vacíos
                siguienteQ.push(NULL);
                siguienteQ.push(NULL);
            }

            for (int j = 0; j < espEntre - 2; j++) cout << " ";
        }

        cout << "\n\n"; 
        q = siguienteQ;
    }
    cout << setfill(' '); // Reestablece el relleno por defecto de la consola
    cout << "* Nota: Los numeros representan el ID del miembro. Use la opcion 5 para ver detalles completos.\n";
}

// FUNCIÓN AUXILIAR DE VALIDACIÓN
int pedirEnteroValido() {
    int numero;
    while (!(cin >> numero) || numero < 0) {
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        cout << "[!] Error. Ingrese unicamente un numero entero positivo: ";
    }
    return numero;
}

int main() {
    NodoMiembro* raiz = NULL;
    int opcion, id, nivel;
    string nombre, rol;
    bool exito;

    system("cls"); 

    do {
        cout << "\n=======================================================\n";
        cout << "     SISTEMA DINAMICO DE LINAJES ARQUEOLOGICOS (ABB)   \n";
        cout << "=======================================================\n";
        cout << " 1. Insertar nuevo miembro de la civilizacion\n";
        cout << " 2. Buscar miembro por ID\n";
        cout << " 3. Eliminar miembro (Reestructuracion automatica)\n";
        cout << " 4. Mostrar estructura VERTICAL del Arbol\n";
        cout << " 5. Listar Cronologia Dinastica (Inorden)\n";
        cout << " 6. Consultar miembros por Generacion (Nivel)\n";
        cout << " 7. Rastrear Ancestros de un miembro\n";
        cout << " 8. Consultar Descendientes de una sublinea\n";
        cout << " 9. Limpiar historial de la pantalla\n"; 
        cout << " 10. Salir del programa\n";
        cout << "=======================================================\n";
        cout << " Seleccione una opcion: ";
        
        opcion = pedirEnteroValido(); 

        switch (opcion) {
            case 1:
                cout << "\n--- REGISTRO DE NUEVO MIEMBRO ---\n";
                cout << "Ingrese ID cronologico: ";
                id = pedirEnteroValido(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
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
                cout << "\n--- BUSQUEDA DE REGISTROS ARQUEOLOGICOS ---\n";
                cout << "Ingrese el ID a buscar: ";
                id = pedirEnteroValido();
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
                cout << "\n--- ELIMINACION Y REESTRUCTURACION ---\n";
                cout << "Ingrese el ID del miembro a eliminar: ";
                id = pedirEnteroValido();
                exito = false;
                raiz = eliminarNodo(raiz, id, exito);
                if (exito) {
                    cout << "\n[Exito] Miembro eliminado. El arbol se ha reestructurado.\n";
                } else {
                    cout << "\n[Error] El ID " << id << " no existe.\n";
                }
                break;

            case 4:
                cout << "\n--- REPRESENTACION VERTICAL DEL ARBOL ---\n\n";
                mostrarArbolNormalPorNiveles(raiz);
                break;

            case 5:
                cout << "\n--- CRONOLOGIA DINASTICA GENERADA (INORDEN) ---\n";
                if (raiz == NULL) {
                    cout << "El arbol esta vacio.\n";
                } else {
                    recorridoInorden(raiz);
                }
                break;

            case 6:
                cout << "\n--- CONSULTA FILTRADA POR GENERACION ---\n";
                cout << "Ingrese el nivel de generacion a consultar (Raiz = 0): ";
                nivel = pedirEnteroValido();
                cout << "\nMiembros pertenecientes a la generacion " << nivel << ":\n";
                exito = false;
                consultaPorGeneracion(raiz, nivel, 0, exito);
                if (!exito) {
                    cout << " -> No se encontraron miembros en este nivel.\n";
                }
                break;

            case 7:
                cout << "\n--- RASTREO DE ANCESTROS DIRECTOS ---\n";
                cout << "Ingrese el ID del miembro objetivo: ";
                id = pedirEnteroValido();
                cout << "\nCamino ascendente de ancestros:\n";
                if (!rastrearAncestros(raiz, id)) {
                    cout << " -> El ID no existe o es la propia raiz.\n";
                }
                break;

            case 8:
                cout << "\n--- CONSULTA DE DESCENDENCIA DE SUBARBOL ---\n";
                cout << "Ingrese el ID del ancestro base: ";
                id = pedirEnteroValido();
                cout << "\n";
                consultaDescendientes(raiz, id);
                break;

            case 9:
                system("cls");
                cout << "[Historial Limpiado]\n";
                break;

            case 10:
                cout << "\nCerrando el Sistema Arqueologico. ¡Hasta pronto!\n\n";
                break;

            default:
                cout << "\n[Opcion Invalida] Seleccione un numero del 1 al 10.\n";
                break;
        }
        cout << "\n";
        if (opcion != 10 && opcion != 9) {
            system("pause"); 
        }
    } while (opcion != 10);

    return 0;
}
