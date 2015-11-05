#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtConcurrent/QtConcurrent>
#include <QIODevice>

//#include <QVariant>
//#include <QSqlQuery>
//#include <QSqlError>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    elapsedTime = 0.;
    currentX = 0;
    currentY = 0;
    prevX = 0;
    prevY = 0;
    currentSpeed = 0;
    currentDirection = 0;
    currentTime = "";
    currentXPos = 0;
    currentYPos = 0;
    currentZPos = 0;
    lastRefresh = -1e100;
    maxTimeWithoutRefreshThreshold = 10;

    playing = false;
    refreshData();
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &MainWindow::on_moveLocation);

    _osmHandler = OSMHandlerPtr::create("map.sqlite");
    _signalDetector = WaySignalDetectorPtr::create(_osmHandler);
    _mapCache = MapCachePtr::create(_osmHandler);

    //QFile file("driving_20150922.xml");
    //QFile file("HY_0011_20151028_101141_N_2CH.xml");
    QFile file("HY_0012_20151028_101241_N_2CH.xml");

    file.open(QFile::ReadOnly);
    xmlReader.setDevice(&file);

    xmlReader.readNextStartElement();

    if (xmlReader.isStartElement() && xmlReader.name() == "root")
    {
        xmlReader.readNextStartElement();
        if (xmlReader.name() == "driving" || xmlReader.name() == "region")
        {
            while (xmlReader.readNextStartElement())
            {
                if (xmlReader.name() == "data")
                {
                    QString time;
                    QString latitude;
                    QString longitude;
                    QString x;
                    QString y;
                    QString z;
                    QString wheel;
                    QString speed;
                    while (xmlReader.readNextStartElement())
                    {
                        if (xmlReader.name() == "time")
                        {
                            time = xmlReader.readElementText();
                        }
                        else if (xmlReader.name() == "latitude")
                        {
                            latitude = xmlReader.readElementText();
                        }
                        else if (xmlReader.name() == "longitude")
                        {
                            longitude = xmlReader.readElementText();
                        }
                        else if (xmlReader.name() == "x" || xmlReader.name() == "eCompass_x")
                        {
                            x = xmlReader.readElementText();
                        }
                        else if (xmlReader.name() == "y" || xmlReader.name() == "eCompass_y")
                        {
                            y = xmlReader.readElementText();
                        }
                        else if (xmlReader.name() == "z" || xmlReader.name() == "eCompass_z")
                        {
                            z = xmlReader.readElementText();
                        }
                        else if (xmlReader.name() == "wheel")
                        {
                            wheel = xmlReader.readElementText();
                        }
                        else if (xmlReader.name() == "speed")
                        {
                            speed = xmlReader.readElementText();
                        }
                    }
                    _coordinates.append(GPSCoordinatePtr::create(time,
                                                                 latitude, longitude,
                                                                 x, y, z, wheel, speed));
                }
            }
        }
    }
    processDeltaTimes();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::processDeltaTimes()
{
    if (_coordinates.count() > 0)
    {
        GPSCoordinatePtr coord = _coordinates.at(0);
        QString s = coord->time().replace("[", "").replace("]", "");
        QTime t = QTime::fromString(s, "hh:mm:ss");
        for (int i = 1; i < _coordinates.count(); ++i)
        {
            GPSCoordinatePtr nextCoord = _coordinates.at(i);
            s = nextCoord->time().replace("[", "").replace("]", "");
            QTime t1 = QTime::fromString(s, "hh:mm:ss");
            int delta = t1.msecsSinceStartOfDay() - t.msecsSinceStartOfDay();
            coord->setDelayToNextCoord(delta <= 0 ? 10 : delta);
            coord = nextCoord;
            t = t1;
        }
    }
}

void MainWindow::on_btnSetup_released()
{
    // Since it's a dummy test. Let's hardcode this part.

    currentDirection = 0;
    currentSpeed = 0;
    elapsedTime = 0.;
    refreshData();
}

void MainWindow::refreshData()
{
    ui->txtLongitude->setText(QString::number(currentX));
    ui->txtLatitude->setText(QString::number(currentY));
    ui->btnPlayStop->setText(playing ? "Stop" : "Play");

    ui->lblSpeed->setText(QString::number(currentSpeed));
    ui->lblDirection->setText(QString::number(currentDirection));
    ui->lblTime->setText(currentTime);
    ui->lblX->setText(QString::number(currentXPos));
    ui->lblY->setText(QString::number(currentYPos));
    ui->lblZ->setText(QString::number(currentZPos));
}

void MainWindow::on_btnPlayStop_released()
{
    if (!playing)
    {
        playing = true;
        _timer->singleShot(10, this, SLOT(on_moveLocation()));
    }
    else
    {
        playing = false;
        _timer->stop();
        elapsedTime = 0;
    }
    refreshData();
}

void MainWindow::on_moveLocation()
{
    if (prevX != currentX)
        prevX = currentX;
    if (prevY != currentY)
        prevY = currentY;

    currentX = _coordinates.at(elapsedTime)->longitude();
    currentY = _coordinates.at(elapsedTime)->latitude();

    //qDebug() << "prevX " << prevX << " prevY " << prevY;
    //qDebug() << "currentX " << currentX << " currentY " << currentY;

    currentDirection = _coordinates.at(elapsedTime)->wheel();
    currentSpeed = _coordinates.at(elapsedTime)->speed();

    currentTime = _coordinates.at(elapsedTime)->time();
    currentXPos = _coordinates.at(elapsedTime)->x();
    currentYPos = _coordinates.at(elapsedTime)->y();
    currentZPos = _coordinates.at(elapsedTime)->z();
    int delayToNext = _coordinates.at(elapsedTime)->delayToNextCoord();
    // Remove this
    if (currentSpeed == 0)
        delayToNext = 1;
    //end of Remove This
    elapsedTime++;
    if (elapsedTime >= _coordinates.count())
    {
        elapsedTime = 0;
    }
    if (((elapsedTime - lastRefresh) > maxTimeWithoutRefreshThreshold) ||
            ((currentSpeed > 0) && ((prevX != currentX) || (prevY != currentY))))
    {
        gatherCurrentPositionData(currentX, currentY, currentSpeed);
        lastRefresh = elapsedTime;
    }
    refreshData();
    _timer->singleShot(delayToNext, this, SLOT(on_moveLocation()));
}


void MainWindow::gatherCurrentPositionData(double X, double Y, double speed)
{
    //QFuture<void> future = QtConcurrent::run(this, &MainWindow::queryDatabase, X, Y);
    queryDatabase(X, Y, speed);
}

void MainWindow::queryDatabase(double X, double Y, double speed)
{
    double direction = 0;
    double deltaX = currentX - prevX;
    double deltaY = currentY - prevY;

    direction = 3 * 3.141592654 / 2 + atan2(deltaY, deltaX);
    //direction = (currentDirection -90) * 3.141592654 / 180.;
    //direction = 3.141592654 / 2 - direction;
    direction = (direction > 2. * 3.141592654) ? direction - 2. * 3.141592654 : direction;
    direction = (direction < 0) ? direction + 2. * 3.141592654 : direction;
    direction = speed > 0 ? direction : 0;


    QList<NodeAssociatedToWayPtr> nodes = _signalDetector->getUpcommingSignals(X, Y, direction, speed);
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

    WayPtr way = _signalDetector->getCurrentWay(X, Y, direction, speed);
    double orientation = 0;
    QPair<double, double> snappedPos;
    if (!way.isNull())
    {
        orientation = way->getOrientation(X, Y, direction);
        snappedPos = way->projectPoint(X, Y);
        qDebug() << X << ", " << Y << "==> " << snappedPos.first << ", " << snappedPos.second;
    }
    QList<WayPtr> intersectionWays = _signalDetector->getIntersectionWays(intersections);
    double maxDist = _signalDetector->getMaxDistance();

    QList<WayPtr> features = _mapCache->getLinearFeatures(X, Y, 1.5 * maxDist);
    QList<NodeAssociatedToWayPtr> pts = _mapCache->getPointFeatures(X, Y, 1.5 * maxDist);
    intersectionWays = _mapCache->nearestWays(X, Y, 1.5 * maxDist);
    /*ui->forwardViewWidget->setMaxDistance(maxDist / 3);
    ui->forwardViewWidget->setVehicleCoordinates(X, Y);
    ui->forwardViewWidget->setWays(intersectionWays);
    ui->forwardViewWidget->setVehicleDirection(direction);
    ui->forwardViewWidget->setLinearFeatures(features);
    ui->forwardViewWidget->setPointFeatures(pts);
    ui->forwardViewWidget->setRotation(orientation);
    ui->forwardViewWidget->setSelectedWay(way.isNull() ? 0 : way->id());
    ui->forwardViewWidget->drawVehicle(false);
    ui->forwardViewWidget->repaint();*/

    
    //QList<WayPtr> features = _mapCache->getLinearFeatures(X, Y, 1.5 * maxDist);
    //QList<NodeAssociatedToWayPtr> pts = _mapCache->getPointFeatures(X, Y, 1.5 * maxDist);
    intersectionWays = _mapCache->nearestWays(X, Y, 1.5 * maxDist);
    ui->mapWidget->setMaxDistance(maxDist / 5);
    ui->mapWidget->setVehicleCoordinates(X, Y, snappedPos.first, snappedPos.second);
    ui->mapWidget->setWays(intersectionWays);
    ui->mapWidget->setVehicleDirection(direction);
    ui->mapWidget->setLinearFeatures(features);
    ui->mapWidget->setPointFeatures(pts);
    ui->mapWidget->repaint();

    ui->forwardWidget2->setWays(features);
    ui->forwardWidget2->setSelectedWay(way.isNull() ? 0 : way->id());
    ui->forwardWidget2->setVehicleDirection(direction);
    ui->forwardWidget2->setRotation(orientation);
    ui->forwardWidget2->setPointFeatures(nodes);
    ui->forwardWidget2->repaint();
}
