#include "taskfive.h"
#include <QMainWindow>
#include <cmath>
#include <iostream>
#include <vector>
#include "helper.h"
#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif
taskfive::taskfive()
{
    maxGradient = NULL;
    binaryEdges = NULL;
    gradient = NULL;
    gradientx = NULL;
    gradienty = NULL;
}

taskfive::~taskfive()
{
    helper::destroyArray(gradient,M);
    helper::destroyArray(maxGradient,M);
    helper::destroyArray(binaryEdges,M);
    helper::destroyArray(gradientx,M);
    helper::destroyArray(gradienty,M);
}

void taskfive::applyGauss(QImage &image, QImage &tempImage, double psigma, int size)
{

    double** kernel = helper::createArrayD(size,size);
    int W = size;
    double sigma = psigma;
    double mean = W/2;
    double sum = 0.0;
    for (int x = 0; x < W; ++x)
        for (int y = 0; y < W; ++y) {
            kernel[x][y] = exp( -0.5 * (pow((x-mean)/sigma, 2.0) + pow((y-mean)/sigma,2.0)) )
                             / (2 * M_PI * sigma * sigma);
               sum += kernel[x][y];
           }
    for (int x = 0; x < W; ++x)
       for (int y = 0; y < W; ++y){
           kernel[x][y] /= sum;

       }
    int width = image.width();
    int height = image.height();
    M = width;
    N = height;
    int K = size/2;
    int L = size/2;

    for(int u=L; u<=width-L-1; u++) {
        for(int v=K; v<=height-K-1; v++){
            int sums = 0;
            for(int j=-L; j<=L; j++){
                for(int i=-K; i<=K; i++){

                    QRgb p = tempImage.pixel(u+j,v+i);
                    float c = kernel[j+L][i+K];
                    sums = sums + c *qGreen(p);
                }
            }


            int q = (int)floor(sums + 0.5);
            if(q<0) q=0;
            if(q>255) q=255;
            image.setPixel(u,v,QColor(q,q,q).rgb());
        }

    }
    helper::destroyArray(kernel,size);
}

void taskfive::applyRadiant(QImage& image)
{

    int dx[3][3] = {{1,0,-1},{2,0,-2},{1,0,-1}};
    int dy[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};

    gradient = helper::createArrayD(M,N);
    maxGradient = helper::createArrayD(M,N);
    binaryEdges = helper::createArrayD(M,N);
    gradientx = helper::createArrayD(M,N);
    gradienty = helper::createArrayD(M,N);

    for (int i=1; i < M-2; i++){
        for (int j=1; j < N-2; j++)
        {
            double sum_x=0;
            double sum_y=0;
            for(int n=-1; n<=1; n++)
                for(int m=-1; m<=1; m++)
                {
                    QRgb p = image.pixel(n+i,m+j);
                    int pixel = qGreen(p);
                    sum_x+=pixel*dx[n+1][m+1];
                    sum_y+=pixel*dy[n+1][m+1];
                }
            double sum=sqrt(sum_x*sum_x+sum_y*sum_y);

            gradient[i][j] = sum;
            gradientx[i][j] = sum_x;
            gradienty[i][j] = sum_y;
            binaryEdges[i][j] = 0;
        }
    }
}

void taskfive::applyMaximum(double tlow)
{
    t_low = tlow;
    for (int i=1; i < M-2; i++){
        for (int j=1; j < N-2; j++)
        {
            int sector = getOrientationSector(gradientx[i][j],gradienty[i][j]);
            if(isLocalMax(sector,i,j)){
                maxGradient[i][j] = gradient[i][j];
            }
        }
    }

}



void taskfive::applyCannyEdge(QImage& image, double thi)
{
    for (int i=1; i < M-2; i++){
        for (int j=1; j < N-2; j++)
        {
            binaryEdges[i][j]=0;
        }
    }

    t_high = thi;
    int width = image.width();
    int height = image.height();
    M = width;
    N = height;

    for (int i=1; i < width-2; i++){
        for (int j=1; j < height-2; j++)
        {
            if(maxGradient[i][j]>=t_high && binaryEdges[i][j]==0){
                traceAndThreshold(i,j);
            }

        }
    }

    for (int i=1; i < width-2; i++){
        for (int j=1; j < height-2; j++)
        {
            if(binaryEdges[i][j]==1){
                image.setPixel(i,j,QColor(0,0,0).rgb());
            }else{
                image.setPixel(i,j,QColor(255,255,255).rgb());
            }
        }
    }


}

int taskfive::getOrientationSector(int dx, int dy)
{
    double dxN = cos(M_PI/8)*dx-sin(M_PI/8)*dy;
    double dyN = sin(M_PI/8)*dx+cos(M_PI/8)*dy;

    if(dyN<0){
        dxN = -dxN;
        dyN = -dyN;
    }


    if(dxN>=0 && dxN>=dyN){
        return 0;
    }else if(dxN>=0 && dxN<dyN){
        return 1;
    }else if(dxN<0 && -dxN<dyN){
        return 2;
    }else if(dxN<0 && -dxN>=dyN){
        return 3;
    }
    return -1;
}

bool taskfive::isLocalMax(int sector,int i, int j)
{
    int mc = gradient[i][j];
    if(mc<t_low){
        return false;
    }

    int ml = 0;
    int mr = 0;
    if(sector == 0){
        ml = gradient[i-1][j];
        mr = gradient[i+1][j];
    }else if(sector == 1){
        ml = gradient[i-1][j-1];
        mr = gradient[i+1][j+1];
    }else if(sector == 2){
        ml = gradient[i][j-1];
        mr = gradient[i][j+1];
    }else if(sector == 3){
        ml = gradient[i-1][j+1];
        mr = gradient[i+1][j-1];
    }

    return ml<=mc && mc>=mr;

}

void taskfive::traceAndThreshold(int i, int j)
{
    bool done = false;
    while(!done){
        binaryEdges[i][j] = 1;
        int ul = std::max(i-1,0);
        int ur = std::min(i+1,M-1);
        int ut = std::max(j-1,0);
        int ub = std::min(j+1,N-1);

        for(int u = ul; u<=ur; u++){
            for(int v = ut; v<=ub; v++){
                if(maxGradient[u][v]>=t_low && binaryEdges[u][v]==0){
                    i = u; j = v;
                    break;
                }else{
                    done = true;
                }
            }
        }
    }
}

void taskfive::applyUSM(QImage &image, QImage &tempImage, double psigma, int size, double weight)
{
    usmweight = weight;
    double** kernel = helper::createArrayD(size,size);
    int W = size;
    double sigma = psigma;
    double mean = W/2;
    double sum = 0.0;
    for (int x = 0; x < W; ++x)
        for (int y = 0; y < W; ++y) {
            kernel[x][y] = exp( -0.5 * (pow((x-mean)/sigma, 2.0) + pow((y-mean)/sigma,2.0)) )
                             / (2 * M_PI * sigma * sigma);
               sum += kernel[x][y];
           }
    for (int x = 0; x < W; ++x)
       for (int y = 0; y < W; ++y){
           kernel[x][y] /= sum;

       }
    int width = image.width();
    int height = image.height();
    M = width;
    N = height;
    int K = size/2;
    int L = size/2;

    for(int u=L; u<=width-L-1; u++) {
        for(int v=K; v<=height-K-1; v++){
            int sums = 0;
            for(int j=-L; j<=L; j++){
                for(int i=-K; i<=K; i++){

                    QRgb p = tempImage.pixel(u+j,v+i);
                    float c = kernel[j+L][i+K];
                    sums = sums + c *qGreen(p);
                }
            }


            int q = (int)floor(sums + 0.5);
            if(q<0) q=0;
            if(q>255) q=255;
            image.setPixel(u,v,QColor(q,q,q).rgb());
        }

    }
    helper::destroyArray(kernel,size);

    for (int i=1; i < M-2; i++){
        for (int j=1; j < N-2; j++)
        {

            QRgb p = image.pixel(i,j);
            int IH = qGreen(p);

            QRgb p2 = tempImage.pixel(i,j);
            int I = qGreen(p2);
            double m = I-IH;
            double rpixel = I + usmweight*m;
            int pixel = rpixel;
            if(pixel<0) pixel=0;
            if(pixel>255) pixel=255;
            image.setPixel(i,j,QColor(pixel,pixel,pixel).rgb());
        }
    }

}
