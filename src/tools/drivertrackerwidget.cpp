#include "drivertrackerwidget.h"
#include "ui_drivertrackerwidget.h"

#include <QKeyEvent>

DriverTrackerWidget::DriverTrackerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DriverTrackerWidget)
{
    ui->setupUi(this);
}

DriverTrackerWidget::~DriverTrackerWidget()
{
    delete ui;
}

void DriverTrackerWidget::update()
{
    ui->driverRadar->update();
    ui->driverTracker->update();
}

void DriverTrackerWidget::setup()
{
    ui->driverRadar->setupDrivers();
    ui->driverTracker->setupDrivers();
}

void DriverTrackerWidget::loadSettings(QSettings *settings)
{
    ui->splitter->restoreState(settings->value("ui/tracker_splitter_pos").toByteArray());
    restoreGeometry(settings->value("ui/driver_tracker_geometry").toByteArray());
}

void DriverTrackerWidget::saveSettings(QSettings *settings)
{
    settings->setValue("ui/tracker_splitter_pos", ui->splitter->saveState());
    settings->setValue("ui/driver_tracker_geometry", saveGeometry());
}

void DriverTrackerWidget::on_pushButton_clicked()
{
    close();
}

void DriverTrackerWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        close();
}

void DriverTrackerWidget::exec()
{
    setWindowTitle("Driver tracker: " + EventData::getInstance().getEventInfo().eventName);
    show();
}