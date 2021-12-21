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

namespace Ui {
class ConnectDialog;
}

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

    std::unique_ptr<RobotService::Stub> _stub; //grpc存根
    std::shared_ptr<Channel> _channel;

    RobotStateResponse response;

private:
    QString ipAddress = "0.0.0.0";
    int     port = 30001;

public:
    const QString getHardwareType();
    inline const double  getMinCyclicTime() { return response.robot_state().hw_state().min_cycle_time(); }
    inline const double  getMaxCyclicTime() { return response.robot_state().hw_state().max_cycle_time(); }
    inline const double  getCurrCylicTime() { return response.robot_state().hw_state().current_cycle_time(); }

    inline const int getJointNum() { return response.robot_state().joint_states_size(); }

    const QString getJointStatus(int id);
    inline const QString getJointName(int id) { return QString(response.robot_state().joint_states(id).name().c_str()); }
    inline const double  getJointPosition(int id) { return response.robot_state().joint_states(id).position(); }
    inline const double  getJointVelocity(int id) { return response.robot_state().joint_states(id).velocity(); }
    inline const double  getJointTorque(int id) { return response.robot_state().joint_states(id).acceleration(); }
    inline const double  getJointLoad(int id) { return response.robot_state().joint_states(id).load(); }


    inline void shutdown() { timerState->stop(); _channel.reset(); _stub.release(); emit connectState(false); }

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
