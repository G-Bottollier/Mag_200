#ifndef MAG200CORE_H
#define MAG200CORE_H

#include <QtCore>

#include "mag200commands.h"
#include "Tools.h"

#define MODE_NDASDK 0
#define MODE_STATUS 1

#define CLOCK_SPEED 100 // in ms

/*
 * @brief The Mag200Core class represents the core functionality of the Mag200 application.
 *
 * The Mag200Core class is responsible for managing the execution of Mag200 Thread, including initialization, termination,
 * processing of data, and logging. It provides functionality to start, stop, pause, and resume the core processing loop.
 * Additionally, it handles sending status information and notifications to connected components.
 *
 * @date 31/05/2023
 */
class Mag200Core : public QObject
{
    Q_OBJECT

  private:
    bool m_HasToStop = false;
    bool m_HasToPause = false;
    bool m_HasToSendLog = false;

    qint64 m_NbProcessed = 1;
    qint64 m_TimeElapsed = 0;
    qint64 m_AverageTime = 0;
    qint64 m_TotalTime = 0;

    void LogClear();
    void LogTime();

  protected:
    QMutex m_Mutex;
    QStringList Log;
    QElapsedTimer timer;
    int m_LastError = -1;
    bool m_UseSleep = true;

    virtual void Processing() = 0;
    virtual int Initialize();
    virtual int Terminate() = 0;

  public:
    Mag200Core(QObject *parent = nullptr);
    virtual ~Mag200Core();

    void StopReadLog();
    void StartReadLog();

    void PauseMag200Core();
    void UnPauseMag200Core();

    void InvokeMoveToThread(QThread *thread);

    virtual void SendAllStatus();
    
  public slots:
    void StartMag200Core();
    void StopMag200Core();

    void MoveToThread(QThread *thread);

  signals:
    // Notify New data available
    void SendNewLog(QStringList log);

    void SendNewInfo(QString info);
    void SendNewError(QString m_State);
    void SendNewWarning(QString warning);
    
    // Notify Processing advancement
    void StartProcessing(QString title, QString description); 
    void UpdateProcessing(QString title, QString description);
    void EndedProcessing();

    void started();
    void finished();
};

#endif // MAG200CORE_H
