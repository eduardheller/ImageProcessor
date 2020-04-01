#ifndef TASKTWO_H
#define TASKTWO_H

class QImage;
class QLabel;

class tasktwo
{
public:
    tasktwo();
    void changeBrightness(QImage& image,QImage& temp, float rvalue);
    void changeContrast(QImage& image,QImage& temp, float rvalue);
    void calculateAvg(QImage& grayimage, float &average, float& varianz);
    void autoContrast(QImage& image, QImage& grayScaledImage, float min, float max);
    void drawHistogramm(QImage& grayimage, QLabel& histogrammLabel1, QLabel& histogrammLabel2);

private:
    void apply(QImage& image,QImage& temp);
    float contrast;
    float brightness;
};

#endif // TASKTWO_H
