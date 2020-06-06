#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Devuelve un número aleatorio en un rango
int aleatorio(int minimo, int maximo) {
  return minimo + rand() / (RAND_MAX / (maximo - minimo + 1) + 1);
}

int main ()
{
    //crear aareglo
    long long int n; /*tamaño del arreglo*/
    printf("Introduzca tamaño del arreglo: ");
    scanf("%lli",&n);
    int arreglo [n];
    //llenar el arreglo
    srand(getpid());
    for (int i=0; i<n; i++){
        arreglo[i] = aleatorio(0, 10000);
    }
    for (int i=0;i<n;i++){
        printf("Elemento numero %d = %d", i+1, arreglo[i]);
        printf("\n");
    }


    /*return 0;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Send(&offset, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
    MPI_Recv(&offset, 1, MPI_INT, source, tag1, MPI_COMM_WORLD, &status);
    MPI_Finalize();*/
    return 0;
} /* fin del menu*/
