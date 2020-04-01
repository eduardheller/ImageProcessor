#ifndef TASKFOUR_H
#define TASKFOUR_H

#include <vector>
class QImage;

class taskfour
{
public:
    taskfour();
    void linearFilter(QImage &image, QImage &tempImage, std::vector<std::vector<int>> matrix, int length);
    void linearFilterZero(QImage &image, QImage &tempImage, std::vector<std::vector<int>> matrix, int length);
    void linearFilterConst(QImage &image, QImage &tempImage, std::vector<std::vector<int>> matrix, int length);
    void linearFilterMirror(QImage &image, QImage &tempImage, std::vector<std::vector<int>> matrix, int length);
};

#endif // TASKFOUR_H
