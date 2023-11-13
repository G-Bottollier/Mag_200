#ifndef COMMANDSMANAGER_H
#define COMMANDSMANAGER_H

#include <QObject>
#include "Chapter6Command.h"

// Convenience macro to access the CommandsManager instance
#define qCommands (static_cast<CommandsManager*>(CommandsManager::Instance()))

// This class allow to send CH6 command and receive acknowledge
// It save and emit data as needed for mag200
class CommandsManager : public QObject
{
  Q_OBJECT

  #pragma region COMMAND .PLAY

  private:
    void Parse_CMD_PLAY(QList<QStringList> lines);

  signals:
    void SendPlayAcknowledge(int error);

  #pragma endregion

  #pragma region COMMAND .STOP

  private:
    void Parse_CMD_STOP(QList<QStringList> lines);

  signals:
    void SendStopAcknowledge(int error);

  #pragma endregion

  #pragma region COMMAND .FILES

  private:
    QDateTime m_fileStart;
    QDateTime m_fileEnd;

    void Parse_CMD_FILES(QList<QStringList> lines);
    QDateTime ParseDate(QString date);

    void SetFileStart(QDateTime fileStart);
    void SetFileEnd(QDateTime fileEnd);

  public:
    QDateTime  FileStart();
    QDateTime  FileEnd();

  signals:
    void SendNewFileDate(QDateTime fileStart, QDateTime fileEnd);

  #pragma endregion

  #pragma region COMMAND .STATUS

  private:
    int m_Status = -1;
    int m_Percentage = 0;

    void Parse_CMD_STATUS(QList<QStringList> lines);

    void SetStatus(int status);
    void SetPercentage(int percentage);

  public:
    int  Status();
    int  Percentage();

  signals:
    void SendStatusAcknowledge();
    void SendNewStatus(int state);
    void SendNewPercentage(int percentage);
  #pragma endregion

  #pragma region COMMAND .CONFIG STATUS

  private:
    void Parse_CMD_CONFIG_STATUS(QList<QStringList> lines);

  #pragma endregion

  #pragma region COMMAND .CONFIG STATUS N

  private:
    void Parse_CMD_CONFIG_STATUS_N(QList<QStringList> lines);

  #pragma endregion

  #pragma region COMMAND .TMATS CHECKSUM

  private:
    QString m_Checksum;

    void Parse_CMD_TMATS_CHECKSUM(QList<QStringList> lines);

    void SetChecksum(QString checksum);

  public:
    QString Checksum();

  signals:
    void SendChecksumAcknowledge();
    void SendNewChecksum(QString checksum);
  #pragma endregion

  #pragma region COMMAND .TMATS READ

  private:
    void Parse_CMD_TMATS_READ(QList<QStringList> lines);

  signals:
    void SendNewTmats(QString tmats);
  #pragma endregion

  private:
    bool m_IsConnected = false;

    void Setup();
    void SetupEvent();
    
    int IsError(QList<QStringList> lines);
    bool IsAcknowledge(QList<QStringList> lines);

  private:
    explicit CommandsManager(QObject *parent = nullptr);

  public:
    ~CommandsManager();
    static CommandsManager * Instance();

    bool IsConnected();
    void NewCommand(int code, QStringList args = QStringList());

  public slots:
    void Connected();
    void Disconnected();
    void NewAcknowledge(int code, QList<QStringList> lines);
    
  signals:
    void SendNewCommand(int code, QStringList args);
};

#endif // COMMANDSMANAGER_H
