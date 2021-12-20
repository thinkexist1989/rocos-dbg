#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

bool AboutDialog::event(QEvent *event)
{
    if(event->type() == QEvent::ActivationChange) {
        if(QApplication::activeWindow() != this) {
            this->close();
        }
    }

    return QDialog::event(event);
}
