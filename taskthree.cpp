#include "taskthree.h"
#include <QMainWindow>
#include <QtWidgets>
#include <iostream>
taskthree::taskthree()
{

}

void taskthree::drawHistogramm(QImage &grayimage,QLabel& histogrammLabel1, QLabel& histogrammLabel2){
    int width = grayimage.width();
    int height = grayimage.height();
    int grayScales[256] = {};

    for(int i = 0; i<width; i++){

        for(int j = 0; j<height; j++){
            QRgb pixel = grayimage.pixel(i,j);
            int intens = qGreen(pixel);
            grayScales[intens]++;

        }

    }

    /*
     *
     * Histogramm Zeichnen
     *
     *
     */
    float grayScalesRel[256] = {};
    int max = 0;
    for(int i = 0; i<256; i++){
        if(grayScales[i]>max)
            max = grayScales[i];
        grayScalesRel[i] = grayScales[i]/(float)(width*height);
    }

    float grayScaleKum[256] = {};
    for(int i = 0; i<256; i++){
        for(int j = 0; j<=i; j++){
             grayScaleKum[i] += grayScalesRel[j];
        }

    }

    QPixmap pixmap(256,120);
    pixmap.fill(QColor("gray"));

    QPainter painter(&pixmap);
    painter.setPen( Qt::black );

    for(int i = 0; i<256; i++){
        float perc = grayScales[i]/(float)max;

        int height = 120.0*perc;
        painter.drawLine(i,120,i,120-height);
    }

    histogrammLabel1.setPixmap(pixmap);

    painter.end();


    QPixmap pixmap2(256,120);
    pixmap2.fill(QColor("gray"));

    QPainter painter2(&pixmap2);
    painter2.setPen( Qt::black );

    for(int i = 0; i<256; i++){
        float perc = grayScaleKum[i];

        int height = 120.0*perc;
        painter2.drawLine(i,120,i,120-height);

    }

    histogrammLabel2.setPixmap(pixmap2);
}

bool taskthree::referenceAusgleich(const QString &fileName,QImage &image, QImage &tempImage)
{

    QImage* referenceImage = new QImage(fileName);
    *referenceImage = referenceImage->convertToFormat(QImage::Format_Grayscale8);

    int width = referenceImage->width();
    int height = referenceImage->height();
    int M = width * height;
    int K = 256;

    int grayScalesReference[256] = {};

    for(int i = 0; i<width; i++){

        for(int j = 0; j<height; j++){
            QRgb pixel = referenceImage->pixel(i,j);
            int intens = qGreen(pixel);
            grayScalesReference[intens]++;
        }
    }

    float grayScalesRelRef[256] = {};
    for(int i = 0; i<256; i++){
        grayScalesRelRef[i] = grayScalesReference[i]/(float)(width*height);
    }


    for(int j = 1; j < K; j++){
        grayScalesRelRef[j] = grayScalesRelRef[j-1] + grayScalesRelRef[j];
    }


    width = image.width();
    height = image.height();
    M = width * height;
    K = 256;

    float *y = new float[width*height];
    float *u = new float[width*height];
    float *v = new float[width*height];
    int k = 0;
    int grayScales[256] = {};


    k++;
    for(int i = 0; i<width; i++){

        for(int j = 0; j<height; j++){
            QRgb pixel = image.pixel(i,j);
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);
            y[k] = 0.299*red + 0.587*green + 0.114*blue;
            u[k] = (blue-y[k])*0.493;
            v[k] = (red-y[k])*0.877;
            image.setPixel(i,j,QColor(y[k],y[k],y[k],0).rgb());
            grayScales[(int)y[k]]++;
            k++;
        }
    }

    float grayScalesRel[256] = {};
    for(int i = 0; i<256; i++){
        grayScalesRel[i] = grayScales[i]/(float)(width*height);
    }

    for(int j = 1; j < K; j++){
        grayScalesRel[j] = grayScalesRel[j-1] + grayScalesRel[j];
    }




    for(int i = 0; i<K; i++){
        float intensity = grayScalesRel[i];

        int pixel;
        if(intensity<=grayScalesRelRef[0]){
            pixel = 0;
        }else if(intensity>=1){
            pixel = K-1;
        }else{
            int n = 255;
            while(n>=0 && grayScalesRelRef[n]>intensity){
                n -= 1;
            }
            pixel = n+(intensity-grayScalesRelRef[n])*((n+1)-n)/(grayScalesRelRef[n+1]-grayScalesRelRef[n]);
        }


        int z = 0;
        for(int k = 0; k<width; k++){

            for(int j = 0; j<height; j++){
                QRgb realPixel = tempImage.pixel(k,j);

                int intens = qGreen(realPixel);
                if(intens == i){
                    y[z] = pixel;
                    int blue = y[z]+u[z]/0.493;
                    int red = y[z]+v[z]/0.877;
                    int green = y[z]-0.39393*u[z]-0.58081*v[z];

                    red = std::min(255,std::max(0,red));
                    green = std::min(255,std::max(0,green));
                    blue = std::min(255,std::max(0,blue));


                    image.setPixel(k,j,QColor(red,green,blue).rgb());
                }
                z++;
            }
        }

    }
    delete[] y;
    delete[] u;
    delete[] v;
    return true;

}

void taskthree::linearAusgleich(QImage &image)
{


    int width = image.width();
    int height = image.height();

    int M = width * height;
    int K = 256;
    int grayScales[256] = {};
    float *y = new float[width*height];
    float *u = new float[width*height];
    float *v = new float[width*height];
    int k = 0;
    for(int i = 0; i<width; i++){

        for(int j = 0; j<height; j++){
            QRgb pixel = image.pixel(i,j);
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);
            y[k] = 0.299*red + 0.587*green + 0.114*blue;
            u[k] = (blue-y[k])*0.493;
            v[k] = (red-y[k])*0.877;

            image.setPixel(i,j,QColor(y[k],y[k],y[k],0).rgb());
            grayScales[(int)y[k]]++;
            k++;
        }
    }

    for(int j = 1; j < K; j++){
        grayScales[j] = grayScales[j-1] + grayScales[j];
    }

    k = 0;
    for(int i = 0; i<width; i++){
        for(int j = 0; j<height; j++){
            QRgb pixel = image.pixel(i,j);
            y[k] = grayScales[qGreen(pixel)]*(K-1) / M;
            int blue = y[k]+u[k]/0.493;
            int red = y[k]+v[k]/0.877;
            int green = y[k]-0.39393*u[k]-0.58081*v[k];

            red = std::min(255,std::max(0,red));
            green = std::min(255,std::max(0,green));
            blue = std::min(255,std::max(0,blue));

            k++;
            image.setPixel(i,j,QColor(red,green,blue).rgb());
        }
    }

    delete[] y;
    delete[] u;
    delete[] v;
}


void taskthree::gauss(QImage &image,QImage &grayScaledImage, float rvalue){

    float gaussValues[256] = {};
    float o = rvalue;
    float c = 128.0;
    for(int i = 0; i<256; i++){
        gaussValues[i] = (1.0/(o*sqrt(2*M_PI)))*exp(-0.5*pow((i-c)/o,2));
    }

    for(int i = 1; i<256; i++){
        gaussValues[i] += gaussValues[i-1];
    }


    int width = image.width();
    int height = image.height();
    int K = 256;

    int grayScales[256] = {};
    float *y = new float[width*height];
    float *u = new float[width*height];
    float *v = new float[width*height];
    int k = 0;
    for(int i = 0; i<width; i++){

        for(int j = 0; j<height; j++){
            QRgb pixel = image.pixel(i,j);
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);
            y[k] = 0.299*red + 0.587*green + 0.114*blue;
            u[k] = (blue-y[k])*0.493;
            v[k] = (red-y[k])*0.877;

            image.setPixel(i,j,QColor(y[k],y[k],y[k],0).rgb());
            grayScales[(int)y[k]]++;
            k++;
        }
    }

    float grayScalesRel[256] = {};
    for(int i = 0; i<256; i++){
        grayScalesRel[i] = grayScales[i]/(float)(width*height);
    }

    for(int j = 1; j < K; j++){
        grayScalesRel[j] = grayScalesRel[j-1] + grayScalesRel[j];
    }



    for(int i = 0; i<256; i++){
        int n = 255;


        while(grayScalesRel[i]<=gaussValues[n] && n>0){
            n--;
        }
        int z = 0;
        for(int k = 0; k<width; k++){
            for(int j = 0; j<height; j++){

                QRgb pixel = grayScaledImage.pixel(k,j);
                int intens = qGreen(pixel);
                if(intens == i){
                    y[z] = n;
                    int blue = y[z]+u[z]/0.493;
                    int red = y[z]+v[z]/0.877;
                    int green = y[z]-0.39393*u[z]-0.58081*v[z];

                    red = std::min(255,std::max(0,red));
                    green = std::min(255,std::max(0,green));
                    blue = std::min(255,std::max(0,blue));
                    image.setPixel(k,j,QColor(red,green,blue).rgb());
                }
                z++;
            }
        }

    }
    delete[] y;
    delete[] u;
    delete[] v;

}

