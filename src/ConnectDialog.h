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


class ConnectDialog : public QDialog {
Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = nullptr);

    ~ConnectDialog();

public:
    QString getHardwareType();

    inline double getMinCyclicTime() const { return robot_state_response_.robot_state().hw_state().min_cycle_time(); }

    inline double getMaxCyclicTime() const { return robot_state_response_.robot_state().hw_state().max_cycle_time(); }

    inline double
    getCurrCyclicTime() const { return robot_state_response_.robot_state().hw_state().current_cycle_time(); }

    inline int getJointNum() const { return robot_state_response_.robot_state().joint_states_size(); }

    ////////////ε³θηΆζ//////////////
    QString getJointStatus(int id);

    inline QString getJointName(int id) const {
        return QString{robot_state_response_.robot_state().joint_states(id).name().c_str()};
    }

    inline double getJointPosition(int id) const {
        return robot_state_response_.robot_state().joint_states(id).position();
    }

    inline double getJointVelocity(int id) const {
        return robot_state_response_.robot_state().joint_states(id).velocity();
    }

    inline double getJointTorque(int id) const {
        return robot_state_response_.robot_state().joint_states(id).acceleration();
    }

    inline double getJointLoad(int id) const { return robot_state_response_.robot_state().joint_states(id).load(); }

    ////////////ε³θδΏ‘ζ―//////////////
    inline double getJointCntPerUnit(int id) const {
        return robot_info_response_.robot_info().joint_infos().at(id).cnt_per_unit();
    }

    inline double getJointTorquePerUnit(int id) const {
        return robot_info_response_.robot_info().joint_infos().at(id).torque_per_unit();
    }

    inline double getJointRatio(int id) const { return robot_info_response_.robot_info().joint_infos().at(id).ratio(); }

    inline int32_t getJointPosZeroOffset(int id) const {
        return robot_info_response_.robot_info().joint_infos().at(id).pos_zero_offset();
    }

    inline QString getJointUserUnitName(int id) const {
        return QString{robot_info_response_.robot_info().joint_infos().at(id).user_unit_name().c_str()};
    }


    void powerOn();

    void powerOff();

    void powerOn(int id);

    void powerOff(int id);

    void setJointMode(int id, int mode);

    void shutdown();

    void setSync(int sync);


    //////////Single Axis Move/////////////////
    void moveSingleAxis(int id, double pos, double max_vel = -1, double max_acc = -1, double max_jerk = -1,
                        double least_time = -1);

    void stopSingleAxis(int id);

    //////////Multi Axis Move/////////////////
    void moveMultiAxis(const QVector<double> &pos, const QVector<double> &max_vel, const QVector<double> &max_acc,
                       const QVector<double> &max_jerk, double least_time = -1);

    void stopMultiAxis();

signals:

    void jointPositions(QVector<double> &jntPos); //θ§£ζε°ε³θδ½η½?οΌει δΏ‘ε·
    void cartPose(QVector<double> &pose); //θ§£ζε°η¬ε‘ε°η©Ίι΄δ½η½?οΌει δΏ‘ε·
    void speedScaling(double f100); // ιεΊ¦ηΌ©ζΎε ζ° 25.0
    void logging(QByteArray &ba); //θΏεηζ₯εΏδΏ‘ζ―

    void newStateComming(void);

    void connectState(bool isConnected);

public slots:

    void getRobotState();

private slots:

    void on_connectButton_clicked();

    void on_ipAddressEdit_textChanged(const QString &ip);

    void on_portEdit_textChanged(const QString &p);

public:
    bool use_raw_data { false };

    QVector<double>  cnt_per_unit;
    QVector<double>  torque_per_unit;
    QVector<double>  load_per_unit;
    QVector<int32_t>  pos_zero_offset;
    QVector<double>  ratio;
    QVector<QString> user_unit_name;
    QVector<QString> torque_unit_name;
    QVector<QString> load_unit_name;

private:
    bool event(QEvent *event) override;

    Ui::ConnectDialog *ui;

    QTimer *timer_state_;

    std::unique_ptr<RobotService::Stub> stub_; //grpcε­ζ Ή
    std::shared_ptr<Channel> channel_;

    RobotInfoResponse robot_info_response_;
    RobotStateResponse robot_state_response_;

    int sync_ { 1 }; // 0->none 1->time 2->phase

private:
    QString ip_address_ { "172.31.1.37" };

    int port_  { 30001 };

    bool is_connected_  { false };

};

#endif // CONNECTDIALOG_H
