/* (EVOL_MAG200_CFG) JMB 05/06/23 : Fichier de configuration global dans lca mc_mag200.cfg */

#include "stdafx.h"
#include "SettingsManager.h"

// Disable the "unreferenced" warning for certain headers
#pragma warning ( disable : 4100 ) // Unreferenced warning
#include <decl10.h>
#include <decl40.h>
#include <base.h>
#include <w_magini.h>
#include <n_owl.h>
#include <mwstrm.h>
#include <commlib.h>
#include <magpath.h>
#include <w_key.h>
#include <mwshv.h>
#include <lang.h>
#include <mag200_str.h>
#pragma warning ( default : 4100 )


#define APP_NAME            "mag200"

#define CONFIG_FOLDER       "config"
#define SETTING_FILE        "mag200.ini"
#define LAYOUT_FILE         "layout.json"
#define LANGUAGE_PATH       ":/Language/"

/*
 * @brief Constructor for the SettingsManager class.
 *
 * @param parent The parent QObject.
 *
 * @date 31/05/2023
 */
SettingsManager::SettingsManager(QObject *parent) : QObject(parent)
{
}

/*
 * @brief Destructor for the SettingsManager class.
 *
 * @date 31/05/2023
 */
SettingsManager::~SettingsManager()
{
}

/*
 * @brief Returns the instance of SettingsManager.
 *
 * This function returns a pointer to the instance of SettingsManager.
 * If the instance does not exist yet, it creates a static instance of SettingsManager.
 * Subsequent calls to this function will always return the same instance.
 *
 * @return A pointer to the instance of SettingsManager.
 *
 * @date 31/05/2023
 */
SettingsManager * SettingsManager::Instance()
{
  // Singleton pattern: return the same instance of SettingsManager
  static SettingsManager *Instance = new SettingsManager();
  return Instance;
}

/*
* @brief Initialize the SettingsManager.
*
* This function initializes the SettingsManager by performing several initialization steps:
*     1: Initialize Path..............Set up paths
*     2: Initialize settings..........Set up .ini Settings
*     3: Initialize Languages.........Find the language pack
*     4: Initialize translator........Install the app language pack
*     5: Initialize theme.............Install the app theme
*     6: Initialize screens...........Move the app to the white listed screen
*     7: Initialize file watcher......Install file watcher to the CFG file path
*     8: Initialize key...............Check if the dongle is connected
*     9: Initialize Magali............Perform Magali init function
*    10: Initialize Mag200............Perform Mag200 init
*    11: Initialize Layout............Read layout file
*    12: Initialize Configuration.....Read CFG file
*
* @date 31/05/2023
* @return Returns INITIALIZE_OK (0) if all steps are successful, otherwise returns an m_State code.
*/
int SettingsManager::Initialize()
{
  int step = 0;
  int success = 0;

  // Initialize magali
  MagInit_StructInit(TRUE);

  /********************************/
  /*  GUI Undisplayable from here */
  /********************************/
  // Only qMessages->Log from here

  // Step 1: Initialize file paths
  success += InitializePath();
  step++;

  // Step 2: Initialize application settings
  success += InitializeSettings();
  step++;

  /*********************************/
  /*  GUI Constructible from here  */
  /*********************************/
  // Construct qMessages GUI
  qMessages->Initialize();

  // Step 3: Initialize application Languages
  success += InitializeLanguages();
  step++;

  // Step 4: Initialize translator for language support
  success += InitializeTranslator();
  step++;

  // Step 5: Initialize application theme
  success += InitializeTheme();
  step++;

  // Step 6: Initialize application Screen
  success += InitializeScreens();
  step++;

  /*******************************/
  /*  GUI Displayable from here  */
  /*******************************/
  // Display qMessages
  qMessages->Open();
  qMessages->SetScrollRange(0, 100, true);
  qMessages->SetTitle(APP_NAME " Initialize");

  // Step 7: Initialize file watcher for config file changes
  success += InitializeWatcher(); /* (DEPRECATED) */
  step++;

  // Step 8: Initialize license key
  success += InitializeKey();
  step++;

  // Step 9: Initialize Magali library
  success += InitializeMagali();
  step++;

  // Step 10: Initialize Mag200 application
  success += InitializeMag200();
  step++;

  // Step 11: Initialize Mag200 layout from file
  success += InitializeLayout();
  step++;

  // Step 12: Initialize Mag200 data from configurations file
  success += InitializeConfigurations(); /* (DEPRECATED) */
  step++;

  // Check if all steps were successful
  if (success == step)
    return INITIALIZE_OK;
  return success;
}

/*
 * @brief Initialize the file paths.
 *
 * This function initializes the file paths by setting various paths
 * using the Magali functions and storing them in member variables.
 * Returns true if the file paths are successfully initialized, false otherwise.
 *
 * @date 31/05/2023
 */
int SettingsManager::InitializePath()
{
  // Log String handle
  QString Log = LOG_METHOD(SettingsManager, InitializePath);

  // Get Working Area Path
  QString waFilename;
  this->m_WorkArea = Get_wa_name();
  if (this->m_WorkArea.isEmpty() == false)
    waFilename = QDir::cleanPath(QDir(Get_wa_path()).filePath(this->m_WorkArea));
  else
    waFilename = QString();

  // Set file paths using magali functions
  this->m_WorkArea_Filename = waFilename;
  this->m_WorkArea_Path = Get_wa_path();
  this->m_Root_Path = GetGlobalPath();
  this->m_GCA_Path = Get_path_gca();
  this->m_LCA_Path = Get_path_lca();

  // Set additional file paths for settings
  this->m_Mag200_LCA = QDir::cleanPath(QDir(this->m_LCA_Path).filePath(CONFIG_FOLDER "/" APP_NAME));
  this->m_Mag200_GCA = QDir::cleanPath(QDir(this->m_GCA_Path).filePath(CONFIG_FOLDER "/" APP_NAME));

  // Set additional filename for settings about CFG
  this->m_Config_Filename = QDir::cleanPath(QDir(this->m_WorkArea_Filename).filePath(MAG200_HW_CONFIG_FILE_NAME));

  // Set additional filename for settings about LCA
  this->m_Layout_Filename = QDir::cleanPath(QDir(this->m_Mag200_LCA).filePath(LAYOUT_FILE));
  this->m_Settings_Filename = QDir::cleanPath(QDir(this->m_Mag200_LCA).filePath(MAG200_SETTINGS_FILE_NAME));

  // Set additional filename for settings about GCA
  this->m_Channels_Filename = QDir::cleanPath(QDir(this->m_Mag200_GCA).filePath(MAG200_CHANNELS_PGS));
  
  // Log End of the function
  qMessages->Log(Log, "DONE");
  return INITIALIZE_OK;
}

/*
 * @brief Initialize the application settings.
 *
 * This function initializes the application settings by reading values from the mag200.ini file
 * If the mag200.ini file does not exist, it creates the file with default values. 
 * Returns true if the settings are successfully initialized, false otherwise.
 *
 * @date 31/05/2023
 */
int SettingsManager::InitializeSettings()
{
  // Log String handle
  const QString Log = LOG_METHOD(SettingsManager, InitializeSettings);

  // Set the path for QSettings to use the mag200.ini file
  QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, this->m_Settings_Filename);
  QSettings *settings = new QSettings(this->m_Settings_Filename, QSettings::IniFormat);

  if (settings->status() != QSettings::NoError)
  {
    const QString accessError = "File .ini Access error ! - ";
    const QString formatError = "File .ini Format error ! - ";
    QString m_State = settings->status() == QSettings::AccessError ? accessError : formatError;
    qMessages->Log(Log, m_State + this->m_Settings_Filename);
  }

  // Check if the settings file exists
  if (QFile::exists(this->m_Settings_Filename) == false)
  {
    qMessages->Log(Log, "Settings File not found writing new .ini file - " + this->m_Settings_Filename);

    // If the file doesn't exist, create it with default values
    settings->setValue(SETTINGS_TYPE,           DEFAULT_TYPE);
    settings->setValue(SETTINGS_THEME,          DEFAULT_THEME);
    for (int scr = 0 ; scr < SETTINGS_SCREEN_COUNT ; scr++)
      settings->setValue(QString("%1%2").arg(SETTINGS_SCREEN_N).arg(scr), DEFAULT_SCREEN);
    settings->setValue(SETTINGS_LANGUAGE,       DEFAULT_LANGUAGE);
    settings->setValue(SETTINGS_SCREEN_DIVIDER, DEFAULT_SCREEN_DIVIDER);

    settings->sync();
  }

  // Read the Type of MAG 200
  this->m_Mag200_Type = settings->value(SETTINGS_TYPE, DEFAULT_TYPE).toString();

  // Read the Theme settings values 
  this->m_Settings_Theme = settings->value(SETTINGS_THEME, DEFAULT_THEME).toString();

  // Read the Screens settings values 
  this->m_Settings_Screens.clear();
  for (int scr = 0 ; scr < SETTINGS_SCREEN_COUNT ; scr++) {
    QString val(settings->value(QString("%1%2").arg(SETTINGS_SCREEN_N).arg(scr), DEFAULT_SCREEN).toString());
    if (!val.isEmpty() && val != DEFAULT_SCREEN)
      this->m_Settings_Screens.append(val);
  }
  this->m_Settings_Divider = settings->value(SETTINGS_SCREEN_DIVIDER, DEFAULT_SCREEN_DIVIDER).toFloat();

  // Read the Language settings values 
  this->m_Settings_Language = settings->value(SETTINGS_LANGUAGE, DEFAULT_LANGUAGE).toString();

  // Log End of the function
  qMessages->Log(Log, "DONE");
  return INITIALIZE_OK;
}

/*
 * @brief Initialize the application theme.
 *
 * This function sets the application theme based on the value
 * Returns true if the theme is successfully set, false otherwise.
 *
 * @date 31/05/2023
 */
int SettingsManager::InitializeTheme()
{
  // Log String handle
  const QString Log = LOG_METHOD(SettingsManager, InitializeTheme);

  // Set the theme based on the settings value
  qThemes->SetTheme(this->m_Settings_Theme);

  qMessages->Log(Log, "DONE");
  return INITIALIZE_OK;
}

/*
 * @brief Initialize the file watcher for config file changes.
 *
 * This function creates a QFileSystemWatcher to monitor changes to the config file.
 * Returns true if the file watcher is successfully initialized, false otherwise.
 *
 * @date 31/05/2023
 */
int SettingsManager::InitializeWatcher()
{
  // Log String handle
  const QString Log = LOG_METHOD(SettingsManager, InitializeWatcher);

  bool success = true;

  // Create a new QFileSystemWatcher to monitor changes to the config file
  if (FileWatcher != nullptr)
    FileWatcher->deleteLater();
  FileWatcher = new QFileSystemWatcher();

  // If WorkAreaFilename is empty then disconnected from Working area
  if (this->m_WorkArea_Filename.isEmpty() == true)
  {
    qMessages->Log(Log, "Watcher can't connect to an empty Work Area");
    return INITIALIZE_OK;
  }

  // create (empty) mc_config.cfg if does not exist 
  QFile file(this->m_Config_Filename);
  if (file.exists(this->m_Config_Filename) == false)
    if (file.open(QIODevice::WriteOnly))
      file.close();

  // Add the config file path to the watcher
  success = FileWatcher->addPath(this->m_Config_Filename);

  // Check if adding the file to the watcher was successful
  if (success == false)
  {
    qMessages->Log(Log, "Failed to add the CFG file to the watcher - " + this->m_Config_Filename);
    return INITIALIZE_WATCHER_ERROR;
  }

  // Call before the connect to update the checksum 
  NewFileSettings(this->m_Config_Filename);

  // Call NewFileSettings() when the config file changes
  QObject::connect(FileWatcher, &QFileSystemWatcher::fileChanged, this, &SettingsManager::NewFileSettings);
  
  if (success == false)
    return false;

  // Log End of the function
  qMessages->Log(Log, "DONE");
  return INITIALIZE_OK;
}

/*
 * @brief Initialize the license key.
 *

 * Initializes the license key by checking if the current process code matches the expected code
 * If the process code doesn't match or the key is not plugged in (except for the demo version)
 *
 * @date 31/05/2023
 */
int SettingsManager::InitializeKey()
{
  // Log String handle
  const QString Log = LOG_METHOD(SettingsManager, InitializeKey);

  // Set the description for the message
  qMessages->SetDescription(tr("CHECKING_DONGLE"), true);

  // Check if the current process code matches the expected code for the MAG200 process
  if (GetCurProcessCode() != PROCESS_MAG200)
  {
    // Free the task and return false if the process code doesn't match
    KEY_FreeTask(PROCESS_MAG200);

    qMessages->Log(Log, "Key code doesn't match !");
    return INITIALIZE_WATCHER_ERROR;
  }
  qMessages->SetScrollPosition(MessagesManager::Auto);

  // Manually initialize the key to avoid issues with multiple CPUs
  KEY_InitTask(GetCurProcessCode());

  if (!KEY_IsKeyPlugged() && !VERSION_IsDemo(0)) {
    // Display a message if the key is not plugged in and it's not a demo version
    KEY_DisplayNoKey(CurLanguage);
    KEY_FreeTask(PROCESS_MAG200);

    qMessages->Log(Log, "No Key found and it's not a demo version");
    return INITIALIZE_WATCHER_ERROR;
  }
  qMessages->SetScrollPosition(MessagesManager::Auto);

  // Log End of the function
  qMessages->Log(Log, "DONE");
  return INITIALIZE_OK;
}

/*
 * @brief Initialize the Magali library.
 *

 * This function initializes the Magali library by performing several initialization steps.
 * including initializing the network and the global context. 
 * Returns true if the Magali library is successfully initialized, false otherwise.
 *
 * @date 31/05/2023
 */
int SettingsManager::InitializeMagali()
{
  // Log String handle
  const QString Log = LOG_METHOD(SettingsManager, InitializeMagali);

  int m_State = 0;

  // Set the description for the message
  qMessages->SetDescription(tr("INIT_PATH"), true);

  // Initialize network
  qMessages->SetDescription(tr("INIT_NETWORK"), true);
  m_State = ShmComm_Init();
  m_State = CfgComm_Load(NULL);
  qMessages->SetScrollPosition(MessagesManager::Auto);

  // Initialize global context
  qMessages->SetDescription(tr("INIT_CONTEXT"), true);
  m_State = ShV_Open();
  ShV_RefreshLib();
  qMessages->SetScrollPosition(MessagesManager::Auto);

  // Log End of the function
  qMessages->Log(Log, "DONE");
  return INITIALIZE_OK;
}

/*
 * @brief Initialize the Mag200 application.
 *

 * This function initializes the Mag200 application including:
 *  - the organization setting
 *  - the application details
 *  - the application icon
 *  - the application storage and drive
 *  - the application thread priority
 * Returns true if the Mag200 application is successfully initialized, false otherwise.
 *
 * @date 31/05/2023
 */
int SettingsManager::InitializeMag200()
{
  // Log String handle
  const QString Log = LOG_METHOD(SettingsManager, InitializeMag200);

  // Set the description for the message
  qMessages->SetDescription(tr("INIT_APP"), true);

  // Set the organization details
  this->m_Organization_Name = "Hensoldt";
  this->m_Organization_Domain = "hensoldt.com";

  // Set the application details
  this->m_App_Name = APP_NAME;
  this->m_App_Path = qApp->applicationDirPath();
  this->m_App_Icon = ":/Icons/MAG_200_DPS.png";
  this->m_App_Filename = qApp->applicationFilePath();

  // Set the application storage and drive
  this->m_App_Storage = StoragesManager::StorageFromPath(this->m_App_Path);
  this->m_App_Drive = this->m_App_Storage.rootPath();

  this->m_Mag200_Types = QStringList() << DPS << TGS << RnR;

  // Set the application icon and organization/domain details
  qApp->setWindowIcon(QIcon(m_App_Icon));
  QCoreApplication::setApplicationName(this->m_App_Name);
  QCoreApplication::setOrganizationName(this->m_Organization_Name);
  QCoreApplication::setOrganizationDomain(this->m_Organization_Domain);
  qMessages->SetScrollPosition(MessagesManager::Auto);

  // Set the description for the message
  qMessages->SetDescription(tr("INIT_THREAD"), true);
  QThread::currentThread()->setPriority(QThread::HighPriority);
  qMessages->SetScrollPosition(MessagesManager::Auto);

  // Log End of the function
  qMessages->Log(Log, "DONE");
  return INITIALIZE_OK;
}

int SettingsManager::InitializeLayout()
{
  // Log String handle
  const QString Log = LOG_METHOD(SettingsManager, InitializeLayout);

  // Open json file
  QFile fJson(this->m_Layout_Filename);
  if (!fJson.open(QIODevice::ReadOnly))
  {
    qMessages->Log(Log, "Failed to open layout file - " + this->m_Layout_Filename);
    return INITIALIZE_LAYOUT_ERROR;
  }

  // Read file data
  QByteArray data = fJson.readAll();
  if (data.isNull())
  {
    qMessages->Log(Log, "Failed to read layout file");
    return INITIALIZE_LAYOUT_ERROR;
  }

  // Read the JSON document from the layout file
  QJsonDocument doc = QJsonDocument::fromJson(data);
  if (doc.isNull())
  {
    qMessages->Log(Log, "Error JSON invalid format");
    return INITIALIZE_LAYOUT_ERROR;
  }

  // Retrieve the layout data from the JSON document
  QJsonValue objLayout = doc.object().value("layout");
  if (objLayout.isNull())
  {
    qMessages->Log(Log, "Object 'layout' doesn't exist");
    return INITIALIZE_LAYOUT_ERROR;
  }

  // Convert the layout data to a JSON array
  QJsonArray layout = objLayout.toArray();

  // Clear the InputsPosition vector
  this->m_LayoutPositions.clear();

  // Initialize the overlap variable
  QPoint overlap(0, 0);

  // Iterate over the layout array
  for (int i = 0; i < layout.size(); i++)
  {
    // Get the current object from the layout array
    QJsonObject obj = layout[i].toObject();

    // Use QVector3D for :
    // X = x coordinate
    // Y = y coordinate
    // Z = 0 / 1 bool is used
    QVector3D position(0, 0, 0);

    // Define the keys for the x and y values
    QString strX = "x";
    QString strY = "y";

    // Get the x and y values from the object, assign default values of 0 if null
    int x = obj.value(strX).isNull() ? 0 : obj.value(strX).toInt();
    int y = obj.value(strY).isNull() ? i : obj.value(strY).toInt();

    // Check for overlap and update the overlap variable if necessary
    x < overlap.x() ? overlap.x() : x;
    y < overlap.y() ? overlap.y() : y;

    // Set the values of the position vector
    position.setX(x);
    position.setY(y);
    position.setZ(true); // bool not used

    // Check if the InputsPosition vector already contains the position
    if (this->m_LayoutPositions.contains(position) == true)
      continue;

    // Add the position to the InputsPosition vector
    this->m_LayoutPositions.append(position);

    // Update the overlap variable
    if (y > overlap.y())
    {
      overlap.setX(0);
      overlap.setY(y);
    }
  }

  // Log End of the function
  qMessages->Log(Log, "DONE");
  return INITIALIZE_OK;
}

/*
* @brief Initialize the list of supported languages of the application.
*
* This function retrieve all languages pack from the Translations qrc file
*
* @date 31/05/2023
*/
int SettingsManager::InitializeScreens()
{
  // Log String handle
  const QString Log = LOG_METHOD(SettingsManager, InitializeScreens);

  qScreens->SetWhiteList(m_Settings_Screens);
  qScreens->SetDivider(m_Settings_Divider);
  qScreens->MoveApplication();

  // Log End of the function
  qMessages->Log(Log, "DONE");
  return INITIALIZE_OK;
}

/*
* @brief Initialize the list of supported languages of the application.
*
* This function retrieve all languages pack from the Translations qrc file
*
* @date 31/05/2023
*/
int SettingsManager::InitializeLanguages()
{
  // Log String handle
  const QString Log = LOG_METHOD(SettingsManager, InitializeLanguages);

  // Clear the languages pack list
  this->m_Languages.clear();

  // Get an iterator from the translation qrc file
  QDirIterator dirIterator(LANGUAGE_PATH, QDirIterator::Subdirectories);

  // Fill the languages pack list
  while (dirIterator.hasNext())
  {
    QFileInfo fileInfo = dirIterator.next();
    QString language = fileInfo.suffix();

    // If the language pack file exist
    if (fileInfo.exists() == false)
      continue;

    // If the language pack isn't already in the list
    if (this->m_Languages.contains(language) == true)
      continue;

    // Add the language pack to the list
    this->m_Languages.insert(0, language);
  }

  // If language pack is empty
  if (this->m_Languages.size() == 0)
  {
    qMessages->Log(Log, "No languages pack installed");
    return INITIALIZE_LANGUAGES_ERROR;
  }

  // Log End of the function
  qMessages->Log(Log, "DONE");

  // If the selected language from INI file exist 
  if (this->m_Languages.contains(this->m_Settings_Language))
    return INITIALIZE_OK;

  // Set settings language to the default language of the list
  this->m_Settings_Language = this->m_Languages.first();
  return INITIALIZE_OK;
}

/*
 * @brief Initialize the translator for language support.
 *
 * This function initializes the QTranslator instance loading and installing the translator
 * Returns true if the translator is successfully initialized, false otherwise.
 *
 * @date 31/05/2023
 */
int SettingsManager::InitializeTranslator()
{
  // Log String handle
  const QString Log = LOG_METHOD(SettingsManager, InitializeTranslator);

  // Create a new QTranslator instance
  this->Translator = new QTranslator();

  // Get the file info for the language translation file

  QFileInfo fileInfo(LANGUAGE_PATH "TEXT." + this->m_Settings_Language);

  // Load the translator with the language translation file
  bool isLoaded = this->Translator->load(fileInfo.absoluteFilePath());
  if (isLoaded == false)
  {
    qMessages->Log(Log, "Failed to load Translator - " + fileInfo.absoluteFilePath());
    return INITIALIZE_TRANSLATOR_ERROR;
  }

  // Install the translator to be used for application translations
  bool isInstalled = QCoreApplication::installTranslator(this->Translator);
  if (isInstalled == false)
  {
    qMessages->Log(Log, "Failed to install the Translator - " + fileInfo.suffix());
    return INITIALIZE_TRANSLATOR_ERROR;
  }
  
  // Log End of the function
  qMessages->Log(Log, "DONE");
  return INITIALIZE_OK;
}

/* Lecture de la configuration Magali (EVOL_MAG200_CFG) */
int SettingsManager::InitializeConfigurations()
{
  // Log String handle
  const QString Log = LOG_METHOD(SettingsManager, InitializeConfigurations);

  // Clear Boards
  qDeleteAll(this->m_Mag200Datas);
  this->m_Mag200Datas.clear();

  // Reading PGS
  QByteArray channelsPGS = qSettings->Channels_Filename().toLatin1();
  Liste lPages = ChargeListePagesFromFn(channelsPGS.data());
  if (lPages == nullptr)
  {
    qMessages->Log(Log, "Failed to read PGS - " + qSettings->Channels_Filename());
    return INITIALIZE_CONFIGURATIONS_ERROR;
  }

  // Opening CFG
  QByteArray configFile = qSettings->Config_Filename().toLatin1();
  FILE *file = fopen(configFile.constData(), "r");
  if (file == nullptr)
  {
    qMessages->Log(Log, "Failed to open CFG - " + qSettings->Config_Filename());
    return INITIALIZE_CONFIGURATIONS_ERROR;
  }

  // Move file pointer to the end
  fseek(file, 0, SEEK_END);
  
  // Check if file is empty
  if (ftell(file) == 0)
  {
    fclose(file);
    qMessages->Log(Log, "CFG File is empty - " + qSettings->Config_Filename());
    qMessages->Log(Log, "DONE");
    return INITIALIZE_OK;
  }
  
  // Move file pointer back to start
  rewind(file);

  // Reading CFG
  Liste lChannels = ChargeListeElements(sizeof(MAG200_Canal), lPages, file);
  fclose(file);

  // If channels is null
  if (lChannels == nullptr)
  {
    qMessages->Log(Log, "Failed to read CFG - " + qSettings->Config_Filename());
    return INITIALIZE_CONFIGURATIONS_ERROR;
  }

  // Temp List
  Liste l = lChannels;
  while (l != nullptr) 
  {
    // Get the mag200 canal that contains datas
    MAG200_Canal *canal = (MAG200_Canal*)l->Element;
    l = l->Suite;

    // If canal is not enable
    if (canal->enabled == false)
      continue;

    // Wrap mag200 canal data into board
    Mag200Data *board = Mag200Data::From_Mag_200_Cfg(canal);

    int ChannelID = board->ChannelID();

    // add board to the list
    this->m_Mag200Datas[ChannelID] = board;
  }

  // Free channels & pages
  if (lChannels)
    Liste_UFree(lChannels);
  if (lPages)
    FreeListePages(lPages);
  
  // Log End of the function
  qMessages->Log(Log, "DONE");
  return INITIALIZE_OK;
}

/*
 * @brief Terminate the SettingsManager.
 *

 * This function performs cleanup tasks and frees resources including :
 *  - closing the task for DRV
 *  - uninitializing COM
 *  - freeing shared memory communication
 *  - freeing shared variable communication
 *  - freeing the MagInit structure
 *  - freeing the key task
 *
 * @date 31/05/2023
 */
void SettingsManager::Terminate()
{
  // Close the task for DRV
  DRV_CloseTask();

  // Uninitialize COM
  CoUninitialize();

  // Free shared memory communication
  ShmComm_Free();

  // Free shared variable communication
  ShV_Free();

  // Free the MagInit structure
  MagInit_StructFree();

  // Free the key task
  KEY_FreeTask(PROCESS_MAG200);
}

bool SettingsManager::ReloadSettings()
{
  int step = 0;
  int success = 0;

  // Step 1: Initialize file paths
  success += InitializePath();
  step++;

  // Step 1: Re-Initialize application settings
  success += InitializeSettings();
  step++;

  // Step 2: Re-Initialize application settings
  success += InitializeLanguages();
  step++;

  // Step 3: Re-Initialize translator for language support
  success += InitializeTranslator();
  step++;

  // Step 4: Re-Initialize application theme
  success += InitializeTheme();
  step++;

  // Check if all steps were successful
  if (success != step)
    return false;

  // Emit a signal to indicate that the settings file has changed
  emit this->SendNewFileSettings();

  return true;
}

void SettingsManager::SetSettings(QString theme, QString language, QString type)
{
  // Set the path for QSettings to use the mag200.ini file
  QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, this->m_Settings_Filename);
  QSettings *settings = new QSettings(this->m_Settings_Filename, QSettings::IniFormat);

  // Check if the settings file exists
  if (QFile::exists(this->m_Settings_Filename) == true)
  {
    // If the file doesn't exist, create it with default values
    settings->setValue(SETTINGS_TYPE, type);
    settings->setValue(SETTINGS_THEME, theme);
    settings->setValue(SETTINGS_LANGUAGE, language);
    settings->sync();
  }
}

void SettingsManager::NewWorkArea(QString)
{
  // Log String handle
  const QString Log = LOG_METHOD(SettingsManager, NewWorkArea);

  QFileInfo currentWorkArea(this->m_WorkArea_Filename);

  QString newWorkArea = Get_wa_name();
  QString oldWorkArea = currentWorkArea.fileName();

  if (newWorkArea == oldWorkArea)
    return;

  qMessages->Log(Log, "New Work Area Found - " + oldWorkArea + " -> " + newWorkArea);

  // Reload Path
  InitializePath();

  // Reload Configurations file
  InitializeConfigurations();

  // Reload Watcher
  InitializeWatcher();

  // Log End of the function
  qMessages->Log(Log, "DONE");
}

/*
 * @brief Slot function for handling new file settings.
 *

 * This function is called when the config file changes. 
 * It compares the checksum of the file with the last known checksum. 
 * If the checksum is different, it emits a signal to indicate
 * that the settings file has changed.
 *
 * @param path The path of the file that changed.
 *
 * @date 31/05/2023
 */
bool SettingsManager::NewFileSettings(QString path)
{
  // Log String handle
  const QString Log = LOG_METHOD(SettingsManager, NewFileSettings);

  // Check if the provided path is the same as the config file path
  if (path != this->m_Config_Filename)
  {
    qMessages->Log(Log, "Invalid CFG Path provided - " + path);
    return false;
  }

  // Initialize variables
  int currentChecksum = -1;
  QFile file(this->m_Config_Filename);

  // Check if the file can be opened
  if (file.open(QFile::ReadOnly) == false)
  {
    qMessages->Log(Log, "Can't open CFG file - " + path);
    return false;
  }

  // Read the file data and calculate the checksum
  QByteArray data = file.readAll();
  currentChecksum = qChecksum(data.data(), data.length());
  file.close();

  // Check if the calculated checksum is valid
  if (currentChecksum == -1)
  {
    qMessages->Log(Log, "Failed to calculate the checksum - " + path);
    return false;
  }

  // If it's the first time, update the last checksum
  if (this->m_LastCheckSum == -1)
    this->m_LastCheckSum = currentChecksum;

  // If the current checksum is the same as the last checksum, the file hasn't changed
  if (currentChecksum == this->m_LastCheckSum)
    return true;

  qMessages->Log(Log, "New CFG file Found - " + path);

  // Update the last checksum
  this->m_LastCheckSum = currentChecksum;

  // Reload Configurations file
  InitializeConfigurations();

  // Emit a signal to indicate that the settings file has changed
  emit this->SendNewFileSettings();

  // Log End of the function
  qMessages->Log(Log, "DONE");
  return INITIALIZE_OK;
}

// Getter methods for various paths and settings

QString SettingsManager::WorkArea_Filename()                const { return this->m_WorkArea_Filename;     }
QString SettingsManager::WorkArea_Path()                    const { return this->m_WorkArea_Path;         }
QString SettingsManager::Root_Path()                        const { return this->m_Root_Path;             }
QString SettingsManager::WorkArea()                         const { return this->m_WorkArea;              }
QString SettingsManager::LCA_Path()                         const { return this->m_LCA_Path;              }
QString SettingsManager::GCA_Path()                         const { return this->m_GCA_Path;              }

QString SettingsManager::Organization_Name()                const { return this->m_Organization_Name;     }
QString SettingsManager::Organization_Domain()              const { return this->m_Organization_Domain;   }

QString SettingsManager::App_Name()                         const { return this->m_App_Name;              }
QString SettingsManager::App_Path()                         const { return this->m_App_Path;              }
QString SettingsManager::App_Icon()                         const { return this->m_App_Icon;              }
QString SettingsManager::App_Drive()                        const { return this->m_App_Drive;             }
QString SettingsManager::App_Filename()                     const { return this->m_App_Filename;          }
QStringList SettingsManager::Languages()                    const { return this->m_Languages;             }
QStorageInfo SettingsManager::App_Storage()                 const { return this->m_App_Storage;           }

QString SettingsManager::Mag200_GCA()                       const { return this->m_Mag200_GCA;            }
QString SettingsManager::Mag200_LCA()                       const { return this->m_Mag200_LCA;            }
QString SettingsManager::Mag200_Type()                      const { return this->m_Mag200_Type;           }
QStringList SettingsManager::Mag200_Types()                 const { return this->m_Mag200_Types;          }
QMap<int, Mag200Data*> SettingsManager::Mag200Datas()       const { return this->m_Mag200Datas;           } /* (EVOL_MAG200_CFG) */
QList<QVector3D> SettingsManager::LayoutPositions()         const { return this->m_LayoutPositions;       }

QStringList SettingsManager::Settings_Screens()             const { return this->m_Settings_Screens;      }
QString SettingsManager::Channels_Filename()                const { return this->m_Channels_Filename;     }
QString SettingsManager::Settings_Filename()                const { return this->m_Settings_Filename;     }
QString SettingsManager::Config_Filename()                  const { return this->m_Config_Filename;       }
QString SettingsManager::Layout_Filename()                  const { return this->m_Layout_Filename;       }

QString SettingsManager::Settings_Theme()                   const { return this->m_Settings_Theme;        }
QString SettingsManager::Settings_Language()                const { return this->m_Settings_Language;     }