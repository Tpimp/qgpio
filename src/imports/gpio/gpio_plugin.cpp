#include <QtQml>

#include <QtGpio/QtGpio>

QT_BEGIN_NAMESPACE

class GpioPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("Gpio"));

        qmlRegisterType<QGpio>(uri, 1, 0, "Gpio");

    }
};

QT_END_NAMESPACE

#include "gpio_plugin.moc"
