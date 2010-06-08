#ifndef LOGGING_H
#define LOGGING_H

#include <QDebug>

#ifndef NDEBUG
# define LOG_DEBUG(msg) qDebug() << "[Debug]" << FORMAT(msg);
# define LOG_WARNING(msg) qWarning() << "[WARNING]" << FORMAT(msg);
# define LOG_ERROR(msg) qCritical() << "[ERROR]" << FORMAT(msg);
#else
# define LOG_DEBUG(msg)
# define LOG_WARNING(msg)
# define LOG_ERROR(msg)
#endif

#endif // LOGGING_H
