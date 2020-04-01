#ifndef HELPER_H
#define HELPER_H
#include <QMainWindow>

class helper
{
public:
    helper();

    static int** createArray(int i, int j);
    static double** createArrayD(int i, int j);

    static void destroyArray(int **arr, int i);
    static void destroyArray(double **arr, int i);
};

#endif // HELPER_H
