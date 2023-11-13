#include "stdafx.h"
#include "StoragesManager.h"

#include <windows.h>
#include <shellapi.h>
#include <tchar.h>
#include <iostream>
#include <commoncontrols.h>

/*
 * @brief Constructor for StoragesManager.
 *
 * @param parent The parent QObject of the StoragesManager object.
 *
 * @date 31/05/2023
 */
StoragesManager::StoragesManager(QObject *parent) : QObject(parent)
{
  Initialize();
}

/*
 * @brief Destructor for StoragesManager.
 *
 * @date 31/05/2023
 */
StoragesManager::~StoragesManager()
{
  Terminate();
}

/*
 * @brief Returns the instance of StoragesManager.
 *
 * This function returns a pointer to the instance of StoragesManager.
 * If the instance does not exist yet, it creates a static instance of StoragesManager.
 * Subsequent calls to this function will always return the same instance.
 *
 * @return A pointer to the instance of StoragesManager.
 *
 * @date 31/05/2023
 */
StoragesManager * StoragesManager::Instance()
{
  static StoragesManager *Instance = new StoragesManager();
  return Instance;
}

/*
 * @brief Initialize the StoragesManager.
 *
 * This function initializes the StoragesManager by updating the list of storages
 * And installing a native event filter.
 *
 * @return true if initialization is successful, false otherwise.
 *
 * @date 31/05/2023
 */
bool StoragesManager::Initialize()
{
  // Clear the list of storages
  this->Storages.clear();

  // Update the list of storages
  // Use a separated thread to cache data
  // Windows cause large delay problem with UNC entries when network is down !
  QtConcurrent::run([=]() { this->Update(); });

  // Create a new native event filter and install it
  this->NativeEventFilter = new StoragesEventFilter();
  qApp->installNativeEventFilter(this->NativeEventFilter);

  return true;
}

/*
 * @brief Terminate the StoragesManager.
 *
 * This function terminates the StoragesManager by clearing the list of storages
 * and removing the native event filter.
 *
 * @date 31/05/2023
 */
void StoragesManager::Terminate()
{
  // Clear the list of storages
  this->Storages.clear();

  // Remove the native event filter
  qApp->removeNativeEventFilter(this->NativeEventFilter);
}

/*
 * @brief Update the list of storages.
 *
 * This function updates the list of storages by retrieving all mounted volumes,
 * filtering out invalid, unready, and read-only storages, and emitting a signal
 * to notify that the mounted volumes have been updated.
 *
 * @date 31/05/2023
 */
void StoragesManager::Update()
{
  // Get the list of all mounted volumes
  QList<QStorageInfo> storages = QStorageInfo::mountedVolumes();

  // Create a filtered list of storages
  QList<QStorageInfo> FilteredStorages;
  for (int i = 0; i < storages.count(); i++)
  {
    // Skip invalid storages
    if (storages[i].isValid() == false)
      continue;

    // Skip unready storages
    if (storages[i].isReady() == false)
      continue;

    // Skip read-only storages
    if (storages[i].isReadOnly() == true)
      continue;

    // Add the storage to the filtered list
    FilteredStorages.append(storages[i]);
  }

  // If the filtered list is the same as the current list, return
  if (FilteredStorages == this->Storages)
    return;

  // Update the list of storages
  this->Storages = FilteredStorages;

  // Emit signal to notify that the mounted volumes have been updated
  emit SendMountedVolumesUpdate();
}

/*
 * @brief Get the display name of a storage.
 *
 * @param info The QStorageInfo object representing the storage.
 * @return The display name of the storage.
 *
 * @date 31/05/2023
 */
QString StoragesManager::Name(QStorageInfo info)
{
  QString displayName = "";
  QString name = info.name();
  QString rootPath = info.rootPath();

  displayName += name.isEmpty() == true ? tr("DISK") : name;
  displayName += " ";
  displayName += rootPath.size() <= 1 ? "" : "(" + rootPath.remove("/") + ")";

  return displayName;
}

QIcon StoragesManager::IconFromPath(QString path)
{
  QFileIconProvider provider;
  return provider.icon(QFileInfo(path));
}

QPixmap StoragesManager::PixmapFromPath(QString path, QSize size) 
{
  HICON hIcon;
  HRESULT error;
  SHFILEINFO sfi;
  HIMAGELIST* imageList;

  QString wPath = path;
  wPath.replace("/", "\\");

  // Convert the QString to wchar_t array
  wchar_t lpcwPath[1024];
  wPath.toWCharArray(lpcwPath);

  // Ensure the wchar_t array is null-terminated
  lpcwPath[path.length()] = 0;

  // Get the SHIL_JUMBO icon
  error = SHGetImageList(SHIL_JUMBO, IID_IImageList, (void**)&imageList);
  if (FAILED(error) == true)
    return IconFromPath(path).pixmap(size);

  error = SHGetFileInfo(lpcwPath, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX);
  if (FAILED(error) == true)
    return IconFromPath(path).pixmap(size);

  error = ((IImageList*)imageList)->GetIcon(sfi.iIcon, ILD_TRANSPARENT, &hIcon);
  if (FAILED(error) == true)
    return IconFromPath(path).pixmap(size);

  QIcon jumboIcon = QIcon(QtWin::fromHICON(hIcon));
  QPixmap jumboPixmap = jumboIcon.pixmap(size);

  // Check if this pixmap is a "real" jumbo
  if (Tools::IsJumboPixmap(jumboPixmap))
  {
    DestroyIcon(hIcon);
    hIcon = NULL;

    return jumboPixmap;
  }

  // Get the SHIL_EXTRALARGE icon
  error = SHGetImageList(SHIL_EXTRALARGE, IID_IImageList, (void**)&imageList);
  if (FAILED(error) == true)
    return jumboPixmap;

  error = SHGetFileInfo(lpcwPath, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX);
  if (FAILED(error) == true)
    return jumboPixmap;

  error = ((IImageList*)imageList)->GetIcon(sfi.iIcon, ILD_TRANSPARENT, &hIcon);
  if (FAILED(error) == true)
    return jumboPixmap;

  QIcon largeIcon = QIcon(QtWin::fromHICON(hIcon));

  return largeIcon.pixmap(size).scaled(size * 0.75);
}




/*
 * @brief Get the available space percentage of a storage.
 *
 * @param info The QStorageInfo object representing the storage.
 * @return The available space percentage of the storage.
 *
 * @date 31/05/2023
 */
int StoragesManager::AvailableSpace(QStorageInfo info)
{
  qint64 max = info.bytesTotal();
  qint64 free = info.bytesFree();
  qint64 used = max - free;
  return (int)((used * 100) / max);
}

/*
 * @brief Get the color for the available space of a storage.
 *
 * @param info The QStorageInfo object representing the storage.
 * @return The color for the available space of the storage.
 *
 * @date 31/05/2023
 */
Color StoragesManager::AvailableSpaceColor(QStorageInfo info)
{
  int percentage = AvailableSpace(info);
  double ratio = 1 - (percentage / 100.);

  if (ratio < 0.05) return RED_01;
  if (ratio <  0.2) return ORANGE_01;
  if (ratio <  0.5) return YELLOW_01;
  if (ratio <  1.0) return GREEN_02;
  return GREEN_02;
}

/*
 * @brief Get the storage information from a given path.
 *
 * @param path The path for which to retrieve the storage information.
 * @return The QStorageInfo object representing the storage.
 *
 * @date 31/05/2023
 */
QStorageInfo StoragesManager::StorageFromPath(QString path)
{
  QStorageInfo info(path);
  return info;
}

/*
 * @brief Get the list of mounted volumes.
 *
 * @return The list of mounted volumes.
 *
 * @date 31/05/2023
 */
QList<QStorageInfo> StoragesManager::MountedVolumes()
{
  return this->Storages;
}

/*
* @brief Get the list of mounted volumes using windows API.
*
*  Update() use Qt API for cache the list of storages
*
*  Windows cause large delay problem with UNC entries when network is down !
*  And Qt API use windows API and thats why we use a separated thread to cache data.
*
*  We can use this code later to use windows API and fix large delay problem.
*  Using timeout OR filter drive (We need path, names and disk space
*
* @return The list of mounted volumes.
*
* @date 31/05/2023
*/
void StoragesManager::WindowsVolumesInformations()
{
  TCHAR drives[256];
  GetLogicalDriveStrings(256, drives);

  for (TCHAR* drive = drives; *drive; drive += _tcslen(drive) + 1)
  {
    UINT driveType = GetDriveType(drive);

    // Get volume information
    TCHAR volumeName[256];
    TCHAR fileSystemName[256];
    DWORD serialNumber;
    GetVolumeInformation(drive, volumeName, 256, &serialNumber, NULL, NULL, fileSystemName, 256);

    // Get disk space
    ULARGE_INTEGER freeBytesAvailable;
    ULARGE_INTEGER totalNumberOfBytes;
    ULARGE_INTEGER totalNumberOfFreeBytes;
    GetDiskFreeSpaceEx(drive, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes);

    // Get icon and display name
    SHFILEINFO shfi;
    SHGetFileInfo(drive, 0, &shfi, sizeof(shfi), SHGFI_ICON | SHGFI_DISPLAYNAME);

    // Print information
    std::wcout << L"Drive: " << drive << std::endl;
    std::wcout << L"Volume Name: " << volumeName << std::endl;
    std::wcout << L"File System: " << fileSystemName << std::endl;
    std::wcout << L"Total Space: " << totalNumberOfBytes.QuadPart << std::endl;
    std::wcout << L"Free Space: " << totalNumberOfFreeBytes.QuadPart << std::endl;
    std::wcout << L"Display Name: " << shfi.szDisplayName << std::endl;

    // DISPLAY TO TEST
    QPixmap pixmap = QtWin::fromHICON(shfi.hIcon);
    QLabel *label = new QLabel;
    label->setPixmap(pixmap);
    label->show();

    // Remember to destroy the icon after you're done
    DestroyIcon(shfi.hIcon);
  }
}

/***********************/
/* NATIVE EVENT FILTER */
/***********************/

/*
 * @brief Native event filter for handling device change events.
 *
 * This function is responsible for handling device change events,
 * specifically for Windows events. It updates the list of mounted volumes
 * when a device change event occurs.
 *
 * @param eventType The type of the event.
 * @param message A pointer to the event message.
 * @param result Unused.
 * @return true if the event is handled, false otherwise.
 *
 * @date 31/05/2023
 */
bool StoragesEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *)
{
  // Only handle windows event type
  if (eventType != "windows_generic_MSG")
    return false;

  // Get the message type
  MSG * msg = static_cast<MSG*> (message);
  int messageType = msg->message;

  // If the message is not a device change type, return false
  if (messageType != WM_DEVICECHANGE)
    return false;

  // Update the list of mounted volumes
  qStorages->Update();

  return false;
}
