#include "stdafx.h"
#include "ConfigurationsManager.h"

ConfigurationsManager::ConfigurationsManager(QObject *parent) : QObject(parent)
{
  Setup();
  SetupEvent();
}

ConfigurationsManager::~ConfigurationsManager()
{

}

ConfigurationsManager * ConfigurationsManager::Instance()
{
  // Create a static instance of ScreensManager (singleton pattern)
  static ConfigurationsManager *Instance = new ConfigurationsManager();
  return Instance;
}

void ConfigurationsManager::Setup()
{
  // Instanciate the timer
  m_StatusTimer = new QTimer();
  m_StatusTimer->setInterval(100);

  m_ChecksumTimer = new QTimer();
  m_ChecksumTimer->setInterval(100);

  this->m_Tmats = new Tmats(this);

  // Start the timer
  m_StatusTimer->start();
  m_ChecksumTimer->start();
}

void ConfigurationsManager::SetupEvent()
{
  QObject::connect(m_StatusTimer, &QTimer::timeout, this, &ConfigurationsManager::AskStatus);
  QObject::connect(m_ChecksumTimer, &QTimer::timeout, this, &ConfigurationsManager::AskChecksum);
  QObject::connect(qCommands, &CommandsManager::SendNewChecksum, this, &ConfigurationsManager::AskTmats);

  QObject::connect(qCommands, &CommandsManager::SendStatusAcknowledge, this, &ConfigurationsManager::StatusAcknowledge);
  QObject::connect(qCommands, &CommandsManager::SendChecksumAcknowledge, this, &ConfigurationsManager::ChecksumAcknowledge);

  QObject::connect(qCommands, &CommandsManager::SendNewTmats, this, &ConfigurationsManager::NewTmats);
}

void ConfigurationsManager::AskStatus()
{
  if (qCommands->IsConnected() == false)
    return;

  // Ask regularly for status of the R&R
  qCommands->NewCommand(CMD_STATUS);
  this->m_StatusTimer->stop();
}

void ConfigurationsManager::AskChecksum()
{
  if (qCommands->IsConnected() == false)
    return;

  // Ask regularly for see is there is a Tmats change
  qCommands->NewCommand(CMD_TMATS_CHECKSUM);
  this->m_ChecksumTimer->stop();
}

void ConfigurationsManager::AskTmats()
{
  qCommands->NewCommand(CMD_TMATS_READ);
}

void ConfigurationsManager::StatusAcknowledge()
{
  this->m_StatusTimer->start();
}

void ConfigurationsManager::ChecksumAcknowledge()
{
  this->m_ChecksumTimer->start();
}

void ConfigurationsManager::NewTmats(QString tmats)
{
  m_Tmats->Clear();
  m_Tmats->DecodeTmats(tmats);

  emit this->SendNewUpdate();
}