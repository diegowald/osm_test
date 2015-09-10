#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <spatialite/sqlite3.h>
//#include <sqlite3.h>
#include <nodeassociatedtoway.h>

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
    void gatherCurrentPositionData(double X, double Y);
    void queryDatabase(double X, double Y);

    long  getNearestWayNode(double X, double Y);
    long getWayByNode(long nearestNode);

    QList<NodeAssociatedToWayPtr> getInfoNodes(long way_id);

private:
    Ui::MainWindow *ui;

    double minX;
    double minY;
    double maxX;
    double maxY;
    double currentX;
    double currentY;
    double elapsedTime;
    bool playing;

    QTimer *timer;
    sqlite3 *db;
};

#endif // MAINWINDOW_H
