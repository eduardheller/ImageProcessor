#ifndef TASKTHREE_H
#define TASKTHREE_H

class QImage;
class QLabel;
class QString;
class taskthree
{
public:
    taskthree();
    void linearAusgleich(QImage &image);
    void drawHistogramm(QImage& grayimage, QLabel& histogrammLabel1, QLabel& histogrammLabel2);
    bool referenceAusgleich(const QString &fileName,QImage &image, QImage &tempImage);
    void gauss(QImage &image,QImage &grayScaledImage, float rvalue);

};

#endif // TASKTHREE_H
