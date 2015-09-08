#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <sqlite3.h>
#include <QSqlDriver>

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

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("map.sqlite");

    qDebug() << db.open();

    QVariant v = db.driver()->handle();

    qDebug() << v.typeName();
    qDebug() << v.data();
    if (v.isValid() && qstrcmp(v.typeName(), "sqlite3*")==0)
    {
        // v.data() returns a pointer to the handle
        sqlite3 *handle = *static_cast<sqlite3 **>(v.data());
        //sqlite3 *handle = (sqlite3*)(v.data());
        if (handle != 0)
        { // check that it is not NULL
            sqlite3_enable_load_extension(handle, 1);
        }
        else
        {
            qDebug() << "handle is null....";
        }
    }



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSetup_released()
{
    // Since it's a dummy test. Let's hardcode this part.
    minX = -61.759002;
    minY = -32.548983;
    maxX = -61.759002;
    maxY = -32.548983 + 0.020833;
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
        currentY = minX;
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
    double delta = 0.020833 / 100;
    currentY+= delta;
    elapsedTime++;
    if (elapsedTime > 100)
    {
        currentY = minY;
        elapsedTime = 0;
    }
    gatherCurrentPositionData(currentX, currentY);
    refreshData();
}


void MainWindow::gatherCurrentPositionData(double X, double Y)
{
    QFuture<void> future = QtConcurrent::run(this, &MainWindow::queryDatabase, X, Y);
}

void MainWindow::queryDatabase(double X, double Y)
{
    QString sql =
            " select * from osm_way_tags where osm_way_tags.way_id in ( "
            " select way_id from osm_way_refs "
            " inner join osm_nodes on osm_way_refs.node_id = osm_nodes.node_id "
            "  where "
            " way_id in ( "
            " select way_id from osm_way_tags "
            " where k = 'highway') "
            " and "
            " Distance(osm_nodes.geometry, PointFromText('POINT ( %1 %2)')) < 5 "
            " )";
    QString s = sql.arg(X).arg(Y);
    //qDebug() << s;
    QSqlQuery query(s);

    /*if (query.exec())
    {*/
        while (query.next())
        {

        }
    /*}
    else
    {*/
        QString e = query.lastError().text();
        //qDebug() << e;
    //}
}
