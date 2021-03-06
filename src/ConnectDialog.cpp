#include "ConnectDialog.h"
#include "ui_ConnectDialog.h"

#include <QDebug>


ConnectDialog::ConnectDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ConnectDialog) {
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);

    ip_address_ = ui->ipAddressEdit->text();
    port_ = ui->portEdit->text().toInt();


    timer_state_ = new QTimer(this);
    connect(timer_state_, &QTimer::timeout, this, &ConnectDialog::getRobotState); //获取机器人状态

}

ConnectDialog::~ConnectDialog() {
    shutdown();
//    usleep(100000);
    delete ui;
}

bool ConnectDialog::event(QEvent *event) {
    if (event->type() == QEvent::ActivationChange) {
        if (QApplication::activeWindow() != this) {
            this->close();
        }
    }

    return QDialog::event(event);
}

QString ConnectDialog::getHardwareType() {
    switch (robot_state_response_.robot_state().hw_state().hw_type()) {
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

QString ConnectDialog::getJointStatus(int id) {
    switch (robot_state_response_.robot_state().joint_states(id).status()) {
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

void ConnectDialog::powerOn() {
    RobotCommandRequest request;
    RobotCommandResponse response;

    request.mutable_command()->mutable_enabled();

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = stub_->WriteRobotCommmand(&context, request, &response);

    if (status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    } else {
        std::cerr << "Send command Error" << std::endl;
    }
}

void ConnectDialog::powerOff() {
    RobotCommandRequest request;
    RobotCommandResponse response;

    request.mutable_command()->mutable_disabled();

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = stub_->WriteRobotCommmand(&context, request, &response);

    if (status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    } else {
        std::cout << "Send command Error" << std::endl;
    }
}

void ConnectDialog::powerOn(int id) {
    RobotCommandRequest request;
    RobotCommandResponse response;

    request.mutable_command()->mutable_single_axis_command()->mutable_enabled()->set_id(id);

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = stub_->WriteRobotCommmand(&context, request, &response);

    if (status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    } else {
        std::cout << "Send command Error" << std::endl;
    }
}

void ConnectDialog::powerOff(int id) {
    RobotCommandRequest request;
    RobotCommandResponse response;

    request.mutable_command()->mutable_single_axis_command()->mutable_disabled()->set_id(id);

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = stub_->WriteRobotCommmand(&context, request, &response);

    if (status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    } else {
        std::cerr << "Send command Error" << std::endl;
    }
}

void ConnectDialog::setJointMode(int id, int mode) {
    RobotCommandRequest request;
    RobotCommandResponse response;

    request.mutable_command()->mutable_single_axis_command()->mutable_mode()->set_id(id);
    request.mutable_command()->mutable_single_axis_command()->mutable_mode()->set_value(static_cast<rocos::Mode>(mode));

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = stub_->WriteRobotCommmand(&context, request, &response);

    if (status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    } else {
        std::cerr << "Send command Error" << std::endl;
    }
}

void ConnectDialog::shutdown() {
    timer_state_->stop();
    while (channel_.use_count())
        channel_.reset();
//    stub_.reset();
//    delete stub_.release();
    is_connected_ = false;
    emit connectState(false);
}

void ConnectDialog::setSync(int sync) {
    RobotCommandRequest request;
    RobotCommandResponse response;

    sync_ = sync;

    request.mutable_command()->mutable_multi_axis_command()->mutable_sync()->set_value(
            static_cast<rocos::Synchronization>(sync_));

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = stub_->WriteRobotCommmand(&context, request, &response);

    if (status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    } else {
        std::cerr << "Send command Error" << std::endl;
    }
}

void
ConnectDialog::moveSingleAxis(int id, double pos, double max_vel, double max_acc, double max_jerk, double least_time) {
    RobotCommandRequest request;
    RobotCommandResponse response;

    auto move = request.mutable_command()->mutable_single_axis_command()->mutable_move();
    move->set_id(id);
    move->set_pos(pos);
    move->set_max_vel(max_vel);
    move->set_max_acc(max_acc);
    move->set_max_jerk(max_jerk);
    move->set_least_time(least_time);

    move->set_raw_data(use_raw_data);

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = stub_->WriteRobotCommmand(&context, request, &response);

    if (status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    } else {
        std::cerr << "Send command Error" << std::endl;
    }
}

void
ConnectDialog::moveMultiAxis(const QVector<double> &pos, const QVector<double> &max_vel, const QVector<double> &max_acc,
                             const QVector<double> &max_jerk, double least_time) {
    RobotCommandRequest request;
    RobotCommandResponse response;

    auto move = request.mutable_command()->mutable_multi_axis_command()->mutable_move();

    for (int i = 0; i < pos.size(); i++) {
        move->add_target_pos(pos[i]);
        move->add_max_vel(max_vel[i]);
        move->add_max_acc(max_acc[i]);
        move->add_max_jerk(max_jerk[i]);
    }

    move->set_least_time(least_time);

    move->set_raw_data(use_raw_data);

    move->set_sync(static_cast<rocos::Synchronization>(sync_)); //设置同步

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = stub_->WriteRobotCommmand(&context, request, &response);

    if (status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    } else {
        std::cerr << "Send command Error" << std::endl;
    }
}

void ConnectDialog::on_connectButton_clicked() {
    if (is_connected_)
        return;

//    if(channel_)
//        channel_.reset();
    channel_ = grpc::CreateChannel(QString("%1:%2").arg(ip_address_).arg(port_).toStdString(),
                                   grpc::InsecureChannelCredentials());
//    std::cout << channel_->GetState(true) <<std::endl;

//    if(stub_) {
////       delete stub_.release();
//        stub_.reset();
//    }

    stub_ = RobotService::NewStub(channel_);


    //获取一下RobotInfo,顺便测试一下连接否成功
    RobotInfoRequest request;
//    RobotInfoResponse response;
    Status status;

     //之前发现如果退出程序，重启只发送一次会卡住，所以多发几次，但这个问题需要仔细研究一下
    // 后来发现不是这个原因造成的,是由于channel_没有释放造成的
    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    status = stub_->ReadRobotInfo(&context, request, &robot_info_response_);
//    }

    if (status.ok()) {
        emit connectState(true);
        timer_state_->start(100);
        this->close();
        is_connected_ = true;

        cnt_per_unit.resize(robot_info_response_.robot_info().joint_infos_size());
        torque_per_unit.resize(robot_info_response_.robot_info().joint_infos_size());
        load_per_unit.resize(robot_info_response_.robot_info().joint_infos_size());
        ratio.resize(robot_info_response_.robot_info().joint_infos_size());
        pos_zero_offset.resize(robot_info_response_.robot_info().joint_infos_size());

        user_unit_name.resize(robot_info_response_.robot_info().joint_infos_size());
        torque_unit_name.resize(robot_info_response_.robot_info().joint_infos_size());
        load_unit_name.resize(robot_info_response_.robot_info().joint_infos_size());

        for(int i = 0; i < robot_info_response_.robot_info().joint_infos_size(); ++i) {
            cnt_per_unit[i] = robot_info_response_.robot_info().joint_infos().at(i).cnt_per_unit();
            torque_per_unit[i] = robot_info_response_.robot_info().joint_infos().at(i).torque_per_unit();
            //TODO: load_per_unit
            user_unit_name[i] = QString::fromStdString(robot_info_response_.robot_info().joint_infos().at(i).user_unit_name());
            //TODO: torque_unit_name 及 load_unit_name

            qDebug() << "Joint " << i << ": ";
            qDebug() << "    cnt_per_unit: " << robot_info_response_.robot_info().joint_infos().at(i).cnt_per_unit();
            qDebug() << "    torque_per_unit: " << robot_info_response_.robot_info().joint_infos().at(i).torque_per_unit();
            qDebug() << "    ratio: " << robot_info_response_.robot_info().joint_infos().at(i).ratio();
            qDebug() << "    pos_zero_offset: " << robot_info_response_.robot_info().joint_infos().at(i).pos_zero_offset();
            qDebug() << "    user_unit_name: " << robot_info_response_.robot_info().joint_infos().at(i).user_unit_name().c_str();

        }

    } else {
        emit connectState(false);
        is_connected_ = false;
    }

}

void ConnectDialog::on_ipAddressEdit_textChanged(const QString &ip) {
    ip_address_ = ip;
//    std::cout << "ip" << std::endl;
}

void ConnectDialog::on_portEdit_textChanged(const QString &p) {
    port_ = p.toInt();
//    std::cout << "port" << std::endl;
}


void ConnectDialog::getRobotState() {
    RobotStateRequest request;

    if(use_raw_data) {
        request.set_raw_data(true);
    }

//    RobotStateResponse robot_state_response_;
    robot_state_response_.Clear();

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = stub_->ReadRobotState(&context, request, &robot_state_response_);
    if (status.ok()) {
        emit newStateComming();
//        std::cout << "Joint state size: " << robot_state_response_.robot_state().joint_states_size() << std::endl;
//        std::cout << "Joint state status: " << robot_state_response_.robot_state().joint_states(0).status() << std::endl;
//        std::cout << "slave num: " << robot_state_response_.robot_state().hw_state().slave_num() << std::endl;
//        std::cout << "hw type: " << robot_state_response_.robot_state().hw_state().hw_type() << std::endl;
//        std::cout << "curr cycle time: " << robot_state_response_.robot_state().hw_state().current_cycle_time() << std::endl;
    } else {
        std::cout << status.error_code() << ": " << status.error_message() << std::endl;
    }
}

void ConnectDialog::stopSingleAxis(int id) {
    RobotCommandRequest request;
    RobotCommandResponse response;

    auto stop = request.mutable_command()->mutable_single_axis_command()->mutable_stop();
    stop->set_id(id);

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = stub_->WriteRobotCommmand(&context, request, &response);

    if (status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    } else {
        std::cerr << "Send command Error" << std::endl;
    }
}

void ConnectDialog::stopMultiAxis() {
    RobotCommandRequest request;
    RobotCommandResponse response;

    auto stop = request.mutable_command()->mutable_multi_axis_command()->mutable_stop();

    ClientContext context; //这个只能使用一次，每次请求都需要重新创建
    Status status = stub_->WriteRobotCommmand(&context, request, &response);

    if (status.ok()) {
//        std::cout << "Send command Ok" << std::endl;
    } else {
        std::cerr << "Send command Error" << std::endl;
    }
}
