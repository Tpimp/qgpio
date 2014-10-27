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


class QGpioWatcher : public QThread
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

        /*  Following is the linux epoll version of the above (possibly interrupt driven)
        void run()
        {
            int n;
            int epfd = epoll_create(1);
            int fd = open("/sys/class/gpio/gpio204/value", O_RDWR | O_NONBLOCK);
            printf("open returnzed %d: %s\n", fd, strerror(errno));
            if(fd > 0) {
                char buf = 0;

                struct epoll_event ev;
                struct epoll_event events;
                ev.events = EPOLLPRI;
                ev.data.fd = fd;
                while(mRunning)
                {
                    n = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
                    qDebug() << "epoll_ctl returned " <<  n <<  QString(strerror(errno));


                    n = epoll_wait(epfd, &events, 1, -1);
                    qDebug() << "epoll returned " << n << QString(strerror(errno));

                    if(n > 0) {
                        n = lseek(fd, 0, SEEK_SET);
                        qDebug() << "seek %d bytes: " << n << QString(strerror(errno));
                        n = read(fd, &buf, 1);
                        //emit valueChanged(buf == '1');
                        qDebug() << "read %d bytes: " << n << QString(strerror(errno));
                        qDebug() << "buf = " << buf;
                    }
                }
            }
            close(fd);

        QString file_path_value("/sys/class/gpio/gpio" + QString::number(pin_number));
        file_path_value.append("/value");
        qDebug() << "File path " << file_path_value;
        QFile gpio_value_sysfs(file_path_value);
        if(!gpio_value_sysfs.open(QIODevice::ReadOnly))
        {
            qDebug() << "Error could not open value for read @" << file_path_value;
            return;
        }
        int file_flags(fcntl(gpio_value_sysfs.handle(),F_GETFL,0));
        if(file_flags == -1)
        {
            qDebug() << "Failed to fetch file flags @" << file_path_value;
        }
        file_flags |= O_NONBLOCK;
        int error = fcntl(gpio_value_sysfs.handle(),F_SETFL,file_flags);
        if(error == -1)
        {
            qDebug() << "Failed to set file flags @" << file_path_value;
        }
        //set up the epol event
        int epoll_handle(epoll_create1(0));
        if(epoll_handle == -1)
        {
            qDebug() << "Failed to create epoll handle for QGpioWatcher on Pin Number#" << pin_number;
            return;
        }
        event.data.fd = gpio_value_sysfs.handle();
        event.events = EPOLLPRI | EPOLLET;
        char value_read;
        error  = epoll_ctl(epoll_handle,EPOLL_CTL_ADD,gpio_value_sysfs.handle(),&event);
        if(error == -1)
        {
            qDebug() << "Failed to add Epoll handle to controller - QGpioWatcher on Pin Number#" << pin_number;
            return;
        }


        //error = listen(gpio_value_sysfs,SOMAXCONN);
        events = new epoll_event[64]; // create a buffer place for all the events
        int event_count(0),current_event_index(0);
        while(mRunning)
        {
            qDebug() << "Reading Epol Wait";
            event_count = epoll_wait(epoll_handle,events,64,-1);
            current_event_index = 0;
            do{
                if((events[current_event_index].events & EPOLLERR) ||
                   //(events[current_event_index].events & EPOLLHUP) ||
                   ((!events[current_event_index].events & EPOLLIN)))
                {
                    qDebug() << "errored- QGpioWatcher on Pin Number#" << pin_number;
                    qDebug() << "File reported error " << gpio_value_sysfs.errorString().toLatin1();
                    continue;
                /
                if((events[current_event_index].data.fd == gpio_value_sysfs.handle()))
                {
                    // Value has changed
                    //gpio_value_sysfs.open(QIODevice::ReadOnly);
                    char read_value;
                    gpio_value_sysfs.read(&read_value,1);
                    qDebug() << "Value Changed to " << value_read;
                    //gpio_value_sysfs.
                    //gpio_value_sysfs.close();
                    emit valueChanged(read_value == '1');

                }
            }while(current_event_index++ < event_count);
        }
    }*/
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
    static int THREAD_SLEEP_TIME;
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
