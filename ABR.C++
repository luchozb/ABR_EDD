#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

//   BLOQUE 1: DEFINICIÓN DE ESTRUCTURAS

// Nodo para representar un Proceso (Gestor de Procesos y Planificador CPU)
struct Proceso {
    int id;
    char nombre[50];
    int prioridad;
    char estado[30];
    Proceso *siguiente; // Puntero al siguiente eslabón
};

// Nodo para simular bloques del Gestor de Memoria (Pila LIFO)
struct Memoria {
    int idAsignado;
    float tamanoMB;
    Memoria *anterior; // Puntero al elemento de abajo en la pila
};

//   BLOQUE 2: VARIABLES GLOBALES

// Punteros de control para las tres estructuras principales exigidas
Proceso *listaProcesos = NULL; // Cabeza de la Lista Enlazada Simple
Proceso *colaCPU = NULL;       // Frente de la Cola de Prioridad
Memoria *pilaMemoria = NULL;   // Tope de la Pila Dinámica

//   BLOQUE 3: VALIDACIÓN Y BÚSQUEDA

// Recorre secuencialmente la lista para verificar si un ID ya existe
bool existeProceso(int id) {
    Proceso *actual = listaProcesos;
    while (actual != NULL) {
        if (actual->id == id) return true; // ID duplicado detectado
        actual = actual->siguiente;
    }
    return false;
}

// Búsqueda secuencial exhaustiva por ID o por Nombre
void buscarProceso() {
    if (listaProcesos == NULL) {
        cout << "\nLa lista de procesos esta vacia.\n";
        return;
    }
    
    int criterio;
    cout << "\n=== BUSCAR PROCESO ===\n";
    cout << "1. Buscar por ID\n";
    cout << "2. Buscar por Nombre\n";
    cout << "Seleccione un criterio: ";
    cin >> criterio;

    Proceso *actual = listaProcesos;
    bool encontrado = false;

    if (criterio == 1) {
        int idBuscar;
        cout << "Ingrese el ID a buscar: ";
        cin >> idBuscar;
        while (actual != NULL) {
            if (actual->id == idBuscar) {
                encontrado = true;
                break;
            }
            actual = actual->siguiente;
        }
    } else if (criterio == 2) {
        char nombreBuscar[50];
        cin.ignore();
        cout << "Ingrese el Nombre a buscar: ";
        cin.getline(nombreBuscar, 50);
        while (actual != NULL) {
            if (strcmp(actual->nombre, nombreBuscar) == 0) {
                encontrado = true;
                break;
            }
            actual = actual->siguiente;
        }
    } else {
        cout << "Criterio invalido.\n";
        return;
    }

    if (encontrado) {
        cout << "\n--- Proceso Encontrado ---\n";
        cout << "ID: " << actual->id << "\nNombre: " << actual->nombre 
             << "\nPrioridad: " << actual->prioridad << "\nEstado: " << actual->estado << endl;
    } else {
        cout << "Error: proceso no encontrado en el sistema.\n";
    }
}

//   BLOQUE 4: GESTIÓN DE LA LISTA PRINCIPAL

// Crea un nuevo nodo de proceso, valida sus campos y lo añade al final de la lista
void registrarProceso() {
    Proceso *nuevo = new Proceso;

    cout << "\n=== REGISTRAR NUEVO PROCESO ===\n";
    cout << "Ingrese ID del proceso: ";
    cin >> nuevo->id;

    // Validación de ID positivo y único
    if (nuevo->id <= 0 || existeProceso(nuevo->id)) {
        cout << "Error: Ya existe un proceso registrado con ese ID o es invalido.\n";
        delete nuevo;
        return;
    }

    cin.ignore();
    cout << "Ingrese nombre del proceso: ";
    cin.getline(nuevo->nombre, 50);

    cout << "Ingrese prioridad (1 a 5): ";
    cin >> nuevo->prioridad;

    // Validación de rango de prioridad
    if (nuevo->prioridad < 1 || nuevo->prioridad > 5) {
        cout << "Error: prioridad invalida.\n";
        delete nuevo;
        return;
    }

    strcpy(nuevo->estado, "Listo"); // Estado inicial por defecto
    nuevo->siguiente = NULL;

    // Inserción al final de la Lista General de Procesos
    if (listaProcesos == NULL) {
        listaProcesos = nuevo; // Primer nodo del sistema
    } else {
        Proceso *actual = listaProcesos;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente; // Avanza hasta el último nodo
        }
        actual->siguiente = nuevo; // Enlaza el nuevo nodo
    }

    cout << "Proceso registrado correctamente en la lista.\n";
}

// Recorre e imprime en pantalla todos los procesos registrados secuencialmente
void mostrarProcesos() {
    if (listaProcesos == NULL) {
        cout << "\nNo hay procesos registrados en el sistema.\n";
        return;
    }

    Proceso *actual = listaProcesos;
    cout << "\n=== LISTA DE PROCESOS ACTIVOS ===\n";
    while (actual != NULL) {
        cout << "ID: " << actual->id << " | Nombre: " << actual->nombre 
             << " | Prioridad: " << actual->prioridad << " | Estado: " << actual->estado << endl;
        actual = actual->siguiente; // Salta al siguiente nodo
    }
}

// Localiza un proceso por ID y permite reescribir su valor de prioridad
void modificarPrioridad() {
    int id, nuevaPrioridad;
    cout << "\n=== MODIFICAR PRIORIDAD ===\n";
    cout << "Ingrese ID del proceso: ";
    cin >> id;

    Proceso *actual = listaProcesos;
    while (actual != NULL) {
        if (actual->id == id) {
            cout << "Prioridad actual: " << actual->prioridad << "\n";
            cout << "Ingrese nueva prioridad (1-5): ";
            cin >> nuevaPrioridad;

            if (nuevaPrioridad < 1 || nuevaPrioridad > 5) {
                cout << "Prioridad invalida.\n";
                return;
            }
            actual->prioridad = nuevaPrioridad;
            cout << "Prioridad actualizada correctamente.\n";
            return;
        }
        actual = actual->siguiente;
    }
    cout << "Proceso no encontrado.\n";
}

// Localiza un proceso por ID y permite reescribir su cadena de estado
void cambiarEstado() {
    int id;
    cout << "\n=== MODIFICAR ESTADO ===\n";
    cout << "Ingrese el ID del proceso a transformar: ";
    cin >> id;

    Proceso *actual = listaProcesos;
    while (actual != NULL) {
        if (actual->id == id) {
            cout << "Proceso localizado. Estado previo: " << actual->estado << "\n";
            cin.ignore();
            cout << "Ingrese nuevo estado (Listo / Ejecutando / Bloqueado): ";
            cin.getline(actual->estado, 30);
            cout << "Transicion de estado completada con exito.\n";
            return;
        }
        actual = actual->siguiente;
    }
    cout << "Error: El ID especificado no existe en el sistema.\n";
}

// Desenlaza un nodo de la lista general buscándolo por ID o Nombre y libera su memoria
void eliminarProceso() {
    if (listaProcesos == NULL) {
        cout << "\nLa lista de procesos esta vacia.\n";
        return;
    }

    int criterio;
    cout << "\n=== ELIMINAR PROCESO ===\n";
    cout << "1. Eliminar por ID\n";
    cout << "2. Eliminar por Nombre\n";
    cout << "Seleccione un criterio: ";
    cin >> criterio;

    Proceso *actual = listaProcesos;
    Proceso *anterior = NULL;
    bool encontrado = false;

    if (criterio == 1) {
        int idEliminar;
        cout << "Ingrese el ID del proceso a eliminar: ";
        cin >> idEliminar;
        while (actual != NULL) {
            if (actual->id == idEliminar) {
                encontrado = true;
                break;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
    } else if (criterio == 2) {
        char nombreEliminar[50];
        cin.ignore();
        cout << "Ingrese el Nombre del proceso a eliminar: ";
        cin.getline(nombreEliminar, 50);
        while (actual != NULL) {
            if (strcmp(actual->nombre, nombreEliminar) == 0) {
                encontrado = true;
                break;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
    } else {
        cout << "Criterio invalido.\n";
        return;
    }

    if (encontrado) {
        if (anterior == NULL) {
            listaProcesos = actual->siguiente; // El nodo a eliminar era la cabeza
        } else {
            anterior->siguiente = actual->siguiente; // Puentea el nodo intermedio
        }
        delete actual; // Destrucción del objeto en la memoria dinámica
        cout << "Proceso eliminado correctamente.\n";
    } else {
        cout << "Error: proceso no encontrado.\n";
    }
}

//   BLOQUE 5: COLA DE PRIORIDAD (PLANIFICADOR CPU)

// Inserta un clon de un proceso en una cola ordenada de mayor a menor prioridad
void encolarCPU() {
    int id;
    cout << "\n=== ENCOLAR EN CPU ===\n";
    cout << "Ingrese ID del proceso para enviar a CPU: ";
    cin >> id;

    // Buscar el proceso original en la lista general
    Proceso *buscado = listaProcesos;
    while (buscado != NULL && buscado->id != id) {
        buscado = buscado->siguiente;
    }

    if (buscado == NULL) {
        cout << "Proceso no encontrado en la lista principal.\n";
        return;
    }

    strcpy(buscado->estado, "Ejecutando");

    // Construcción del nodo réplica para la Cola
    Proceso *nuevo = new Proceso;
    nuevo->id = buscado->id;
    strcpy(nuevo->nombre, buscado->nombre);
    nuevo->prioridad = buscado->prioridad;
    strcpy(nuevo->estado, buscado->estado);
    nuevo->siguiente = NULL;

    // Algoritmo de Inserción Ordenada (De Mayor prioridad a Menor)
    if (colaCPU == NULL || nuevo->prioridad < colaCPU->prioridad) {
        nuevo->siguiente = colaCPU; // Inserta al principio si tiene la prioridad más alta (menor número = más urgente)
        colaCPU = nuevo;
    } else {
        Proceso *actual = colaCPU;
        // Avanza mientras el siguiente elemento tenga mayor o igual prioridad que el nuevo
        while (actual->siguiente != NULL && actual->siguiente->prioridad <= nuevo->prioridad) {
            actual = actual->siguiente;
        }
        nuevo->siguiente = actual->siguiente; // Ajusta punteros intermedios
        actual->siguiente = nuevo;
    }
    cout << "Proceso encolado para CPU segun su prioridad.\n";
}

// Procesa el elemento al frente de la cola (Mayor prioridad), cambiándole el estado y liberándolo
void ejecutarCPU() {
    if (colaCPU == NULL) {
        cout << "\nError: La cola de CPU esta vacia.\n";
        return;
    }

    Proceso *temp = colaCPU; // Almacena el frente temporalmente
    cout << "\n=== DESPACHANDO A CPU ===\n";
    cout << "ID: " << temp->id << " | Nombre: " << temp->nombre << " | Prioridad: " << temp->prioridad << endl;

    // Sincroniza el cambio de estado en la Lista Principal
    Proceso *actual = listaProcesos;
    while (actual != NULL) {
        if (actual->id == temp->id) {
            strcpy(actual->estado, "Terminado/Listo");
            break;
        }
        actual = actual->siguiente;
    }

    colaCPU = colaCPU->siguiente; // Avanza el frente de la cola
    delete temp;                  // Libera el nodo procesado de la CPU

    cout << "Proceso ejecutado y retirado de la cola de CPU.\n";
}

// Recorre e imprime los elementos en espera dentro de la Cola de CPU
void mostrarColaCPU() {
    if (colaCPU == NULL) {
        cout << "\nLa cola de procesos de la CPU esta vacia.\n";
        return;
    }

    Proceso *actual = colaCPU;
    int pos = 1;
    cout << "\n=== COLA DE PROCESOS - CPU ===\n";
    cout << "Posicion | ID Proceso | Prioridad | Nombre\n";
    cout << "--------------------------------------------\n";
    while (actual != NULL) {
        cout << "    " << pos << "    |     " << actual->id << "      |     " << actual->prioridad << "     | " << actual->nombre << endl;
        actual = actual->siguiente;
        pos++;
    }
    cout << "--------------------------------------------\n";
    cout << "Total de procesos en espera: " << pos - 1 << "\n";
}

//   BLOQUE 6: PILA DE MEMORIA (GESTOR LIFO)

// Inserta un bloque de memoria en el tope de la estructura (Operación PUSH)
void asignarMemoria() {
    int idProc;
    cout << "\n=== ASIGNAR BLOQUE DE MEMORIA (PUSH) ===\n";
    cout << "Ingrese el ID del proceso: ";
    cin >> idProc;

    if (!existeProceso(idProc)) {
        cout << "Error: el proceso no existe en el sistema.\n";
        return;
    }

    Memoria *nuevo = new Memoria;
    nuevo->idAsignado = idProc;

    cout << "Ingrese el tamano del bloque (en MB): ";
    cin >> nuevo->tamanoMB;

    if (nuevo->tamanoMB <= 0) {
        cout << "Tamano invalido.\n";
        delete nuevo;
        return;
    }

    // Mecánica de Pila: El nuevo siempre apunta a la antigua cabeza (hacia abajo)
    nuevo->anterior = pilaMemoria;
    pilaMemoria = nuevo; // El nuevo nodo pasa a ser el tope

    cout << "Bloque asignado correctamente (Push).\n";
}

// Extrae y destruye el último bloque de memoria añadido en el tope (Operación POP)
void liberarMemoria() {
    if (pilaMemoria == NULL) {
        cout << "\nError: La pila de memoria esta vacia. No hay bloques que liberar.\n";
        return;
    }

    Memoria *temp = pilaMemoria; // Captura el nodo del tope

    cout << "\n=== LIBERAR BLOQUE DE MEMORIA (POP) ===\n";
    cout << "Liberando bloque del proceso ID: " << temp->idAsignado << " | Memoria liberada: " << temp->tamanoMB << " MB\n";

    pilaMemoria = pilaMemoria->anterior; // El tope baja al elemento inferior
    delete temp;                         // Se libera la memoria física

    cout << "Bloque liberado correctamente (Pop).\n";
}

// Recorre e imprime la pila de memoria desde el tope hasta la base
void mostrarMemoria() {
    if (pilaMemoria == NULL) {
        cout << "\nLa pila de memoria esta vacia.\n";
        return;
    }

    Memoria *actual = pilaMemoria;
    bool esTope = true;
    cout << "\n=== ESTADO ACTUAL - PILA DE MEMORIA ===\n";
    cout << "ID Proceso | Memoria (MB)\n";
    cout << "-------------------------\n";
    while (actual != NULL) {
        cout << "    " << actual->idAsignado << "     |     " << actual->tamanoMB;
        if (esTope) {
            cout << "  <- TOPE";
            esTope = false;
        }
        cout << "\n";
        actual = actual->anterior;
    }
    cout << "-------------------------\n";
}

//   BLOQUE 7: ENTRADA/SALIDA DE ARCHIVOS

// Escribe secuencialmente los datos de la Lista Principal en un archivo plano de texto
void guardarArchivo() {
    ofstream archivo("procesos.txt");
    if (!archivo) {
        cout << "Error al crear archivo.\n";
        return;
    }

    Proceso *actual = listaProcesos;
    while (actual != NULL) {
        archivo << actual->id << "\n";
        archivo << actual->nombre << "\n";
        archivo << actual->prioridad << "\n";
        archivo << actual->estado << "\n";
        actual = actual->siguiente;
    }

    archivo.close();
    cout << "Datos guardados exitosamente en 'procesos.txt'\n";
}

// Lee el archivo de texto y reconstruye la lista general
void cargarArchivo() {
    ifstream archivo("procesos.txt");
    if (!archivo) {
        cout << "No existe archivo previo. Iniciando base de datos vacia.\n";
        return;
    }

    int idTemporal;
    // Control de lectura basado en la obtención del ID primario para evitar registros fantasma
    while (archivo >> idTemporal) {
        Proceso *nuevo = new Proceso;
        nuevo->id = idTemporal;
        archivo.ignore();

        archivo.getline(nuevo->nombre, 50);
        archivo >> nuevo->prioridad;
        archivo.ignore();
        archivo.getline(nuevo->estado, 30);

        nuevo->siguiente = NULL;

        // Reconstrucción de la Lista General por el extremo final
        if (listaProcesos == NULL) {
            listaProcesos = nuevo;
        } else {
            Proceso *actual = listaProcesos;
            while (actual->siguiente != NULL) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
    }

    archivo.close();
}

//   BLOQUE 8: LIMPIEZA DE MEMORIA Y MENÚ

// Recorre de forma exhaustiva todas las estructuras dinámicas aplicando "delete" para evitar Fugas de Memoria
void liberarTodaLaMemoria() {
    // 1. Vacía la lista de procesos
    while(listaProcesos != NULL) {
        Proceso *temp = listaProcesos;
        listaProcesos = listaProcesos->siguiente;
        delete temp;
    }
    // 2. Vacía la cola remanente de la CPU
    while(colaCPU != NULL) {
        Proceso *temp = colaCPU;
        colaCPU = colaCPU->siguiente;
        delete temp;
    }
    // 3. Vacía la pila de asignaciones de memoria
    while(pilaMemoria != NULL) {
        Memoria *temp = pilaMemoria;
        pilaMemoria = pilaMemoria->anterior;
        delete temp;
    }
}

// Interfaz por Consola basada en Switch-Case para invocar las rutinas de control
void menu() {
    int opcion;
    do {
        cout << "\n===== SISTEMA DE GESTION DE PROCESOS (SIMULADOR OS) =====\n";
        cout << "1. Registrar proceso\n2. Mostrar todos los procesos\n3. Buscar proceso\n";
        cout << "4. Modificar prioridad\n5. Cambiar estado\n6. Eliminar proceso\n";
        cout << "7. Encolar proceso en CPU\n8. Desencolar/Ejecutar en CPU\n9. Mostrar cola CPU\n";
        cout << "10. Asignar memoria (PUSH)\n11. Liberar memoria (POP)\n12. Mostrar pila de memoria\n";
        cout << "13. Guardar datos en archivo\n0. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:     registrarProceso(); break;
            case 2: mostrarProcesos(); break;
            case 3:     buscarProceso(); break;
            case 4:     modificarPrioridad(); break;
            case 5: cambiarEstado(); break;
            case 6:     eliminarProceso(); break;
            case 7: encolarCPU(); break;
            case 8: ejecutarCPU(); break;
            case 9:     mostrarColaCPU(); break;
            case 10:    asignarMemoria(); break;
            case 11:    liberarMemoria(); break;
            case 12:    mostrarMemoria(); break;
            case 13:    guardarArchivo(); break;
            case 0:
            	
                 guardarArchivo();
                 liberarTodaLaMemoria(); // Llama a la rutina de destrucción total
                cout << "Datos guardados y memoria de estructuras liberada. Saliendo...\n";
                break;
            default:
                cout << "Opcion invalida. Intente nuevamente.\n";
        }
    } while (opcion != 0);
}

// Función principal del programa
int main() {
    cargarArchivo();    // Recupera la sesión anterior si el archivo "procesos.txt" existe
    menu();             // Inicia el ciclo interactivo de la aplicación
    return 0;
}