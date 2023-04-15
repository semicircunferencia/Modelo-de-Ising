/*
MODELO DE ISING

Tenemos una red de NxN espines, pudiendo valer cada uno 1 ó -1. Cada espín interactúa con los 4 más próximos de
alrededor. Concretamente, si un par de espines contiguos son paralelos, la energía disminuye una unidad. Si son
antiparalelos, aumenta una unidad. De este modo, la configuración más estable si la temperatura es 0
consiste en todos los espines iguales.

¿Cómo averiguamos la evolución del conjunto de espines? Con un algoritmo de Metrópolis:

0) Dar una temperatura entre 0 y 5. Generar una configuración inicial de espines, quizá aleatoriamente
o todos iguales

1) Elegir un punto al azar de la red (n,m)

2) Evaluar p=min(1, exp(DeltaE/T)), siendo DeltaE=-2*s(n,m)[s(n+1,m)+s(n-1,m)+s(n,m+1)+s(n,m-1)] (es decir,
sumar el incremento de energía). CONDICIONES DE CONTORNO PERIÓDICAS

3) Generar un número aleatorio entre 0 y 1. Si es menor que p, cambiar el signo del espín

4) Volver a 1)
*/

#include <iostream>
#include <fstream> // Ficheros
#include <cmath>
#include <iomanip> // Formato en los ficheros
#include <gsl/gsl_rng.h> // Números aleatorios. Cortesía de gsl
#include <sys/time.h> // Tiempo

// El número de espines es N^2
#define N 5

using namespace std;

long int semillatiempo();
void inicializarespines(int espines[][N], int caso, gsl_rng *tau);
void iteracion(int espines[][N], double temp, gsl_rng *tau);
double incEnergia(int espines[][N], int i, int j);

/**************************************************** FUNCIÓN PRINCIPAL ****************************************************/
int main(void) {

    // Declaro las variables
    int espines[N][N];
    double temp;
    int pasosMC;
    int caso;

    // Inicializo el puntero de números aleatorios
    gsl_rng *tau;
    tau=gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(tau,semillatiempo());

    // Pido los parámetros de la simulación
    cout << "Escoja una temperatura entre 0 y 5\n";
    cin >> temp;

    cout << "Escoja el número de pasos Monte Carlo\n";
    cin >> pasosMC;

    cout << "Elija una inicialización. 1 = Aleatorios, 2 = Todos positivos, 3 = Todos negativos\n";
    cin >> caso;

    // Inicializo los espines, dependiendo del caso que se haya escogido
    inicializarespines(espines, caso, tau);

    // Comienzo a iterar


    return 0;
}
/***************************************************************************************************************************/

/*Función semillatiempo. Crea una semilla para generar números aleatorios, basada en el tiempo actual.*/
long int semillatiempo()
{
    struct timeval tv;
    gettimeofday(&tv,0);
    return (tv.tv_sec + tv.tv_usec);
}

/*Función inicializarespines. Simplemente inicializa los espines*/
void inicializarespines(int espines[][N], int caso, gsl_rng *tau) {

    // Si los inicializamos aleatoriamente (caso 1)
    if(caso==1) {
        cout << "\n";

        // Asigno valores
        for(int i=0; i<N; i++) {
            for(int j=0; j<N; j++) {

                // Esta función da 0 ó 1 con probabilidad 1/2. Tengo que cambiar los 0 a -1
                espines[i][j]=gsl_rng_uniform_int(tau,2);
                if(espines[i][j]==0) espines[i][j]=-1;

                cout << espines[i][j] << "\n";
            }
        }
    }

    // Si todos positivos (caso 2)
    else if(caso==2) {
        for(int i=0; i<N; i++) {
            for(int j=0; j<N; j++) espines[i][j]=1;
        }
    }

    // Si todos negativos (caso 3)
    else {
        for(int i=0; i<N; i++) {
            for(int j=0; j<N; j++) espines[i][j]=-1;
        }
    }

    return;
}

/*Función iteracion. Es el núcleo del programa. Realiza los pasos que determinan si se cambia de signo algún
espín escogido aleatoriamente*/
void iteracion(int espines[][N], double temp, gsl_rng *tau) {

    double p;
    double numerito;

    // En primer lugar, escojo un espín (i,j) al azar
    int i=gsl_rng_uniform_int(tau, N);
    int j=gsl_rng_uniform_int(tau, N);

    // Calculo la p correspondiente
    numerito=exp(incEnergia(espines, i, j)/temp);
    
    if(numerito>=1) p=1;
    else p=numerito;

    /*Calculo un número aleatorio entre 0 y 1. Si es <p, cambio el signo del espín. Utilizo el mismo double numerito
    que ahora no sirve de nada más*/
    numerito=gsl_rng_uniform(tau);
    if(numerito<p) espines[i][j]=-espines[i][j];

}

/*Función incEnergia. Calcula el incremento de energía que supondría cambiar el espín (i,j) por su opuesto.
Tiene en cuenta las condiciones periódicas*/
double incEnergia(int espines[][N], int i, int j) {

    // Calculo i+1, i-1, j+1, j-1
    int imas1=((i+1)%N + N)%N;
    int imenos1=((i-1)%N + N)%N;
    int jmas1=((j+1)%N + N)%N;
    int jmenos1=((j-1)%N + N)%N;

    return -2*espines[i][j]*(espines[imas1][j]+espines[imenos1][j]+espines[i][jmas1]+espines[i][jmenos1]);
}