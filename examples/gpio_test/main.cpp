#include <QtGui/QGuiApplication>
#include <QTimer>
#include <QDebug>
#include <QtQml>

#include <qgpiowatcher.h>
#include <qgpio.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QList<int> inPins;
    inPins << 204 << 199; // CAREFUL TO SUPPLY VALID Input Capable PINS


    QGpioWatcher gpio_thread;
    gpio_thread.setThreadSleepTime(3000);
    gpio_thread.start();


    QQmlContext * root_c(engine.rootContext());
    foreach(const int & pin_number, inPins)
    {
        QGpio * pin_created(gpio_thread.exportPin(pin_number,QGpio::In,QGpio::Both,&app));
        pin_created->setActiveLow(0);
        pin_created->setEdgeType(QGpio::Both);
        if(pin_created) // connect c++ slot
        {
            QObject::connect(pin_created,&QGpio::valueChanged,[=](const bool & new_value){
                qDebug() << "Pin " << pin_created->pinNumber() << "changed value to " << new_value;
            });
            QString pin_name("GPIO");
            pin_name += QString::number(pin_number);
            qDebug() << "Creating context property link" << pin_name;
            root_c->setContextProperty(pin_name, pin_created);
        }
        else
            qDebug() << "Pin "<< pin_number << " is null!";
    }
    // objects must be exposed to qml through QList<QObject*>
    // we must cast a list
    root_c->setContextProperty("GpioList",QVariant::fromValue(gpio_thread.exportedObjects()));
    root_c->setContextProperty("GpioWatcher",&gpio_thread);

    QObject::connect(&app, &QGuiApplication::aboutToQuit, &gpio_thread, &QThread::terminate);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    int ret_val(app.exec());
    return ret_val;
}
