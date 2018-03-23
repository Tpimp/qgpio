#ifndef QGPIOWATCHER_H
#define QGPIOWATCHER_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QDebug>
#include <QFlags>
#include <QHash>
#include "qgpio.h"
#include <QDir>
#include <QList>


class Q_QTGPIO_EXPORT QGpioWatcher : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE
    {

        mInPins = new QList<QGpio*>;
        mOutPins = new QList<QGpio*>;
        mRunning = true;
        while(mRunning)
        {
            for(int i(0); i < mInPins->size(); ++i)
            {
                mInPins->at(i)->pinValue();
            }
            for(int i(0); i < mOutPins->size(); ++i)
            {
                mOutPins->at(i)->pinValue();
            }
            usleep(THREAD_SLEEP_TIME);
        }
    }

public slots:
    void setRunning(const bool & running)
    {
        mRunning = running;
    }
    QGpio * exportPin(int pin_number, QGpio::DirectionType direction = QGpio::Low,
                         QGpio::EdgeType edgetype = QGpio::None, QObject *parent = 0)
    {
        bool registered;
        QString file_path(QGpio::GPIO_CLASS_PATH);
        file_path.append("/export");
        QFile gpio_export(file_path);
        QString pin_str(QString::number(pin_number));
        QString pin_path(QGpio::GPIO_PATH + pin_str);
        QDir pin_directory(pin_path);
        QGpio * gpio(nullptr);
        if(gpio_export.open(QIODevice::WriteOnly))
        {
            // opened export file
            gpio_export.write(pin_str.toLatin1());
            gpio_export.close();

            //check if directory exists now
            if((registered = pin_directory.exists()))
            {
                qDebug() << "Successfully exported" << pin_path;
                gpio = new QGpio(QString::number(pin_number),
                                             direction,
                                             edgetype,
                                             parent);

                if(direction == QGpio::In)
                {
                    qDebug() << "Adding GPIO " << pin_number << "to InPins";
                    mInPins->append(gpio);
                }
                else
                {
                    qDebug() << "Adding GPIO " << pin_number << "to OutPins";
                    mOutPins->append(gpio);
                }
            }
            else
            {
                qDebug() << "Failed to export" << pin_path;
            }
        }
        else
        {
            qWarning()<< "Could not open GPIO Export file?!?!?!\n" << gpio_export.errorString();
        }
        return gpio;
    }
    QList<QGpio *>   exportedPins(){
        return (*mInPins+*mOutPins);
    }

    bool  unexportPin(int pin_number)
    {
        QString pin_str(QString::number(pin_number));
        QGpio * gpio_to_unregister(nullptr);
        QGpio * gpio;
        for(int i(0); i< mInPins->size(); i++)
        {
            gpio = mInPins->at(i);
            if(pin_str == gpio->pinNumber())
            {
                gpio_to_unregister = gpio;
                mInPins->removeOne(gpio);
                goto Remove_GPIO_Pin;
            }

        }
        for(int i(0); i< mOutPins->size(); i++)
        {
            gpio = mOutPins->at(i);
            if(pin_str == gpio->pinNumber())
            {
                gpio_to_unregister = gpio;
                mOutPins->removeOne(gpio);
                goto Remove_GPIO_Pin;
            }
        }
        Remove_GPIO_Pin:
        if(gpio_to_unregister)
            delete gpio_to_unregister;
        return QGpio::unexportPin(pin_str);
    }

    QList<QObject*> exportedObjects()
    {
        QObjectList  pins;
        for(int i = 0; i < mInPins->size(); ++i)
        {
            pins.append(reinterpret_cast<QObject*>(mInPins->at(i)));
        }
        for(int i = 0; i < mOutPins->size(); ++i)
        {
            pins.append(reinterpret_cast<QObject*>(mOutPins->at(i)));
        }
        return pins;
    }


    void watchGpioItem(QGpio * item)
    {
        if(!item)
            return;
        if(item->pinDirection() == QGpio::In)
            mInPins->append(item);
        else
            mOutPins->append(item);
    }

signals:


public:
    int THREAD_SLEEP_TIME;

    void setThreadSleepTime(int sleep_time)
    {
        if (sleep_time != this->THREAD_SLEEP_TIME)
            this->THREAD_SLEEP_TIME = sleep_time;
    }

    QGpio * getGpioPin(int pin_number)
    {
        QString pin_str(QString::number(pin_number));
        QGpio * gpio;
        for(int i(0); i < mInPins->length(); i++)
        {
            gpio = mInPins->at(i);
            if(pin_str == gpio->pinNumber())
                return gpio;
        }
        for(int i(0); i < mOutPins->length(); i++)
        {
            gpio = mOutPins->at(i);
            if(pin_str == gpio->pinNumber())
                return gpio;
        }
        return nullptr;
    }

    QList<QGpio *>    getOutPins(){return *mOutPins;}
    QList<QGpio *>    getInPins(){return *mInPins;}
    ~QGpioWatcher()
    {
        mRunning = false;
        QGpio * gpio;
        while(!mInPins->isEmpty())
        {
            gpio = mInPins->front();
            unexportPin(gpio->pinNumber().toInt());
        }
        while(!mOutPins->isEmpty())
        {
            gpio = mOutPins->front();
            unexportPin(gpio->pinNumber().toInt());
        }
        delete mInPins;
        delete mOutPins;
        terminate();
    }

private:
    QList<QGpio *>*     mOutPins;
    QList<QGpio *>*     mInPins;
    bool                mRunning;


};

#endif // QGpioWatcher_H
