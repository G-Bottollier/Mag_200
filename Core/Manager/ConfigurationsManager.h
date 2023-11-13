#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <QObject>
#include "Tmats.h"
#include "CommandsManager.h"

// Convenience macro to access the MessagesManager instance
#define qConfigurations (static_cast<ConfigurationsManager*>(ConfigurationsManager::Instance()))

class ConfigurationsManager : public QObject
{
  Q_OBJECT

  private:
    Tmats *m_Tmats = nullptr;

    QTimer *m_StatusTimer;
    QTimer *m_ChecksumTimer;
    
    void Setup();
    void SetupEvent();

  private:
    // Constructor
    explicit ConfigurationsManager(QObject *parent = nullptr);

  public:
    // Destructor
    ~ConfigurationsManager();

    static ConfigurationsManager * Instance();

  public slots:
    void AskTmats();
    void AskStatus();
    void AskChecksum();

    void StatusAcknowledge();
    void ChecksumAcknowledge();

    void NewTmats(QString tmats);

  signals:
    void SendNewUpdate();
};

#endif // CONFIGURATIONMANAGER_H
