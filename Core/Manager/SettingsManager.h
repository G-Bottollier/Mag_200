#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QtWidgets>

#include "ThemesManager.h"
#include "ScreensManager.h"
#include "MessagesManager.h"
#include "StoragesManager.h"

#include "Mag200Data.h"

#if defined(CMAG200SHELLGUI_LIB)
#  define CMag200ShellGui_EXPORT Q_DECL_EXPORT
#else
#  define CMag200ShellGui_EXPORT Q_DECL_IMPORT
#endif

// Convenience macro to access the SettingsManager instance
#define qSettings (static_cast<SettingsManager*>(SettingsManager::Instance()))

#define INITIALIZE_OK                     true

#define INITIALIZE_KEY_ERROR              1001
#define INITIALIZE_PATH_ERROR             1002
#define INITIALIZE_THEME_ERROR            1003
#define INITIALIZE_MAGALI_ERROR           1004
#define INITIALIZE_MAG200_ERROR           1005
#define INITIALIZE_LAYOUT_ERROR           1006
#define INITIALIZE_SCREENS_ERROR          1007
#define INITIALIZE_WATCHER_ERROR          1008
#define INITIALIZE_SETTINGS_ERROR         1009
#define INITIALIZE_LANGUAGES_ERROR        1010
#define INITIALIZE_TRANSLATOR_ERROR       1011
#define INITIALIZE_CONFIGURATIONS_ERROR   1012

#define DPS "DPS"
#define TGS "TGS"
#define RnR "R&R"

#define MAG200_DPS 0
#define MAG200_TGS 1
#define MAG200_RnR 2

/*
 * @brief The SettingsManager class represents the settings manager of the application.
 *
 * This class is responsible for managing various settings related to the MAG200 application.
 * It handles initialization, termination, and retrieval of different settings such as file paths,
 * organization information, application details, and configuration files.
 *
 * @date 31/05/2023
 */
class CMag200ShellGui_EXPORT SettingsManager : public QObject
{
  Q_OBJECT

  private:
    // Magali
    QString m_WorkArea_Filename;
    QString m_WorkArea_Path;
    QString m_Root_Path;
    QString m_WorkArea;
    QString m_LCA_Path;
    QString m_GCA_Path;

    // Organization
    QString m_Organization_Name;
    QString m_Organization_Domain;

    // Application
    QString m_App_Name;
    QString m_App_Path;
    QString m_App_Icon;
    QString m_App_Drive;
    QString m_App_Filename;
    QStorageInfo m_App_Storage;

    // Mag200
    QString m_Mag200_LCA;
    QString m_Mag200_GCA;
    QString m_Mag200_Type;
    QMap<int, Mag200Data*> m_Mag200Datas;
    QList<QVector3D> m_LayoutPositions;

    // Settings
    QString m_Config_Filename;
    QString m_Layout_Filename;
    QString m_Settings_Filename;
    QString m_Channels_Filename;

    // Settings INI file
    QString m_Settings_Theme;
    QString m_Settings_Language;
    
    QStringList m_Languages;
    QStringList m_Mag200_Types;
    QStringList m_Settings_Screens;
    float       m_Settings_Divider;

    QSettings *settings = nullptr;
    QTranslator *Translator = nullptr;

    // Config File Watcher
    int m_LastCheckSum = -1;
    QFileSystemWatcher *FileWatcher = nullptr;

    int InitializeKey();
    int InitializePath();
    int InitializeTheme();
    int InitializeMagali();
    int InitializeMag200();
    int InitializeLayout();
    int InitializeScreens();
    int InitializeWatcher();
    int InitializeSettings();
    int InitializeLanguages();
    int InitializeTranslator();
    int InitializeConfigurations();

  private:
    // Constructor
    explicit SettingsManager(QObject *parent = nullptr);

  public:
    // Destructor
    ~SettingsManager();

    static SettingsManager * Instance();

    int Initialize();
    void Terminate();

    bool ReloadSettings();
    void SetSettings(QString theme, QString language, QString type);

    // Magali
    QString WorkArea_Filename() const;
    QString WorkArea_Path() const;
    QString Root_Path() const;
    QString WorkArea() const;
    QString LCA_Path() const;
    QString GCA_Path() const;

    // Organization
    QString Organization_Name() const;
    QString Organization_Domain() const;

    // Application
    QString App_Name() const;
    QString App_Path() const;
    QString App_Icon() const;
    QString App_Drive() const;
    QString App_Filename() const;
    QStringList Languages() const;
    QStorageInfo App_Storage() const;

    // Mag200
    QString Mag200_GCA() const;
    QString Mag200_LCA() const;
    QString Mag200_Type() const;
    QStringList Mag200_Types() const;
    QMap<int, Mag200Data*> Mag200Datas() const;
    QList<QVector3D> LayoutPositions() const;

    // Settings
    QStringList Settings_Screens() const;
    QString Channels_Filename() const;
    QString Settings_Filename() const;
    QString Config_Filename() const;
    QString Layout_Filename() const;
    
    // Theme
    QString Settings_Theme() const;
    QString Settings_Language() const;

  public slots:
    void NewWorkArea(QString name);
    bool NewFileSettings(QString path);

  signals:
    void SendNewFileSettings();
};

#endif // SETTINGSMANAGER_H
