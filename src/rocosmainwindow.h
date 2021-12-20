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

private slots:
    void on_actionAbout_triggered();

    void on_actionConnect_triggered();

private:
    Ui::RocosMainWindow *ui;
    AboutDialog* aboutDlg = Q_NULLPTR;
    ConnectDialog* connectDlg = Q_NULLPTR;

};
#endif // ROCOSMAINWINDOW_H
