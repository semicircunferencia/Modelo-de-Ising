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

#define N 5
#define temp 1


using namespace std;

long int semillatiempo();
void inicializarespines(int espines[][N], int caso);
gsl_rng *tau;

/**************************************************** FUNCIÓN PRINCIPAL ****************************************************/
int main(void) {

    // Declaro las variables
    int espines[N][N];
    int caso; // La inicialización de los espines dependerá del usuario
    extern gsl_rng *tau;

    // Inicializo los espines
    cout << "Elija una inicialización. 1 = Aleatorios, 2 = Todos positivos, 3 = Todos negativos\n";
    cin >> caso;
    inicializarespines(espines, caso);

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
void inicializarespines(int espines[][N], int caso) {

    // Si los inicializamos aleatoriamente (caso 1)
    if(caso==1) {

        // Inicializo la semilla
        long int semilla=semillatiempo();
        extern gsl_rng *tau;
        tau=gsl_rng_alloc(gsl_rng_taus);
        gsl_rng_set(tau,semilla);


        // Asigno valores
        for(int i=0; i<N; i++) {
            for(int j=0; j<N; j++) {

                // Esta función da 0 ó 1 con probabilidad 1/2. Tengo que cambiar los 0 a -1
                espines[i][j]=gsl_rng_uniform_int(tau,2);
                if(espines[i][j]==0) espines[i][j]=-1;
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