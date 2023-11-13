#ifndef MAG200CONTROLLER_H
#define MAG200CONTROLLER_H

#include "MessagesManager.h"
#include "CommandsManager.h"
#include "NdaSdkCore.h"
#include "Chapter6Core.h"
#include "SharedMemoryCore.h"
#include "FileCopyCore.h"

/*
 * @brief The Mag200Controller class is responsible for managing the Mag200Core functionality.
 *
 * This class handles the setup and control of the NdaSdkCore and SharedMemoryCore instances,
 * which are responsible for the core functionality of the Mag200 device. It sets up the necessary
 * threads and connects the signals and slots between the components to enable communication and
 * coordination.
 *
 * @date 31/05/2023
 */
class Mag200Controller : public QObject
{
  Q_OBJECT

  private:
    QString LastStatus = "";

    NdaSdkCore *m_NdaSdkCore = nullptr;
    QThread *m_NdaSdkThread = nullptr;

    SharedMemoryCore *m_SharedMemoryCore = nullptr;
    QThread *m_SharedMemoryThread = nullptr;

    Chapter6Core *m_Chapter6Core = nullptr;
    QThread *m_Chapter6Thread = nullptr;

    FileCopyCore *m_FileCopyCore = nullptr;
    QThread *m_FileCopyThread = nullptr;

    void Setup();
    void SetupEvent();

  public:
    Mag200Controller(QObject *parent);
    ~Mag200Controller();
    
    void StartMag200Core();
    void StopMag200Core();

    void PauseMag200Core();
    void UnPauseMag200Core();

    void SendAllStatus();

  public slots:
    // General status
    void NewTime(QString date);
    void NewState_ACQ(bool state);
    void NewState_STK(bool state);

    // Input status
    void NewTestArea(QString testArea);
    void NewStatus(int ChannelID, int Type, QVariant Value);

    // Notify New data available
    void NewChannelData(QVector<double> ptsX, QVector<double> ptsY);

    void SendStatusFromIndex(int index);

    void StartReadLog();
    void StopReadLog();

    void StartReadChannelData(QString channel);
    void StopReadChannelData();

    // Notify Processing advancement
    void StartProcessing(QString title, QString description);
    void UpdateProcessing(QString title, QString description);
    void EndedProcessing();

    void NewLogNdaSdk(QStringList list);
    void NewLogSharedMemory(QStringList list);

    void NewInfo(QString info);
    void NewError(QString m_State);
    void NewWarning(QString Warning);

    void NewCopyFile(QString src, QString dst);
    void NewCopyProgress(qint64 progress);
    void NewCancelCopy();
    void ResetCopyThread();

    void NewCommand(int code, QStringList args);

  signals:
    // General status
    void SendNewTime(QString date);
    void SendNewState_ACQ(bool state);
    void SendNewState_STR(bool state);

    // Input status
    void SendNewTestArea(QString testArea);
    void SendNewStatus(int ChannelID, int Type, QVariant Value);
    void SendNewAcknowledge(int code, QList<QStringList> args);

    // Notify New data available
    void SendNewChannelData(QVector<double> ptsX, QVector<double> ptsY);

    // Notify copy 
    void SendCopyProgress(qint64 progress);
};

#endif // MAG200CONTROLLER_H
