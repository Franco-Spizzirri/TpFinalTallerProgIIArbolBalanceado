#include <stdio.h>
#include <stdlib.h>
//Operaciones principales en un arbol AVL: busqueda, insercion y eliminacion, junto con las rotaciones necesarias para mantener el balance del arbol.

//Estructura del nodo:

typedef struct nodo
{
    int dato;
    int altura;
    struct nodo* h_izq;
    struct nodo* h_der;
}Nodo;

// Estructura para la cola
typedef struct Cola
{
Nodo ** arreglo ; // Arreglo de punteros a nodos
int frente ;
int ultimo ;
int capacidad ;
} Cola ;


// Funciones:
int altura(Nodo* nodo);
int factorBalanceo(Nodo* nodo);
Nodo* rotacionDerecha(Nodo* y);
Nodo* rotacionIzquierda(Nodo* x);
Nodo* rotacionIzquierdaDerecha(Nodo* nodo);
Nodo* rotacionDerechaIzquierda(Nodo* nodo);
Nodo* insertar(Nodo* nodo, int valor);
Nodo* nuevoNodo(int valor);
Nodo* obtenerMinimo(Nodo* nodo);
Nodo* eliminar(Nodo* nodo, int valor);
Cola* crearCola(int capacidad);
int colaVacia(Cola* cola);
void encola(Cola* cola, Nodo* nodo);
Nodo* desencola(Cola* cola);
void recorridoHorizontal(Nodo* nodo);
void imprimir(Nodo* nodo);
void actualizarAltura(Nodo* nodo);
int max(int a, int b);

int main()
{
    Nodo* raiz = NULL;
    int numero;
    char opcion;

while(opcion != '3')
{
    printf("\nMenu: \n");
    printf("Ingrese 1 si quiere agregar un elemento al arbol. \n");
    printf("Ingrese 2 si quiere eliminar un elemento del arbol. \n");
    printf("Ingrese 3 para terminar el programa. \n");
    printf("Elige una opcion: \n");

    scanf(" %c", &opcion);

    switch(opcion)
    {
    case '1':
        printf("Ingrese un numero para el Arbol AVL: \n");
        scanf("%d", &numero);
        raiz = insertar(raiz, numero);
        printf("Recorrido horizontal con %d agregado: \n", numero);
        recorridoHorizontal(raiz);
        printf("\nAltura actual del Arbol AVL: %d\n", altura(raiz));
        break;

    case '2':
        if(raiz == NULL)
        {
            printf("Arbol AVL vacio. No se puede eliminar el elemento. \n");
            printf("\nAltura actual del Arbol AVL: %d\n", altura(raiz));
            break;
        }
        else
        {
            printf("Ingrese el elemento que quieres eliminar: \n");
            scanf("%d", &numero);
            raiz = eliminar(raiz, numero);
            printf("Recorrido horizontal con %d eliminado: \n", numero);
            recorridoHorizontal(raiz);
            printf("\nAltura actual del Arbol AVL: %d\n", altura(raiz));
            break;
        }

    case '3':
        printf("Programa finalizado, Arbol AVL final con los datos insertados/eliminados: \n");
        recorridoHorizontal(raiz);
        printf("\nAltura actual del Arbol AVL: %d\n", altura(raiz));
        break;

    default:
        printf("Opcion no valida. Intenta de nuevo: \n");
        break;
    }
}

return 0;
}

int altura(Nodo* nodo)
{
    if (nodo == NULL )
    {
        return 0;
    }
    else
    {
        return nodo->altura;
    }
}

int factorBalanceo(Nodo* nodo)
{
    if(nodo == NULL)
    {
        return 0;
    }

    else
    {
        return (altura(nodo->h_izq) - altura(nodo->h_der));
    }
}
// rotacion simple a la derecha
Nodo* rotacionDerecha(Nodo* y)
{
    Nodo* x = y->h_izq; // variable auxiliar para no perder referencia.
    Nodo* t = x->h_der; // variable temporal para hacer el cambio.

    x->h_der = y;
    y->h_izq = t;

    actualizarAltura(y);
    actualizarAltura(x);

    return x;
}

//rotacion simple a la izquierda
Nodo* rotacionIzquierda(Nodo* x)
{
    Nodo* y = x->h_der;
    Nodo* t = y->h_izq;

    y->h_izq = x;
    x->h_der = t;

    actualizarAltura(x);
    actualizarAltura(y);

    return y;
}
//Rotacion doble izquierda-derecha (doble a la derecha)
Nodo* rotacionIzquierdaDerecha(Nodo* nodo)
{
    nodo->h_izq = rotacionIzquierda(nodo->h_izq);
    return rotacionDerecha(nodo);
}

//Rotacion doble derecha-izquierda (doble a la izquierda)
Nodo* rotacionDerechaIzquierda(Nodo* nodo)
{
    nodo->h_der = rotacionDerecha(nodo->h_der);
    return rotacionIzquierda(nodo);
}

Nodo* nuevoNodo(int valor)
{
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    nuevo->dato = valor;
    nuevo->h_izq = NULL;
    nuevo->h_der = NULL;
    nuevo->altura = 1;
    return nuevo;
}

Nodo* insertar(Nodo* nodo, int valor)
{
    if(nodo == NULL)
    {
        return nuevoNodo(valor);
    }

    if(valor < nodo->dato)
    {
        nodo->h_izq = insertar(nodo->h_izq, valor);
    }

    else if(valor > nodo->dato)
    {
        nodo->h_der = insertar(nodo->h_der, valor);
    }

    else
    {
        return nodo;
    }

    // Actualizamos la altura del nodo

    nodo->altura = 1 + max(altura(nodo->h_izq), altura(nodo->h_der));

    //Calcular factor balanceo:

    int balance = factorBalanceo(nodo);

    // Casos de desbalance (rotaciones)

    if(balance > 1 && valor < nodo->h_izq->dato) // caso izq-izq
    {
        return rotacionDerecha(nodo);
    }

    else if(balance < -1 && valor > nodo->h_der->dato)// caso der-der.
    {
        return rotacionIzquierda(nodo);
    }

    else if(balance > 1 && valor > nodo->h_izq->dato)//caso rotacion doble a la derecha
    {
        return rotacionIzquierdaDerecha(nodo);
    }

    else if(balance < -1 && valor < nodo->h_der->dato) //caso rotacion doble a la izq
    {
        return rotacionDerechaIzquierda(nodo);
    }

    return nodo;
}

Nodo* obtenerMinimo(Nodo* nodo)
{
    while(nodo->h_izq != NULL)
    {
        nodo = nodo->h_izq;
    }

    return nodo->dato;
}

Nodo* eliminar(Nodo* nodo, int valor)
{
    if(nodo == NULL)
    {
        return nodo;
    }

    else if(valor < nodo->dato)
    {
        nodo->h_izq = eliminar(nodo->h_izq, valor);
    }

    else if(valor > nodo->dato)
    {
        nodo->h_der = eliminar(nodo->h_der, valor);
    }

    else
    {
        //Nodo con 1 hijo.
        if(nodo->h_izq == NULL || nodo->h_der == NULL)
        {
            if(nodo->h_izq != NULL)
            {
                nodo = nodo->h_izq;
            }
            else if(nodo->h_der == NULL)
            {
                nodo = nodo->h_der;
            }
        }

        else
        {
            Nodo* sucesor = obtenerMinimo(nodo->h_der);
            nodo->dato = sucesor->dato;
            nodo->h_der = eliminar(nodo->h_der, sucesor->dato);
        }
    }

    if(nodo == NULL)
    {
        return nodo;
    }

    //Actualizar altura
    nodo->altura = 1 + max(altura(nodo->h_izq), altura(nodo->h_der));


    //Calcular factor balanceo:
    int balance = factorBalanceo(nodo);

    //Casos de desbalance

    if(balance > 1 && factorBalanceo(nodo->h_izq)>= 0)
    {
        return rotacionDerecha(nodo);
    }

    else if(balance > 1 && factorBalanceo(nodo->h_izq) < 0)
    {
        return rotacionIzquierdaDerecha(nodo);
    }

    else if(balance < -1 && factorBalanceo(nodo->h_der) <= 0)
    {
        return rotacionIzquierda(nodo);
    }

    else if(balance < -1 && factorBalanceo(nodo->h_der) > 0)
    {
        return rotacionDerechaIzquierda(nodo);
    }

    return nodo;
}

Cola*crearCola(int capacidad)
{
    Cola* cola = (Cola*)malloc(sizeof(Cola));
    cola->arreglo = (Nodo**)malloc(capacidad*sizeof(Nodo*));
    cola->frente = 0;
    cola->ultimo = -1;
    cola->capacidad = capacidad;
    return cola;
}

int colaVacia(Cola* cola)
{
    return cola->ultimo < cola->frente;
}

void encola(Cola* cola, Nodo* nodo)
{
    cola->ultimo++;
    cola->arreglo[cola->ultimo] = nodo;
}

Nodo* desencola(Cola* cola)
{
    Nodo* nodo = cola->arreglo[cola->frente];
    cola->frente++;
    return nodo;
}

void recorridoHorizontal(Nodo* nodo)
{
    if(nodo == NULL)
    {
        return;
    }

    Cola* cola = crearCola(1<<nodo->altura);
    encola(cola, nodo);

    while(!colaVacia(cola))
    {
        Nodo* nodoNuevo = desencola(cola);
        imprimir(nodoNuevo);

        if(nodoNuevo->h_izq != NULL)
        {
            encola(cola, nodoNuevo->h_izq);
        }

        if(nodoNuevo->h_der != NULL)
        {
            encola(cola, nodoNuevo->h_der);
        }
    }
}

void imprimir(Nodo* nodo)
{
    if(nodo != NULL)
    {
        printf("%d ", nodo->dato);
    }
}

void actualizarAltura(Nodo* nodo)
{
    nodo->altura = 1 + max(altura(nodo->h_izq), altura(nodo->h_der));
}

int max(int a, int b)
{
    if(a > b)
    {
        return a;
    }

    else
    {
        return b;
    }
}
