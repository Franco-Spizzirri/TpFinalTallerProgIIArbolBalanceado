#include <stdio.h>
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
    int cont;
    int numero;
    char respuesta;

    printf("Cuantos numeros queres ingresar?:\n ");
    scanf("%d", &cont);
    getchar();

    for(int i=0; i<cont; i++)
    {
       printf("Ingrese un valor para el Arbol AVL:\n");
       scanf("%d", &numero);
       getchar();
       raiz = insertar(raiz, numero); // Llamamos a la función insertar y actualizamos la raíz
    }

printf("Recorrido en nivel del Arbol AVL: \n");
recorridoHorizontal(raiz);

printf("\nQueres agregar un elemento mas al arbol? toca(s/n):\n");
respuesta = getchar();

if(respuesta == 's' || respuesta == 'S')
{
    printf("Ingrese un valor para el Arbol AVL:\n");
    scanf("%d", &numero);
    raiz = insertar(raiz, numero); // Llamamos a la función insertar y actualizamos la raíz
    getchar();
    printf("Recorrido en nivel del Arbol AVL: \n");
    recorridoHorizontal(raiz);
}

else if(respuesta == 'n' || respuesta == 'N')
{
    printf("Fin del programa.\n");
    return 0;
}

else
{
    printf("Respuesta no valida. Ingresa 's' para si o 'n' para no\n");
}

printf("\nQueres eliminar un elemento del arbol? toca(s/n)\n");
scanf("%c", &respuesta);

if(respuesta == 's' || respuesta == 'S')
{
    printf("Que elemento deseas eliminar?:\n");
    scanf("%d", &numero);

    raiz = eliminar(raiz, numero); // Llamamos a la función insertar y actualizamos la raíz

    printf("Recorrido en nivel del Arbol AVL despues de eliminar %d: \n", numero);
    recorridoHorizontal(raiz);
}
else if(respuesta == 'n' || respuesta == 'N')
{
    printf("No se elimino ningun elemento.\n");
    return 0;
}

else
{
    printf("Respuesta no valida. Ingresa 's' para si o 'n' para no\n");
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
        return (altura(nodo->h_izq) - altura(nodo->h_der)); // return para devolver el resultado de las alturas.
    }
}


// rotacion simple a la derecha
Nodo* rotacionDerecha(Nodo* y)
{
    Nodo* x = y->h_izq; // variable auxiliar para no perder referencia.
    Nodo* t = x->h_der; // variable temporal para hacer el cambio.

    x->h_der = y; // x se hace raiz del subarbol, su hijo derecho para a ser y.
    y->h_izq = t; // hay q actualizar el hijo izquierdo de Y (que era donde apuntaba antes). ahora apunta a t, que era el hijo derecho original de X.
                  // O sea, ese hijo izquierdo q no "apunta" a nada, hacemos q apunte a T como h.der.

    actualizarAltura(y);
    actualizarAltura(x);

    return x; // Ahora `x` es la nueva raíz del subárbol
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

Nodo* nuevoNodo(int valor) // le tenes que dar una direccion y un valor al nuevo nodo
{
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo)); //asigno la memoria para un Nodo.
    nuevo->dato = valor; //le asigno un valor a ese nodo.
    nuevo->h_izq = NULL;
    nuevo->h_der = NULL; // inicializo ambos hijos en null.
    nuevo->altura = 1; // inicializo la altura en 1.
    return nuevo;
}

Nodo* insertar(Nodo* nodo, int valor)
{
    if(nodo == NULL)
    {
        return nuevoNodo(valor);
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

    else if(balance < -1 && valor < nodo->h_der->dato)
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

    return nodo->dato; // ya encontrado, devolvemos el nodo con valor minimo
}

int eliminar(Nodo* nodo, int valor)
{
    if(nodo == NULL)
    {
        return nodo;
    }

    else if(valor < nodo->dato) // cuando el valor es menor que ek valor del nodo actual, esta en el subarbol izq.
    {
        nodo->h_izq = eliminar(nodo->h_izq, valor);
    }

    else if(valor > nodo->dato)
    {
        nodo->h_der = eliminar(nodo->h_der, valor);
    }

    else
    {
        //Nodo no encontrado
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
            nodo->dato = sucesor->dato; //Una vez que encontramos el sucesor, tomamos el valor de ese nodo y lo copiamos al nodo que estamos eliminando. De esta manera, el nodo que estamos eliminando es reemplazado por el valor del sucesor.
            nodo->h_der = eliminar(nodo->h_der, sucesor->dato); //Ahora, necesitamos eliminar el sucesor en el subárbol derecho, ya que hemos copiado su valor al nodo a eliminar. Es importante que eliminemos el sucesor, ya que ha sido copiado al nodo original, pero aún sigue existiendo en el árbol.
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
    Cola* cola = (Cola*)malloc(sizeof(Cola)); //asignamos memoria a la cola
    cola->arreglo = (Nodo**)malloc(capacidad*sizeof(Nodo*));//reservando memoria para un arreglo de tamaño capacidad (int) donde cada elemento es un puntero a Nodo. Solo contiene direcciones de memoria, no el nodo en si.
    cola->frente = 0;// es el indice donde se va a colocar el primer elemento de la cola. Señala el primer elemento que se va a sacar.
    cola->ultimo = -1; // es la ultima posicion ocupada en la cola, lugar donde se agregara el primer elemento. Señala que el primer elemento se encuentra en la posicion 0 del arreglo (xq cuando se inserte un elemento pasa de -1 a 0).
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
    Nodo* nodo = cola->arreglo[cola->frente];//Obtiene el nodo que está en el frente de la cola
    cola->frente++;//Incrementa el índice `frente` para apuntar al siguiente nodo
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
