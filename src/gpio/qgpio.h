#ifndef QGPIO_H
#define QGPIO_H

#include <QObject>
#include <QFile>
#include <QList>
#include <QDebug>

class QGpio : public QObject
{
    Q_OBJECT
    Q_ENUMS(EdgeType)
    Q_ENUMS(DirectionType)
    // Properties
    Q_PROPERTY(bool value READ pinValue WRITE setPin NOTIFY valueChanged)
    Q_PROPERTY(QGpio::DirectionType direction READ pinDirection WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QGpio::EdgeType edge READ pinEdgeType WRITE  setEdgeType NOTIFY edgeTypeChanged)
    Q_PROPERTY(QString  number READ pinNumber WRITE setPinNumber NOTIFY pinNumberChanged)
    Q_PROPERTY(bool     active_low READ pinActiveLow WRITE setActiveLow NOTIFY pinSetActiveLow)
public:
    // Enums
    enum EdgeType{ None, Falling, Rising, Both};
    enum DirectionType{ In, Out, High, Low};

    QGpio(){}
    QGpio(QString number, QGpio::DirectionType direction = QGpio::Low,
                            QGpio::EdgeType edgtype = QGpio::None, QObject *parent = 0);
    QGpio(const QGpio & right):
        QObject(right.parent())
    {
        qDebug() << "Copy Made of GPIO" << right.mPinNumber;
        mPinNumber = right.mPinNumber;
        mEdge =  right.mEdge;
        mDirection = right.mDirection;
        mActiveLow = right.mActiveLow;
        mValue = right.mValue;
        mValueFile.setFileName( right.mValueFile.fileName());
        mValueFile.open(QIODevice::ReadOnly);
    }

    bool            pinValue();
    void            setPin(bool value);

    DirectionType   pinDirection();
    bool            setDirection(const DirectionType & direction);

    QGpio::EdgeType setEdgeType(const QGpio::EdgeType & new_type);
    QGpio::EdgeType pinEdgeType();

    QString         pinNumber();
    bool            setPinNumber(QString pin_number);

    bool            pinActiveLow();
    bool            setActiveLow(bool active_low);

    static bool exportPin(QString pin_number);
    static bool unexportPin(QString pin_number);
    static bool pinExported(QString pin_number);
    ~QGpio();
    // In case sysfs is defined elsewhere
    static const char * GPIO_PATH;
    static const char * GPIO_CLASS_PATH;
signals:

    void valueChanged(bool new_value);
    void directionChanged(QGpio::DirectionType direction);
    void edgeTypeChanged(QGpio::EdgeType  type);
    void pinNumberChanged(QString  number);
    void pinSetActiveLow(bool  active_low);

public slots:


private:

    //
    bool              mValue;
    DirectionType     mDirection;
    EdgeType          mEdge;
    QString           mPinNumber;
    bool              mActiveLow;

    // The interface described through sysfs is actually a device node (on linux)
    // So a file handle can be kept open
    QFile            mValueFile;

};

Q_DECLARE_METATYPE(QList<QGpio>)
Q_DECLARE_METATYPE(QList<QGpio*>)



#endif // QGpio_H
