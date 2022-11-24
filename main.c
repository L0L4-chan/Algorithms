#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#define K_VALUE 1000




///ARBOL -----------------------------------------------------------------------------------------




struct nodo {
    int elem;
    int num_repeticiones;
    struct nodo *izq, *der;
};
typedef struct nodo *posicion;
typedef struct nodo *arbol;

arbol creararbol(); /* devuelve un arbol vacio */
static struct nodo *crearnodo(int e);
arbol insertar(int e, arbol a);
int esarbolvacio(arbol);
posicion buscar(int, arbol);
arbol eliminararbol(arbol); /* borra todos los nodos liberando la
memoria y devolviendo un arbol vacio */
posicion hijoizquierdo(arbol);
posicion hijoderecho(arbol);
int elemento(posicion);
int numerorepeticiones(posicion);
int altura(arbol);
void visualizar(arbol); /* imprime el contenido del arbol */

bool tiene_hijo_izquierdo(arbol a);
bool tiene_hijo_derecho(arbol a);


static struct nodo *crearnodo(int e) {
    struct nodo *p = malloc(sizeof(struct nodo));
    if (p == NULL) {
        printf("memoria agotada\n"); exit(EXIT_FAILURE);
    }
    p->elem = e;
    p->num_repeticiones = 1;
    p->izq = NULL;
    p->der = NULL;
    return p;
}

arbol insertar(int e, arbol a){
    if (a == NULL)
        return crearnodo(e);
    else if (e < a->elem)
        a->izq = insertar(e, a->izq);
    else if (e > a->elem)
        a->der = insertar(e, a->der);
    else
        a->num_repeticiones++;
    return a;
}


arbol creararbol(){
    return NULL;
}
int esarbolvacio(arbol a){
        return a==NULL;
}

posicion buscar(int e,arbol a){
    posicion pos = NULL;
    if(a==NULL)printf("NULLL");
    if (e < a->elem && tiene_hijo_izquierdo(a))
        pos = buscar(e, hijoizquierdo(a));
    else if (e > a->elem && tiene_hijo_derecho(a))
        pos = buscar(e, hijoderecho(a));
    else if(e==a->elem)
        pos=a;
    return pos;
}

arbol eliminararbol(arbol a){
    if (tiene_hijo_izquierdo(a)==true)
        eliminararbol(hijoizquierdo(a));
    if (tiene_hijo_derecho(a)==true)
        eliminararbol(hijoderecho(a));
    free(a);
    return NULL;
}

posicion hijoizquierdo(arbol a){
    return  a->izq;
}
posicion hijoderecho(arbol a){
    return a->der;
}
bool tiene_hijo_izquierdo(arbol a){
    return(hijoizquierdo(a) !=NULL);
}
bool tiene_hijo_derecho(arbol a){
    return(hijoderecho(a) !=NULL);
}

int elemento(posicion p){
    return p->elem;
}
int numerorepeticiones(posicion p){
    return p->num_repeticiones;
}

void visualizar(arbol a){
    if (a != NULL){
        if (tiene_hijo_izquierdo(a)==true){
            printf("(");
            visualizar(a->izq);
            printf(")");
        }
        printf("%d",a->elem);
        if (tiene_hijo_derecho(a)==true){
            printf("(");
            visualizar(a->der);
            printf(")");
        }
    } else
        printf("()");
}

int altura(arbol a){
    if (esarbolvacio(a)){return -1;}
    else{
        int hlc = -1;
        if (tiene_hijo_izquierdo(a)==true) {hlc = altura(a->izq);}
        int hrc = -1;
        if (tiene_hijo_derecho(a)==true) {hrc = altura(a->der);}
        return 1 + ((hlc > hrc)?hlc:hrc);
    }
}


///END ARBOL -----------------------------------------------------------------------------------------





/** Struct de información de algoritmo:
 *  @param doAlgorithm Función que ejecuta el algoritmo
 *  @param subestimate Función de calculo para el valor subestimado
 *  @param adjusted Función de calculo para el valor ajustado
 *  @param sobreestimate Función de calculo para el valor sobreestimado
 */
typedef struct timeProcessFuncions{
    double (*equation)(double time, int n);
}timeProcessFuncions;
typedef struct datacollection{
    double time[2][10];
    int k[2][10];
}datacollection;

//Definiciones generales
void fill_random_vector(int v [], int n);
double get_time_ms();
void inicializar_semilla();




//Definiciones genéricas
void test();
void print_data(double totalTime,int n,int k,double (*pFunction)(double, int));
datacollection measure_tree(int n, int mult, int iterations);
void print_head();
void show(datacollection data,int iterations, int n,timeProcessFuncions tPF[]);





/// researchFuncions
double constant (double time, int n){return time/pow(n,0.99);}
double logn (double time, int n){return time/pow(n,1.09);}
double expn (double time, int n){return time/pow(n,1.25);}

void test() {
    printf("Prueba arbol a");
    arbol a =  creararbol( );
    printf("Esta el arbol vacio: %d \n", esarbolvacio(a));
    printf(" %d\n" , altura(a));
    a = insertar(8,a);
    a = insertar(4,a);
    a = insertar(1,a);
    a = insertar(9,a);
    a = insertar(10,a);
    a = insertar(13,a);
    printf("Esta el arbol vacio: %d\n", esarbolvacio(a));
    printf("Altura %d\n" , altura(a));
    visualizar(a);
    printf("\n");
    printf("elemento en el hijo derecho es %d\n",elemento(hijoderecho(a)));
    printf("elemento en el hijo izquierdo es %d\n",elemento(hijoizquierdo(a)));
    a = eliminararbol(a);
    printf("Esta el arbol vacio: %d \n", esarbolvacio(a));
    printf("Prueba arbol b");
    arbol b = creararbol( );
    b = insertar(3,b);
    b = insertar(1,b);
    b = insertar(2,b);
    b = insertar(3,b);
    b = insertar(5,b);
    printf("Esta el arbol vacio: %d\n", esarbolvacio(b));
    visualizar(b);
    printf("\n");
    printf("Altura %d\n" , altura(a));
    printf("El elemento %d",elemento(buscar(5,b)));
    printf(" tiene %d repeticiones\n" , numerorepeticiones(buscar(5,b)));
    printf("El elemento %d",elemento(buscar(3,b)));
    printf(" tiene %d repeticiones\n" , numerorepeticiones(buscar(3,b)));
    printf("elemento en el hijo derecho es %d\n",elemento(hijoderecho(b)));
    printf("elemento en el hijo izquierdo es %d\n",elemento(hijoizquierdo(b)));
    b = eliminararbol(b);
    printf("Esta el arbol vacio: %d \n", esarbolvacio(b));
}

int main(){

    //test();
    //return 1;

    inicializar_semilla();
    //test();

    double (*researchFuncions[])(double, int)  = {constant,logn,expn,NULL};
    datacollection  data = measure_tree(1000,2,10);

    timeProcessFuncions tpfs[2];
    for(int i=0; researchFuncions[i]!=NULL;i++){
        printf("Mediciones %d",i+1);
        printf("------------------------------------------------------");
        //Cotas insercion
        tpfs[0].equation=researchFuncions[i];
        //Cotas Busqueda
        tpfs[1].equation=researchFuncions[i];
        show(data,10,1000,tpfs);

    }
}


/**
 * Funcion utilizada para medir el tiempo
 * @return Tiempo actual del sistema
 */
double get_time_ms() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}
/**
 * Funcion para inicializar una semilla en base al tiempo,
 * debe llamarse en el main
 */
void inicializar_semilla() {
    srand(time(NULL));
}
/**
 * Funcion para rellenar un vector con numeros aleatorios entre -n y +n
 * @param v Vector
 * @param n Tamaño del vector
 */
void fill_random_vector(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}
/** Print Data comun:
 * @param totalTime Tiempo total usado para la aplicacion algoritmo
 * @param n  Número de iteraciones
 * @param k  Número de repeticiones de la aplicacion del algoritmo con el
 * mismo n hasta obtener un tiempo valido
 * @param subestimate  Función para calcular la cota subestimada
 * @param adjusted  Función para calcular la cota ajustada
 * @param sobreestimate  Función para calcular la cota sobreestimada
 */
void print_data(double totalTime, int n, int k,
                double (*subestimate)(double, int))
{
    double x,iterationTime;
    if(k==0){
        iterationTime=totalTime;
    }else
    {
        iterationTime=totalTime/((double )k);
    }
    x=(*subestimate)(iterationTime,n);
    printf("%12d%15.3f%15.10f%12d%20.10f\n",n,totalTime,x,k,iterationTime);// imprimimos por pantalla
}
/** Función genérica para medir el tiempo de un algoritmo de ordenación de vectores
 *  @param n  Inicio del valor n (tamaño de vector)
 *  @param mult  Multiplicidad de n
 *  @param iterations  Numero de veces que se hace la medición
 *  @param algo  Información del algoritmo
 *  @param initVector  Función usada para rellenar el vector de valores
 */
datacollection measure_tree(const int n,int mult,int iterations){
    double tstart, tfinish, timeIns[iterations], timeSearch[iterations];// variables locales
    int kIns[iterations],kSearch[iterations], nW=n,h,i,j,t;
    arbol tree;
    arbol (*treeFuncions[])(int, arbol)  = {insertar, buscar,NULL};
    datacollection data;
    int vector[(n * (int)pow(mult, iterations - 1))];//Se inicializa el vector a su tamaño maximo
    for (i = 0; i < iterations; i++) {//TODO: time limit
        tree=creararbol();//arbol init
        for(h=0; treeFuncions[h]!=NULL;h++) {
            data.k[h][i] = 0;
            fill_random_vector(vector, nW);
            tstart = get_time_ms();
            for (j = 0; j < nW; j++)
                if(h==0){
                    tree = treeFuncions[h](vector[j], tree);
                }else
                {
                    treeFuncions[h](vector[j],tree);
                }
            tfinish = get_time_ms();
            data.time[h][i] = tfinish - tstart;//Calculatamos el tiempo
            if (data.time[h][i] < 500) { //Comprobación de si el tiempo es suficiente para una medicion valida
                while (data.time[h][i] < 500) {
                    data.k[h][i]=data.k[h][i]==0 ? K_VALUE:data.k[h][i] * 10;//Incrementamos o iniciamos K
                    tstart = get_time_ms();
                    for (t = 0; t < data.k[h][i]; t++) {
                        fill_random_vector(vector, nW);
                        for (j = 0; j < nW; j++)
                            if(h==0){
                                tree = treeFuncions[h](vector[j], tree);
                            }else
                            {
                                treeFuncions[h](vector[j],tree);
                            }
                    }
                    tfinish = get_time_ms();
                    data.time[h][i] = tfinish - tstart;
                    tstart = get_time_ms();
                    for (j = 0; j < data.k[h][i]; j++) {
                        fill_random_vector(vector, nW);
                    }
                    tfinish = get_time_ms();
                    data.time[h][i] = data.time[h][i] - (tfinish - tstart);
                }
            }
        }
        nW = nW * mult;
        tree=eliminararbol(tree); //arbol close
    }

return data;
}

void show(datacollection data,int iterations, int n,timeProcessFuncions tPF[]){
    printf("\n           n       t_ins(n)      t_busc(n)\n");
    int nW,i,h;
    arbol (*treeFuncions[])(int, arbol)  = {insertar, buscar,NULL};
    nW=n;
    for (i = 0; i < iterations; i++) {
        printf("%12d%15.3f%15.6f\n", nW,
               data.k[0][i]==0?data.time[0][i]:data.time[0][i]/((double)data.k[0][i]),
               data.k[1][i]==0?data.time[1][i]:data.time[1][i]/((double)data.k[1][i]));// imprimimos por pantalla
        nW=nW*2;
    }
    for(h=0; treeFuncions[h]!=NULL;h++){
        nW=n; //Reset de n
        print_head();
        for(i=0;i<iterations;i++){
            print_data(data.time[h][i], nW, data.k[h][i], tPF[h].equation);
            nW=nW*2;
        }
    }
}
/**
 * Funcion que imprime por pantalla una cabecera preestablecida para el grafico del tiempo
 */
void print_head()
{
    printf("\n   Parametro      T. medido   C. calculada           K        T'= T/K\n");
}
