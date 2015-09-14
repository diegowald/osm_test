#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
//#include <QVariant>
//#include <QSqlQuery>
//#include <QSqlError>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    minX = 0;
    minY = 0;
    maxX = 0;
    maxY = 0;
    elapsedTime = 0.;
    currentX = minX;
    currentY = minY;
    playing = false;
    refreshData();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::on_moveLocation);

    _osmHandler = OSMHandlerPtr::create("map.sqlite");
    _signalDetector = WaySignalDetectorPtr::create(_osmHandler);

    /*    if (sqlite3_open("map.sqlite", &db) == SQLITE_OK)
    {
    }

    if (!sqlite3_enable_load_extension(db, 1))
    {
        std::string e = sqlite3_errmsg(db);
        QString e1 = QString::fromStdString(e);

    }

    sqlite3_stmt *statement;
    QString sql = "SELECT load_extension(\"/usr/lib/x86_64-linux-gnu/libspatialite.so.5\")";
    if (sqlite3_prepare(db, sql.toStdString().c_str(), -1, &statement, 0) == SQLITE_OK)
    {

    }
    else
    {
        std::string e = sqlite3_errmsg(db);
        QString e1 = QString::fromStdString(e);
        //qDebug() << e1;
    }*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSetup_released()
{
    // Since it's a dummy test. Let's hardcode this part.
    minX = -62.3155083;
    minY = -38.7235012;
    maxX = -62.2728802;
    maxY = -38.6909236;
    elapsedTime = 0.;
    refreshData();
}

void MainWindow::refreshData()
{
    ui->txtLongFrom->setText(QString::number(minX));
    ui->txtLatFrom->setText(QString::number(minY));
    ui->txtLongTo->setText(QString::number(maxX));
    ui->txtLatTo->setText(QString::number(maxY));
    ui->txtLongitude->setText(QString::number(currentX));
    ui->txtLatitude->setText(QString::number(currentY));
    ui->btnPlayStop->setText(playing ? "Stop" : "Play");
}

void MainWindow::on_btnPlayStop_released()
{
    if (!playing)
    {
        playing = true;
        currentY = minY;
        currentX = minX;
        timer->start(1000);
    }
    else
    {
        playing = false;
        timer->stop();
        elapsedTime = 0;
    }
    refreshData();
}

void MainWindow::on_moveLocation()
{
    double delta = (maxY - minY) / 100;
    qDebug() << delta;
    currentY+= delta;
    delta = (maxX - minX) / 100;
    qDebug() << delta;
    currentX += delta;
    elapsedTime++;
    if (elapsedTime > 100)
    {
        currentY = minY;
        elapsedTime = 0;
    }
    qDebug() << "x= " << currentX << " y= " << currentY;
    gatherCurrentPositionData(currentX, currentY);
    refreshData();
}


void MainWindow::gatherCurrentPositionData(double X, double Y)
{
    //QFuture<void> future = QtConcurrent::run(this, &MainWindow::queryDatabase, X, Y);
    queryDatabase(X, Y);
}

void MainWindow::queryDatabase(double X, double Y)
{
    double direction = -1.;
    QList<NodeAssociatedToWayPtr> nodes = _signalDetector->getUpcommingSignals(X, Y, direction);
    ui->nodeInformation->setRowCount(0);

    foreach (NodeAssociatedToWayPtr node, nodes)
    {
        int row = ui->nodeInformation->rowCount() + 1;

        ui->nodeInformation->setRowCount(row);
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(node->X()));
        ui->nodeInformation->setItem(row -1, 0, item);

        item = new QTableWidgetItem(QString::number(node->Y()));
        ui->nodeInformation->setItem(row -1, 1, item);

        item = new QTableWidgetItem(node->toString());
        ui->nodeInformation->setItem(row -1, 2, item);
        //row++;
    }

    QList<NodeAssociatedToWayPtr> intersections = _signalDetector->getUpcommingIntersections(X, Y, direction);
    ui->intersections->setRowCount(0);

    foreach (NodeAssociatedToWayPtr node, intersections)
    {
        int row = ui->intersections->rowCount() + 1;

        ui->intersections->setRowCount(row);
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(node->X()));
        ui->intersections->setItem(row -1, 0, item);

        item = new QTableWidgetItem(QString::number(node->Y()));
        ui->intersections->setItem(row -1, 1, item);

        item = new QTableWidgetItem(node->toString());
        ui->intersections->setItem(row -1, 2, item);
        //row++;
    }

    ui->forwardFrame->updateScene(X, Y, nodes, intersections);
}
