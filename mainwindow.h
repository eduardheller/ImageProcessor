#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "tasktwo.h"
#include "taskthree.h"
#include "taskfour.h"
#include "taskfive.h"
#include "tasksix.h"
#include "fstream"

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QTextEdit;
class QVBoxLayout;
class QTabWidget;
class QPushButton;
class QDoubleSpinBox;
class QSpinBox;
class QSlider;
class QLabel;

namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool loadFile(const QString &);
    void updateImageDisplay();

    ~MainWindow();




private slots:
    void open();
    void openReference();
    void print();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();

    void taskOneRedCross();
    void changeBrightness();
    void changeContrast();
    void getAvgBrightnessAndContrast();
    void autoContrast();
    void linearAusgleich();
    void gauss();
    void linearFilter();
    void linearFilterZero();
    void linearFilterConst();
    void linearFilterMirror();

    void cannyEdge();
    void usmFilter();

    void harrys();

    void changeTable();

private:
    Ui::MainWindow *ui;

    // in diesen Beiden Methoden sind Änderungen nötig bzw. sie dienen als
    // Vorlage für eigene Methoden.
    void generateControlPanels();

   // Ab hier technische Details die nicht für das Verständnis notwendig sind.
   void startLogging();

   void createActions();
   void updateActions();
   void scaleImage(double factor);
   void adjustScrollBar(QScrollBar *scrollBar, double factor);
   void renewLogging();

   double scaleFactor;
   QImage* image;
   QImage* grayScaledImage;
   QImage* tempImage;
   QImage* referenceImage;
   std::fstream logFile;

   tasktwo t2;
   taskthree t3;
   taskfour t4;
   taskfive t5;
   tasksix t6;

   double tlow;
   double thigh;
   double gaußSize;
   double sigma;
   bool edgeStarted;
};

#endif // MAINWINDOW_H
