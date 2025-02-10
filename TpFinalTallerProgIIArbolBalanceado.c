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
Nodo* nuevoNodo(int valor); //es puntero a nodo xq le estoy pasando un nuevo valor, pero se lo devuelvo con una direccion.
int obtenerMinimo(Nodo* nodo);
int eliminar(Nodo* nodo, int valor);
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
    Nodo* raiz = NULL; // esta es la raiz de mi arbol, cada vez q inserto un numero se actualiza ese puntero segun el resutlado de la insercion.
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
            printf("Arbol AVL vacio. No se puede eliminar un elemento. \n");
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
            printf("\nAltura actual del Arbol AVL: %d", altura(raiz));
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

int altura(Nodo* nodo) // va a devolver un entero con la altura del nodo.
{
    if (nodo == NULL ) // si el nodo es NULL, quiere decir que no hay un nodo (arbol vacio o raiz de un arbol vac�o).
    {
        return 0; // entonces retorna la altura, que va a ser 0
    }
    else
    {
        return nodo->altura; // en caso de que no, simplemente devuelve la altura que est� almacenada en el nodo mismo.
    }
}

int factorBalanceo(Nodo* nodo)
{
    if(nodo == NULL)
    {
        return 0; // si el nodo es vacio, el balanceo es 0.
    }

    else
    {
        return (altura(nodo->h_izq) - altura(nodo->h_der)); // return para devolver el resultado de la resta de la altura del hijo izquierdo menos el derecho.
    }
}

// rotacion simple a la derecha
Nodo* rotacionDerecha(Nodo* y) // toma puntero a un nodo Y (raiz del subarbol) y devuelve un puntero al nuevo nodo que ser� la ra�z del subarbol despues de realizar la rotacion. Es de tipo *nodo ya que la raiz del subarbol puede cambiar durante la rotacion.
{
    Nodo* x = y->h_izq; // variable auxiliar para no perder referencia.
    Nodo* t = x->h_der; // variable temporal para hacer el cambio.

    x->h_der = y; // x se hace raiz del subarbol, su hijo derecho para a ser y.
    y->h_izq = t; // hay q actualizar el hijo izquierdo de Y (que era donde apuntaba antes). ahora apunta a t, que era el hijo derecho original de X.
                  // O sea, ese hijo izquierdo q no "apunta" a nada, hacemos q apunte a T como h.izq.

    actualizarAltura(y);
    actualizarAltura(x);

    return x; // Ahora `x` es la nueva ra�z del sub�rbol
}

//rotacion simple a la izquierda

Nodo* rotacionIzquierda(Nodo* x)
{
    Nodo* y = x->h_der;
    Nodo* t = y->h_izq;

    y->h_izq = x; //pasa a ser raiz del subarbol, X es el hijo izquierdo ahora.
    x->h_der = t; // apunto el hijo derecho a la variable temporal que antes apuntaba al hijo izq de Y.

    actualizarAltura(x);
    actualizarAltura(y);

    return y; //retornas Y como la nueva raiz del arbol.
}
//Rotacion doble izquierda-derecha (doble a la derecha)
Nodo* rotacionIzquierdaDerecha(Nodo* nodo)
{
    nodo->h_izq = rotacionIzquierda(nodo->h_izq); //rotacion izquierda sobre el hijo izquierdo. Aca queda Y arriba y X a la izquierda, la temporal a la derecha.
    return rotacionDerecha(nodo);// rotacion derecha sobre el nodo original. Aca el nodo pasa a ser la nueva Y, y el hijo izquierdo la nueva X. Por eso despues vos rotas el nodo origial hacia la derecha y subis el hijo izquierdo como nueva raiz. Por que se actualizan las alturas.
}

//Rotacion doble derecha-izquierda (doble a la izquierda)
Nodo* rotacionDerechaIzquierda(Nodo* nodo)
{
    nodo->h_der = rotacionDerecha(nodo->h_der);//rotacion derecha sobre hijo derecho. Aca queda X arriba e Y a la derecha, T a la izquierda.
    return rotacionIzquierda(nodo);//rotacion izquierda sobre el nodo original. Aca despues se actualiza y el nodo original es X y el hijo derecho Y. Por eso despues rotas el nodo original hacia la izquierda y subis el hijo derecho.
}

Nodo* nuevoNodo(int valor) // le tenes que dar una direccion y un valor al nuevo nodo
{
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo)); //asigno la memoria para un Nodo nuevo.
    nuevo->dato = valor; //el dato del nodo se lo asignas ahora al valor del nodo.
    nuevo->h_izq = NULL;
    nuevo->h_der = NULL; // inicializo ambos hijos en null.
    nuevo->altura = 1; // inicializo la altura en 1.
    return nuevo;// retornas el nodo nuevo.
}

Nodo* insertar(Nodo* nodo, int valor) // tenes que insertar el nodo con su  direccion y su valor
{
    if(nodo == NULL)
    {
        return nuevoNodo(valor); // si el arbol esta vacio, creo un nuevo nodo raiz.
    }

    if(valor < nodo->dato) // si el valor es menor al dato, lo insertamos en el subarbol izq.
    {
        nodo->h_izq = insertar(nodo->h_izq, valor);
    }

    else if(valor > nodo->dato)// si es mayor, en el derecho
    {
        nodo->h_der = insertar(nodo->h_der, valor);
    }

    else // estamos queriendo insertar un valor duplicado (mismo valor) y no se permiten en este arbol. Retornamos el nodo actual sin hacer nada.
    {
        return nodo;
    }

    // Actualizamos la altura del nodo

    nodo->altura = 1 + max(altura(nodo->h_izq), altura(nodo->h_der)); //sacas el maximo entre ambos subarboles y se le suma 1.

    //Calcular factor balanceo:

    int balance = factorBalanceo(nodo);

    // Casos de desbalance (rotaciones)

    if(balance > 1 && valor < nodo->h_izq->dato) // se inserta un nodo en el subarbol izquierdo del hijo izquierdo.
    {
        return rotacionDerecha(nodo);
    }

    else if(balance < -1 && valor > nodo->h_der->dato)//se inserta un nodo en el subarbol derecho del hijo derecho.
    {
        return rotacionIzquierda(nodo);
    }

    else if(balance > 1 && valor > nodo->h_izq->dato)//se inserta un nodo en el subarbol derecho del hijo izquierdo.
    {
        return rotacionIzquierdaDerecha(nodo);
    }

    else if(balance < -1 && valor < nodo->h_der->dato) //se inserta un nodo en el subarbol izquierdo del hijo derecho.
    {
        return rotacionDerechaIzquierda(nodo);
    }

    return nodo;
}

int obtenerMinimo(Nodo* nodo)
{
    while(nodo->h_izq != NULL) // mientras no encontremos el valor minimo
    {
        nodo = nodo->h_izq; // nos movemos al hijo izquierdo
    }

    return nodo->dato; // ya encontrado (cuando es = NULL), devolvemos el nodo con valor minimo
}

int eliminar(Nodo* nodo, int valor)
{
    if(nodo == NULL)
    {
        return nodo; // si el arbol esta vacio, no elimina nada. Retorna el nodo original o NULL en este caso.
    }

    else if(valor < nodo->dato) // cuando el valor es menor que el valor del nodo actual, esta en el subarbol izq.
    {
        nodo->h_izq = eliminar(nodo->h_izq, valor);
    }

    else if(valor > nodo->dato)
    {
        nodo->h_der = eliminar(nodo->h_der, valor);
    }

    else
    {
        //Caso de que tenga un hijo.
        if(nodo->h_izq == NULL || nodo->h_der == NULL)
        {
            if(nodo->h_izq != NULL)
            {
                nodo = nodo->h_izq;// si tiene hijo izquierdo, lo sustituye.
            }
            else if(nodo->h_der == NULL)
            {
                nodo = nodo->h_der;// si tiene hijo derecho, lo sustituye.
            }
        }

        else
        {
            Nodo* sucesor = obtenerMinimo(nodo->h_der);
            nodo->dato = sucesor->dato; //Una vez que encontramos el sucesor, tomamos el valor de ese nodo y lo copiamos al nodo que estamos eliminando. De esta manera, el nodo que estamos eliminando es reemplazado por el valor del sucesor.
            nodo->h_der = eliminar(nodo->h_der, sucesor->dato); //Ahora, necesitamos eliminar el sucesor en el sub�rbol derecho, ya que hemos copiado su valor al nodo a eliminar. Es importante que eliminemos el sucesor, ya que ha sido copiado al nodo original, pero a�n sigue existiendo en el �rbol.
        }
    }

    if(nodo == NULL)
    {
        return nodo;
    }

    //Actualizar altura
    nodo->altura = 1 + max(altura(nodo->h_izq), altura(nodo->h_der)); //sacas el maximo entre ambos subarboles y se le suma 1.


    //Calcular factor balanceo:
    int balance = factorBalanceo(nodo);

    //Casos de desbalance

    if(balance > 1 && factorBalanceo(nodo->h_izq)>= 0) // eliminamos un nodo en el subarbol derecho de h_izq. Se usa cuando el subarbol izquierdo es mas alto y su hijo izquierdo tambien.
    {
        return rotacionDerecha(nodo);
    }

    else if(balance > 1 && factorBalanceo(nodo->h_izq) < 0)// eliminamos un nodo en el subarbol derecho de h_izq. Se usa cuando el subarbol izquierdo es mas alto, pero su hijo derecho es mas alto.
    {
        return rotacionIzquierdaDerecha(nodo);
    }

    else if(balance < -1 && factorBalanceo(nodo->h_der) <= 0)//eliminamos un nodo en el subarbol izquierdo de h_der. Se usa cuando el subarbol derecho es mas alto y su hijo derecho tambien.
    {
        return rotacionIzquierda(nodo);
    }

    else if(balance < -1 && factorBalanceo(nodo->h_der) > 0)// eliminamos un nodo en el subarbol izquierdo de h_der. Se usa cuando el subarbol derecho es mas alto, pero su hijo izquierdo es mas alto.
    {
        return rotacionDerechaIzquierda(nodo);
    }

    return nodo;
}

Cola*crearCola(int capacidad)
{
    Cola* cola = (Cola*)malloc(sizeof(Cola)); //asignamos memoria a la cola
    cola->arreglo = (Nodo**)malloc(capacidad*sizeof(Nodo*));//reservando memoria para un arreglo de tama�o capacidad (int) donde cada elemento es un puntero a Nodo. Solo contiene direcciones de memoria, no el nodo en si.
    cola->frente = 0;// es el indice donde se va a colocar el primer elemento de la cola. Se�ala el primer elemento que se va a sacar.
    cola->ultimo = -1; // es la ultima posicion ocupada en la cola, lugar donde se agregara el primer elemento. Se�ala que el primer elemento se encuentra en la posicion 0 del arreglo (xq cuando se inserte un elemento pasa de -1 a 0).
    cola->capacidad = capacidad;// asignamos el parametro capacidad al campo capacidad de la estructura cola, garantizando q tenga un limite en la cantidad de elementos que puede almacenar (tamb para verificar si esta llena o vacia).
    return cola;
}

int colaVacia(Cola* cola)
{
    return cola->ultimo < cola->frente;// comparas el ultimo valor con el valor frente. Si ultimo es menor a frente, la cola va a estar vacia. Lo contrario, esta llena. Retorna 1 si esta vacia, 0 si no.
}

void encola(Cola* cola, Nodo* nodo)
{
    cola->ultimo++;//Incrementas ultimo para apuntar al siguiente espacio
    cola->arreglo[cola->ultimo] = nodo;// Asignas el puntero al nodo en la ultima posicion, o sea nodo es igual a la ultima posicion del arreglo de la cola.
}

Nodo* desencola(Cola* cola)
{
    Nodo* nodo = cola->arreglo[cola->frente];//Obtiene el nodo que est� en el frente de la cola
    cola->frente++;//Incrementa el �ndice `frente` para apuntar al siguiente nodo
    return nodo; //Devuelve el nodo desencolado
}

void recorridoHorizontal(Nodo* nodo)
{
    if(nodo == NULL)
    {
        return;
    }

    Cola* cola = crearCola(1<<nodo->altura); // creas una nueva cola.
    encola(cola, nodo);// encolas/insertas el nodo raiz

    while(!colaVacia(cola))// mientras la cola no este vacia:
    {
        Nodo* nodoNuevo = desencola(cola);// desencolas el siguiente nodo
        imprimir(nodoNuevo);

        if(nodoNuevo->h_izq != NULL)
        {
            encola(cola, nodoNuevo->h_izq);// si tiene hijo izquierdo, lo encolas
        }

        if(nodoNuevo->h_der != NULL)
        {
            encola(cola, nodoNuevo->h_der); // si tiene hijo derecho, lo encolas tambien
        }
    } // imprimis el nodo desencolado y luego verificas sus hijos para agregar a la cola, si es que existen. Xq vos sacas 1 no mas, si tiene hijos hay q agregarlos a la cola, no pueden quedar ahi en el aire.
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
