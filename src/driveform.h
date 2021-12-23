#ifndef DRIVEFORM_H
#define DRIVEFORM_H

#include <QWidget>
#include <paramdialog.h>

namespace Ui {
class DriveForm;
}

class DriveForm : public QWidget
{
    Q_OBJECT

public:
    explicit DriveForm(QWidget *parent = nullptr);
    ~DriveForm();

    void setName(const QString& name);
    void setActualPosition(double pos);
    void setActualVelocity(double vel);
    void setActualTorque(double tor);
    void setLoadTorque(double load);
    void setAxisStatus(const QString& status);
    void setMotionStatus(const QString& status);

    int getOpMode();
    double getPtpAbsolute();
    double getPtpRelative();

    double getMaxVel() { return _maxVel; }
    double getMaxAcc() { return _maxAcc; }
    double getMaxJerk(){ return _maxJerk; }

    void setMaxVel(double value) { _maxVel = value; }
    void setMaxAcc(double value) { _maxAcc = value; }
    void setMaxJerk(double value) { _maxJerk = value; }

private slots:
    void on_ptpAbs_textChanged(const QString &arg1);

    void on_ptpRel_textChanged(const QString &arg1);

    void on_parameters_clicked();

private:
    Ui::DriveForm *ui;

    double _pos = 0.0;
    double _vel = 0.0;
    double _acc = 0.0;
    double _tor = 0.0;
    double _load = 0.0;

    double _ptpAbs = 0.0;
    double _ptpRel = 0.0;

    double _maxVel = 100000.0;
    double _maxAcc = 100000.0;
    double _maxJerk =100000.0;
};

#endif // DRIVEFORM_H
