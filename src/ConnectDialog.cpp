#include "ConnectDialog.h"
#include "ui_ConnectDialog.h"

#include <QDebug>



ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);

    tcpSocket = new QTcpSocket;

    connect(tcpSocket, &QTcpSocket::disconnected, this, [=](){connectedToRobot(false);});
    connect(tcpSocket, &QTcpSocket::readyRead, this, &ConnectDialog::parseRobotMsg); //解析机器人状态

    timerState = new QTimer(this);
    connect(timerState, &QTimer::timeout, this, &ConnectDialog::getRobotState); //获取机器人状态

}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::on_connectButton_clicked()
{
    connectedToRobot(true);
}

void ConnectDialog::on_ipAddressEdit_textChanged(const QString &ip)
{
    ipAddress = ip;
}

void ConnectDialog::on_portEdit_textChanged(const QString &p)
{
    port = p.toInt();
}

void ConnectDialog::connectedToRobot(bool con)
{

}

void ConnectDialog::setRobotEnabled(bool enabled)
{

}

void ConnectDialog::setJointSpeedScaling(double factor)
{

}

void ConnectDialog::getJointSpeedScaling()
{

}

void ConnectDialog::setCartesianSpeedScaling(double factor)
{
}

void ConnectDialog::getCartesianSpeedScaling()
{

}

void ConnectDialog::setToolSpeedScaling(double factor)
{
}

void ConnectDialog::getToolSpeedScaling()
{

}

void ConnectDialog::startScript(QString script)
{

}

void ConnectDialog::stopScript()
{

}

void ConnectDialog::pauseScript()
{

}

void ConnectDialog::continueScript()
{

}

void ConnectDialog::setZeroCalibration()
{

}

bool ConnectDialog::event(QEvent *event)
{
    if(event->type() == QEvent::ActivationChange) {
        if(QApplication::activeWindow() != this) {
            this->close();
        }
    }

    return QDialog::event(event);
}

void ConnectDialog::getRobotState()
{

}

void ConnectDialog::parseRobotMsg()
{

}

void ConnectDialog::jointJogging(int id, int dir)
{

}

void ConnectDialog::cartesianJogging(int frame, int freedom, int dir)
{

}
