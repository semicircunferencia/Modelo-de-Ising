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

using namespace std;

long int semillatiempo();

gsl_rng *tau;

int main(void) {
    extern gsl_rng *tau;
    double x[10];

    long int semilla=semillatiempo();
    tau=gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(tau,semilla);

    for(int i=0; i<10; i++) {
        x[i]=gsl_rng_uniform(tau);
        cout << x[i] << "\n";
    }

    return 0;
}

/*FUNCIÓN SEMILLATIEMPO. Crea una semilla para generar números aleatorios, basada en el tiempo actual.*/
long int semillatiempo()
{
    struct timeval tv;
    gettimeofday(&tv,0);
    return (tv.tv_sec + tv.tv_usec);
}