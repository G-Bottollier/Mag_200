#ifndef SHAREDMEMORYCORE_H
#define SHAREDMEMORYCORE_H

#include "Mag200Core.h"

#include "Status.h"
#include "Mag200Data.h"

#define MAX_ACQ 20

#define SHARED_MEMORY_OK 0
#define SHARED_MEMORY_IDLE 10
#define SHARED_MEMORY_NO_STATUS -420

/*
 * @brief The SharedMemoryCore class represents the core functionality for shared memory operations.
 *
 * This class is derived from the Mag200Core class and is responsible for initializing shared memory,
 * reading status information, processing data, and managing the interaction between different components.
 *
 * @date 31/05/2023
 */
class SharedMemoryCore : public Mag200Core
{
  Q_OBJECT

  private:
    int m_State = 0;
    int NB_MAX_STATUS = 0;
    
    QString m_TestArea;
    QMap<int, QVector<int>> m_Status;
    QMap<int, QVector<int>> m_StatusACQ;
    
    int ReadStatus();
    int ReadTestArea();

    void ResetAllStatus();
    void CheckStatus(int ChannelID, T_Status status);

    int CheckError(int m_State);

  protected:
    void Processing();
    int Initialize();
    int Terminate();

  public:
      SharedMemoryCore(QObject *parent = nullptr);
      ~SharedMemoryCore();

      void SendAllStatus();
      void SendStatus(int ChannelID);

    signals:
      void SendNewTestArea(QString testArea);
      void SendNewStatus(int ChannelID, int Type, QVariant Value);
};

#endif // SHAREDMEMORYCORE_H
