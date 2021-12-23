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


    // 多轴控制显示
    gridLayout = new QGridLayout(ui->AxesGrpBox);
//    gridLayout->addLayout(ui->AxesGrpBox, 1,0,1,4);
//    for(int i = 0; i < 8 ; i++) {
//        DriveForm* driveForm = new DriveForm(this);
//        gridLayout->addWidget(driveForm, i / 4, i % 4);
//    }

//    ui->multiAxis->setLayout(gridLayout);

//    gridLayout->add
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


    ////////// MultiAxis ////////////////
    if(multiAxes.size() != connectDlg->getJointNum()) { //驱动器数量有变化
        multiAxes.clear();
        for(int i = 0; i < connectDlg->getJointNum(); i++) {
            multiAxes.push_back(new DriveForm(this));
            gridLayout->addWidget(multiAxes[i], i/4, i%4);
        }
    }
    else {
        for(int i =0; i < multiAxes.size(); i++) {
            multiAxes[i]->setName(connectDlg->getJointName(i));
            multiAxes[i]->setActualPosition(connectDlg->getJointPosition(i));
            multiAxes[i]->setActualVelocity(connectDlg->getJointVelocity(i));
            multiAxes[i]->setActualTorque(connectDlg->getJointTorque(i));
            multiAxes[i]->setLoadTorque(connectDlg->getJointLoad(i));
            multiAxes[i]->setAxisStatus(connectDlg->getJointStatus(i));
        }
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
        _enableState = true;
    }
    else {
        //Status指示灯
        ui->statusViz->setPixmap(QPixmap(":/res/light_red.png").scaled(100, 100, Qt::KeepAspectRatio));
        _enableState =false;
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

void RocosMainWindow::on_stopBtn_clicked()
{
    if(!_enableState) {
        return;
    }

    // TODO:
    double sign = ui->actualVel->text().toDouble() > 0 ? 1 : -1;
//    connectDlg->moveSingleAxis(drivePropId, ui->actualPos->text().toDouble() + sign* pow(_max_vel, 2)/(2*_max_acc), _max_vel, _max_acc, _max_jerk, -1);
    connectDlg->moveSingleAxis(drivePropId, ui->actualPos->text().toDouble(), _max_vel, _max_acc, _max_jerk, -1);
}

void RocosMainWindow::on_ptpRelPlus_clicked()
{
    if(!_enableState) {
        return;
    }

    double target_pos = ui->actualPos->text().toDouble() + ui->ptpRel->text().toDouble();

    connectDlg->moveSingleAxis(drivePropId, target_pos, _max_vel, _max_acc, _max_jerk, -1);

}

void RocosMainWindow::on_ptpRelMinus_clicked()
{
    if(!_enableState) {
        return;
    }

    double target_pos = ui->actualPos->text().toDouble() - ui->ptpRel->text().toDouble();

    connectDlg->moveSingleAxis(drivePropId, target_pos, _max_vel, _max_acc, _max_jerk, -1);
}


void RocosMainWindow::on_maxVel_textChanged(const QString &arg1)
{
    _max_vel = arg1.toDouble();
    std::cout << "Max vel: " << _max_vel << std::endl;

}

void RocosMainWindow::on_maxAcc_textChanged(const QString &arg1)
{
    _max_acc = arg1.toDouble();
    std::cout << "Max acc: " << _max_acc << std::endl;

}

void RocosMainWindow::on_maxJerk_textChanged(const QString &arg1)
{
    _max_jerk = arg1.toDouble();
    std::cout << "Max jerk: " << _max_jerk << std::endl;
}


void RocosMainWindow::on_ptpAbs1Move_clicked()
{
    if(!_enableState) {
        return;
    }

    double _ptpAbs1 = ui->ptpAbs1->text().toDouble();
    connectDlg->moveSingleAxis(drivePropId, _ptpAbs1, _max_vel, _max_acc, _max_jerk, -1);
}

void RocosMainWindow::on_ptpAbs2Move_clicked()
{
    if(!_enableState) {
        return;
    }

    double _ptpAbs2 = ui->ptpAbs2->text().toDouble();
    connectDlg->moveSingleAxis(drivePropId, _ptpAbs2, _max_vel, _max_acc, _max_jerk, -1);
}

void RocosMainWindow::on_enabledAll_clicked()
{
    connectDlg->powerOn();
}

void RocosMainWindow::on_disabledAll_clicked()
{
    connectDlg->powerOff();
}

void RocosMainWindow::on_ptpRelPlusAll_clicked()
{
    QVector<double> pos, max_vel, max_acc, max_jerk;

    for(int i = 0; i < multiAxes.size(); i++) {
        pos.push_back(connectDlg->getJointPosition(i) + multiAxes[i]->getPtpRelative());
        max_vel.push_back(multiAxes[i]->getMaxVel());
        max_acc.push_back(multiAxes[i]->getMaxAcc());
        max_jerk.push_back(multiAxes[i]->getMaxJerk());
    }

    connectDlg->moveMultiAxis(pos, max_vel, max_acc, max_jerk);

}

void RocosMainWindow::on_ptpRelMinusAll_clicked()
{
    QVector<double> pos, max_vel, max_acc, max_jerk;

    for(int i = 0; i < multiAxes.size(); i++) {
        pos.push_back(connectDlg->getJointPosition(i) - multiAxes[i]->getPtpRelative());
        max_vel.push_back(multiAxes[i]->getMaxVel());
        max_acc.push_back(multiAxes[i]->getMaxAcc());
        max_jerk.push_back(multiAxes[i]->getMaxJerk());
    }

    connectDlg->moveMultiAxis(pos, max_vel, max_acc, max_jerk);
}

void RocosMainWindow::on_ptpAbs1MoveAll_clicked()
{
    QVector<double> pos, max_vel, max_acc, max_jerk;

    for(int i = 0; i < multiAxes.size(); i++) {
        pos.push_back(multiAxes[i]->getPtpAbsolute());
        max_vel.push_back(multiAxes[i]->getMaxVel());
        max_acc.push_back(multiAxes[i]->getMaxAcc());
        max_jerk.push_back(multiAxes[i]->getMaxJerk());
    }

    connectDlg->moveMultiAxis(pos, max_vel, max_acc, max_jerk);
}

void RocosMainWindow::on_syncNone_clicked()
{
    connectDlg->setSync(0);
}

void RocosMainWindow::on_syncTime_clicked()
{
    connectDlg->setSync(1);
}

void RocosMainWindow::on_syncPhase_clicked()
{
    connectDlg->setSync(2);
}
