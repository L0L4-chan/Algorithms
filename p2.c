#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>
/* obtiene la hora actual en microsegundos */

#define ITERATIONS 6
#define K_VALUE 1000

#define UMBRAL 1

#define ORD_INS_MULT 2
#define ORD_RAPIDA_MULT 2
#define ORD_INS_START 500
#define ORD_RAPIDA_START 500


/* Struct de informacion de algoritmo:
 * Contiene:
 *  doAlgorithm -> funcion que ejecuta el algoritmo
 *  subestimate -> funcion de calculo para el valor subestimado
 *  adjusted -> funcion de calculo para el valor ajustado
 *  sobreestimate -> funcion de calculo para el valor sobreestimado
 */
typedef struct vectorOrderAlgorithm{
    void (*doAlgorithm)(int v[],int n);
    double (*subestimate)(double time,int n);
    double (*adjusted)(double time,int n);
    double (*sobreestimate)(double time,int n);
}vectorOrderAlgorithm;




void ascendente(int v[], int n);
void descendente(int v[], int n);
void aleatorio(int v[], int n);
bool is_ordered( int v[] , int n);
void print_vector(int v[], int n);
double microsegundos();
void inicializar_semilla();

void ord_ins (int v [], int n);
double ord_ins_subestimate (double time, int n);
double ord_ins_adjusted (double time, int n);
double ord_ins_sobreestimate (double time, int n);

void ord_rapida (int v [], int n);
void rapida_aux(int v[], int izq, int der);
void intercambiar(int *a, int *b);
double ord_rapida_subestimate (double time, int n);
double ord_rapida_adjusted (double time, int n);
double ord_rapida_sobreestimate (double time, int n);

void test();
void printData(double totalTime, int n, int k, double (*subestimate)(double,int)
        , double (*adjusted)(double,int), double (*sobreestimate)(double,int));
void measureVectorOderAlgorithm(int n, int mult,int iterations,
                                vectorOrderAlgorithm algo, void (*initVector)(int[],int));
void printHead();


double ord_ins_subestimate (double time, int n){return time/pow(n,1.8);}
double ord_ins_adjusted (double time, int n) {return time/ pow(n,2);}
double ord_ins_sobreestimate (double time, int n){return time/pow(n,2.2);}


double ord_rapida_subestimate (double time, int n){return time/pow(n,1.8);}
double ord_rapida_adjusted (double time, int n) {return time/ pow(n,2);}
double ord_rapida_sobreestimate (double time, int n){return time/pow(n,2.2);}


int main(){
    inicializar_semilla();
    //test();

    //Inicializar ordenacion por inserciom
    vectorOrderAlgorithm insertionOrder;
    insertionOrder.doAlgorithm=ord_ins;
    //Parametros de cotas
    insertionOrder.subestimate=ord_ins_subestimate;
    insertionOrder.adjusted=ord_ins_adjusted;
    insertionOrder.sobreestimate=ord_ins_sobreestimate;

    printf("\nAlgoritmo Insercion\n");
    printHead();
    measureVectorOderAlgorithm(ORD_INS_START,ORD_INS_MULT,ITERATIONS,insertionOrder,aleatorio);
    /**/


    //Inicializar quicksort
    vectorOrderAlgorithm quickSort;
    quickSort.doAlgorithm=ord_rapida;
    //Parametros de cotas
    quickSort.subestimate=ord_rapida_subestimate;
    quickSort.adjusted=ord_rapida_adjusted;
    quickSort.sobreestimate=ord_rapida_sobreestimate;
    //Test del algoritmo
    printHead();
    printf("\nAlgoritmo Quick Sort\n");
    measureVectorOderAlgorithm(ORD_RAPIDA_START,ORD_RAPIDA_MULT,ITERATIONS,quickSort,descendente);
    printHead();


    return 0;
    //TODO: check if descendente is valid peor caso

}











double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}
void inicializar_semilla() {
    srand(time(NULL));
}

void ascendente(int v [], int n) {
    int i;
    for (i=0; i < n; i++)
        v[i] = i;
}
void descendente(int v [], int n) {
    int i;
    for (i=0; i < n; i++)
        v[i] = n-i;
}
void aleatorio(int v [], int n) {/* se generan números pseudoaleatorio entre -n y +n */
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}

bool is_ordered(int v[] , int n){
    int i;
    for (i = 0; i<(n-1); i++) {
        if (v[i] > v[i + 1]) {
            return false;
        }
    }
    return true;
}

void print_vector(int v[] , int n){
    int i;
    for (i = 0; i < n; ++i) {
        printf("%d",v[i]);
    }
}



void ord_ins(int v[] , int n){
    int i,x,j;

    for (i=1;i<n;i++){
        x= v[i];
        j = i-1;
        while(j>=0 && v[j]>x){
            v[j+1] = v[j];
            j = j-1;
        }
        v[j+1]= x;
    }
}


//  funcion auxiliar de quicksort
void intercambiar(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}
// funcion auxiliar de quicksort
void rapida_aux(int v[], int izq, int der){//revisar los parametros por si deben ser calculados dentro
    int x, pivote, i, j ;
    if (izq + UMBRAL <= der){
        x = (rand() % (der- izq +1)) + izq ;//confirmar que la llamada a randon en en rango de izq a der es correcta
        pivote = v[x];
        intercambiar(&v[izq], &v[x]);
        i = izq +1;
        j = der;
        while(i<=j){
            while(i<= der && v[i]< pivote){
                i = i+1;
            }
            while(v[j]> pivote){
                j = j-1;
            }
            if(i<=j){
                intercambiar(&v[i], &v[j]);
                i = i+1;
                j = j-1;
            }
        }
        intercambiar(&v[izq], &v[j]);
        rapida_aux(v, izq, (j-1) );
        rapida_aux(v,j+1,der );
    }
}

/**
 * Algoritmo de ordenación rapida
 * @param v vector
 * @param n tamaño del vector o posición final
 */

void ord_rapida(int v [], int n) {// quicksort
    rapida_aux(v, 0, n - 1); // funcion auxiliar
    if (UMBRAL >1){ // umbral constante con la que debemos experimentar entre valores, los optimos se encuentran entre 12 y 15
        ord_ins(v,n);// se llama finalmente al algoritmo de ordenacion por inserción, una vez que el vector esta casi ordenado(mejor caso para ond_inv)
    }
}



/** Print Data comun:
 * @param totalTime tiempo total usado para la aplicacion algoritmo
 * @param n  numero de iteraciones
 * @param k  numero de repeticiones de la aplicacion del algoritmo con el
 * mismo n hasta obtener un tiempo valido
 * @param subestimate  funcion para calcular la cota subestimada
 * @param adjusted  funcion para calcular la cota ajustada
 * @param sobreestimate  funcion para calcular la cota sobreestimada
 */
void printData(double totalTime, int n, int k, double (*subestimate)(double,int)
        , double (*adjusted)(double,int), double (*sobreestimate)(double,int))
{
    double x,y,z,iterationTime;
    if(k==0){
        iterationTime=totalTime;
    }else
    {
        iterationTime=totalTime/((double )k);
    }
    x=/*iterationTime/ pow(n,0.5);*/(*subestimate)(iterationTime,n);
    y=/*iterationTime/ n;*/(*adjusted)(iterationTime,n);
    z=/*iterationTime/ pow(n,1.2);*/(*sobreestimate)(iterationTime,n);
    printf("%12d%15.3f%15.6f%15.6f%15.8f%12d%20.10f\n", n, totalTime, x, y, z, k, iterationTime);// imprimimos por pantalla
}




/** Print Data comun:
 *  @param init  inicio del valor n (tamaño de vector)
 *  @param mult  multiplicidad de n
 *  @param iterations  numero de veces que se hace la medicion
 *  @param algo  informacion del algoritmo
 *  @param initVector  funcion usada para rellenar el vector de valores
 */

void measureVectorOderAlgorithm(int init, int mult,int iterations,
                                vectorOrderAlgorithm algo, void (*initVector)(int[],int)) { // funcion para la medicion del tiempo
    double t1, t2, time;// variables locales
    int k,i,j;
    int vector[(init*(int)pow(mult,iterations-1))];
    k=0;// inicializamos la variable local K
    for (i = 0; i < iterations; i++) { //bucle
        initVector(vector, init);
        t1 = microsegundos();// medimos el tiempo
        algo.doAlgorithm(vector, init);
        t2 = microsegundos();// medimos el tiempo
        //Calculatamos el tiempo real, que sera la diferencia entre la segunda medida de tiempo y la primera
        time = t2 - t1;
        if (time < 500) {//comporbamos si la medicion es valida
            while (time < 500) {//mientras el tiempo obtenido no se valido repetiremos este bucle
                k = k == 0 ? K_VALUE : k * 10;//Incrementamos o iniciamos K
                t1 = microsegundos();// medimos el tiempo
                for (j = 0; j < k; j++) {//bucle que se repetira k veces para obtener un tiempo valido
                    initVector(vector, init);
                    algo.doAlgorithm(vector, init);
                }
                t2 = microsegundos();//medimos el tiempo
                time = t2 - t1;//Calculatamos el tiempo real, que sera la diferencia entre la segunda medida de tiempo y la primera
                t1 = microsegundos();//medimos el tiempo
                for (j = 0; j < k; j++) {
                    initVector(vector, init);
                }
                t2 = microsegundos();//medimos el tiempo
                time = time - (t2 - t1);
            }
        }
        printData(time, init, k, algo.subestimate, algo.adjusted,algo.sobreestimate);// llamamos a la funcion de impresion de datos
        k = 0; //ponemos k a cero para la siguiente vuelta
        init = init * mult; //Incrementamos el indice
    }
}


void printHead()
{
    printf("\n   Parametro      T. medido      C. subest      C. teoric      C. sobree           K        T'= T/K\n");
}


void test()
{
    int l = 10;
    int v[l];
    printf("Test ascendente\n");
    ascendente(v,l);
    for(int i = 0; i<l;i++) printf("%d , ",v[i]);

    printf("\nTest order, asc\n");
    printf("is order: %d\n", is_ordered(v,l));

    printf("Test descendente\n");
    descendente(v,l);
    for(int i = 0; i<l;i++) printf("%d , ",v[i]);

    printf("\nTest order, desc\n");
    printf("is order: %d\n", is_ordered(v,l));


    printf("Test random\n");
    aleatorio(v,l);
    for(int i = 0; i<l;i++) printf("%d , ",v[i]);
    printf("\nTest order, rand\n");
    printf("is order: %d\n", is_ordered(v,l));


    printf("Test insercion\n");
    descendente(v,l);
    ord_ins(v,l);
    printf("is order: %d\n", is_ordered(v,l));



    printf("Test quick\n");
    descendente(v,l);
    ord_rapida(v,l);
    printf("is order: %d\n", is_ordered(v,l));
}