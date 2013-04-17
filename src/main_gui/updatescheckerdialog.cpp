#include "updatescheckerdialog.h"
#include "ui_updatescheckerdialog.h"

#include "../core/f1ltcore.h"
#include "../net/networksettings.h"

UpdatesCheckerDialog::UpdatesCheckerDialog(QWidget *parent) :
    QDialog(parent, Qt::Window),
    ui(new Ui::UpdatesCheckerDialog)
{
    ui->setupUi(this);
}

UpdatesCheckerDialog::~UpdatesCheckerDialog()
{
    delete ui;
}

void UpdatesCheckerDialog::checkForUpdates()
{
    req = QNetworkRequest(NetworkSettings::getInstance().getVersionUrl());
    req.setRawHeader("User-Agent","f1lt");

    if (NetworkSettings::getInstance().usingProxy())
        manager.setProxy(NetworkSettings::getInstance().getProxy());
    else
        manager.setProxy(QNetworkProxy::NoProxy);

    reply = manager.get(req);

    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

bool UpdatesCheckerDialog::newVersionIsAvailable(QString version)
{
    QString currVersion = F1LTCore::programVersion();

    if (currVersion != version)
    {
        QStringList lstOld = currVersion.split(".");
        QStringList lstNew = version.split(".");

        for (int i = 0; i < lstOld.size() && i < lstNew.size(); ++i)
        {
            int vOld = lstOld[i].toInt();
            int vNew = lstNew[i].toInt();

            if (vNew > vOld)
                return true;
        }
    }

    return false;
}

void UpdatesCheckerDialog::loadSettings(QSettings &settings)
{
    restoreGeometry(settings.value("ui/updatechecker_geometry").toByteArray());
}

void UpdatesCheckerDialog::saveSettings(QSettings &settings)
{
    settings.setValue("ui/updatechecker_geometry", saveGeometry());
}

void UpdatesCheckerDialog::finished()
{
    QString version = reply->readAll().constData();
    if (newVersionIsAvailable(version))
    {
        ui->textEdit->setText("<p align=\"center\"><h1>F1LT new version (" + version + ") is available!</h1></p>");
        ui->textEdit->append("<p align=\"center\"><img src=\":/ui_icons/icon.png\"/></p>");
        ui->textEdit->append("<p align=\"center\"><h3>You can download it at <a href=\"http://f1lt.pl\">F1LT home site</a>.</h3></p>");

        if (!isVisible())
            emit newVersionAvailable();
    }
    else
    {
        ui->textEdit->setText("<p align=\"center\"><h3>You already have the latest version installed.</h3></p>");
        ui->textEdit->append("<p align=\"center\"><img src=\":/ui_icons/icon.png\"/></p>");
    }
}

void UpdatesCheckerDialog::error(QNetworkReply::NetworkError)
{
}

void UpdatesCheckerDialog::show(bool check)
{
    QDialog::show();

    if (check)
    {
        ui->textEdit->setText("Checking for updates...");
        checkForUpdates();
    }
}

void UpdatesCheckerDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    accept();
}
