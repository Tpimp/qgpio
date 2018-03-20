#ifndef QTGPIO_GLOBAL_H
#define QTGPIO_GLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#  if defined(QT_BUILD_QTGPIO_LIB)
#    define Q_QTGPIO_EXPORT Q_DECL_EXPORT
#  else
#    define Q_QTGPIO_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_QTGPIO_EXPORT
#endif

QT_END_NAMESPACE

#endif // QTGPIO_GLOBAL_H
