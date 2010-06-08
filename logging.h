#ifndef LOGGING_H
#define LOGGING_H

#include <QDebug>

#ifndef QT_NO_DEBUG_OUTPUT
# define FORMAT(msg) __FUNCTION__ << " (" << __FILE__ << ":" << __LINE__ << ") " << msg
# define LOG_DEBUG(msg) qDebug().nospace() << "[Debug] " << FORMAT(msg)
# define LOG_WARNING(msg) qWarning().nospace() << "[WARNING] " << FORMAT(msg)
# define LOG_ERROR(msg) qCritical().nospace() << "[ERROR] " << FORMAT(msg)
int log_indentlevel = 0;
# define LOG_ENTRY() do { qDebug().nospace() << "-- " << __FUNCTION__ << " entered"; log_indentlevel++; } while(0)
# define LOG_EXIT() do { qDebug().nospace() << "-- " << __FUNCTION__ << " exited"; log_indentlevel--; } while(0)
#else
# define LOG_DEBUG(msg)
# define LOG_WARNING(msg)
# define LOG_ERROR(msg)
# define LOG_ENTRY()
# define LOG_EXIT()
#endif

#endif // LOGGING_H
