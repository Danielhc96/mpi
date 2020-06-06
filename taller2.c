#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Devuelve un número aleatorio en un rango
int aleatorio(int minimo, int maximo) {
  return minimo + rand() / (RAND_MAX / (maximo - minimo + 1) + 1);
}

void SortArray (int array[],int first,int last){
    int i,j,p,t;
    // i se hace igual al índice del primer elemento
    i= first;
    // y j igual al índice del último elemento
    j= last;
    // p se hace igual al elemento pivote del arreglo
    p= array[(first+last)/2];
    do {
        // se hace la partición del arreglo
        while (array[i]<p) i++;
        while (p<array[j]) j--;
        if (i<=j) {
                // se intercambian los elementos i-esimo y j-esimo del arreglo
                t= array[i];
                array[i]= array[j];
                array[j]= t;
                i++; j--;
            }
    } while (i<=j);
    if (first<j) SortArray(array,first,j);
    if (i<last) SortArray(array,i,last);
}

int main (int argc, char *argv[]){
    //crear areglo
    int i;
    int n; /*tamaño del arreglo*/
    printf("Introduzca tamaño del arreglo: ");
    scanf("%d",&n);
    int arreglo[n];
    //llenar el arreglo con numeros aleatorios
    srand(getpid());
    for (i=0; i<n; i++){
        arreglo[i] = aleatorio(0, 10000);
    }
    // variables pa lo que se viene
    int mi_rango; /*rango del proceso*/
    int p; /*numero de procesos*/
    int tamp; /*tamaño del pedazo*/
    int resto; /*la sobra del arreglo*/
    int maestro; /*la parte del maestro (pedazo + resto)*/
    int dest;
    int source;
    int tag1;
    int tag2;
    MPI_Status status;

    //Inicio de mpi para enviar a maquinas
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD,&mi_rango);
    tamp = (n / p);
    resto = (n % p);
    tag2 = 1;
    tag1 = 2;
    int arreglo1[tamp + resto];
    int arreglo2[tamp];
    /*parte del maestro*/
    if (mi_rango == 0){
        /*enviar a cada tarea su parte de la matriz*/
        maestro = tamp + resto;
        for (dest=1; dest<p; dest++) {
        MPI_Send(&maestro, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD); /*se envia posicion inicial al proceso p*/
        MPI_Send(&arreglo2[0], tamp, MPI_INT, dest, tag2, MPI_COMM_WORLD); /*se envia dato inicial al proceso p*/
        maestro = maestro + tamp; /*nueva posicion inicial*/
        }

        /*trabajo del maestro*/
        maestro = 0;
        for (i=0; i<maestro; i++){
            arreglo1[i]=arreglo[i];
        }
        SortArray(arreglo1,0, tamp + resto);
        for (i=0; i<maestro; i++){
            arreglo[i]=arreglo1[i];
        }

        /* espera para recibir los resultados de cada maquina */
        for (i=1; i<p; i++) {
            source = i;
            MPI_Recv(&maestro, 1, MPI_INT, source, tag1, MPI_COMM_WORLD, &status);
            MPI_Recv(&arreglo2[0], tamp, MPI_INT, source, tag2, MPI_COMM_WORLD, &status);
        }
        /*mostrar arreglo ordenado*/
        for (i=0;i<n;i++){
        printf("Elemento numero %d = %d", i+1, arreglo[i]);
        printf("\n");
        }

    }/*termina la parte del maestro*/

    /*parte de los esclavos*/
    if (mi_rango > 0) {
        /* Recive parte del arreglo */
        source = 0;
        MPI_Recv(&maestro, 1, MPI_INT, source, tag1, MPI_COMM_WORLD, &status);
        MPI_Recv(&arreglo2[0], tamp, MPI_INT, source, tag2, MPI_COMM_WORLD, &status);

        /* hace el trabajo */

        for (i=0; i<tamp; i++){
            arreglo2[i]=arreglo[i+maestro];
        }
        SortArray(arreglo2,0, tamp);
        for (i=0; i<maestro; i++){
            arreglo[i+maestro]=arreglo1[i];
        }

        /* envia los resultados*/
        dest = 0;
        MPI_Send(&maestro, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
        MPI_Send(&arreglo2[0], tamp, MPI_INT, 0, tag2, MPI_COMM_WORLD);

        } /* terminan los esclavos */

    MPI_Finalize();
    return 0;
} /* fin del menu*/
