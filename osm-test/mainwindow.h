#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <sqlite3.h>

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
