#include "stdafx.h"
#include "CommandsManager.h"

CommandsManager::CommandsManager(QObject *parent) : QObject(parent)
{
  // Setup
  Setup();

  // Setup event
  SetupEvent();
}

CommandsManager::~CommandsManager()
{

}

/*
* @brief Returns the instance of CommandsManager.
*
* This function returns a pointer to the instance of CommandsManager.
* If the instance does not exist yet, it creates a static instance of CommandsManager.
* Subsequent calls to this function will always return the same instance.
*
* @return A pointer to the instance of CommandsManager.
*
* @date 14/06/2023
*/
CommandsManager * CommandsManager::Instance()
{
  // Create a static instance of ScreensManager (singleton pattern)
  static CommandsManager *Instance = new CommandsManager();
  return Instance;
}

bool CommandsManager::IsConnected()
{
  return this->m_IsConnected;
}

void CommandsManager::NewCommand(int code, QStringList args)
{
  emit this->SendNewCommand(code, args);
}

void CommandsManager::Connected()
{
  this->m_IsConnected = true;
}

void CommandsManager::Disconnected()
{
  this->m_IsConnected = false;
}

void CommandsManager::NewAcknowledge(int code, QList<QStringList> lines)
{
  if (IsAcknowledge(lines) == false)
    return;
    
  switch (code)
  {
    default: return;
    case CMD_PLAY: this->Parse_CMD_PLAY(lines); return; 
    case CMD_STOP: this->Parse_CMD_STOP(lines); return; 
    case CMD_FILES: this->Parse_CMD_FILES(lines); return;
    case CMD_STATUS: this->Parse_CMD_STATUS(lines); return;
  }
}

void CommandsManager::Setup()
{
}

void CommandsManager::SetupEvent()
{
}

int CommandsManager::IsError(QList<QStringList> lines)
{
  QStringList args = lines.first();
  if (args.size() == 0)
    return -1;

  QString type = args.takeFirst();
  if (type != "E")
    return 0;
  
  return args.takeFirst().toInt();
}

bool CommandsManager::IsAcknowledge(QList<QStringList> lines)
{
  QStringList args = lines.last();
  if (args.size() == 0)
    return false;

  return args.first() == "*";
}

#pragma region COMMAND .PLAY

void CommandsManager::Parse_CMD_PLAY(QList<QStringList> lines)
{
  emit this->SendPlayAcknowledge(this->IsError(lines));
}

#pragma endregion

#pragma region COMMAND .STOP

void CommandsManager::Parse_CMD_STOP(QList<QStringList> lines)
{
  emit this->SendStopAcknowledge(this->IsError(lines));
}

#pragma endregion

#pragma region COMMAND .FILES

void CommandsManager::Parse_CMD_FILES(QList<QStringList> lines)
{
  QStringList args = lines.first();

  if (args.size() != 6)
    return;

  QString index = args.takeFirst();
  QString filename = args.takeFirst();
  QString block = args.takeFirst();
  QString filesize = args.takeFirst();

  QDateTime start_date = this->ParseDate(args.takeFirst());
  if (start_date.isNull() == true)
    return;

  QDateTime end_date = this->ParseDate(args.takeFirst());
  if (start_date.isNull() == true)
    return;

  this->SetFileStart(start_date);
  this->SetFileEnd(end_date);
}

QDateTime CommandsManager::ParseDate(QString strDate)
{
  // CH6 Date format : 123-10:04:52.000
  const QString timeFormat = "hh:mm:ss.zzz";
  QStringList date_part = strDate.split('-');

  if (date_part.size() != 2)
    return QDateTime();

  QString day_part = date_part.first();
  QString time_part = date_part.last();

  // Increment days from Jan 1, offset by -1
  QDate date = QDate(2023, 1, 1).addDays(day_part.toInt() - 1);
  QTime time = QTime::fromString(time_part, timeFormat);

  return QDateTime(date, time);
}

void CommandsManager::SetFileStart(QDateTime fileStart)
{
  if (fileStart == this->m_fileStart)
    return;

  this->m_fileStart = fileStart;
  emit SendNewFileDate(this->m_fileStart, this->m_fileEnd);
}

void CommandsManager::SetFileEnd(QDateTime fileEnd)
{
  if (fileEnd == this->m_fileEnd)
    return;

  this->m_fileEnd = fileEnd;
  emit SendNewFileDate(this->m_fileStart, this->m_fileEnd);
}

QDateTime CommandsManager::FileStart()
{
  return this->m_fileStart;
}

QDateTime CommandsManager::FileEnd()
{
  return this->m_fileEnd;
}

#pragma endregion

#pragma region COMMAND .STATUS

void CommandsManager::Parse_CMD_STATUS(QList<QStringList> lines)
{
  emit this->SendStatusAcknowledge();
  QStringList args = lines.first();

  if (args.size() < 4)
    return;

  QString type = args.takeFirst();
  if (type != "S")
    return;

  int state = args.takeFirst().toInt();
  this->SetStatus(state);

  // Not used
  QString nonCriticalWarningcount = args.takeFirst();
  QString criticalWarningCount = args.takeFirst();

  if (args.size() == 0)
  {
    this->SetPercentage(0);
    return;
  }

  QString percent = args.takeFirst();
  percent.remove("%");

  this->SetPercentage(percent.toInt());
}

void CommandsManager::SetStatus(int state)
{
  if (state == this->m_Status)
    return;

  this->m_Status = state;
  emit SendNewStatus(this->m_Status);
}

void CommandsManager::SetPercentage(int percentage)
{
  if (percentage == this->m_Percentage)
    return;

  this->m_Percentage = percentage;
  emit SendNewPercentage(this->m_Percentage);
}

int CommandsManager::Status()        { return this->m_Status;    }
int CommandsManager::Percentage()   { return this->m_Percentage; }

#pragma endregion

#pragma region COMMAND .CONFIG STATUS

void CommandsManager::Parse_CMD_CONFIG_STATUS(QList<QStringList> lines)
{
}

#pragma endregion

#pragma region COMMAND .CONFIG STATUS N

void CommandsManager::Parse_CMD_CONFIG_STATUS_N(QList<QStringList> lines)
{
  QStringList args = lines.first();

  if (args.size() != 21)
    return;

  QString index = args.takeFirst();

  QString code = args.takeFirst();

  QString BS = args.takeFirst();
  QString SF = args.takeFirst();
  QString ID = args.takeFirst();
  QString FreqBit = args.takeFirst();
}

#pragma endregion

#pragma region COMMAND .TMATS CHECKSUM

void CommandsManager::Parse_CMD_TMATS_CHECKSUM(QList<QStringList> lines)
{
  emit this->SendChecksumAcknowledge();

  QStringList args = lines.first();

  if (args.size() != 1)
    return;

  QString arg = args.first();

  QStringList datas = arg.split('-');

  QString TestArea = datas.first();
  QString Checksum = datas.last();

  this->SetChecksum(Checksum);
}

void CommandsManager::SetChecksum(QString checksum)
{
  if (checksum == this->m_Checksum)
    return;

  this->m_Checksum = checksum;
  emit this->SendNewChecksum(this->m_Checksum);
}

QString CommandsManager::Checksum() { return this->m_Checksum; }

#pragma endregion

#pragma region COMMAND .TMATS READ

void CommandsManager::Parse_CMD_TMATS_READ(QList<QStringList> lines)
{
  QString tmats;

  for (QStringList line : lines)
  {
    tmats += line.join("");
  }

  emit this->SendNewTmats(tmats);
}

#pragma endregion