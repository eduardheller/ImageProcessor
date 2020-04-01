#include "taskone.h"
#include <QMainWindow>
#include <iostream>
class QImage;
class QColor;

taskone::taskone(QImage& image, float rvalue)
{
    float x2 = image.width();
    float y2 = image.height();
    float m = y2/x2;
    float percentSlider = (rvalue+1.0)/100.0;

    float length = x2*(1-percentSlider);

    int margin = (int)length/2;

    for(int i = margin; i<x2-margin; i++){

        image.setPixel(i,m*i,QColor(255,0,0,0).rgb());
    }
    for(int i = x2-margin; i>margin; i--){
        image.setPixel(i,(x2 - i)*m,QColor(255,0,0,0).rgb());
    }
}

