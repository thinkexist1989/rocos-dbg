#include "paramdialog.h"
#include "ui_paramdialog.h"

#include <driveform.h>

ParamDialog::ParamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParamDialog)
{
    ui->setupUi(this);

    ui->maxVel->setText(QString::number(((DriveForm*)parent)->getMaxVel()));
    ui->maxAcc->setText(QString::number(((DriveForm*)parent)->getMaxAcc()));
    ui->maxJerk->setText(QString::number(((DriveForm*)parent)->getMaxJerk()));
}

ParamDialog::~ParamDialog()
{
    delete ui;
}

void ParamDialog::on_buttonBox_accepted()
{
    DriveForm* p = (DriveForm*)parent();

    p->setMaxVel(ui->maxVel->text().toDouble());
    p->setMaxAcc(ui->maxAcc->text().toDouble());
    p->setMaxJerk(ui->maxJerk->text().toDouble());
}

void ParamDialog::on_buttonBox_rejected()
{

}
