#include "helper.h"

helper::helper()
{

}


int **helper::createArray(int u, int v)
{
    int** ret = (int**)malloc(u*sizeof(int*));
    for(int i = 0; i<u; i++){
        ret[i] = (int*) malloc(v*sizeof(int));
    }
    return ret;
}

double **helper::createArrayD(int u, int v)
{
    double** ret = (double**)malloc(u*sizeof(double*));
    for(int i = 0; i<u; i++){
        ret[i] = (double*) malloc(v*sizeof(double));
    }
    return ret;
}

void helper::destroyArray(int **arr, int u)
{
    for(int i = 0; i<u; i++){
        free(arr[i]);
    }
    free(arr);
}

void helper::destroyArray(double **arr, int u)
{
    for(int i = 0; i<u; i++){
        free(arr[i]);
    }
    free(arr);
}



