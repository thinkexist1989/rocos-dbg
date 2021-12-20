#include "rocosmainwindow.h"
#include "./ui_rocosmainwindow.h"

RocosMainWindow::RocosMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RocosMainWindow)
{
    ui->setupUi(this);

    aboutDlg = new AboutDialog(this);
    connectDlg = new ConnectDialog(this);

}

RocosMainWindow::~RocosMainWindow()
{
    delete ui;
}


void RocosMainWindow::on_actionAbout_triggered()
{
    aboutDlg->show();
}

void RocosMainWindow::on_actionConnect_triggered()
{
    connectDlg->show();
}
