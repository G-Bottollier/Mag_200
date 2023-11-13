#include "stdafx.h"
#include "Chapter6Command.h"

// STATIC MEMBERS !
bool                Chapter6Command::IsInitialized = false;
QHash<int, QString> Chapter6Command::m_Codes;

Chapter6Command::Chapter6Command(QObject *parent) : QObject(parent)
{
  if(this->IsInitialized == false)
    this->InitializeCodes();
}

Chapter6Command::~Chapter6Command()
{

}

QList<QStringList> Chapter6Command::Lines()
{
  return this->m_Lines;
}

void Chapter6Command::clear()
{
  command.clear();
  data.clear();

  hasAcknowledge = false;

  m_Lines.clear();
}

bool Chapter6Command::IsAcknowledge()
{
  hasAcknowledge = data.endsWith("*\r\n");
  if (hasAcknowledge == false)
    return false;
  return true;
}

bool Chapter6Command::ReadData()
{
  QString command = QString::fromLocal8Bit(this->data);

  QStringList lines = command.split('\n', QString::SkipEmptyParts);
  for (int i = 0; i < lines.size(); i++)
  {
    // Remove \t, \n, \v, \f, \r character
    QString line = lines[i].trimmed();

    // Split args by space
    QStringList args = lines[i].split(" ", QString::SkipEmptyParts);

    if (args.size() != 0)
    {
      int index = args.size() - 1;
      args[index] = args[index].trimmed();
    }

    // Append to the list of args
    m_Lines.append(args);
  }

  if (m_Lines.size() == 0)
    return false;

  return true;
}

bool Chapter6Command::SendCommand(QTcpSocket *socket)
{
  if (this->command.isEmpty() == true)
    return false;

  QByteArray bytes = this->command.toLocal8Bit();
  bytes += "\r\n";

  QDataStream Out(&bytes, QIODevice::WriteOnly);
  Out.setVersion(QDataStream::Qt_5_5);

  qint64 size = socket->write(bytes);

  if (size != bytes.size())
    return false;

  bool written = socket->waitForBytesWritten(1000);

  if (written == false)
    return false;

  return true;
}

bool Chapter6Command::ReadAcknowledge(QTcpSocket * socket)
{
  this->data += socket->readAll();
  bool OK = false;

  OK = IsAcknowledge();
  if(OK == false)
    return false;

  OK = ReadData();
  if (OK == false)
    return false;

  return true;
}

bool Chapter6Command::DataOverflow()
{
  return this->data.size() > MAX_SIZE;
}

bool Chapter6Command::HasAcknowledge()
{
  return this->hasAcknowledge;
}

bool Chapter6Command::MakeCommand(QPair<int, QStringList> command)
{
  int code = command.first;
  QStringList args = command.second;

  if (m_Codes.keys().contains(code) == false)
    return false; // unsupported

  this->command = m_Codes[code];

  if (args.size() == 0)
    return true;

  this->command += " ";
  this->command += args.join(' ');

  return true;
}

void Chapter6Command::InitializeCodes()
{
  /* DEFAULT COMMANDS */
  m_Codes[CMD_PLAY] = ".PLAY";
  m_Codes[CMD_STOP] = ".STOP";
  m_Codes[CMD_FILES] = ".FILES";
  m_Codes[CMD_STATUS] = ".STATUS";

  /* TMATS COMMANDS */
  m_Codes[CMD_TMATS_CHECKSUM] = ".TMATS CHECKSUM";
  m_Codes[CMD_TMATS_WRITE] = ".TMATS WRITE";
  m_Codes[CMD_TMATS_GET] = ".TMATS GET";
  m_Codes[CMD_TMATS_READ] = ".TMATS READ";

  IsInitialized = true;

  return;

  // !!! UNSUPORTED !!!
  m_Codes[CMD_RCC_106]          = ".RCC-106";
  m_Codes[CMD_TIME]             = ".TIME";
  m_Codes[CMD_PUBLISH_FILE]     = ".PUBLISH_FILE";
  m_Codes[CMD_EVENT]            = ".EVENT";
  m_Codes[CMD_PUBLISH]          = ".PUBLISH";
  m_Codes[CMD_DISMOUNT]         = ".DISMOUNT";
  m_Codes[CMD_ASSIGN]           = ".ASSIGN";
  m_Codes[CMD_VERBOSE]          = ".VERBOSE";
  m_Codes[CMD_DATE]             = ".DATE";
  m_Codes[CMD_PUBLISH_TCP]      = ".PUBLISH_TCP";
  m_Codes[CMD_DUB]              = ".DUB";
  m_Codes[CMD_PUBLISH_CFG]      = ".PUBLISH_CFG";
  m_Codes[CMD_ERASE]            = ".ERASE";

  m_Codes[CMD_TMATS]            = ".TMATS";
  m_Codes[CMD_CRITICAL]         = ".CRITICAL";
  m_Codes[CMD_HELP]             = ".HELP";
  m_Codes[CMD_QUEUE]            = ".QUEUE";
  m_Codes[CMD_RESET]            = ".RESET";
  m_Codes[CMD_CONFIG]           = ".CONFIG";
  m_Codes[CMD_COPY]             = ".COPY";
  m_Codes[CMD_BBREAD]           = ".BBREAD";
  m_Codes[CMD_MOUNT]            = ".MOUNT";
  m_Codes[CMD_LOOP]             = ".LOOP";
  m_Codes[CMD_MEDIA]            = ".MEDIA";
  m_Codes[CMD_IRIG106]          = ".IRIG106";
  m_Codes[CMD_TCPPORTS]         = ".TCPPORTS";
  m_Codes[CMD_REPLAY]           = ".REPLAY";
  m_Codes[CMD_SETUP]            = ".SETUP";
  m_Codes[CMD_SANITIZE]         = ".SANITIZE";
  m_Codes[CMD_RECORD]           = ".RECORD";
  m_Codes[CMD_OUT_CRATE]        = ".OUT_CRATE";
  m_Codes[CMD_DRIVE]            = ".DRIVE";
  m_Codes[CMD_BBSECURE]         = ".BBSECURE";
  m_Codes[CMD_BBLIST]           = ".BBLIST";
  m_Codes[CMD_VOLUME]           = ".VOLUME";
  m_Codes[CMD_HEALTH]           = ".HEALTH";
  m_Codes[CMD_IRIG_106]         = ".IRIG-106";
  m_Codes[CMD_FIND]             = ".FIND";
  m_Codes[CMD_PAUSE]            = ".PAUSE";
  m_Codes[CMD_ETOELOOP]         = ".ETOELOOP";
  m_Codes[CMD_BIT]              = ".BIT";
  m_Codes[CMD_RESUME]           = ".RESUME";
  m_Codes[CMD_STREAM]           = ".STREAM";

  // TMATS SUB COMMANDS
  m_Codes[CMD_TMATS_DELETE]     = ".TMATS DELETE";
  m_Codes[CMD_TMATS_CHECKSUM]   = ".TMATS CHECKSUM";
  m_Codes[CMD_TMATS_WRITE]      = ".TMATS WRITE";
  m_Codes[CMD_TMATS_GET]        = ".TMATS GET";
  m_Codes[CMD_TMATS_READ]       = ".TMATS READ";
  m_Codes[CMD_TMATS_SAVE]       = ".TMATS SAVE";
  m_Codes[CMD_TMATS_VERSION]    = ".TMATS VERSION";
}