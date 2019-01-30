#ifndef GLOBAL_H
#define GLOBAL_H

#include <QMutex>
#include <QWaitCondition>

QMutex* dbmutex;
QWaitCondition* db_execution_wait;
QMutexLocker* locker;



#endif // GLOBAL_H
