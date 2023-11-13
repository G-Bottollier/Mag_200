#include "stdafx.h"
#include "ScreensManager.h"

/*
* @brief ScreensManager constructor
*
* @param parent The parent object of the ScreensManager.
*
* @date 14/06/2023
*/
ScreensManager::ScreensManager(QObject *parent) : QObject(parent)
{
  // Setup
  Setup();

  // Setup event
  SetupEvent();
}

/*
* @brief ScreensManager destructor
*
* @date 14/06/2023
*/
ScreensManager::~ScreensManager()
{

}

/*
* @brief Returns the instance of ScreensManager.
*
* This function returns a pointer to the instance of ScreensManager.
* If the instance does not exist yet, it creates a static instance of ScreensManager.
* Subsequent calls to this function will always return the same instance.
*
* @return A pointer to the instance of ScreensManager.
*
* @date 14/06/2023
*/
ScreensManager * ScreensManager::Instance()
{
  // Create a static instance of ScreensManager (singleton pattern)
  static ScreensManager *Instance = new ScreensManager();
  return Instance;
}

/*
* @brief Setup function
*
* This function performs the initial setup for the ScreensManager by logging the connected screens.
* It retrieves the UID of all connected screens and logs their information.
*
* @date 14/06/2023
*/
void ScreensManager::Setup()
{
  // Log String handle
  const QString Log = LOG_METHOD(ScreensManager, Setup);

  // Get all connected screens from windows
  QStringList devicesID = this->Screens();

  // Log all connected screen
  QString str = "Screen %1 : ";
  for (int i = 0; i < devicesID.size(); i++)
  {
    QStringList infos = devicesID[i].split(SEPARATOR);
    QString id = infos[DEVICE_MODEL] + SEPARATOR + infos[DEVICE_ID];
    qMessages->Log(Log, str.arg(i) + id);
  }
}

/*
* @brief SetupEvent function
*
* This function sets up event handling for the ScreensManager.
* It connects the screenAdded and screenRemoved signals of the QGuiApplication
*
* @date 16/04/2023
*/
void ScreensManager::SetupEvent()
{
  QObject::connect(qApp, &QGuiApplication::screenAdded, this, &ScreensManager::MoveApplication);
  QObject::connect(qApp, &QGuiApplication::screenRemoved, this, &ScreensManager::MoveApplication);
}

/*
* @brief Move the Application to the white listed screen
*
* This function updates the current screen and moves the application windows accordingly.
* It checks if the application needs to be moved and then retrieves the top-level widgets
* of the application and moves them based on the current screen.
*
* @date 14/06/2023
*/
void ScreensManager::MoveApplication()
{
  // Update the current screen
  bool hasNewScreen = UpdateCurrentScreen();

  // Check if we need to move window
  bool hasLostWindow = AllWindowsOnCurrentScreen();

  // Get all top level widget of the application
  const QList<QWidget*> topLevelWidgets = qApp->topLevelWidgets();

  // For each widget in topLevelWidgets
  for (QWidget* widget : topLevelWidgets)
  {
    // Check if the window handle exists
    QWindow *window = widget->windowHandle();
    if (window == nullptr)
      continue;

    updateMainWindow(widget, hasNewScreen, hasLostWindow);
    updateMessagesManager(widget, hasNewScreen, hasLostWindow);
  }
}

/*
* @brief MoveWindow function
*
* This function moves the given widget to the center of the current screen.
*
* @param widget The widget to be moved.
*
* @date 14/06/2023
*/
void ScreensManager::MoveWindow(QWidget *widget)
{
  QRect windowRect = QRect(QPoint(0, 0), widget->size());
  QRect currentRect = this->m_CurrentScreen->geometry();

  widget->move(currentRect.center() - windowRect.center());
}

/*
* @brief MoveMainWindow function
*
* This function moves the main window to the top-left corner of the current screen.
* It sets the window flags and shows the window based on the screen type.
*
* @param widget The main window widget.
*
* @date 14/06/2023
*/
void ScreensManager::MoveMainWindow(QWidget *widget)
{
  QMainWindow* mainWindow = qobject_cast<QMainWindow*>(widget);
  if (mainWindow == nullptr)
    return;

  if (this->m_UseDefaultScreen == true || this->m_Divider > 1)
  {
    // If this is the primary screen use as default screen
    mainWindow->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
    mainWindow->show();
  }
  else
  {
    // This is the white listed screen
    mainWindow->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    mainWindow->showFullScreen();
  }

  // Get the current rect of the screen
  QRect currentRect = this->m_CurrentScreen->geometry();

  // Set the position of the MainWindow to the top-left corner of the current rectangle
  if (this->m_Divider > 1) 
  {
    // Get the new App Rect according the divider
    QRect AppRect(currentRect.topLeft(), currentRect.size() / this->m_Divider);

    // Move the app to the center of the screen
    widget->move(currentRect.center() - AppRect.center());
  }
  else
    widget->move(currentRect.topLeft());
}

/*
* @brief SetWhiteList function
*
* This function sets the white listed UID of the screens.
*
* @param screensId The list of screen UID to be white listed.
*
* @date 14/06/2023
*/
void ScreensManager::SetWhiteList(QStringList screens)
{
  this->m_WhiteList = screens;
}

void ScreensManager::SetDivider(float divider)
{
  this->m_Divider = divider;
}

/*
* @brief Screens function
*
* This function retrieves the UID of all connected screens from Windows.
*
* @return The list of screen UID.
*
* @date 14/06/2023
*/
QStringList ScreensManager::Screens()
{
  DWORD screenID = 0;
  QChar separator('#');
  QStringList devicesID;
  DISPLAY_DEVICEA dispDevice;
  ZeroMemory(&dispDevice, sizeof(dispDevice));
  dispDevice.cb = sizeof(dispDevice);

  // Iterate through display devices
  while (EnumDisplayDevicesA(NULL, screenID, &dispDevice, 0))
  {
    QString name(dispDevice.DeviceName);
    // Check if the display device has a device interface name
    if (EnumDisplayDevicesA(name.toLatin1().data(), 0, &dispDevice, EDD_GET_DEVICE_INTERFACE_NAME))
    {
      // Skip if the display device is not active
      if (!(dispDevice.StateFlags & DISPLAY_DEVICE_ACTIVE))
        continue;

      // Skip if the display device is a mirroring driver
      if (dispDevice.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)
        continue;

      QString id;
      id += name;
      id += separator;
      id += dispDevice.DeviceID;

      devicesID.append(id);
    }
    ++screenID;

    // Break if screenID exceeds a certain limit
    if (screenID >= 1000)
      break;
  }
  return devicesID;
}

QScreen * ScreensManager::CurrentScreen()
{
  return this->m_CurrentScreen;
}

/*
* @brief UpdateCurrentScreen function
*
* This function updates the current screen based on the white listed screens.
*
* @return True if the current screen has been updated, false otherwise.
*
* @date 14/06/2023
*/
bool ScreensManager::UpdateCurrentScreen()
{
  // Get white listed screens
  QList<QScreen*> whiteListedScreens = WhiteListedScreens();

  this->m_UseDefaultScreen = whiteListedScreens.size() == 0;

  // Use primary screen as default if no screen found
  if (this->m_UseDefaultScreen == true)
    whiteListedScreens.append(QGuiApplication::primaryScreen());

  // Get the new screen
  QScreen *newScreen = whiteListedScreens.first();

  // If the new screen is the same as the current, no further action is needed
  if (newScreen == this->m_CurrentScreen)
    return false;

  // Disconnect any existing connections from the previous Screen
  if (this->m_CurrentScreen != nullptr)
    this->m_CurrentScreen->disconnect(this);

  // Assign the new screen as the current screen
  this->m_CurrentScreen = newScreen;

  // Connect the current screen
  QObject::connect(this->m_CurrentScreen, &QScreen::geometryChanged, this, &ScreensManager::MoveApplication);
  QObject::connect(this->m_CurrentScreen, &QScreen::availableGeometryChanged, this, &ScreensManager::MoveApplication);

  return true;
}

bool ScreensManager::AllWindowsOnCurrentScreen()
{
  // Get the current screen geometry
  QRect geometry = m_CurrentScreen->geometry();

  // Get all top-level widgets of the application
  const QList<QWidget*> topLevelWidgets = qApp->topLevelWidgets();

  // Iterate through each top-level widget
  for (QWidget* widget : topLevelWidgets)
  {
    // Check if the window handle exists
    QWindow *window = widget->windowHandle();
    if (window == nullptr)
      continue;

    QPoint position = widget->geometry().topLeft();

    // Check if the widget is on the current screen
    if (geometry.contains(position) == false)
      return false;
  }

  return true;
}

void ScreensManager::updateMainWindow(QWidget *widget, bool hasNewScreen, bool hasLostWindow)
{
  // Check if the window is the main window
  QMainWindow* mainWindow = qobject_cast<QMainWindow*>(widget);

  if (mainWindow == nullptr)
    return;

  QSize size = this->m_CurrentScreen->size();

  //Mode popup redemissionne
  if (this->m_Divider > 1) {
    size.setWidth(size.width() / this->m_Divider);
    size.setHeight(size.height() / this->m_Divider);
    widget->setFixedSize(size);
  }

  // Ensure the widget is visible
  bool m_IsVisible = widget->isVisible();

  // No need to move the application
  if (m_IsVisible == true && hasNewScreen == false && hasLostWindow == true)
    return;

  // setFlags, Show and Move the mainWindow
  MoveMainWindow(widget);
}

void ScreensManager::updateMessagesManager(QWidget *widget, bool, bool)
{
  // Check if the window is the main window
  MessagesManager* messages = qobject_cast<MessagesManager*>(widget);

  if (messages == nullptr)
    return;

  QSize size = this->m_CurrentScreen->size();

  double m_MinValue = qMin(size.width(), size.height());
  double m_MaxValue = qMax(size.width(), size.height());

  // ensure 0
  m_MaxValue = qMax(1.0, m_MaxValue);
  qreal minRatio = 0.5;
  qreal ratio = m_MinValue / m_MaxValue;

  ratio = ratio < minRatio ? minRatio : ratio;

  int width = size.width() * ratio;
  int height = size.height() * minRatio;

  widget->setFixedSize(QSize(width, height));
  MoveWindow(widget);
}

/*
* @brief WhiteListedScreens function
*
* This function retrieves the white listed screens based on the screen UID.
* Return an empty list screens are not found !
*
* @return The list of white listed screens.
*
* @date 14/06/2023
*/
QList<QScreen*> ScreensManager::WhiteListedScreens()
{
  // Initialize list to return
  QList<QScreen*> whiteListedScreens;

  // Screens list from windows
  QStringList devicesID = this->Screens();

  // Screen list from Qt (before Qt 5.9 only name of the screen is available)
  QList<QScreen*> screens = qApp->screens();

  for (int i = 0; i < devicesID.size(); i++)
  {
    // Get the screen infos
    QStringList infos = devicesID[i].split(SEPARATOR);
    if (infos.size() < NB_DEVICE_INFO)
      continue;

    // Construct the screen UID
    QString uid = infos[DEVICE_MODEL] + SEPARATOR + infos[DEVICE_ID];

    // Skip if the id is not in the screenWhiteListed
    if (m_WhiteList.contains(uid) == false)
      continue;

    for (int j = 0; j < screens.size(); j++)
    {
      QScreen *screen = screens[j];
      QString name = screen->name();

      // Skip if the screen name is not in the screenNames list
      if (name.contains(infos[DEVICE_NAME]) == false)
        continue;

      // Add to the list the white listed screen found
      whiteListedScreens.append(screen);
    }
  }
  return whiteListedScreens;
}