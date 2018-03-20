#ifndef QTSOFTPWM_GLOBAL_H
#define QTSOFTPWM_GLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#  if defined(QT_BUILD_QTSOFTPWM_LIB)
#    define Q_QTSOFTPWM_EXPORT Q_DECL_EXPORT
#  else
#    define Q_QTSOFTPWM_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_QTSOFTPWM_EXPORT
#endif

QT_END_NAMESPACE

#endif // QTSOFTPWM_GLOBAL_H
