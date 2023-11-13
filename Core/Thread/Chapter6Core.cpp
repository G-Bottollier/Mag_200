#include "stdafx.h"
#include "Chapter6Core.h"

Chapter6Core::Chapter6Core(QObject *parent) : Mag200Core(parent)
{
  // We don't need sleep because we are waiting for command to send !
  this->m_UseSleep = false;

  m_Command = new Chapter6Command();

  qRegisterMetaType<QList<QStringList>>("ch6_args");
}

Chapter6Core::~Chapter6Core()
{
  m_Command->deleteLater();
}

int Chapter6Core::Initialize()
{
  int m_State = CH6_IDLE;

  m_Socket = new QTcpSocket();
  m_Socket->setSocketOption(QAbstractSocket::LowDelayOption, 1); // Try to communicate fast
  m_Socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1); // Enable TCP keep-alive mechanism

  QObject::connect(m_Socket, &QTcpSocket::connected, this, &Chapter6Core::Connected);
  QObject::connect(m_Socket, &QTcpSocket::disconnected, this, &Chapter6Core::Disconnected);

  QObject::connect(m_Socket, &QTcpSocket::readyRead, this, &Chapter6Core::ReadyRead);
  QObject::connect(m_Socket, static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &Chapter6Core::Error);
  
  return m_State;
}

void Chapter6Core::Processing()
{
  // State machine on socket
  switch (this->m_Socket->state())
  {
    default:
    case QAbstractSocket::HostLookupState:      return;
    case QAbstractSocket::UnconnectedState:     this->m_Socket->connectToHost(QHostAddress::LocalHost, 2323);     return;
    case QAbstractSocket::ConnectingState:      this->m_Socket->waitForConnected(1000);                           return;
    case QAbstractSocket::ClosingState:         this->m_Socket->waitForDisconnected(1000);                        return;
    case QAbstractSocket::ConnectedState:       break;
  }

  // Check command error
  if (this->m_State < 0)
    this->m_State = error();

  // State machine on Command
  switch (this->m_State)
  {
    default:
    case CH6_IDLE:        QThread::msleep(100);                             return;
    case CH6_NEW_CMD:     this->m_State = this->CommandProcessing();        return;   // Process the command
    case CH6_ACK_WAIT:    this->m_State = this->WaitingAcknowledge();       return;   // Command timeout
    case CH6_ACK_DATA:    this->m_State = this->AcknowledgeProcessing();    return;   // Acknowledge the Command
    case CH6_ACK_OK:      this->m_State = this->ResetCommand();             return;   // Clear command
  }
}

int Chapter6Core::Terminate()
{
  m_Socket->deleteLater();
  m_Socket = nullptr;
  return 0;
}

void Chapter6Core::SendAllStatus()
{
}

void Chapter6Core::Connected()
{
  emit this->SendConnected();
}

void Chapter6Core::Disconnected()
{
  emit this->SendDisconnected();
}

void Chapter6Core::ReadyRead()
{
  // Process the acknowledge
  this->m_State = CH6_ACK_DATA;
}

void Chapter6Core::NewCommand(int code, QStringList args)
{
  QMutexLocker locker(&this->m_Mutex);

  // FIFO processing
  // Enqueue the command
  this->m_queue.enqueue(qMakePair(code, args));

  // State remains unchanged if a command is in progress
  if(this->m_State == CH6_IDLE)
    this->m_State = CH6_NEW_CMD;
}

void Chapter6Core::Error(QAbstractSocket::SocketError socketError)
{
  switch (socketError)
  {
    default:
      qDebug() << socketError;
      return;

    case QAbstractSocket::RemoteHostClosedError:
      this->m_Socket->abort();
      return;

    case QAbstractSocket::SocketTimeoutError:
      return;
  }
}

int Chapter6Core::error()
{
  if(this->m_queue.size() == 0)
    return CH6_IDLE;

  QPair<int, QStringList> command = this->m_queue.dequeue();
  qDebug() << "ERROR : " << this->m_State << " & COMMAND : " << command.first;
  
  return this->m_State;
}

int Chapter6Core::WaitingAcknowledge()
{
  // 100 ms before timeout
  const qint64 timeout = 10000 /* ms */ * 1e6;
  qint64 elapsed = m_Delay.nsecsElapsed();

  if (elapsed > timeout)
    return CH6_CMD_TIMEOUT;

  bool t = this->m_Socket->waitForReadyRead(10);

  return this->m_State;
}

int Chapter6Core::ResetCommand()
{
  // Command queue not empty
  if (m_queue.size() != 0)
    return CH6_NEW_CMD;

  // Reset state machine
  this->m_Command->clear();
  return CH6_IDLE;
}

int Chapter6Core::CommandProcessing()
{
  bool OK = false;
  this->m_Command->clear();

  OK = this->m_queue.size() != 0;
  if (OK == false)
    return CH6_IDLE;

  OK = this->m_Command->MakeCommand(this->m_queue.head());
  if (OK == false)
    return CH6_CMD_UNSUPPORTED;

  OK = this->m_Command->SendCommand(this->m_Socket);
  if (OK == false)
    return CH6_SEND_ERROR;

  m_Delay.restart();

  return CH6_ACK_WAIT;
}

int Chapter6Core::AcknowledgeProcessing()
{
  bool OK = this->m_Command->ReadAcknowledge(this->m_Socket);
  bool Overflow = this->m_Command->DataOverflow();
  bool ACK = this->m_Command->HasAcknowledge();

  if(Overflow == true)
    return CH6_ACK_ERROR;

  if (ACK == false)
    return CH6_ACK_WAIT;

  if (OK == false)
    return CH6_ACK_ERROR;

  qDebug() << "CMD RECEIVE IN : " << m_Delay.nsecsElapsed() / 1e6;
  QPair<int, QStringList> command = this->m_queue.dequeue();

  emit SendNewAcknowledge(command.first, this->m_Command->Lines());
  this->m_Command->clear();

  return CH6_ACK_OK;
}
