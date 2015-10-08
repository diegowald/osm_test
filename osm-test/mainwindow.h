#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "nodeassociatedtoway.h"
#include "osmhandler.h"
#include "waysignaldetector.h"
#include "forwardview.h"
#include "mapcache.h"
#include <QXmlStreamReader>
#include "gpscoordinate.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnSetup_released();

    void on_btnPlayStop_released();

    void on_moveLocation();

private:
    void refreshData();
    void gatherCurrentPositionData(double X, double Y, double speed);
    void queryDatabase(double X, double Y, double speed);
    void processDeltaTimes();

private:
    Ui::MainWindow *ui;

    double currentX;
    double currentY;
    double currentSpeed;
    double currentDirection;
    QString currentTime;
    double currentXPos;
    double currentYPos;
    double currentZPos;

    double prevX;
    double prevY;
    double elapsedTime;
    double lastRefresh;
    double maxTimeWithoutRefreshThreshold;
    bool playing;

    QTimer *_timer;
    OSMHandlerPtr _osmHandler;
    WaySignalDetectorPtr _signalDetector;
    MapCachePtr _mapCache;

    QXmlStreamReader xmlReader;
    QList<GPSCoordinatePtr> _coordinates;
};

#endif // MAINWINDOW_H
