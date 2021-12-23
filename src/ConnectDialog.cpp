#include "ConnectDialog.h"
#include "ui_ConnectDialog.h"

#include <QDebug>



ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);

    ipAddress = ui->ipAddressEdit->text();
    port = ui->portEdit->text().toInt();


    timerState = new QTimer(this);
    connect(timerState, &QTimer::timeout, this, &ConnectDialog::getRobotState); //获取机器人状态

}

ConnectDialog::~ConnectDialog()
{
    shutdown();
//    usleep(100000);
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

void ConnectDialog::powerOn()
{
    RobotCommandRequest request;
    RobotCommandResponse response;

    request.mutable_command()->mutable_enabled();

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = _stub->WriteRobotCommmand(&context, request, &response);

    if(status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    }
    else {
        std::cerr << "Send command Error" << std::endl;
    }
}

void ConnectDialog::powerOff()
{
    RobotCommandRequest request;
    RobotCommandResponse response;

    request.mutable_command()->mutable_disabled();

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = _stub->WriteRobotCommmand(&context, request, &response);

    if(status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    }
    else {
        std::cout << "Send command Error" << std::endl;
    }
}

void ConnectDialog::powerOn(int id)
{
    RobotCommandRequest request;
    RobotCommandResponse response;

    request.mutable_command()->mutable_single_axis_command()->mutable_enabled()->set_id(id);

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = _stub->WriteRobotCommmand(&context, request, &response);

    if(status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    }
    else {
        std::cout << "Send command Error" << std::endl;
    }
}

void ConnectDialog::powerOff(int id)
{
    RobotCommandRequest request;
    RobotCommandResponse response;

    request.mutable_command()->mutable_single_axis_command()->mutable_disabled()->set_id(id);

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = _stub->WriteRobotCommmand(&context, request, &response);

    if(status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    }
    else {
        std::cerr << "Send command Error" << std::endl;
    }
}

void ConnectDialog::setJointMode(int id, int mode)
{
    RobotCommandRequest request;
    RobotCommandResponse response;

    request.mutable_command()->mutable_single_axis_command()->mutable_mode()->set_id(id);
    request.mutable_command()->mutable_single_axis_command()->mutable_mode()->set_value(static_cast<rocos::Mode>(mode));

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = _stub->WriteRobotCommmand(&context, request, &response);

    if(status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    }
    else {
        std::cerr << "Send command Error" << std::endl;
    }
}

void ConnectDialog::shutdown()
{
    timerState->stop();
    while(_channel.use_count())
        _channel.reset();
//    _stub.reset();
//    delete _stub.release();
    _isConnected = false;
    emit connectState(false);
}

void ConnectDialog::setSync(int sync)
{
    RobotCommandRequest request;
    RobotCommandResponse response;

    request.mutable_command()->mutable_multi_axis_command()->mutable_sync()->set_value(static_cast<rocos::Synchronization>(sync));

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = _stub->WriteRobotCommmand(&context, request, &response);

    if(status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    }
    else {
        std::cerr << "Send command Error" << std::endl;
    }
}

void ConnectDialog::moveSingleAxis(int id, double pos, double max_vel, double max_acc, double max_jerk, double least_time)
{
    RobotCommandRequest request;
    RobotCommandResponse response;

    auto move = request.mutable_command()->mutable_single_axis_command()->mutable_move();
    move->set_id(id);
    move->set_pos(pos);
    move->set_max_vel(max_vel);
    move->set_max_acc(max_acc);
    move->set_max_jerk(max_jerk);
    move->set_least_time(least_time);

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = _stub->WriteRobotCommmand(&context, request, &response);

    if(status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    }
    else {
        std::cerr << "Send command Error" << std::endl;
    }
}

void ConnectDialog::moveMultiAxis(const QVector<double> &pos, const QVector<double> &max_vel, const QVector<double> &max_acc, const QVector<double> &max_jerk, double least_time)
{
    RobotCommandRequest request;
    RobotCommandResponse response;

    auto move = request.mutable_command()->mutable_multi_axis_command()->mutable_move();

    for(int i = 0; i < pos.size(); i++) {
        move->add_target_pos(pos[i]);
        move->add_max_vel(max_vel[i]);
        move->add_max_acc(max_acc[i]);
        move->add_max_jerk(max_jerk[i]);
    }

    move->set_least_time(least_time);

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = _stub->WriteRobotCommmand(&context, request, &response);

    if(status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    }
    else {
        std::cerr << "Send command Error" << std::endl;
    }
}

void ConnectDialog::on_connectButton_clicked()
{
    if(_isConnected)
        return;

//    if(_channel)
//        _channel.reset();
    _channel = grpc::CreateChannel(QString("%1:%2").arg(ipAddress).arg(port).toStdString(), grpc::InsecureChannelCredentials());
//    std::cout << _channel->GetState(true) <<std::endl;

//    if(_stub) {
////       delete _stub.release();
//        _stub.reset();
//    }

    _stub = RobotService::NewStub(_channel);


    //测试一下连接否成功
    RobotStateRequest request;
    RobotStateResponse response;
    Status status;

    for(int i = 0; i < 1; i++) { //TODO:之前发现如果退出程序，重启只发送一次会卡住，所以多发几次，但这个问题需要仔细研究一下
        ClientContext context; //这个只能使用一次，每次请求都需要重新创建
        status = _stub->ReadRobotState(&context, request, &response);
    }

    if(status.ok()) {
        emit connectState(true);
        timerState->start(100);
        this->close();
        _isConnected = true;
    }
    else {
        emit connectState(false);
        _isConnected = false;
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
    response.Clear();

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = _stub->ReadRobotState(&context, request, &response);
    if (status.ok()) {
        emit newStateComming();
//        std::cout << "Joint state size: " << response.robot_state().joint_states_size() << std::endl;
//        std::cout << "Joint state status: " << response.robot_state().joint_states(0).status() << std::endl;
//        std::cout << "slave num: " << response.robot_state().hw_state().slave_num() << std::endl;
//        std::cout << "hw type: " << response.robot_state().hw_state().hw_type() << std::endl;
//        std::cout << "curr cycle time: " << response.robot_state().hw_state().current_cycle_time() << std::endl;
    }
    else {
        std::cout << status.error_code() << ": " << status.error_message() << std::endl;
    }
}
