#ifndef TASKSIX_H
#define TASKSIX_H

class QImage;

class tasksix
{
public:
    struct edge{
        int u;
        int v;
        double q;

        bool operator > (const edge& str) const
        {
            return (q > str.q);
        }
    };


    tasksix();
    void applyHarris(QImage &image, QImage &tempImage, double palpha, double pdmin, double pth, int radius);
    bool isLocalMax(int u, int v);


private:
    double **gradient;
    double **gradientx;
    double **gradienty;
    double **Q;
    int M;
    int N;
};

#endif // TASKSIX_H
