/*******************************************************************************
 *                                                                             *
 *   F1LT - unofficial Formula 1 live timing application                       *
 *   Copyright (C) 2012-2013  Mariusz Pilarek (pieczaro@gmail.com)             *
 *                                                                             *
 *   This program is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by      *
 *   the Free Software Foundation, either version 3 of the License, or         *
 *   (at your option) any later version.                                       *
 *                                                                             *
 *   This program is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *   GNU General Public License for more details.                              *
 *                                                                             *
 *   You should have received a copy of the GNU General Public License         *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                             *
 *******************************************************************************/


#include "drivertrackerinfo.h"
#include "../core/eventdata.h"
#include <QPainter>

#include "../core/colorsmanager.h"

DriverTrackerInfo::DriverTrackerInfo(QWidget *parent) :
    QWidget(parent)
{
    labelBig = QPixmap(":/ui_icons/label-big.png");
    labelInfoLong = QPixmap(":/ui_icons/label-info-long.png");
}

void DriverTrackerInfo::setup()
{
    ImagesFactory::getInstance().getCarThumbnailsFactory().loadCarThumbnails(240, true);

    QSize size = QSize(labelBig.width(), labelBig.height() + 3*labelInfoLong.height() + 6);
    setMinimumSize(size);
}

void DriverTrackerInfo::paintEvent(QPaintEvent *)
{
    if (driverData != 0)
    {
        QPainter p;
        p.begin(this);

        paintDriverInfo(&p);
        paintLapsInfo(&p);
        paintGapsInfo(&p);

        p.end();
    }
}

void DriverTrackerInfo::paintDriverInfo(QPainter *p)
{
    int x = (width() - labelBig.width())/2;

    QPixmap car = ImagesFactory::getInstance().getCarThumbnailsFactory().getCarThumbnail(driverData->getNumber(), 240);
    int carX = x + (labelBig.width() - car.width())/2 - 2;
    int carY = (71 - car.height())/2;


    QColor color = ColorsManager::getInstance().getCarColor(driverData->getNumber());
    int numX = x + 105;
    int numY = 70;
    p->setBrush(color);
    p->drawRect(numX, numY, 30, 20);

    p->drawPixmap(x, 0, labelBig);
    p->drawPixmap(carX, carY, car);

    numX = x + 20;
    numY = 100;

    QString txt = QString::number(driverData->getPosition());

    if (driverData->getPosition() < 10)
        txt = " " + txt;

    p->setFont(QFont("Arial", 20, 100));
    p->setPen(QColor(ColorsManager::getInstance().getColor(LTPackets::BACKGROUND)));

    if (driverData->isInPits())
        p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::PIT)));

    p->drawText(numX, numY, txt);

    txt = driverData->getDriverName();
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
    p->setFont(QFont("Arial", 10, 100));
    numX = x + 140;
    numY = 85;
    p->drawText(numX, numY, txt);

    numX = x + 120;
    numY = 105;
    txt = SeasonData::getInstance().getTeamName(driverData->getNumber());
    p->drawText(numX, numY, txt);



    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::BACKGROUND)));
    txt = QString::number(driverData->getNumber());
    if (driverData->getNumber() < 10)
        txt = " " + txt;
    numX = x + 115;
    numY = 85;
    p->drawText(numX, numY, txt);


    numX = x + 63;
    numY = 74;
    QPixmap helmet = ImagesFactory::getInstance().getHelmetsFactory().getHelmet(driverData->getNumber(), 30);
    p->drawPixmap(numX, numY, helmet);
}

void DriverTrackerInfo::paintLapsInfo(QPainter *p)
{
    int x = (width() - labelBig.width())/2;
    int y = labelBig.height() + 2;

    p->drawPixmap(x, y, labelInfoLong);

    int nX = x + 20;
    int nY = y + 15;

    p->setFont(QFont("Arial", 10, 100));
    p->setPen(QColor(ColorsManager::getInstance().getColor(LTPackets::BACKGROUND)));

    //===== Current lap label=======
    QString txt = "Current lap:";
    p->drawText(nX, nY, txt);

    nX = x + 115;
    LapData lap = driverData->getLastLap();

//    if (EventData::getInstance().getEventType() == LTPackets::QUALI_EVENT)
//        lap = driverData->getSessionRecords().getBestQualiLap(EventData::getInstance().getQualiPeriod());


    //===== Current lap time=======
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));

    if (EventData::getInstance().getEventType() != LTPackets::RACE_EVENT && !driverData->getLapData().isEmpty())
        txt = driverData->getLapData().last().getTime().toString();

    else
        txt = lap.getTime().toString();


    p->drawText(nX, nY, txt);

    //===== Current sector 1 time=======
    p->setPen(ColorsManager::getInstance().getDefaultColor(driverData->getColorData().sectorColor(1)));
    nX += 65;
    txt = lap.getSectorTime(1).toString();
    p->drawText(nX, nY, txt);

    //===== Current sector 2 time=======
    p->setPen(ColorsManager::getInstance().getDefaultColor(driverData->getColorData().sectorColor(2)));
    nX += 35;
    txt = lap.getSectorTime(2).toString();
    p->drawText(nX, nY, txt);

    //===== Current sector 3 time=======
    p->setPen(ColorsManager::getInstance().getDefaultColor(driverData->getColorData().sectorColor(3)));
    nX += 35;
    txt = lap.getSectorTime(3).toString();
    p->drawText(nX, nY, txt);

    y = labelBig.height() + labelInfoLong.height() + 4;

    p->drawPixmap(x, y, labelInfoLong);

    nX = x + 20;
    nY = y + 15;

    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::BACKGROUND)));
    txt = "Best lap:";
    p->drawText(nX, nY, txt);

    //===== Best lap time=======
    lap = driverData->getSessionRecords().getBestLap();
    txt = lap.getTime().toString();
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::GREEN)));
    nX = x + 115;
    p->drawText(nX, nY, txt);

    //===== Best sector 1 time=======
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
    if (driverData->getSessionRecords().getBestSector(1).second == lap.getLapNumber())
        p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::GREEN)));
    nX += 65;
    txt = lap.getSectorTime(1).toString();
    p->drawText(nX, nY, txt);

    //===== Best sector 2 time=======
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
    if (driverData->getSessionRecords().getBestSector(2).second == lap.getLapNumber())
        p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::GREEN)));
    nX += 35;
    txt = lap.getSectorTime(2).toString();
    p->drawText(nX, nY, txt);

    //===== Best sector 3 time=======
    p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE)));
    if (driverData->getSessionRecords().getBestSector(3).second == lap.getLapNumber())
        p->setPen(QColor(ColorsManager::getInstance().getDefaultColor(LTPackets::GREEN)));
    nX += 35;
    txt = lap.getSectorTime(3).toString();
    p->drawText(nX, nY, txt);

}

void DriverTrackerInfo::paintGapsInfo(QPainter *p)
{
    int x = (width() - labelBig.width())/2;
    int y = labelBig.height() + 2 * labelInfoLong.height() + 6;

    p->drawPixmap(x, y, labelInfoLong);

    int nX = x + 20;
    int nY = y + 15;

    p->setFont(QFont("Arial", 10, 100));
    p->setPen(QColor(ColorsManager::getInstance().getColor(LTPackets::BACKGROUND)));

    int prevPos = driverData->getPosition() - 1;
    int nextPos = driverData->getPosition() + 1;

    QColor prevColor = ColorsManager::getInstance().getDefaultColor(LTPackets::RED);
    QColor nextColor = ColorsManager::getInstance().getDefaultColor(LTPackets::GREEN);

    if (prevPos == 0)
    {
        prevPos = 2;
        nextPos = 3;

        prevColor = ColorsManager::getInstance().getDefaultColor(LTPackets::GREEN);
    }

    if (nextPos > EventData::getInstance().getDriversData().size())
    {
        nextPos = driverData->getPosition() - 1;
        prevPos = driverData->getPosition() - 2;

        nextColor = ColorsManager::getInstance().getDefaultColor(LTPackets::RED);

    }
    p->drawText(nX, nY, QString("Gaps:"));

//    y += labelInfoShort.height() + 2;
//    p->drawPixmap(x, y, labelInfoShort);
//    p->setPen(QColor(SeasonData::getInstance().getColor(LTPackets::BACKGROUND)));

//    nX = x + 20;
//    p->drawText(nX, y+15, QString("Gap to P%1").arg(nextPos));

    QString prevGap, nextGap;
    DriverData *prevDrv = EventData::getInstance().getDriverDataByPosPtr(prevPos);
    DriverData *nextDrv = EventData::getInstance().getDriverDataByPosPtr(nextPos);


    if (prevDrv != 0 && nextDrv != 0)
    {
        if (EventData::getInstance().getEventType() == LTPackets::RACE_EVENT)
        {
            prevGap = EventData::getInstance().calculateInterval(*driverData, *prevDrv, -1);
            nextGap = EventData::getInstance().calculateInterval(*driverData, *nextDrv, -1);
        }
        else if (EventData::getInstance().getEventType() == LTPackets::PRACTICE_EVENT)
        {
            prevGap = DriverData::calculateGap(driverData->getSessionRecords().getBestLap().getTime(), prevDrv->getSessionRecords().getBestLap().getTime());
            nextGap = DriverData::calculateGap(driverData->getSessionRecords().getBestLap().getTime(), nextDrv->getSessionRecords().getBestLap().getTime());

            if (driverData->getPosition() != 1)
                prevGap = "+" + prevGap;

            if (driverData->getPosition() == EventData::getInstance().getDriversData().size())
                nextGap = "+" + nextGap;
        }
        else
        {
            int qPeriod = EventData::getInstance().getQualiPeriod();
            prevGap = DriverData::calculateGap(driverData->getSessionRecords().getBestQualiLap(qPeriod).getTime(), prevDrv->getSessionRecords().getBestQualiLap(qPeriod).getTime());
            nextGap = DriverData::calculateGap(driverData->getSessionRecords().getBestQualiLap(qPeriod).getTime(), nextDrv->getSessionRecords().getBestQualiLap(qPeriod).getTime());

            if (driverData->getPosition() != 1)
                prevGap = "+" + prevGap;

            if (driverData->getPosition() == EventData::getInstance().getDriversData().size())
                nextGap = "+" + nextGap;
        }

        p->setPen(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE));
        nX = x + 115;
        p->drawText(nX, nY, QString("P%1:").arg(prevPos));

        p->setPen(prevColor);
        nX += 32;
        p->drawText(nX, nY, prevGap);

        p->setPen(ColorsManager::getInstance().getDefaultColor(LTPackets::WHITE));
        nX += 55;
        p->drawText(nX, nY, QString("P%1:").arg(nextPos));

        p->setPen(nextColor);
        nX += 32;
        p->drawText(nX, nY, nextGap);
    }

}

