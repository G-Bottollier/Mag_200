#ifndef THEMESMANAGER_H
#define THEMESMANAGER_H

#include <QtWidgets>

// Convenience macro to access the SettingsManager instance
#define qThemes (static_cast<ThemesManager*>(ThemesManager::Instance()))

/*
 * @brief The ThemesManager class represents a manager for application themes.
 *
 * This class provides functionality to manage themes in the application. 
 * It allows setting the theme.
 * retrieving theme-specific colors and icons
 * The available themes are defined by the Theme enum.
 *
 * @date 31/05/2023
 */
class ThemesManager : public QObject
{
  Q_OBJECT

public :
  enum Theme
  {
    Dark,
    Light
  };
  Q_ENUM(Theme);

private:
  Theme theme = Theme::Dark;

  QPalette DarkTheme();
  QPalette LightTheme();

private:
  // Constructor
  explicit ThemesManager(QObject *parent = nullptr);

public:
  // Destructor
  ~ThemesManager();

  static ThemesManager * Instance();
  static QStringList Themes();

  bool IsDark();
  void SetTheme(QString strTheme);
  
  QString Icons(QString name);
  QColor Color(QPalette::ColorRole role, QPalette::ColorGroup group = QPalette::Active);

  // Status theme
  QPalette WarningTheme(QPalette p);
  QPalette ErrorTheme(QPalette p);
  QPalette InfoTheme(QPalette p);
};

#endif // THEMESMANAGER_H