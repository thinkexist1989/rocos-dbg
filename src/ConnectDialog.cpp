#include "ConnectDialog.h"
#include "ui_ConnectDialog.h"

#include <QDebug>



ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);


    timerState = new QTimer(this);
    connect(timerState, &QTimer::timeout, this, &ConnectDialog::getRobotState); //获取机器人状态

}

ConnectDialog::~ConnectDialog()
{
    delete ui;
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

const QString ConnectDialog::getHardwareType()
{
    switch(response.robot_state().hw_state().hw_type()) {
    case rocos::HardwareState::HardwareType::HardwareState_HardwareType_HW_TYPE_UNKNOWN:
        return tr("Unkown");
    case rocos::HardwareState::HardwareType::HardwareState_HardwareType_HW_TYPE_SIM:
        return tr("Simulation");
    case rocos::HardwareState::HardwareType::HardwareState_HardwareType_HW_TYPE_ETHERCAT:
        return tr("EtherCAT");
    default:
        return "";

    }
}

const QString ConnectDialog::getJointStatus(int id)
{
    switch (response.robot_state().joint_states(id).status()) {
    case rocos::JointState_Status::JointState_Status_STATUS_FAULT:
        return tr("Fault");
    case rocos::JointState_Status::JointState_Status_STATUS_DISABLED:
        return tr("Disabled");
    case rocos::JointState_Status::JointState_Status_STATUS_ENABLED:
        return tr("Enabled");
    default:
        return "";
    }
}

void ConnectDialog::on_connectButton_clicked()
{
    _channel.reset();
    _channel = grpc::CreateChannel(QString("%1:%2").arg(ipAddress).arg(port).toStdString(), grpc::InsecureChannelCredentials());

    _stub.release();
    _stub = RobotService::NewStub(_channel);


    //测试一下连接否成功
    RobotStateRequest request;
    RobotStateResponse response;

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = _stub->ReadRobotState(&context, request, &response);

    if(status.ok()) {
        emit connectState(true);
        timerState->start(100);
    }
    else {
        emit connectState(false);
    }

}

void ConnectDialog::on_ipAddressEdit_textChanged(const QString &ip)
{
    ipAddress = ip;
//    std::cout << "ip" << std::endl;
}

void ConnectDialog::on_portEdit_textChanged(const QString &p)
{
    port = p.toInt();
//    std::cout << "port" << std::endl;
}


void ConnectDialog::getRobotState()
{
    RobotStateRequest request;

//    RobotStateResponse response;

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = _stub->ReadRobotState(&context, request, &response);
    if (status.ok()) {
        emit newStateComming();
        std::cout << "Joint state size: " << response.robot_state().joint_states_size() << std::endl;
        std::cout << "Joint state status: " << response.robot_state().joint_states(0).status() << std::endl;
        std::cout << "slave num: " << response.robot_state().hw_state().slave_num() << std::endl;
        std::cout << "hw type: " << response.robot_state().hw_state().hw_type() << std::endl;
        std::cout << "curr cycle time: " << response.robot_state().hw_state().current_cycle_time() << std::endl;
    }
}
