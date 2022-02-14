#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QTimer>

#include <memory>

#include <grpcpp/grpcpp.h>
#include "robot_service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using rocos::RobotService;
using rocos::RobotStateRequest;
using rocos::RobotStateResponse;

using rocos::RobotCommand;
using rocos::RobotCommandRequest;
using rocos::RobotCommandResponse;

using rocos::RobotInfoRequest;
using rocos::RobotInfoResponse;

namespace Ui {
class ConnectDialog;
}

enum Synchronization {
    SYNC_NONE = 0,
    SYNC_TIME = 1,
    SYNC_PHASE = 2
};

enum Mode {
    MODE_CSP = 0,
    MODE_CSV = 1,
    MODE_CST = 2,
};


class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = nullptr);
    ~ConnectDialog();

public slots:

    void getRobotState();

private:
    Ui::ConnectDialog *ui;

    QTimer* timerState;

    bool event(QEvent *event) override;

    std::unique_ptr<RobotService::Stub> stub_; //grpc存根
    std::shared_ptr<Channel> channel_;

    RobotStateResponse response_;

private:
    QString ipAddress_ = "172.31.1.37";
    int     port_ = 30001;

    bool isConnected_ = false;

public:
    const QString getHardwareType();
    inline const double  getMinCyclicTime() { return response_.robot_state().hw_state().min_cycle_time(); }
    inline const double  getMaxCyclicTime() { return response_.robot_state().hw_state().max_cycle_time(); }
    inline const double  getCurrCylicTime() { return response_.robot_state().hw_state().current_cycle_time(); }

    inline const int getJointNum() { return response_.robot_state().joint_states_size(); }

    const QString getJointStatus(int id);
    inline const QString getJointName(int id) { return QString(response_.robot_state().joint_states(id).name().c_str()); }
    inline const double  getJointPosition(int id) { return response_.robot_state().joint_states(id).position(); }
    inline const double  getJointVelocity(int id) { return response_.robot_state().joint_states(id).velocity(); }
    inline const double  getJointTorque(int id) { return response_.robot_state().joint_states(id).acceleration(); }
    inline const double  getJointLoad(int id) { return response_.robot_state().joint_states(id).load(); }


    void powerOn();
    void powerOff();

    void powerOn(int id);
    void powerOff(int id);

    void setJointMode(int id, int mode);

    void shutdown();

    void setSync(int sync);



    //////////Single Axis Move/////////////////
    void moveSingleAxis(int id, double pos, double max_vel = -1, double max_acc = -1, double max_jerk = -1, double least_time = -1);

    //////////Multi Axis Move/////////////////
    void moveMultiAxis(const QVector<double>& pos, const QVector<double>& max_vel, const QVector<double>& max_acc, const QVector<double>& max_jerk, double least_time = -1);

signals:
    void jointPositions(QVector<double>& jntPos); //解析到关节位置，发送 信号
    void cartPose(QVector<double>& pose); //解析到笛卡尔空间位置，发送 信号
    void speedScaling(double f100); // 速度缩放因数 25.0
    void logging(QByteArray& ba); //返回的日志信息

    void newStateComming(void);
    void connectState(bool isConnected);

private slots:
    void on_connectButton_clicked();
    void on_ipAddressEdit_textChanged(const QString &ip);
    void on_portEdit_textChanged(const QString &p);
};

#endif // CONNECTDIALOG_H
