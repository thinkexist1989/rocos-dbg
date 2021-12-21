#ifndef ROCOSMAINWINDOW_H
#define ROCOSMAINWINDOW_H

#include <QMainWindow>
#include <AboutDialog.h>
#include <ConnectDialog.h>

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

    void on_driveList_currentRowChanged(int currentRow);

private:
    Ui::RocosMainWindow *ui;
    AboutDialog* aboutDlg = Q_NULLPTR;
    ConnectDialog* connectDlg = Q_NULLPTR;

    int drivePropId = -1; //默认不显示
    bool _isConnected = false;

};
#endif // ROCOSMAINWINDOW_H
