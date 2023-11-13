#ifndef SCREENSMANAGER_H
#define SCREENSMANAGER_H

#include <QtWidgets>

#include "MessagesManager.h"

// Convenience macro to access the SettingsManager instance
#define qScreens (static_cast<ScreensManager*>(ScreensManager::Instance()))

#define NB_DEVICE_INFO  5
#define DEVICE_NAME     0
#define DEVICE_MODEL    2
#define DEVICE_ID       3
#define SEPARATOR       "#"
#define IS_MAG_200      "IsMag200"

/*
* @brief ScreensManager class
*
* The ScreensManager class is responsible for managing the screens connected to the system.
* It provides functionality to move the application to the whitelisted screen
* to retrieve the list of connected screens and move application windows accordingly.
*
* @date 14/06/2023
*/
class ScreensManager : public QObject
{
  Q_OBJECT

  private:
    bool m_IsFlagInitialized = false;
    bool m_UseDefaultScreen = false;
    float m_Divider = 1;

    QScreen *m_CurrentScreen = nullptr;
    
    QStringList m_WhiteList;
    QList<QScreen*> WhiteListedScreens();

    void Setup();
    void SetupEvent();

    bool UpdateCurrentScreen();
    bool AllWindowsOnCurrentScreen();

    void updateMainWindow(QWidget *widget, bool hasNewScreen, bool hasLostWindow);
    void updateMessagesManager(QWidget *widget, bool hasNewScreen, bool hasLostWindow);

    void MoveWindow(QWidget *window);
    void MoveMainWindow(QWidget *mainWindow);

  private:
    // Constructor
    explicit ScreensManager(QObject *parent = nullptr);

  public:
    // Destructor
    ~ScreensManager();

    static ScreensManager * Instance();

    QStringList Screens();
    QScreen* CurrentScreen();

    void SetWhiteList(QStringList screens);
    void SetDivider(float divider);

  public slots:
    void MoveApplication();
};

#endif // SCREENSMANAGER_H
