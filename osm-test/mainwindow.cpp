#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <QVariant>
#include <sqlite3.h>
#include <iostream>

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

    if (sqlite3_open("map.sqlite", &db) == SQLITE_OK)
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
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSetup_released()
{
    // Since it's a dummy test. Let's hardcode this part.
    minX = -62.2932;
    minY = -38.7319;
    maxX = -62.2579;
    maxY = -38.7132;
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
    currentY+= delta;
    delta = (maxX - minX) / 100;
    currentX += delta;
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


long MainWindow::getNearestWayNode(double X, double Y)
{
    sqlite3_stmt *statement;

    QString sql =
            " select node_id, abs(x - %1) + abs(y- %2) as dst "
            " from osm_nodes where node_id in "
            " (select node_id from osm_way_refs where way_id in "
            " (select way_id from osm_way_tags where k = 'highway')) "
            " order by dst limit 1;";

    QString s = sql.arg(X).arg(Y);

    if ( sqlite3_prepare(db, s.toStdString().c_str(), -1, &statement, 0 ) == SQLITE_OK )
    {
        int ctotal = sqlite3_column_count(statement);
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                for ( int i = 0; i < ctotal; i++ )
                {
                    std::string s = (char*)sqlite3_column_text(statement, i);

                    QString res = QString::fromStdString(s);
                    return res.toLong();

                }
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                break;
            }
        }
    }
    return -1;
}

long MainWindow::getWayByNode(long nearestNode)
{
    sqlite3_stmt *statement;

    QString sql =
            " select way_id from osm_way_refs where node_id = %1;";


    QString s = sql.arg(nearestNode);

    if ( sqlite3_prepare(db, s.toStdString().c_str(), -1, &statement, 0 ) == SQLITE_OK )
    {
        int ctotal = sqlite3_column_count(statement);
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                for ( int i = 0; i < ctotal; i++ )
                {
                    std::string s = (char*)sqlite3_column_text(statement, i);

                    QString res = QString::fromStdString(s);
                    return res.toLong();

                }
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                break;
            }
        }
    }
    return -1;

}

QList<NodeAssociatedToWayPtr> MainWindow::getInfoNodes(long way_id)
{
    QMap<QString, NodeAssociatedToWayPtr> result;

    sqlite3_stmt *statement;

    QString sql =
            " select * from osm_node_tags where node_id in (select node_id from osm_way_refs where way_id = %1);";



    QString s = sql.arg(way_id);

    if ( sqlite3_prepare(db, s.toStdString().c_str(), -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                std::string node_id = (char*)sqlite3_column_text(statement, 0);
                std::string sub = (char*)sqlite3_column_text(statement, 1);
                std::string key = (char*)sqlite3_column_text(statement, 2);
                std::string value = (char*)sqlite3_column_text(statement, 3);

                QString aux = QString::fromStdString(node_id);
                if (!result.contains(aux))
                {
                    NodeAssociatedToWayPtr node = NodeAssociatedToWayPtr::create();
                    result[aux] = node;
                }
                result[aux]->addKeyValue(QString::fromStdString(key), QString::fromStdString(value));
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                break;
            }
        }
    }

    return result.values();
}

void MainWindow::queryDatabase(double X, double Y)
{
    int nearestNode = getNearestWayNode(X, Y);

    int nearestWay = getWayByNode(nearestNode);

    QList<NodeAssociatedToWayPtr> nodes = getInfoNodes(nearestWay);
    ui->tableWidget->setRowCount(0);


    foreach (NodeAssociatedToWayPtr node, nodes)
    {
        int row = ui->tableWidget->rowCount() + 1;

        ui->tableWidget->setRowCount(row);
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(node->X()));
        ui->tableWidget->setItem(row -1, 0, item);

        item = new QTableWidgetItem(QString::number(node->Y()));
        ui->tableWidget->setItem(row -1, 1, item);

        item = new QTableWidgetItem(node->toString());
        ui->tableWidget->setItem(row -1, 2, item);
        //row++;
    }
}
