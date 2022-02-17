#ifndef ROCOSMAINWINDOW_H
#define ROCOSMAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

#include <AboutDialog.h>
#include <ConnectDialog.h>

#include <driveform.h>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class RocosMainWindow; }
QT_END_NAMESPACE

class RocosMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    RocosMainWindow(QWidget *parent = nullptr);
    ~RocosMainWindow();

    void updateRobotState();

    void resizeEvent(QResizeEvent *event) override;

    void setDriveStatusViz(bool isEnabled);

private slots:
    void on_actionAbout_triggered();

    void on_actionConnect_triggered();

    void on_actionUnit_triggered();

    void on_driveList_currentRowChanged(int currentRow);

    void on_enableBtn_clicked();

    void on_stopBtn_clicked();

    void on_ptpRelPlus_clicked();

    void on_maxVel_textChanged(const QString &arg1);

    void on_maxAcc_textChanged(const QString &arg1);

    void on_maxJerk_textChanged(const QString &arg1);

    void on_ptpRelMinus_clicked();

    void on_ptpAbs1Move_clicked();

    void on_ptpAbs2Move_clicked();

    void on_enabledAll_clicked();

    void on_disabledAll_clicked();

    void on_ptpRelPlusAll_clicked();

    void on_ptpRelMinusAll_clicked();

    void on_ptpAbs1MoveAll_clicked();

    void on_syncNone_clicked();

    void on_syncTime_clicked();

    void on_syncPhase_clicked();

private:
    void updateUnitDisp();

private:
    Ui::RocosMainWindow *ui;
    AboutDialog* aboutDlg = Q_NULLPTR;
    ConnectDialog* connectDlg = Q_NULLPTR;

    int drivePropId = -1; //默认不显示
    bool _isConnected = false;

    bool _enableState = false;
    double _max_vel = 1;
    double _max_acc = 1;
    double _max_jerk = 10;
    double _ptpAbs1 = 0.0;
    double _ptpAbs2 = 0.0;

    QGridLayout* gridLayout;

    QVector<DriveForm*> multiAxes;

};
#endif // ROCOSMAINWINDOW_H
