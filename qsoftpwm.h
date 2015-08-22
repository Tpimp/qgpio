#ifndef QSOFTPWM_H
#define QSOFTPWM_H

#include <QObject>

class QSoftPwm : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool pwm_enabled READ isEnabled WRITE setEnabled NOTIFY pwmEnableChanged)
    Q_PROPERTY(quint32 period READ pwmPeriod WRITE setPeriod NOTIFY periodChanged)
    Q_PROPERTY(quint32 duty_cycle READ pwmDutyCycle WRITE setDutyCycle NOTIFY dutyCycleChanged)
    Q_PROPERTY(int  number READ pinNumber WRITE setPinNumber NOTIFY pinNumberChanged)

public:

    explicit QSoftPwm(QObject *parent = 0);

    bool    isEnabled();
    void    setEnabled(bool enabled);

    quint32 pwmPeriod();
    bool    setPeriod(quint32 new_period);


    quint32 pwmDutyCycle()
    {
        return mDutyCycle;
    }

    bool    setDutyCycle(quint32 duty_cycle);

    int     pinNumber()
    {
        return mPinNumber;
    }
   bool    setPinNumber(int pin_number);

    // In case sys pwm is defined elsewhere
    static const char * PWM_PATH;
    static const char * PWM_CLASS_PATH;
    static bool pinExported(QString pin_number);

signals:
    void periodChanged(quint32 new_period);
    void pwmEnableChanged(bool enabled);
    void dutyCycleChanged(quint32 duty_cycle);
    void pinNumberChanged(quint32 pin_number);

public slots:


protected:
    bool     mEnabled;
    quint32  mDutyCycle;
    quint32  mPeriod;
    quint32  mPinNumber;

    bool exportPin(QString pin_number);
    bool unexportPin(QString pin_number);


};

#endif // QSOFTPWM_H
