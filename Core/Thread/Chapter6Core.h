#ifndef CHAPTER6CORE_H
#define CHAPTER6CORE_H

#include "Mag200Core.h"

#include <QtGlobal>
#include <QtNetwork>

#include "Chapter6Command.h"

#define CH6_CMD_UNSUPPORTED     -1
#define CH6_CMD_TIMEOUT         -2
#define CH6_SEND_ERROR          -3
#define CH6_ACK_ERROR           -4

#define CH6_IDLE                0
#define CH6_NEW_CMD             1
#define CH6_ACK_OK              2
#define CH6_ACK_WAIT            3
#define CH6_ACK_DATA            4

class Chapter6Core : public Mag200Core
{
  Q_OBJECT

  private:
    QMutex m_Mutex;
    int m_State = 0;
    QElapsedTimer m_Delay;

    QQueue<QPair<int, QStringList>> m_queue;
    Chapter6Command *m_Command = nullptr;
    
    QTcpSocket *m_Socket = nullptr;
    
    int error();

    int ResetCommand();
    int CommandProcessing();
    int WaitingAcknowledge();
    int AcknowledgeProcessing();

  protected:
    void Processing();
    int Initialize();
    int Terminate();
  
  public:
    Chapter6Core(QObject *parent = nullptr);
    ~Chapter6Core();

    void SendAllStatus();
  
  public slots:
    void Connected();
    void Disconnected();

    void ReadyRead();
    void NewCommand(int code, QStringList args);
    void Error(QAbstractSocket::SocketError socketError);

  signals:
    void SendConnected();
    void SendDisconnected();
    void SendNewAcknowledge(int code, QList<QStringList> args);
};

#endif // CHAPTER6CORE_H
