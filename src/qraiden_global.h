#ifndef QRAIDEN_GLOBAL_H
#define QRAIDEN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QRAIDEN_LIBRARY)
#  define QRAIDEN_EXPORT Q_DECL_EXPORT
#else
#  define QRAIDEN_EXPORT Q_DECL_IMPORT
#endif

#endif // QRAIDEN_GLOBAL_H
