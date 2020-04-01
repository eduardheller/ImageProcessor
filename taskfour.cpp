#include "taskfour.h"
#include <QMainWindow>
#include <cmath>
#include <vector>
#include <iostream>
taskfour::taskfour()
{

}

void taskfour::linearFilter(QImage &image, QImage &tempImage, std::vector<std::vector<int>> matrix, int length)
{
    int width = image.width();
    int height = image.height();
    int K = length/2;
    int L = length/2;

    int msum = 0;
    for(int i = 0; i<length; i++){
        for(int j = 0; j<length; j++){
            msum += matrix[i][j];
        }
    }

    double s = 1.0/msum;
    for(int v=L; v<=height-L-1; v++) {
        for(int u=K; u<=width-K-1; u++){
            int sum = 0;
            for(int j=-L; j<=L; j++){
                for(int i=-K; i<=K; i++){
                    QRgb p = tempImage.pixel(u+i,v+j);
                    int c = matrix[j+L][i+K];

                    sum = sum + c *qGreen(p);
                }
            }


            int q = (int)floor(s*sum + 0.5);
            if(q<0) q=0;
            if(q>255) q=255;
            image.setPixel(u,v,QColor(q,q,q).rgb());
        }

    }

}

void taskfour::linearFilterZero(QImage &image, QImage &tempImage, std::vector<std::vector<int> > matrix, int length)
{
    int width = image.width();
    int height = image.height();
    int K = length/2;
    int L = length/2;

    int msum = 0;
    for(int i = 0; i<length; i++){
        for(int j = 0; j<length; j++){
            msum += matrix[i][j];
        }
    }

    double s = 1.0/msum;
    for(int v=0; v<=height-1; v++) {
        for(int u=0; u<=width-1; u++){
            int sum = 0;
            for(int j=-L; j<=L; j++){
                for(int i=-K; i<=K; i++){
                    if(v>height-L-1 || v<L){
                        int c = matrix[j+L][i+K];
                        sum = sum + c *100;
                    }else if(u>width-K-1 || u<K){
                        int c = matrix[j+L][i+K];
                        sum = sum + c *100;
                    }else{

                        QRgb p = tempImage.pixel(u+i,v+j);
                        int c = matrix[j+L][i+K];

                        sum = sum + c *qGreen(p);
                    }


                }
            }


            int q = (int)floor(s*sum + 0.5);
            if(q<0) q=0;
            if(q>255) q=255;
            image.setPixel(u,v,QColor(q,q,q).rgb());
        }

    }
}

void taskfour::linearFilterConst(QImage &image, QImage &tempImage, std::vector<std::vector<int> > matrix, int length)
{
    int width = image.width();
    int height = image.height();
    int K = length/2;
    int L = length/2;

    int msum = 0;
    for(int i = 0; i<length; i++){
        for(int j = 0; j<length; j++){
            msum += matrix[i][j];
        }
    }

    double s = 1.0/msum;
    for(int v=0; v<=height-1; v++) {
        for(int u=0; u<=width-1; u++){
            int sum = 0;
            for(int j=-L; j<=L; j++){
                for(int i=-K; i<=K; i++){
                    if(v<L && j<0){
                        QRgb p = image.pixel(u,0);
                        int c = matrix[j+L][i+K];
                        sum = sum + c *qGreen(p);
                    }else if(u<K && i<0){
                        QRgb p = image.pixel(0,v);
                        int c = matrix[j+L][i+K];
                        sum = sum + c *qGreen(p);
                    }else if(v>height-1-L && j>=0){
                        QRgb p = image.pixel(u,height-1);
                        int c = matrix[j+L][i+K];
                        sum = sum + c *qGreen(p);
                    }else if(u>width-1-K && i>=0){
                        QRgb p = image.pixel(width-1,v);
                        int c = matrix[j+L][i+K];
                        sum = sum + c *qGreen(p);
                    }else{
                        QRgb p = tempImage.pixel(u+i,v+j);
                        int c = matrix[j+L][i+K];
                        sum = sum + c *qGreen(p);
                    }

                }
            }


            int q = (int)floor(s*sum + 0.5);
            if(q<0) q=0;
            if(q>255) q=255;
            image.setPixel(u,v,QColor(q,q,q).rgb());
        }

    }
}

void taskfour::linearFilterMirror(QImage &image, QImage &tempImage, std::vector<std::vector<int> > matrix, int length)
{
    int width = image.width();
    int height = image.height();
    int K = length/2;
    int L = length/2;

    int msum = 0;
    for(int i = 0; i<length; i++){
        for(int j = 0; j<length; j++){
            msum += matrix[i][j];
        }
    }

    double s = 1.0/msum;
    for(int v=0; v<=height-1; v++) {
        for(int u=0; u<=width-1; u++){
            int sum = 0;
            for(int j=-L; j<=L; j++){
                for(int i=-K; i<=K; i++){
                    if(v<L && j<0){
                        QRgb p = image.pixel(u,L);
                        int c = matrix[j+L][i+K];
                        sum = sum + c *qGreen(p);
                    }else if(u<K && i<0){
                        QRgb p = image.pixel(K,v);
                        int c = matrix[j+L][i+K];
                        sum = sum + c *qGreen(p);
                    }else if(v>height-1-L && j>=0){
                        QRgb p = image.pixel(u,height-1-L);
                        int c = matrix[j+L][i+K];
                        sum = sum + c *qGreen(p);
                    }else if(u>width-1-K && i>=0){
                        QRgb p = image.pixel(width-1-K,v);
                        int c = matrix[j+L][i+K];
                        sum = sum + c *qGreen(p);
                    }else{
                        QRgb p = tempImage.pixel(u+i,v+j);
                        int c = matrix[j+L][i+K];
                        sum = sum + c *qGreen(p);
                    }

                }
            }


            int q = (int)floor(s*sum + 0.5);
            if(q<0) q=0;
            if(q>255) q=255;
            image.setPixel(u,v,QColor(q,q,q).rgb());
        }

    }
}

