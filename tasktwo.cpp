#include "tasktwo.h"
#include <QMainWindow>
#include <QtWidgets>
#include <iostream>
class QLabel;
tasktwo::tasktwo() : brightness(0), contrast(1)
{

}

void tasktwo::changeBrightness(QImage &image, QImage &tempImage, float rvalue)
{
    brightness = rvalue - 50;
    apply(image,tempImage);
}

void tasktwo::changeContrast(QImage &image, QImage &tempImage, float rvalue)
{
    contrast = 1.0+(rvalue - 50.0)/50.0;
    apply(image,tempImage);
}

void tasktwo::apply(QImage &image, QImage &tempImage){
    int width = image.width();
    int height = image.height();

    for(int i = 0; i<width; i++){

        for(int j = 0; j<height; j++){
            QRgb pixel = tempImage.pixel(i,j);
            int red = contrast*qRed(pixel) + brightness;
            int green = contrast*qGreen(pixel) + brightness;
            int blue = contrast*qBlue(pixel) + brightness;

            red = std::min(255,std::max(0,red));
            green = std::min(255,std::max(0,green));
            blue = std::min(255,std::max(0,blue));

            image.setPixel(i,j,QColor(red,green,blue,0).rgb());
        }

    }
}



void tasktwo::calculateAvg(QImage& grayimage,float &average, float& varianz){

    varianz = 0.0;

    int width = grayimage.width();
    int height = grayimage.height();
    int grayScales[256] = {};

    int allIntesities = 0;
    for(int i = 0; i<width; i++){

        for(int j = 0; j<height; j++){
            QRgb pixel = grayimage.pixel(i,j);
            int intens = qGreen(pixel);
            allIntesities += intens;
            grayScales[intens]++;

        }

    }

    average = (float)allIntesities/(width*height);

    for(int i = 0; i<width; i++){
        for(int j = 0; j<height; j++){
            QRgb pixel = grayimage.pixel(i,j);
            varianz += pow(qGreen(pixel)-average,2);
        }
    }

    varianz = (float)varianz/(width*height);

}

void tasktwo::autoContrast(QImage& image, QImage& grayScaledImage, float min, float max){


    float smin = min/100.0;
    float smax = max/100.0;
    int amin = 0;
    int amax = 255;

    int width = grayScaledImage.width();
    int height = grayScaledImage.height();
    int grayScales[256] = {};

    int allIntesities = 0;
    for(int i = 0; i<width; i++){

        for(int j = 0; j<height; j++){
            QRgb pixel = grayScaledImage.pixel(i,j);
            int intens = qGreen(pixel);
            allIntesities += intens;
            grayScales[intens]++;
        }

    }
    float a_low = 255.0;

    std::vector<float> vec;
    for(int i = 0; i<256; i++){

        if(grayScales[i]>=(width*height)*smin){
           vec.push_back(i);
        }
    }

    if(vec.size()==0){
        a_low = 0.0;
    }

    for(unsigned int i = 0; i<vec.size(); i++){
        if(vec.at(i)<a_low){
            a_low = vec.at(i);

        }
    }



    float a_high = 0.0;

    std::vector<float> vec2;
    for(int i = 0; i<256; i++){
        if(grayScales[i]<=(width*height)*(1-smax))
           vec2.push_back(i);
    }

    for(unsigned int i = 0; i<vec2.size(); i++){
        if(vec2.at(i)>a_high){
            a_high = vec2.at(i);
        }
    }

    if(vec2.size()==0){
        a_high = 255.0;
    }

    if(smin>=0 && smax<=1 && smin+smax<=1){

        for(int i = 0; i<width; i++){

            for(int j = 0; j<height; j++){
                QRgb pixel = image.pixel(i,j);
                int red = qRed(pixel);
                int green = qGreen(pixel);
                int blue = qBlue(pixel);

                if(red<=a_low){
                    red = amin;
                }else if(a_low<red<a_high){
                    red = amin + (red-a_low)*((amax-amin)/(a_high-a_low));
                }else if(red>=a_high){
                    red = amax;
                }



                if(green<=a_low){
                    green = amin;
                }else if(a_low<green<a_high){
                    green = amin + (green-a_low)*((amax-amin)/(a_high-a_low));
                }else if(green>=a_high){
                    green = amax;
                }

                if(blue<=a_low){
                    blue = amin;
                }else if(a_low<blue<a_high){
                    blue = amin + (blue-a_low)*((amax-amin)/(a_high-a_low));
                }else if(blue>=a_high){
                    blue = amax;
                }


                red = std::min(255,std::max(0,red));
                green = std::min(255,std::max(0,green));
                blue = std::min(255,std::max(0,blue));

                image.setPixel(i,j,QColor(red,green,blue,0).rgb());

            }

        }

    }

}

void tasktwo::drawHistogramm(QImage &grayimage, QLabel &histogrammLabel1, QLabel &histogrammLabel2)
{
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



