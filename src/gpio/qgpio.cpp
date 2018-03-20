#include "qgpio.h"
#include <QDir>
const char * QGpio::GPIO_PATH = "/sys/class/gpio/gpio";
const char * QGpio::GPIO_CLASS_PATH = "/sys/class/gpio/";

QDebug operator<<(QDebug debug, QGpio::EdgeType edge)
{
    switch(edge)
    {
        case QGpio::None:
            debug << "None";
            break;
        case QGpio::Falling:
            debug << "Falling";
            break;
        case QGpio::Rising:
            debug << "Rising";
            break;
        case QGpio::Both:
            debug << "Both";
            break;
        default:
            break;
    }
    return debug;
}


QDebug operator<<(QDebug debug, QGpio::DirectionType direction)
{
    switch(direction)
    {
        case QGpio::In:
            debug << "QGpio::In";
            break;
        case QGpio::Out:
            debug << "QGpio::Out";
            break;
        case QGpio::High:
            debug << "QGpio::High";
            break;
        case QGpio::Low:
            debug << "QGpio::Low";
            break;
        default:
            break;
    }
    return debug;
}


QGpio::QGpio(QString number, DirectionType direction,
                   EdgeType edgetype, QObject *parent):
    QObject(parent), mValueFile((GPIO_PATH + number + "/value"))
{

    // check if GPIO exists

    //qDebug() << "Creating GPIO object for pin" << number;
    if(mValueFile.open(QIODevice::ReadWrite))
    {
        mPinNumber = number;
        setEdgeType(edgetype);
        setDirection(direction);
        setActiveLow(true);
        pinValue();
    }
    else
    {
        qDebug() << "Could not open value file" << mValueFile.fileName();
    }


}

bool QGpio::pinExported(QString pin_number)
{
    QDir   pin(GPIO_PATH + pin_number);
    return pin.exists();
}

bool QGpio::unexportPin(QString pin_number)
{
    if(!pinExported(pin_number))
        return true;

    QString file_path(GPIO_CLASS_PATH);
    file_path.append("/unexport");
    QFile gpio_unexport(file_path);
    if(gpio_unexport.open(QIODevice::WriteOnly))
    {
        // opened unexport file
        gpio_unexport.write(pin_number.toLatin1());
        gpio_unexport.close();
    }
    else
    {
        qWarning()<< "Could not open GPIO Export file?!?!?!\n" << gpio_unexport.errorString();
    }

    return !pinExported(pin_number);
}


bool QGpio::exportPin(QString pin_number)
{
    if(pinExported(pin_number))
        return true;

    QString file_path(GPIO_CLASS_PATH);
    file_path.append("/export");
    QFile gpio_export(file_path);
    qDebug() << "Exporting Pin" << pin_number;
    if(gpio_export.open(QIODevice::WriteOnly))
    {
        // opened export file
        gpio_export.write(pin_number.toLatin1());
        gpio_export.close();
    }
    else
    {
        qWarning()<< "Could not open GPIO Export file?!?!?!\n" << gpio_export.errorString();
        return false;
    }
    return true;
}

bool QGpio::pinValue()
{
    char value;
    mValueFile.read(&value,1);
    mValueFile.seek(0);
    bool read_value(value == '1');
    if(read_value != mValue)
        emit valueChanged(mValue = read_value);
    return read_value;
}


bool QGpio::setPinNumber(QString pin_number)
{
    // if previously assigned
    //qDebug() << "checking  for pin number" << pin_number;
    if(mValueFile.isOpen())
    {
        mValueFile.close();
    }

    if(exportPin(pin_number))
    {
        mPinNumber = pin_number;
        mValueFile.setFileName(GPIO_PATH + mPinNumber +"/value");
    }
    else
       return false;

    //qDebug() << "Changing pin number to" << pin_number;
    if(mValueFile.open(QIODevice::ReadWrite))
    {
        pinValue();
    }
    else
    {
        qDebug() << "Could not open value file" << mValueFile.fileName();
    }
    return true;
}

void QGpio::setPin(bool value)
{
    mValueFile.seek(0);
    mValueFile.write((value? "1":"0"),1);
    mValueFile.seek(0);
}

QGpio::DirectionType  QGpio::pinDirection()
{
    QFile gpio_direction(GPIO_PATH + mPinNumber + "/direction");
    if(gpio_direction.open(QIODevice::ReadOnly))
    {
        char read_direction;
        gpio_direction.read(&read_direction,1);
        gpio_direction.close();
        switch(read_direction)
        {
            case 'i':
                mDirection =  QGpio::In;
                break;
            case 'o':
                mDirection = QGpio::Out;
                break;
            case 'h':
                mDirection = QGpio::High;
                break;
            case 'l':
                mDirection  =QGpio::Low;
                break;
            default:
                break;
        }
        //qDebug() << "Read Direct type" << mDirection;
        emit directionChanged(mDirection);
        return mDirection;
    }
    else
        qDebug() << "Failed to read gpio" << mPinNumber << "direction!";
    return QGpio::In;
}

bool QGpio::setDirection(const DirectionType & direction)
{
    mDirection = direction;
    QFile gpio_direction(GPIO_PATH + mPinNumber + "/direction");
    if(gpio_direction.exists())
    {
        //qDebug() << "Setting pin direction to" << direction << (GPIO_PATH + mPinNumber);
        if(gpio_direction.open(QIODevice::WriteOnly))
        {
            switch(direction)
            {
                case In:
                    gpio_direction.write(QString("in").toLatin1());
                    break;
                case Out:
                    gpio_direction.write(QString("out").toLatin1());
                    break;
                case High:
                    gpio_direction.write(QString("high").toLatin1());
                    break;
                case Low:
                    gpio_direction.write(QString("low").toLatin1());
                    break;
            }
            gpio_direction.close();
        }
        else
            return false;
    }
    else
    {
        qDebug() << "Could not find direction for " << gpio_direction.fileName();
        return false;
    }

    return (direction == pinDirection());
}

QGpio::EdgeType QGpio::setEdgeType(const EdgeType & new_type)
{
    QFile gpio_edge(GPIO_PATH + mPinNumber + "/edge");
    if(gpio_edge.exists())
    {
        //qDebug() << "Setting pin edge to" << new_type << (GPIO_PATH + mPinNumber);
        if(gpio_edge.open(QIODevice::WriteOnly))
        {
            switch(new_type)
            {
                case None:
                    gpio_edge.write(QString("none").toLatin1());
                    break;
                case Falling:
                    gpio_edge.write(QString("falling").toLatin1());
                    break;
                case Rising:
                    gpio_edge.write(QString("rising").toLatin1());
                    break;
                case Both:
                    gpio_edge.write(QString("both").toLatin1());
                    break;
                default:
                    gpio_edge.write(QString("none").toLatin1());
                    break;
            }
            gpio_edge.close();
        }
        mEdge = pinEdgeType();
        if(mEdge == new_type)
            return mEdge;
        qDebug() << "Failed to write edge, Expected:"
                 << new_type
                 << "and got"
                 << mEdge;
    }
    else
        qDebug() << "Could not find edge for " << gpio_edge.fileName();
    return QGpio::None;
}

QGpio::EdgeType QGpio::pinEdgeType()
{
    QFile gpio_edge(GPIO_PATH + mPinNumber + "/edge");
    if(gpio_edge.open(QIODevice::ReadOnly))
    {
        char read_edge;
        gpio_edge.read(&read_edge,1);
        gpio_edge.close();
        switch(read_edge)
        {
            case 'n':
                mEdge =  QGpio::None;
                break;
            case 'f':
                mEdge = QGpio::Falling;
                break;
            case 'r':
                mEdge = QGpio::Rising;
                break;
            case 'b':
                mEdge = QGpio::Both;
                break;
            default:
                break;
        }
//        qDebug() << "Read Edge type" << mEdge;
        emit edgeTypeChanged(mEdge);
        return mEdge;
    }
    else
        qDebug() << "Failed to read gpio" << gpio_edge.fileName();
    return QGpio::None;
}

QString QGpio::pinNumber()
{
   return mPinNumber;
}


bool  QGpio::pinActiveLow()
{
    QFile gpio_activel(GPIO_PATH + mPinNumber + "/active_low");
    if(gpio_activel.open(QIODevice::ReadOnly))
    {
        char read_active_low;
        gpio_activel.read(&read_active_low,1);
        gpio_activel.close();
        mActiveLow = (read_active_low == '1');
        //qDebug() << "Read Active Low" << mActiveLow;
        emit pinSetActiveLow(mActiveLow);
        return mActiveLow;
    }
    else
        qDebug() << "Failed to read gpio" << mPinNumber << "active_low!";
    return false;

}

bool  QGpio::setActiveLow(bool active_low)
{
    QFile gpio_activel(GPIO_PATH + mPinNumber +"/active_low");
    //qDebug() << "Setting Active_low" << active_low << "Pin " << mPinNumber;
    if(gpio_activel.open(QIODevice::WriteOnly))
    {
        gpio_activel.write(QString::number(active_low).toLatin1());
        gpio_activel.close();
        if(active_low == pinActiveLow()) // Double check pin active low was set
            return true;
        qDebug() << "Failed to set active low for GPIO pin " << mPinNumber;
    }
    else
        qDebug() << "Could not find active_low" << gpio_activel.fileName();
    return active_low == pinActiveLow();
}


QGpio::~QGpio()
{
    mValueFile.close();
}
