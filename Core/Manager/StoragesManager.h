#ifndef STORAGESMANAGER_H
#define STORAGESMANAGER_H

#include <QtWin>
#include <QtConcurrent>
#include <QtWidgets>

#include "Tools.h"
#include "Status.h"

// Convenience macro to access the SettingsManager instance
#define qStorages (static_cast<StoragesManager*>(StoragesManager::Instance()))

class StoragesEventFilter;

/*
 * @brief The StoragesManager class represents a manager for storage devices of the application.
 *
 * The StoragesManager class provides functionality to manage storage devices,
 * including initializing, updating, and accessing information about the mounted volumes.
 *
 * @date 31/05/2023
 */
class StoragesManager : public QObject
{
  Q_OBJECT

  private:
    QList<QStorageInfo> Storages;
    StoragesEventFilter *NativeEventFilter = nullptr;

    bool Initialize();
    void Terminate();

    void WindowsVolumesInformations();

  private:
    // Constructor
    explicit StoragesManager(QObject *parent = nullptr);

  public:
    // Destructor
    ~StoragesManager();

    static StoragesManager * Instance();

    void Update();

    QList<QStorageInfo> MountedVolumes();

    // Static
    static QString Name(QStorageInfo info);
    static QIcon IconFromPath(QString path);
    static QStorageInfo StorageFromPath(QString path);
    static QPixmap PixmapFromPath(QString path, QSize size);

    
    static int AvailableSpace(QStorageInfo info);
    static Color AvailableSpaceColor(QStorageInfo info);

  signals:
    void SendMountedVolumesUpdate();
};


/***********************/
/* NATIVE EVENT FILTER */
/***********************/

/*
 * @brief The StoragesEventFilter class represents a native event filter for handling device change events.
 *
 * The StoragesEventFilter class is responsible for handling device change events,
 * specifically for Windows events. It updates the list of mounted volumes
 * when a device change event occurs.
 */
class StoragesEventFilter : public QAbstractNativeEventFilter
{
  public:
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *) override;
};

#endif STORAGESMANAGER_H