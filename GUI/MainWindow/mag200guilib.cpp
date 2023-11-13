/* (EVOL_MAG200_CFG) JMB 05/06/23 : Fichier de configuration global dans lca mc_mag200.cfg */

#include "stdafx.h"
#include "mag200guilib.h"

#include <mwstrt.h>
#include <mag200_str.h>


#define NB_DEVICE_INFO  5
#define DEVICE_NAME     0
#define DEVICE_MODEL    2
#define DEVICE_ID       3
#define IS_MAG_200      "IsMag200"

/*
 * @brief The constructor for the CMag200ShellGui class.
 *
 * This constructor initializes the GUI application by setting up the message manager, instantiating the MAG200 controller,
 * setting up the user interface, and setting up event handlers.
 *
 * @param parent: The parent widget of the main window.
 *
 * @date 31/05/2023
 */
CMag200ShellGui::CMag200ShellGui(QWidget *parent) : QMainWindow(parent)
{
  // Create the window handle
  this->createWinId();

  // Initialize message manager parent
  qMessages->InitParent(this);
  qMessages->SetTitle(tr("MESSAGE_TITLE"));

  // Instantiate MAG200 controller
  qMessages->SetScrollPosition(MessagesManager::Auto);
  qMessages->SetDescription(tr("MESSAGE_LOAD_MAG_200"));
  m_Mag200Controller = new Mag200Controller(this);

  // Setup UI
  qMessages->SetScrollPosition(MessagesManager::Auto);
  qMessages->SetDescription(tr("MESSAGE_START_UI"));
  SetupUI();

  // Setup event handlers
  qMessages->SetScrollPosition(MessagesManager::Auto);
  qMessages->SetDescription(tr("MESSAGE_START_EVENT"));
  SetupEvent();
}

/*
 * @brief The destructor for the CMag200ShellGui class.
 *
 * @date 31/05/2023
 */
CMag200ShellGui::~CMag200ShellGui()
{
}

void CMag200ShellGui::resizeEvent(QResizeEvent * event)
{
  QSize size = event->size();

  qreal ratio = 1.0;
  int height = Tools::GetWidgetHeight(this, size, MainLayout);

  qreal pageManagerRatio = 1.0;
  qreal toolbarRatio = 0.2 * !m_PageManager->UseFullPage();
  qreal statusBarRatio = 0.1 * !m_PageManager->UseFullPage();

  // Calculate widgets height
  int availableHeight = height;
  int toolbarHeight = Tools::GetWidgetHeight(toolbarRatio, height, availableHeight, ratio);
  int statusBarHeight = Tools::GetWidgetHeight(statusBarRatio, height, availableHeight, ratio);
  int pageManagerHeight = Tools::GetWidgetHeight(pageManagerRatio, height, availableHeight, ratio);

  m_ToolbarManager->setFixedSize(size.width(), toolbarHeight);
  m_PageManager->setFixedSize(size.width(), pageManagerHeight);
  m_StatusBar->setFixedSize(size.width(), statusBarHeight);

  QWidget::resizeEvent(event);
}

/*
* @brief Override the Key press event
*
* @date 31/05/2023
*/
void CMag200ShellGui::keyReleaseEvent(QKeyEvent *event)
{
  QWidget::keyReleaseEvent(event);

  // If Ctrl key is not pressed then return
  if ((event->modifiers() == Qt::ControlModifier) == false)
    return;

  // Check pressed key
  switch (event->key())
  {
    default:
      return;

    // Open Log
    case Qt::Key_F1:
      qMessages->OpenLog();
      return;

    // If Ctrl + (top left key) Open Page Settings (Shortcut)
    case Qt::Key_twosuperior: // AZERTY
    case Qt::Key_QuoteLeft:   // QWERTY
      if(m_PageManager)
        m_PageManager->setCurrentIndex(PAGE_SETTINGS);
      return;
  }
}

/*
 * @brief Deletes the central widget of the main window.
 *
 * This function deletes the central widget of the main window by calling deleteLater() on the central widget and processing all pending events.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void CMag200ShellGui::DeleteCentralWidget()
{
  m_ToolbarManager->disconnect();
  m_PageManager->disconnect();
  m_StatusBar->disconnect();

  // Delete the central widget
  this->centralWidget()->deleteLater();
}

/*
 * @brief Reloads the GUI by deleting and re-creating the central widget.
 * 
 * This function reloads the GUI by performing the following steps:
 * 1. Deletes the current central widget.
 * 2. Sets up the user interface.
 * 3. Sets up event connections.
 * 4. Reads the layout file.
 * 5. Reads the config file.
 * 6. Sends all status.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void CMag200ShellGui::Reload()
{
  // (TEMP_DEMO) Prevent reload will starting mw_vtr32
  return;

  // Pause Thread while reloading
  m_Mag200Controller->PauseMag200Core();

  // Delete and re-create the central widget
  DeleteCentralWidget();

  SetupUI();
  SetupEvent();

  QResizeEvent *event = new QResizeEvent(this->size(), this->size());
  this->resizeEvent(event);
  delete event;

  // Reload layout and config files, and send all status
  m_Mag200Controller->SendAllStatus();

  // UnPause Thread while reloading
  m_Mag200Controller->UnPauseMag200Core();

  // Reloaded from magali preparation mean waiting for acquisition
  qMessages->NewWarning(tr("STATUS_RELOAD"));
}

/*
 * @brief Initializes the CMag200ShellGui class.
 *
 * This function initializes the CMag200ShellGui class by performing
 * several initialization tasks:
 * 1. Reads the layout configuration file.
 * 2. Reads the general configuration file.
 * 3. Reads the work area configuration file.
 * 4. Starts the MAG200 core.
 * 5. Closes the messages.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void CMag200ShellGui::Initialize()
{
  // Start MAG200 core
  qMessages->SetDescription(QCoreApplication::tr("START_MAG200_CORE"));
  m_Mag200Controller->StartMag200Core();
  qMessages->SetScrollPosition(MessagesManager::Auto);

  // Move the application to the white listed screen
  qScreens->MoveApplication();
  
  // Close messages
  qMessages->SetScrollPosition(MessagesManager::Max, true);
  qMessages->Close();
}


/*
/* @brief Sets up the user interface for the CMag200ShellGui class.
 *
 * This function sets up the user interface for the CMag200ShellGui class.
 * It creates the main window, sets the object name and window icon,
 * and creates the central widget, main layout, toolbar, page manager, and status bar.
 * The function adds the toolbar, page manager, and status bar to the main layout
 * and sets the central widget for the main window.
 *
 * @return void
 *
 * @date 31/05/2023
/*/
void CMag200ShellGui::SetupUI()
{
  this->setObjectName("MainWindow");
  this->setWindowIcon(QIcon(qSettings->App_Icon()));
  
  // Create central widget
  QWidget *CentralWidget = new QWidget(this);

  // Create main layout
  MainLayout = new QVBoxLayout();
  MainLayout->setMargin(0);
  MainLayout->setSpacing(1);

  // Create toolbar, page manager, and status bar
  m_ToolbarManager = new ToolbarManager();
  m_PageManager = new PagesManager();
  m_StatusBar = new StatusBar();

  // Add toolbar, page manager, and status bar to the main layout
  MainLayout->addWidget(m_ToolbarManager);
  MainLayout->addWidget(m_PageManager);
  MainLayout->addWidget(m_StatusBar);

  // Set the main layout for the central widget
  CentralWidget->setLayout(MainLayout);

  // Set the central widget for the main window
  this->setCentralWidget(CentralWidget);
}

/*
 * @brief Sets up event connections for the CMag200ShellGui class.
 *
 * This function establishes various event connections for different components of the CMag200ShellGui class.
 * It connects signals and slots to enable communication between different objects and trigger appropriate actions.
 * The event connections include connections related to the GUI application, screen changes, toolbar actions,
 * the Mag200Controller's signals, the PageManager's signals, and the StatusBar's signals.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void CMag200ShellGui::SetupEvent()
{
  /*******************/
  /** INTERN OBJECT **/
  /*******************/

  // TOOLBAR
  QObject::connect(m_ToolbarManager, &ToolbarManager::SendLedClicked, m_PageManager, &PagesManager::LedClicked);
  QObject::connect(m_ToolbarManager, &ToolbarManager::SendOpenParentFolder, m_PageManager, &PagesManager::NewOpenParentFolder);

  // MAG 200 CONTROLLER
  QObject::connect(m_Mag200Controller, &Mag200Controller::SendNewStatus, m_ToolbarManager, &ToolbarManager::NewStatus);

  QObject::connect(m_Mag200Controller, &Mag200Controller::SendNewStatus, m_PageManager, &PagesManager::NewStatus);
  QObject::connect(m_Mag200Controller, &Mag200Controller::SendNewChannelData, m_PageManager, &PagesManager::NewChannelData);

  QObject::connect(m_Mag200Controller, &Mag200Controller::SendNewTestArea, m_StatusBar, &StatusBar::NewTestArea);

  QObject::connect(m_Mag200Controller, &Mag200Controller::SendCopyProgress, m_PageManager, &PagesManager::NewCopyProgress);

  // PAGE MANAGER
  QObject::connect(m_PageManager, &PagesManager::UpdateChannelStatus, m_Mag200Controller, &Mag200Controller::SendStatusFromIndex);
  QObject::connect(m_PageManager, &PagesManager::UpdateToolbarIndex, m_ToolbarManager, &ToolbarManager::NewToolbarIndex);

  QObject::connect(m_PageManager, &PagesManager::StartReadLog, m_Mag200Controller, &Mag200Controller::StartReadLog);
  QObject::connect(m_PageManager, &PagesManager::StopReadLog, m_Mag200Controller, &Mag200Controller::StopReadLog);

  QObject::connect(m_PageManager, &PagesManager::SendStartReadChannelData, m_Mag200Controller, &Mag200Controller::StartReadChannelData);
  QObject::connect(m_PageManager, &PagesManager::SendStopReadChannelData, m_Mag200Controller, &Mag200Controller::StopReadChannelData);

  QObject::connect(m_PageManager, &PagesManager::SendCancelCopy, m_Mag200Controller, &Mag200Controller::NewCancelCopy);
  QObject::connect(m_PageManager, &PagesManager::SendCopyFile, m_Mag200Controller, &Mag200Controller::NewCopyFile);

  QObject::connect(m_PageManager, &PagesManager::SendDrivesOpen, m_ToolbarManager, &ToolbarManager::NewDrivesOpen);
  QObject::connect(m_PageManager, &PagesManager::SendFolderOpen, m_ToolbarManager, &ToolbarManager::NewFolderOpen);

  QObject::connect(m_PageManager, &PagesManager::SendUseFullPage, this, &CMag200ShellGui::ResizePageManager);

  /*******************/
  /** EXTERN OBJECT **/
  /*******************/
  if (this->HasUniqueConnection == true)
    return;

  // SETTINGS MANAGER
  QObject::connect(qSettings, &SettingsManager::SendNewFileSettings, this, &CMag200ShellGui::Reload);

  // MAG 200 CONTROLLER
  QObject::connect(m_Mag200Controller, &Mag200Controller::SendNewTestArea, qSettings, &SettingsManager::NewWorkArea);

  this->HasUniqueConnection = true;
}

void CMag200ShellGui::ResizePageManager(bool UseFullPage)
{
  m_ToolbarManager->setVisible(!UseFullPage);
  m_StatusBar->setVisible(!UseFullPage);

  QResizeEvent *event = new QResizeEvent(this->size(), this->size());
  this->resizeEvent(event);
  delete event;
}