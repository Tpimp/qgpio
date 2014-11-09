#include "qsoftpwm.h"

#include <QDir>
#include <QDebug>
#include <QByteArray>


const char * QSoftPwm::PWM_PATH = "/sys/class/soft_pwm";
const char * QSoftPwm::PWM_CLASS_PATH = "/pwm";



QSoftPwm::QSoftPwm(QObject *parent) :
    QObject(parent)
{

}


bool QSoftPwm::pinExported(QString pin_number)
{
    QDir   pin(PWM_PATH + pin_number);
    return pin.exists();
}


bool QSoftPwm::exportPin(QString pin_number)
{
    if(pinExported(pin_number))
        return true;

    QString file_path(PWM_CLASS_PATH);
    file_path.append("/export");
    QFile pwm_export(file_path);
    qDebug() << "Exporting PWM Pin" << pin_number;
    if(pwm_export.open(QIODevice::WriteOnly))
    {
        // opened export file
        pwm_export.write(pin_number.toLatin1());
        pwm_export.close();
    }
    else
    {
        qWarning()<< "Could not open PWM Export file?!?!?!\n" << pwm_export.errorString();
        return false;
    }
    return true;
}

bool QSoftPwm::isEnabled()
{
    QString file_path;
    file_path.append(PWM_CLASS_PATH);
    file_path.append(PWM_PATH);
    file_path.append(QString::number(mPinNumber));
    file_path.append("/enable");

    QFile enabled_file(file_path);
    if(enabled_file.open(QIODevice::WriteOnly))
    {
        QByteArray message(enabled_file.readAll());
        mEnabled = message.at(0);
        enabled_file.close();
    }
    return mEnabled;
}

void  QSoftPwm::setEnabled(bool enabled)
{
    if(enabled != mEnabled)
    {
        if(pinExported(QString::number(mPinNumber)))
        {
            mEnabled = enabled;
            emit pwmEnableChanged(mEnabled);
            // check if a real pwmEnable exists. if it does enable it
            QString file_path;
            file_path.append(PWM_CLASS_PATH);
            file_path.append(PWM_PATH);
            file_path.append(QString::number(mPinNumber));
            file_path.append("/enable");
            QFile enabled_file(file_path);
            if(enabled_file.open(QIODevice::WriteOnly))
            {

                QByteArray array;
                array.append(mEnabled);
                enabled_file.write(array);
            }
            else
            {
                setPeriod(0);
            }
            enabled_file.close();
        }
        else
        {
            qDebug() << "Failed to write enabled to non-exported PWM pin, " << mPinNumber;
        }
    }
}

bool QSoftPwm::setDutyCycle(quint32 duty_cycle)
{

}

bool QSoftPwm::setPeriod(quint32 new_period)
{

}

quint32 QSoftPwm::pwmPeriod()
{

}

bool QSoftPwm::setPinNumber(int pin_number)
{

}







bool QSoftPwm::unexportPin(QString pin_number)
{
    if(pinExported(pin_number))
        return true;

    QString file_path(PWM_CLASS_PATH);
    file_path.append("/unexport");
    QFile pwm_unexport(file_path);
    qDebug() << "Unexporting PWM Pin" << pin_number;
    if(pwm_unexport.open(QIODevice::WriteOnly))
    {
        // opened export file
        pwm_unexport.write(pin_number.toLatin1());
        pwm_unexport.close();
    }
    else
    {
        qWarning()<< "Could not open PWM Export file?!?!?!\n" << pwm_unexport.errorString();
        return false;
    }
    return true;
}
