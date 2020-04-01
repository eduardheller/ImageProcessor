#ifndef TASKFIVE_H
#define TASKFIVE_H

#include <vector>
class QImage;
class taskfive

{
public:
    taskfive();
    ~taskfive();
    void applyGauss(QImage& image, QImage& tempImage, double sigma, int size);
    void applyMaximum(double tlow);
    void applyRadiant(QImage & image);
    void applyCannyEdge(QImage& image, double t_hi);
    int getOrientationSector(int dx, int dy);
    bool isLocalMax(int sector,int i, int j);
    void traceAndThreshold(int i, int j);
    void applyUSM(QImage& image, QImage& tempImage, double sigma, int size, double weight);

private:
    double **maxGradient;
    double **binaryEdges;
    double **gradient;
    double **gradientx;
    double **gradienty;
    int M;
    int N;
    double t_low;
    double t_high;
    double usmweight;
};

#endif // TASKFIVE_H
