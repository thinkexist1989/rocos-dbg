#include "driveform.h"
#include "ui_driveform.h"

DriveForm::DriveForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DriveForm)
{
    ui->setupUi(this);
}

DriveForm::~DriveForm()
{
    delete ui;
}

void DriveForm::setName(const QString &name)
{
    ui->axisName->setText(name);
}

void DriveForm::setActualPosition(double pos)
{
    _pos = pos;
    ui->actualPos->setText(QString::number(_pos));
}

void DriveForm::setActualVelocity(double vel)
{
    _vel = vel;
    ui->actualVel->setText(QString::number(_vel));
}

void DriveForm::setActualTorque(double tor)
{
    _tor = tor;
    ui->actualTor->setText(QString::number(_tor));
}

void DriveForm::setLoadTorque(double load)
{
    _load = load;
    ui->loadTor->setText(QString::number(_load));
}

void DriveForm::setAxisStatus(const QString &status)
{
    ui->axisStatus->setText(status);
}

void DriveForm::setMotionStatus(const QString &status)
{
    ui->motionStatus->setText(status);
}

int DriveForm::getOpMode()
{
    return ui->opMode->currentIndex();
}

double DriveForm::getPtpAbsolute()
{
    return _ptpAbs;
}

double DriveForm::getPtpRelative()
{
    return _ptpRel;
}

void DriveForm::on_ptpAbs_textChanged(const QString &arg1)
{
    _ptpAbs = arg1.toDouble();
}

void DriveForm::on_ptpRel_textChanged(const QString &arg1)
{
    _ptpRel = arg1.toDouble();
}

void DriveForm::on_parameters_clicked()
{
    ParamDialog paramDlg(this);
    paramDlg.exec();
}
