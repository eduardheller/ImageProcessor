#include "tasksix.h"
#include <QMainWindow>
#include <iostream>
#include "helper.h"
#include <vector>
#include <algorithm>
#include <functional>

tasksix::tasksix()
{
    gradient = NULL;
    gradientx = NULL;
    gradienty = NULL;
    Q = NULL;
}


void tasksix::applyHarris(QImage &image, QImage &tempImage, double palpha, double pdmin, double pth, int radius)
{
    double alpha = palpha;
    double th = pth;
    double dmin = pdmin;
    double hpx[3] = {  0.222,0.555,0.222 };
    int width = image.width();
    int height = image.height();
    M = width;
    N = height;

    image = tempImage.copy();
    for(int u=1; u<=width-2; u++) {
        for(int v=1; v<=height-2; v++){
            int sums = 0;
            for(int j=-1; j<=1; j++){
                for(int i=-1; i<=1; i++){
                    QRgb p = tempImage.pixel(u+j,v+i);
                    double cx = hpx[j+1];
                    double cy = hpx[i+1];
                    double c = cx*cy;
                    sums = sums + c *qGreen(p);
                }
            }
            int q = (int)floor(sums + 0.5);
            if(q<0) q=0;
            if(q>255) q=255;
            image.setPixel(u,v,QColor(q,q,q).rgb());
        }

    }

    double gpx[3] = { -0.453014,0.0,0.453014 };

    gradient = helper::createArrayD(M,N);
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
                    sum_x+=pixel*gpx[n+1];
                    sum_y+=pixel*gpx[m+1];
                }
            gradientx[i][j] = sum_x*sum_x;
            gradienty[i][j] = sum_y*sum_y;
            gradient[i][j] = sum_x*sum_y;
        }
    }



    double** C = helper::createArrayD(M,N);
    double** A = helper::createArrayD(M,N);
    double** B = helper::createArrayD(M,N);

    double bx[7] = { 0.1666,1.0,2.5,3.333,2.5,1.0,0.1666};
    for (int i=3; i < M-4; i++){
        for (int j=3; j < N-4; j++)
        {
            double sum_x=0;
            double sum_y=0;
            double sum_xy=0;
            for(int n=-3; n<=3; n++)
                for(int m=-3; m<=3; m++)
                {
                    double cx = bx[n+3];
                    double cy = bx[m+3];
                    double c = cx*cy;
                    sum_x+=gradientx[i+n][j+m]*c;
                    sum_y+=gradienty[i+n][j+m]*c;
                    sum_xy+=gradient[i+n][j+m]*c;
                }

            A[i][j] = sum_x;
            B[i][j] = sum_y;
            C[i][j] = sum_xy;
        }
    }



    for (int i=3; i < M-4; i++){
        for (int j=3; j < N-4; j++)
        {
            gradientx[i][j] = A[i][j];
            gradienty[i][j] = B[i][j];
            gradient[i][j] = C[i][j];
        }
    }

    helper::destroyArray(C,M);
    helper::destroyArray(B,M);
    helper::destroyArray(A,M);



    Q = helper::createArrayD(M,N);

    for (int i=1; i < M-2; i++){
        for (int j=1; j < N-2; j++)
        {
            Q[i][j] = (gradientx[i][j]*gradienty[i][j] - gradient[i][j]*gradient[i][j])-alpha*(gradientx[i][j]+gradienty[i][j])*(gradientx[i][j]+gradienty[i][j]);
        }
    }


    std::vector<edge> cornes;
    for (int i=1; i < M-2; i++){
        for (int j=1; j < N-2; j++)
        {
            if(Q[i][j]>th && isLocalMax(i,j)){
                edge ed;
                ed.u = i;
                ed.v = j;
                ed.q = Q[i][j];
                cornes.push_back(ed);
            }

        }
    }

    std::sort(cornes.begin(), cornes.end(),std::greater<edge>());

    std::vector<edge> goodCorners;
    while(cornes.size()>0){
        edge curr = cornes.at(0);
        goodCorners.push_back(curr);
        cornes.erase(cornes.begin());
        int end = cornes.size();

        for(int j = 0; j<end; j++){
            if(abs(curr.u-cornes.at(j).u)<dmin && abs(curr.v-cornes.at(j).v)<dmin){
                cornes.erase(cornes.begin()+j);
                end--;
                j--;
            }
        }
    }


    int r = radius;
    for(int i = 0; i<goodCorners.size(); i++){
       std::cout << goodCorners.at(i).q << " / x= " << goodCorners.at(i).u << " y=" << goodCorners.at(i).v << std::endl;
       int u = goodCorners.at(i).u;
       int v = goodCorners.at(i).v;
       int x = 0;
       int y = r;
       int d = 5-4*r;

       if((u+x<M-1 && u+x>=0) && (v+y<N-1 && v+y>=0)) image.setPixel(u+x,v+y,QColor(255,0,0).rgb());
       if((u+y<M-1 && u+y>=0) && (v+x<N-1 && v+x>=0)) image.setPixel(u+y,v+x,QColor(255,0,0).rgb());
       if((u+y<M-1 && u+y>=0) && (v+x<N-1 && v+x>=0)) image.setPixel(u+y,v+x,QColor(255,0,0).rgb());
       if((u-y<M-1 && u-y>=0) && (v+x<N-1 && v+x>=0)) image.setPixel(u-y,v+x,QColor(255,0,0).rgb());
       if((u+x<M-1 && u+x>=0) && (v-y<N-1 && v-y>=0)) image.setPixel(u+x,v-y,QColor(255,0,0).rgb());
       if((u-x<M-1 && u-x>=0) && (v-y<N-1 && v-y>=0)) image.setPixel(u-x,v-y,QColor(255,0,0).rgb());
       if((u-y<M-1 && u-y>=0) && (v-x<N-1 && v-x>=0)) image.setPixel(u-y,v-x,QColor(255,0,0).rgb());
       if((u+y<M-1 && u+y>=0) && (v-x<N-1 && v-x>=0)) image.setPixel(u+y,v-x,QColor(255,0,0).rgb());
       if((u-x<M-1 && u-x>=0) && (v+y<N-1 && v+y>=0)) image.setPixel(u-x,v+y,QColor(255,0,0).rgb());


       while(y>x){
           if(d>=0){
               d += 4*(2*(x-y)+5); x++;y--;
           }else{
               d += 4*(2*x+3); x++;
           }
           if((u+x<M-1 && u+x>=0) && (v+y<N-1 && v+y>=0)) image.setPixel(u+x,v+y,QColor(255,0,0).rgb());
           if((u+y<M-1 && u+y>=0) && (v+x<N-1 && v+x>=0)) image.setPixel(u+y,v+x,QColor(255,0,0).rgb());
           if((u+y<M-1 && u+y>=0) && (v+x<N-1 && v+x>=0)) image.setPixel(u+y,v+x,QColor(255,0,0).rgb());
           if((u-y<M-1 && u-y>=0) && (v+x<N-1 && v+x>=0)) image.setPixel(u-y,v+x,QColor(255,0,0).rgb());
           if((u+x<M-1 && u+x>=0) && (v-y<N-1 && v-y>=0)) image.setPixel(u+x,v-y,QColor(255,0,0).rgb());
           if((u-x<M-1 && u-x>=0) && (v-y<N-1 && v-y>=0)) image.setPixel(u-x,v-y,QColor(255,0,0).rgb());
           if((u-y<M-1 && u-y>=0) && (v-x<N-1 && v-x>=0)) image.setPixel(u-y,v-x,QColor(255,0,0).rgb());
           if((u+y<M-1 && u+y>=0) && (v-x<N-1 && v-x>=0)) image.setPixel(u+y,v-x,QColor(255,0,0).rgb());
           if((u-x<M-1 && u-x>=0) && (v+y<N-1 && v+y>=0)) image.setPixel(u-x,v+y,QColor(255,0,0).rgb());

       }

    }
    helper::destroyArray(Q,M);
}

bool tasksix::isLocalMax(int i, int j)
{
    double q_c = Q[i][j];
    double N[8] = { Q[i-1][j-1], Q[i-1][j], Q[i-1][j+1], Q[i][j-1], Q[i][j+1], Q[i+1][j-1], Q[i+1][j+1], Q[i+1][j] };
    bool ret = false;
    for(int i=0; i < 8; i++){
        if(q_c>N[i]){
            ret = true;
        }else{
            ret = false;
            break;
        }
    }
    return ret;

}

