#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
#include <iostream>
#include <cmath>

#include "taskone.h"
#include "tasktwo.h"
#include <vector>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    image = NULL;
    tempImage=NULL;
    grayScaledImage = NULL;

    ui->imageLabel->setBackgroundRole(QPalette::Base);
    ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    createActions();
    generateControlPanels();
    tlow = -1;
    thigh = -1;
    gaußSize = -1;
    sigma = -1;
    edgeStarted = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

/******************** SIGNALS *********************/

void MainWindow::taskOneRedCross(){
    if(image!=NULL){
        taskone t(*image,ui->sliderTask1_01->value());
        updateImageDisplay();
        logFile << "Rotes Kreuz wurde gemalt" << std::endl;
        renewLogging();
    }
}

void MainWindow::changeBrightness(){
    if(image!=NULL){
        t2.changeBrightness(*image,*tempImage,ui->sliderTask2_01->value());
        *grayScaledImage = image->convertToFormat(QImage::Format_Grayscale8);
        t2.drawHistogramm(*grayScaledImage,*ui->lblHistogrammTask2_01,*ui->lblHistogrammTask2_02);
        updateImageDisplay();
    }
}

void MainWindow::changeContrast(){
    if(image!=NULL){
        t2.changeContrast(*image,*tempImage,ui->sliderTask2_02->value());
        *grayScaledImage = image->convertToFormat(QImage::Format_Grayscale8);
        t2.drawHistogramm(*grayScaledImage,*ui->lblHistogrammTask2_01,*ui->lblHistogrammTask2_02);
        updateImageDisplay();
    }
}

void MainWindow::getAvgBrightnessAndContrast(){
    if(image!=NULL){
        std::cout <<"asdas"<< std::endl;
        *grayScaledImage = image->convertToFormat(QImage::Format_Grayscale8);
        float average = 0.0;
        float varianz = 0.0;
        t2.calculateAvg(*grayScaledImage,average,varianz);
        t2.drawHistogramm(*grayScaledImage,*ui->lblHistogrammTask2_01,*ui->lblHistogrammTask2_02);

        QString s = QString::number(average);
        QString v = QString::number(varianz);

        ui->lblTask2_07->setText("Mittlere Helligkeit: "+ s + ", Varianz: "+v);
        logFile << "Mittlere Helligkeit und Varianz berechnet" << std::endl;
        renewLogging();
    }
}


void MainWindow::autoContrast(){
    if(image!=NULL){
        *grayScaledImage = image->convertToFormat(QImage::Format_Grayscale8);
        t2.autoContrast(*image,*grayScaledImage,ui->dSpinBoxTask2_01->value(),ui->dSpinBoxTask2_01->value());
        t2.drawHistogramm(*image,*ui->lblHistogrammTask2_01,*ui->lblHistogrammTask2_02);
        updateImageDisplay();
    }
}

void MainWindow::linearAusgleich(){
    if(image!=NULL){
        t3.linearAusgleich(*image);
        *grayScaledImage = image->convertToFormat(QImage::Format_Grayscale8);
        t3.drawHistogramm(*grayScaledImage,*ui->lblHistogrammTask3_01,*ui->lblHistogrammTask3_02);
        updateImageDisplay();
    }
}

void MainWindow::gauss(){
    if(image!=NULL){
        *grayScaledImage = image->convertToFormat(QImage::Format_Grayscale8);
        t3.gauss(*image,*grayScaledImage,ui->spinBoxTask3_01->value());
        *grayScaledImage = image->convertToFormat(QImage::Format_Grayscale8);
        t3.drawHistogramm(*grayScaledImage,*ui->lblHistogrammTask3_01,*ui->lblHistogrammTask3_02);
        updateImageDisplay();
    }
}

void MainWindow::linearFilter(){
    if(image!=NULL && ui->spinBoxTask4_01->value()%2==1){
        // Create
        std::vector<  std::vector<int> > vec(ui->spinBoxTask4_01->value(), std::vector<int>(ui->spinBoxTask4_01->value()));


        int length = ui->spinBoxTask4_01->value();

        for(int i = 0; i<length; i++){
            for(int j = 0; j<length; j++){
                bool ok;
                int num = ui->tableTask4_01->item(i,j)->text().toInt(&ok);
                if(ok==true){
                    vec[i][j] = num;
                }else{
                    vec[i][j] = 0;
                    logFile << "Eine Zelle ist keine Zahl" << std::endl;
                    renewLogging();
                }
            }
        }

        t4.linearFilter(*image,*tempImage,vec,length);

        *grayScaledImage = image->convertToFormat(QImage::Format_Grayscale8);
        t3.drawHistogramm(*grayScaledImage,*ui->lblHistogrammTask3_01,*ui->lblHistogrammTask3_02);
        updateImageDisplay();
    }else if(ui->spinBoxTask4_01->value()%2==0){
        logFile << "Groeße muss ungerade sein!" << std::endl;
        renewLogging();
    }

}

void MainWindow::linearFilterZero(){
    if(image!=NULL && ui->spinBoxTask4_01->value()%2==1){
        // Create
        std::vector<  std::vector<int> > vec(ui->spinBoxTask4_01->value(), std::vector<int>(ui->spinBoxTask4_01->value()));


        int length = ui->spinBoxTask4_01->value();

        for(int i = 0; i<length; i++){
            for(int j = 0; j<length; j++){
                bool ok;
                int num = ui->tableTask4_01->item(i,j)->text().toInt(&ok);
                if(ok==true){
                    vec[i][j] = num;
                }else{
                    vec[i][j] = 0;
                    logFile << "Eine Zelle ist keine Zahl" << std::endl;
                    renewLogging();
                }
            }
        }

        t4.linearFilterZero(*image,*tempImage,vec,length);

        *grayScaledImage = image->convertToFormat(QImage::Format_Grayscale8);
        t3.drawHistogramm(*grayScaledImage,*ui->lblHistogrammTask3_01,*ui->lblHistogrammTask3_02);
        updateImageDisplay();
    }else if(ui->spinBoxTask4_01->value()%2==0){
        logFile << "Groeße muss ungerade sein!" << std::endl;
        renewLogging();
    }

}

void MainWindow::linearFilterConst(){
    if(image!=NULL && ui->spinBoxTask4_01->value()%2==1){
        // Create
        std::vector<  std::vector<int> > vec(ui->spinBoxTask4_01->value(), std::vector<int>(ui->spinBoxTask4_01->value()));


        int length = ui->spinBoxTask4_01->value();

        for(int i = 0; i<length; i++){
            for(int j = 0; j<length; j++){
                bool ok;
                int num = ui->tableTask4_01->item(i,j)->text().toInt(&ok);
                if(ok==true){
                    vec[i][j] = num;
                }else{
                    vec[i][j] = 0;
                    logFile << "Eine Zelle ist keine Zahl" << std::endl;
                    renewLogging();
                }
            }
        }

        t4.linearFilterConst(*image,*tempImage,vec,length);

        *grayScaledImage = image->convertToFormat(QImage::Format_Grayscale8);
        t3.drawHistogramm(*grayScaledImage,*ui->lblHistogrammTask3_01,*ui->lblHistogrammTask3_02);
        updateImageDisplay();
    }else if(ui->spinBoxTask4_01->value()%2==0){
        logFile << "Groeße muss ungerade sein!" << std::endl;
        renewLogging();
    }
}

void MainWindow::linearFilterMirror(){
    if(image!=NULL && ui->spinBoxTask4_01->value()%2==1){
        // Create
        std::vector<  std::vector<int> > vec(ui->spinBoxTask4_01->value(), std::vector<int>(ui->spinBoxTask4_01->value()));


        int length = ui->spinBoxTask4_01->value();

        for(int i = 0; i<length; i++){
            for(int j = 0; j<length; j++){
                bool ok;
                int num = ui->tableTask4_01->item(i,j)->text().toInt(&ok);
                if(ok==true){
                    vec[i][j] = num;
                }else{
                    vec[i][j] = 0;
                    logFile << "Eine Zelle ist keine Zahl" << std::endl;
                    renewLogging();
                }
            }
        }

        t4.linearFilterMirror(*image,*tempImage,vec,length);

        *grayScaledImage = image->convertToFormat(QImage::Format_Grayscale8);
        t3.drawHistogramm(*grayScaledImage,*ui->lblHistogrammTask3_01,*ui->lblHistogrammTask3_02);
        updateImageDisplay();
    }else if(ui->spinBoxTask4_01->value()%2==0){
        logFile << "Groeße muss ungerade sein!" << std::endl;
        renewLogging();
    }
}

void MainWindow::changeTable(){
    if(ui->spinBoxTask4_01->value()%2==1){
        ui->tableTask4_01->setRowCount(ui->spinBoxTask4_01->value());
        ui->tableTask4_01->setColumnCount(ui->spinBoxTask4_01->value());
    }else{
        logFile << "Groeße muss ungerade sein!" << std::endl;
        renewLogging();
    }

}

void MainWindow::cannyEdge(){
    if(image!=NULL && ui->spinBoxTask5_01->value()%2==1){
        if(!edgeStarted){
            tlow = ui->dSpinBoxTask5_01->value();
            thigh = ui->dSpinBoxTask5_02->value();
            gaußSize = ui->spinBoxTask5_01->value();
            sigma = ui->dSpinBoxTask5_03->value();
            t5.applyGauss(*image,*tempImage,sigma,gaußSize);
            t5.applyRadiant(*image);
            t5.applyMaximum(tlow);
            t5.applyCannyEdge(*image,thigh);
            edgeStarted = true;
        }else{
            if(tlow != ui->dSpinBoxTask5_01->value() && gaußSize == ui->spinBoxTask5_01->value() && sigma == ui->dSpinBoxTask5_03->value()){
                tlow = ui->dSpinBoxTask5_01->value();
                thigh = ui->dSpinBoxTask5_02->value();
                t5.applyMaximum(tlow);
                t5.applyCannyEdge(*image,thigh);
            }else if(thigh != ui->dSpinBoxTask5_02->value() && gaußSize == ui->spinBoxTask5_01->value() && sigma == ui->dSpinBoxTask5_03->value()){
                tlow = ui->dSpinBoxTask5_01->value();
                thigh = ui->dSpinBoxTask5_02->value();
                gaußSize = ui->spinBoxTask5_01->value();
                sigma = ui->dSpinBoxTask5_03->value();
                t5.applyCannyEdge(*image,thigh);
            }else if(gaußSize != ui->spinBoxTask5_01->value() || sigma != ui->dSpinBoxTask5_03->value()){
                tlow = ui->dSpinBoxTask5_01->value();
                thigh = ui->dSpinBoxTask5_02->value();
                gaußSize = ui->spinBoxTask5_01->value();
                sigma = ui->dSpinBoxTask5_03->value();
                t5.applyGauss(*image,*tempImage,sigma,gaußSize);
                t5.applyRadiant(*image);
                t5.applyMaximum(tlow);
                t5.applyCannyEdge(*image,thigh);
            }
        }


        updateImageDisplay();
    }

}

void MainWindow::usmFilter(){
    if(image!=NULL && ui->spinBoxTask5_02->value()%2==1){
        t5.applyUSM(*image,*tempImage,ui->dSpinBoxTask5_05->value(),ui->spinBoxTask5_02->value(),ui->dSpinBoxTask5_04->value());
        updateImageDisplay();
    }
}

void MainWindow::harrys(){
    if(image!=NULL && ui->spinBoxTask5_02->value()%2==1){
        t6.applyHarris(*image,*tempImage,ui->dSpinBoxTask6_02->value(),ui->dSpinBoxTask6_01->value(),ui->spinBoxTask6_01->value(),ui->spinBoxTask6_02->value());
        updateImageDisplay();
    }
}


/*------------------------------------*/

void MainWindow::generateControlPanels()
{

    QObject::connect(ui->btnTask1_01, SIGNAL (clicked()), this, SLOT (taskOneRedCross()));
    QObject::connect(ui->sliderTask2_01, SIGNAL (valueChanged(int)), this, SLOT (changeBrightness()));
    QObject::connect(ui->sliderTask2_02, SIGNAL (valueChanged(int)), this, SLOT (changeContrast()));
    QObject::connect(ui->btnTask2_01, SIGNAL (clicked()), this, SLOT (getAvgBrightnessAndContrast()));
    QObject::connect(ui->btnTask2_02, SIGNAL (clicked()), this, SLOT (autoContrast()));
    QObject::connect(ui->btnTask3_01, SIGNAL (clicked()), this, SLOT (linearAusgleich()));
    QObject::connect(ui->btnTask3_02, SIGNAL (clicked()), this, SLOT (openReference()));
    QObject::connect(ui->btnTask3_03, SIGNAL (clicked()), this, SLOT (gauss()));
    QObject::connect(ui->btnTask4_01, SIGNAL (clicked()), this, SLOT (linearFilter()));
    QObject::connect(ui->btnTask4_02, SIGNAL (clicked()), this, SLOT (linearFilterZero()));
    QObject::connect(ui->btnTask4_03, SIGNAL (clicked()), this, SLOT (linearFilterConst()));
    QObject::connect(ui->btnTask4_04, SIGNAL (clicked()), this, SLOT (linearFilterMirror()));
    QObject::connect(ui->spinBoxTask4_01, SIGNAL (valueChanged(int)), this, SLOT (changeTable()));
    QObject::connect(ui->btnTask5_01, SIGNAL (clicked()), this, SLOT (cannyEdge()));
    QObject::connect(ui->btnTask5_02, SIGNAL (clicked()), this, SLOT (usmFilter()));
    QObject::connect(ui->btnTask6_01, SIGNAL (clicked()), this, SLOT (harrys()));
}


/************************************************************************/

void MainWindow::startLogging()
{
    //LogFile
    logFile.open("log.txt", std::ios::out);
    logFile << "Logging: \n" << std::endl;
}

void MainWindow::renewLogging()
{
    QFile file("log.txt"); // Create a file handle for the file named
    QString line;
    file.open(QIODevice::ReadOnly); // Open the file

    QTextStream stream( &file ); // Set the stream to read from myFile
    ui->logBrowser->clear();
    while(!stream.atEnd()){

        line = stream.readLine(); // this reads a line (QString) from the file
        ui->logBrowser->append(line);
    }
}

bool MainWindow::loadFile(const QString &fileName)
{
    if(image!=NULL)
    {
        delete image;
        delete tempImage;
        delete grayScaledImage;
        image=NULL;
        tempImage=NULL;
        grayScaledImage = NULL;
    }


    image = new QImage(fileName);
    tempImage = new QImage(fileName);
    grayScaledImage = new QImage(fileName);
    *grayScaledImage = grayScaledImage->convertToFormat(QImage::Format_Grayscale8);



    if (image->isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());
        ui->imageLabel->setPixmap(QPixmap());
        ui->imageLabel->adjustSize();
        return false;
    }

    scaleFactor = 1.0;

    updateImageDisplay();

    ui->printAct->setEnabled(true);
    ui->fitToWindowAct->setEnabled(true);
    updateActions();

    if (!ui->fitToWindowAct->isChecked())
        ui->imageLabel->adjustSize();

    setWindowFilePath(fileName);
    logFile << "geladen: " << fileName.toStdString().c_str()  << std::endl;
    renewLogging();

    return true;
}

void MainWindow::updateImageDisplay()
{
    ui->imageLabel->setPixmap(QPixmap::fromImage(*image));
}


void MainWindow::open()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QFileDialog dialog(this, tr("Open File"),
                       picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void MainWindow::openReference(){
    if(image!=NULL){
        QStringList mimeTypeFilters;
        foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
            mimeTypeFilters.append(mimeTypeName);
        mimeTypeFilters.sort();
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        QFileDialog dialog(this, tr("Open File"),
                           picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setMimeTypeFilters(mimeTypeFilters);
        dialog.selectMimeTypeFilter("image/jpeg");

        logFile << "Führe Ausgleich mit Referenzen durch, bitte warten..." << std::endl;
        renewLogging();
        while (dialog.exec() == QDialog::Accepted && !t3.referenceAusgleich(dialog.selectedFiles().first(),*image,*tempImage)) {}
        *grayScaledImage = image->convertToFormat(QImage::Format_Grayscale8);
        t3.drawHistogramm(*grayScaledImage,*ui->lblHistogrammTask3_01,*ui->lblHistogrammTask3_02);
        updateImageDisplay();
    }else{
        logFile << "Ein Bild muss erst geladen werden" << std::endl;
        renewLogging();
    }

}

void MainWindow::print()
{
    /*
    Q_ASSERT(ui->imageLabel->pixmap());
    #if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
        QPrintDialog dialog(&printer, this);
        if (dialog.exec()) {
            QPainter painter(&printer);
            QRect rect = painter.viewport();
            QSize size = imageLabel->pixmap()->size();
            size.scale(rect.size(), Qt::KeepAspectRatio);
            painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
            painter.setWindow(imageLabel->pixmap()->rect());
            painter.drawPixmap(0, 0, *imageLabel->pixmap());
        }
    #endif
    */
}

void MainWindow::zoomIn()
{
    scaleImage(1.25);
}

void MainWindow::zoomOut()
{
    scaleImage(0.8);
}

void MainWindow::normalSize()
{
    ui->imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void MainWindow::fitToWindow()
{
    bool fitToWindow = ui->fitToWindowAct->isChecked();
    ui->scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
               "and QScrollArea to display an image. QLabel is typically used "
               "for displaying a text, but it can also display an image. "
               "QScrollArea provides a scrolling view around another widget. "
               "If the child widget exceeds the size of the frame, QScrollArea "
               "automatically provides scroll bars. </p><p>The example "
               "demonstrates how QLabel's ability to scale its contents "
               "(QLabel::scaledContents), and QScrollArea's ability to "
               "automatically resize its contents "
               "(QScrollArea::widgetResizable), can be used to implement "
               "zooming and scaling features. </p><p>In addition the example "
               "shows how to use QPainter to print an image.</p>"));
}

void MainWindow::updateActions()
{
    ui->zoomInAct->setEnabled(!ui->fitToWindowAct->isChecked());
    ui->zoomOutAct->setEnabled(!ui->fitToWindowAct->isChecked());
    ui->normalSizeAct->setEnabled(!ui->fitToWindowAct->isChecked());
}

void MainWindow::scaleImage(double factor)
{
    Q_ASSERT(ui->imageLabel->pixmap());
    scaleFactor *= factor;
    ui->imageLabel->resize(scaleFactor * ui->imageLabel->pixmap()->size());

    adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);


    ui->zoomInAct->setEnabled(scaleFactor < 10.0);
    ui->zoomOutAct->setEnabled(scaleFactor > 0.05);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void MainWindow::createActions()
{
    ui->openAct->setShortcut(tr("Ctrl+O"));
    connect(ui->openAct, SIGNAL(triggered()), this, SLOT(open()));

    ui->printAct->setShortcut(tr("Ctrl+P"));
    ui->printAct->setEnabled(false);
    connect(ui->printAct, SIGNAL(triggered()), this, SLOT(print()));

    ui->exitAct->setShortcut(tr("Ctrl+Q"));
    connect(ui->exitAct, SIGNAL(triggered()), this, SLOT(close()));

    ui->zoomInAct->setShortcut(tr("Ctrl++"));
    ui->zoomInAct->setEnabled(false);
    connect(ui->zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    ui->zoomOutAct->setShortcut(tr("Ctrl+-"));
    ui->zoomOutAct->setEnabled(false);
    connect(ui->zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    ui->normalSizeAct->setShortcut(tr("Ctrl+S"));
    ui->normalSizeAct->setEnabled(false);
    connect(ui->normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    ui->fitToWindowAct->setEnabled(false);
    ui->fitToWindowAct->setCheckable(true);
    ui->fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(ui->fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    connect(ui->aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    connect(ui->aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}


