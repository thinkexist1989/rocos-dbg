#include "rocosmainwindow.h"
#include "./ui_rocosmainwindow.h"

#include <QListWidget>

RocosMainWindow::RocosMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RocosMainWindow)
{
    ui->setupUi(this);

    aboutDlg = new AboutDialog(this);
    connectDlg = new ConnectDialog(this);

    connect(connectDlg, &ConnectDialog::newStateComming, this, &RocosMainWindow::updateRobotState);
    connect(connectDlg, &ConnectDialog::connectState, this, [=](bool isConnected){
        if(isConnected) {
            ui->actionConnect->setText(tr("Disconnect"));
            ui->actionConnect->setIcon(QIcon(":/res/connected.png"));
            _isConnected = true;
        }
        else {
            ui->actionConnect->setText(tr("Connect"));
            ui->actionConnect->setIcon(QIcon(":/res/disconnected.png"));
            _isConnected = false;
        }
    });

    //Status指示灯
    ui->statusViz->setPixmap(QPixmap(":/res/light_red.png").scaled(100, 100, Qt::KeepAspectRatio));
    ui->statusViz->setDisabled(true);

    QPixmap pixmap(":/res/light_red.png");
    ui->digState1->setPixmap(pixmap.scaled(20, 20, Qt::KeepAspectRatio));
    ui->digState2->setPixmap(pixmap.scaled(20, 20, Qt::KeepAspectRatio));
    ui->digState3->setPixmap(pixmap.scaled(20, 20, Qt::KeepAspectRatio));
    ui->digState4->setPixmap(pixmap.scaled(20, 20, Qt::KeepAspectRatio));
    ui->digState5->setPixmap(pixmap.scaled(20, 20, Qt::KeepAspectRatio));
    ui->digState6->setPixmap(pixmap.scaled(20, 20, Qt::KeepAspectRatio));

}

RocosMainWindow::~RocosMainWindow()
{
    delete ui;
}

void RocosMainWindow::updateRobotState()
{
    ////////Hardware State////////////
    ui->hwTable->setItem(0, 1, new QTableWidgetItem(connectDlg->getHardwareType()));
    ui->hwTable->setItem(1, 1, new QTableWidgetItem(QString::number(connectDlg->getMinCyclicTime())));
    ui->hwTable->setItem(2, 1, new QTableWidgetItem(QString::number(connectDlg->getMaxCyclicTime())));
    ui->hwTable->setItem(3, 1, new QTableWidgetItem(QString::number(connectDlg->getCurrCylicTime())));

    /////// Detected Drives ////////////
    static int prevJntNum = 0;
    if(prevJntNum != connectDlg->getJointNum()) {
        prevJntNum = connectDlg->getJointNum();
        ui->driveList->clear();
        for(int i = 0; i < prevJntNum; i++) {
            ui->driveList->addItem(connectDlg->getJointName(i));
        }
    }

    ////// Drive Properties////////////
    if(drivePropId != -1) {
        ui->driveProp->setItem(0, 1, new QTableWidgetItem(connectDlg->getJointStatus(drivePropId)));
        ui->driveProp->setItem(1, 1, new QTableWidgetItem(QString::number(connectDlg->getJointPosition(drivePropId))));
        ui->driveProp->setItem(2, 1, new QTableWidgetItem(QString::number(connectDlg->getJointVelocity(drivePropId))));
        ui->driveProp->setItem(3, 1, new QTableWidgetItem(QString::number(connectDlg->getJointTorque(drivePropId))));
        ui->driveProp->setItem(4, 1, new QTableWidgetItem(QString::number(connectDlg->getJointTorque(drivePropId))));
        ui->driveProp->setItem(5, 1, new QTableWidgetItem(QString::number(connectDlg->getJointLoad(drivePropId))));
    }

    ////// Single Axis Motion//////////
    if(drivePropId != -1) {
        ui->actualPos->setText(QString::number(connectDlg->getJointPosition(drivePropId)));
        ui->actualVel->setText(QString::number(connectDlg->getJointVelocity(drivePropId)));
        ui->actualTor->setText(QString::number(connectDlg->getJointTorque(drivePropId)));
        ui->load->setText(QString::number(connectDlg->getJointLoad(drivePropId)));

        if(connectDlg->getJointStatus(drivePropId).contains("Enabled")) {
            setDriveStatusViz(true);
            ui->enableBtn->setText(tr("  Enabled"));
            ui->enableBtn->setIcon(QIcon(":/res/light_green.png"));
        }
        else {
            setDriveStatusViz(false);
            ui->enableBtn->setText(tr("  Disabled"));
            ui->enableBtn->setIcon(QIcon(":/res/light_red.png"));
        }

        ui->axisState->setText(connectDlg->getJointStatus(drivePropId));

    }


}

void RocosMainWindow::resizeEvent(QResizeEvent *event)
{

//    QPixmap pixmap(":/res/light_red.png");
//    ui->statusViz->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatioByExpanding));
    //    QMainWindow::resizeEvent(event);
}

void RocosMainWindow::setDriveStatusViz(bool isEnabled)
{
    ui->statusViz->setEnabled(true);
    if(isEnabled) {
        //Status指示灯
        ui->statusViz->setPixmap(QPixmap(":/res/light_green.png").scaled(100, 100, Qt::KeepAspectRatio));
    }
    else {
        //Status指示灯
        ui->statusViz->setPixmap(QPixmap(":/res/light_red.png").scaled(100, 100, Qt::KeepAspectRatio));
    }
}


void RocosMainWindow::on_actionAbout_triggered()
{
    aboutDlg->show();
}

void RocosMainWindow::on_actionConnect_triggered()
{
    if(_isConnected) {
        connectDlg->shutdown();
    }
    else{
        connectDlg->show();
    }
}

void RocosMainWindow::on_driveList_currentRowChanged(int currentRow)
{
    //重新选择驱动器，则更新相应驱动器id
    drivePropId = currentRow;
    ui->tabWidget->setTabText(0, tr("Single Axis Motion [%1]").arg(ui->driveList->currentItem()->text()));

}

void RocosMainWindow::on_enableBtn_clicked()
{
    if(drivePropId == -1 || _isConnected == false) {
        return;
    }

    if(connectDlg->getJointStatus(drivePropId).contains("Enabled")) {
        connectDlg->powerOff(drivePropId);

    }
    else {
        connectDlg->powerOn(drivePropId);
    }
}
