#include "stdafx.h"
#include "ThemesManager.h"

/*
 * @brief Constructor for ThemesManager.
 *
 * @param parent The parent QObject of the ThemesManager object.
 *
 * @date 31/05/2023
 */
ThemesManager::ThemesManager(QObject *parent) : QObject(parent)
{

}

/*
 * @brief Destructor for ThemesManager.
 *
 * @date 31/05/2023
 */
ThemesManager::~ThemesManager()
{

}

/*
 * @brief Returns the instance of ThemesManager.
 *
 * This function returns a pointer to the instance of ThemesManager.
 * If the instance does not exist yet, it creates a static instance of ThemesManager.
 * Subsequent calls to this function will always return the same instance.
 *
 * @return A pointer to the instance of ThemesManager.
 *
 * @date 31/05/2023
 */
ThemesManager * ThemesManager::Instance()
{
  // Create a static instance of ThemesManager (singleton pattern)
  static ThemesManager *Instance = new ThemesManager();
  return Instance;
}

/*
 * @brief Returns a list of available themes.
 *
 * @return QStringList - The list of available themes.
 *
 * @date 31/05/2023
 */
QStringList ThemesManager::Themes()
{
  QStringList themes;
  QMetaEnum metaEnum = QMetaEnum::fromType<Theme>();

  // Iterate over each theme in the Theme enum and add it to the list
  for (int i = 0; i < metaEnum.keyCount(); i++)
  {
    QString theme = metaEnum.valueToKey(i);
    themes.append(theme);
  }

  return themes;
}

/*
 * @brief Checks if the current theme is dark.
 *
 * @return bool - True if the current theme is dark, false otherwise.
 *
 * @date 31/05/2023
 */
bool ThemesManager::IsDark()
{
  // Check if the current theme is set to Theme::Dark
  return theme == Theme::Dark;
}

/*
 * @brief Sets the theme for the application.
 *
 * @param strTheme - The name of the theme to set.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void ThemesManager::SetTheme(QString strTheme)
{
  // Convert the string theme name to the corresponding Theme enum value
  QMetaEnum metaEnum = QMetaEnum::fromType<Theme>();
  Theme theme = (Theme)metaEnum.keyToValue(strTheme.toLatin1());

  // Create a palette object and set the application style to "Fusion"
  QPalette palette;
  QApplication::setStyle(QStyleFactory::create("Fusion"));

  // Set the palette based on the selected theme
  switch (theme)
  {
  case Theme::Dark:
    palette = DarkTheme();  // Use the dark theme palette
    break;

  case Theme::Light:
    palette = LightTheme();  // Use the light theme palette
    break;

  default:
    palette = DarkTheme();  // Default to dark theme if an invalid theme is specified
    strTheme = metaEnum.valueToKey(Theme::Dark);
    break;
  }

  // Set the theme and style properties for the application
  qApp->setProperty("theme", strTheme);
  qApp->setProperty("style", "Fusion");

  // Set the application palette
  qApp->setPalette(palette);

  // Update the current theme
  this->theme = theme;
}

/**********************************/
/*           DARK THEME           */
/**********************************/
/*
 * @brief Returns the dark theme palette.
 *
 * @return QPalette - The dark theme palette.
 *
 * @date 31/05/2023
 */
QPalette ThemesManager::DarkTheme()
{
  QPalette p = QPalette();

  /**********************************/
  /* Light color for shadow effects */
  /**********************************/
  p.setColor(QPalette::Active,    QPalette::Light,            QColor(255, 255, 255, 255));
  p.setColor(QPalette::Disabled,  QPalette::Light,            QColor(255, 255, 255, 255));
  p.setColor(QPalette::Inactive,  QPalette::Light,            QColor(255, 255, 255, 255));

  p.setColor(QPalette::Active,    QPalette::Dark,             QColor(160, 160, 160, 255));
  p.setColor(QPalette::Disabled,  QPalette::Dark,             QColor(160, 160, 160, 255));
  p.setColor(QPalette::Inactive,  QPalette::Dark,             QColor(160, 160, 160, 255));

  p.setColor(QPalette::Active,    QPalette::Mid,              QColor(160, 160, 160, 255));
  p.setColor(QPalette::Disabled,  QPalette::Mid,              QColor(160, 160, 160, 255));
  p.setColor(QPalette::Inactive,  QPalette::Mid,              QColor(160, 160, 160, 255));

  p.setColor(QPalette::Active,    QPalette::Shadow,           QColor(105, 105, 105, 255));
  p.setColor(QPalette::Disabled,  QPalette::Shadow,           QColor(0, 0, 0, 255));
  p.setColor(QPalette::Inactive,  QPalette::Shadow,           QColor(105, 105, 105, 255));

  p.setColor(QPalette::Active,    QPalette::Midlight,         QColor(227, 227, 227, 255));
  p.setColor(QPalette::Disabled,  QPalette::Midlight,         QColor(247, 247, 247, 255));
  p.setColor(QPalette::Inactive,  QPalette::Midlight,         QColor(227, 227, 227, 255));

  p.setColor(QPalette::Active,    QPalette::Highlight,        QColor(42, 130, 218));
  p.setColor(QPalette::Disabled,  QPalette::Highlight,        QColor(42, 130, 218));
  p.setColor(QPalette::Inactive,  QPalette::Highlight,        QColor(42, 130, 218));


  /**********************************/
  /*           Text color           */
  /**********************************/
  p.setColor(QPalette::Active,    QPalette::Text,             QColor(255, 255, 255, 255));
  p.setColor(QPalette::Disabled,  QPalette::Text,             QColor(135, 135, 135, 255));
  p.setColor(QPalette::Inactive,  QPalette::Text,             QColor(255, 255, 255, 255));

  p.setColor(QPalette::Active,    QPalette::BrightText,       QColor(255, 255, 255, 255));
  p.setColor(QPalette::Disabled,  QPalette::BrightText,       QColor(255, 255, 255, 255));
  p.setColor(QPalette::Inactive,  QPalette::BrightText,       QColor(255, 255, 255, 255));

  p.setColor(QPalette::Active,    QPalette::ButtonText,       QColor(255, 255, 255, 255));
  p.setColor(QPalette::Disabled,  QPalette::ButtonText,       QColor(135, 135, 135, 255));
  p.setColor(QPalette::Inactive,  QPalette::ButtonText,       QColor(255, 255, 255, 255));

  p.setColor(QPalette::Active,    QPalette::WindowText,       QColor(255, 255, 255, 255));
  p.setColor(QPalette::Disabled,  QPalette::WindowText,       QColor(135, 135, 135, 255));
  p.setColor(QPalette::Inactive,  QPalette::WindowText,       QColor(255, 255, 255, 255));

  p.setColor(QPalette::Active,    QPalette::ToolTipText,      QColor(255, 255, 255, 255));
  p.setColor(QPalette::Disabled,  QPalette::ToolTipText,      QColor(255, 255, 255, 255));
  p.setColor(QPalette::Inactive,  QPalette::ToolTipText,      QColor(255, 255, 255, 255));

  p.setColor(QPalette::Active,    QPalette::HighlightedText,  QColor(255, 255, 255, 255));
  p.setColor(QPalette::Disabled,  QPalette::HighlightedText,  QColor(255, 255, 255, 255));
  p.setColor(QPalette::Inactive,  QPalette::HighlightedText,  QColor(255, 255, 255, 255));


  /**********************************/
  /*           Base color           */
  /**********************************/
  p.setColor(QPalette::Active,    QPalette::Window,           QColor(53, 53, 53, 255));
  p.setColor(QPalette::Disabled,  QPalette::Window,           QColor(53, 53, 53, 255));
  p.setColor(QPalette::Inactive,  QPalette::Window,           QColor(53, 53, 53, 255));

  p.setColor(QPalette::Active,    QPalette::Button,           QColor(58, 58, 58, 255));
  p.setColor(QPalette::Disabled,  QPalette::Button,           QColor(58, 58, 58, 255));
  p.setColor(QPalette::Inactive,  QPalette::Button,           QColor(58, 58, 58, 255));

  p.setColor(QPalette::Active,    QPalette::Base,             QColor(32, 32, 32, 255));
  p.setColor(QPalette::Disabled,  QPalette::Base,             QColor(16, 16, 16, 255));
  p.setColor(QPalette::Inactive,  QPalette::Base,             QColor(32, 32, 32, 255));

  p.setColor(QPalette::Active,    QPalette::ToolTipBase,      QColor(32, 32, 32, 255));
  p.setColor(QPalette::Disabled,  QPalette::ToolTipBase,      QColor(32, 32, 32, 255));
  p.setColor(QPalette::Inactive,  QPalette::ToolTipBase,      QColor(32, 32, 32, 255));

  p.setColor(QPalette::Active,    QPalette::AlternateBase,    QColor(53, 53, 53, 255));
  p.setColor(QPalette::Disabled,  QPalette::AlternateBase,    QColor(23, 23, 23, 255));
  p.setColor(QPalette::Inactive,  QPalette::AlternateBase,    QColor(53, 53, 53, 255));

  return p;
}

/**********************************/
/*          LIGHT THEME           */
/**********************************/
/*
 * @brief Returns the light theme palette.
 *
 * @return QPalette - The light theme palette.
 *
 * @date 31/05/2023
 */
QPalette ThemesManager::LightTheme()
{
  QPalette p = QPalette();

  /**********************************/
  /* Light color for shadow effects */
  /**********************************/
  p.setColor(QPalette::Active,    QPalette::Light,            QColor(255, 255, 255, 255));
  p.setColor(QPalette::Disabled,  QPalette::Light,            QColor(255, 255, 255, 255));
  p.setColor(QPalette::Inactive,  QPalette::Light,            QColor(255, 255, 255, 255));

  p.setColor(QPalette::Active,    QPalette::Dark,             QColor(160, 160, 160, 255));
  p.setColor(QPalette::Disabled,  QPalette::Dark,             QColor(160, 160, 160, 255));
  p.setColor(QPalette::Inactive,  QPalette::Dark,             QColor(160, 160, 160, 255));

  p.setColor(QPalette::Active,    QPalette::Mid,              QColor(160, 160, 160, 255));
  p.setColor(QPalette::Disabled,  QPalette::Mid,              QColor(160, 160, 160, 255));
  p.setColor(QPalette::Inactive,  QPalette::Mid,              QColor(160, 160, 160, 255));

  p.setColor(QPalette::Active,    QPalette::Shadow,           QColor(105, 105, 105, 255));
  p.setColor(QPalette::Disabled,  QPalette::Shadow,           QColor(0, 0, 0, 255));
  p.setColor(QPalette::Inactive,  QPalette::Shadow,           QColor(105, 105, 105, 255));

  p.setColor(QPalette::Active,    QPalette::Midlight,         QColor(227, 227, 227, 255));
  p.setColor(QPalette::Disabled,  QPalette::Midlight,         QColor(247, 247, 247, 255));
  p.setColor(QPalette::Inactive,  QPalette::Midlight,         QColor(227, 227, 227, 255));

  p.setColor(QPalette::Active,    QPalette::Highlight,        QColor(0, 120, 215, 255));
  p.setColor(QPalette::Disabled,  QPalette::Highlight,        QColor(0, 120, 215, 255));
  p.setColor(QPalette::Inactive,  QPalette::Highlight,        QColor(0, 120, 215, 255));


  /**********************************/
  /*           Text color           */
  /**********************************/
  p.setColor(QPalette::Active,    QPalette::Text,             QColor(0, 0, 0, 255));
  p.setColor(QPalette::Disabled,  QPalette::Text,             QColor(120, 120, 120, 255));
  p.setColor(QPalette::Inactive,  QPalette::Text,             QColor(0, 0, 0, 255));

  p.setColor(QPalette::Active,    QPalette::BrightText,       QColor(255, 255, 255, 255));
  p.setColor(QPalette::Disabled,  QPalette::BrightText,       QColor(255, 255, 255, 255));
  p.setColor(QPalette::Inactive,  QPalette::BrightText,       QColor(255, 255, 255, 255));

  p.setColor(QPalette::Active,    QPalette::ButtonText,       QColor(0, 0, 0, 255));
  p.setColor(QPalette::Disabled,  QPalette::ButtonText,       QColor(120, 120, 120, 255));
  p.setColor(QPalette::Inactive,  QPalette::ButtonText,       QColor(0, 0, 0, 255));

  p.setColor(QPalette::Active,    QPalette::WindowText,       QColor(0, 0, 0, 255));
  p.setColor(QPalette::Disabled,  QPalette::WindowText,       QColor(120, 120, 120, 255));
  p.setColor(QPalette::Inactive,  QPalette::WindowText,       QColor(0, 0, 0, 255));

  p.setColor(QPalette::Active,    QPalette::ToolTipText,      QColor(0, 0, 0, 255));
  p.setColor(QPalette::Disabled,  QPalette::ToolTipText,      QColor(0, 0, 0, 255));
  p.setColor(QPalette::Inactive,  QPalette::ToolTipText,      QColor(0, 0, 0, 255));

  p.setColor(QPalette::Active,    QPalette::HighlightedText,  QColor(255, 255, 255, 255));
  p.setColor(QPalette::Disabled,  QPalette::HighlightedText,  QColor(255, 255, 255, 255));
  p.setColor(QPalette::Inactive,  QPalette::HighlightedText,  QColor(255, 255, 255, 255));


  /**********************************/
  /*           Base color           */
  /**********************************/
  p.setColor(QPalette::Active,    QPalette::Window,           QColor(240, 240, 240, 255));
  p.setColor(QPalette::Disabled,  QPalette::Window,           QColor(240, 240, 240, 255));
  p.setColor(QPalette::Inactive,  QPalette::Window,           QColor(240, 240, 240, 255));

  p.setColor(QPalette::Active,    QPalette::Button,           QColor(240, 240, 240, 255));
  p.setColor(QPalette::Disabled,  QPalette::Button,           QColor(240, 240, 240, 255));
  p.setColor(QPalette::Inactive,  QPalette::Button,           QColor(240, 240, 240, 255));

  p.setColor(QPalette::Active,    QPalette::Base,             QColor(255, 255, 255, 255));
  p.setColor(QPalette::Disabled,  QPalette::Base,             QColor(240, 240, 240, 255));
  p.setColor(QPalette::Inactive,  QPalette::Base,             QColor(255, 255, 255, 255));

  p.setColor(QPalette::Active,    QPalette::ToolTipBase,      QColor(255, 255, 220, 255));
  p.setColor(QPalette::Disabled,  QPalette::ToolTipBase,      QColor(255, 255, 220, 255));
  p.setColor(QPalette::Inactive,  QPalette::ToolTipBase,      QColor(255, 255, 220, 255));

  p.setColor(QPalette::Active,    QPalette::AlternateBase,    QColor(233, 231, 227, 255));
  p.setColor(QPalette::Disabled,  QPalette::AlternateBase,    QColor(247, 247, 247, 255));
  p.setColor(QPalette::Inactive,  QPalette::AlternateBase,    QColor(233, 231, 227, 255));

  return p;
}

/*
 * @brief Returns the warning theme palette based on the given palette.
 *
 * @param p - The base palette.
 *
 * @return QPalette - The warning theme palette.
 *
 * @date 31/05/2023
 */
QPalette ThemesManager::WarningTheme(QPalette p)
{
  // Set colors for warning theme based on the base palette
  p.setColor(QPalette::Base, QColor(202, 81, 0));
  p.setColor(QPalette::Window, QColor(202, 81, 0));
  p.setColor(QPalette::AlternateBase, QColor(202, 81, 0));
  p.setColor(QPalette::WindowText, Qt::white);
  p.setColor(QPalette::Text, Qt::white);

  return p;
}

/*
 * @brief Returns the m_State theme palette based on the given palette.
 *
 * @param p - The base palette.
 *
 * @return QPalette - The m_State theme palette.
 *
 * @date 31/05/2023
 */
QPalette ThemesManager::ErrorTheme(QPalette p)
{
  // Set colors for m_State theme based on the base palette
  p.setColor(QPalette::Base, QColor(255, 0, 0));
  p.setColor(QPalette::Window, QColor(255, 0, 0));
  p.setColor(QPalette::AlternateBase, QColor(255, 0, 0));
  p.setColor(QPalette::WindowText, Qt::white);
  p.setColor(QPalette::Text, Qt::white);

  return p;
}

/*
 * @brief Returns the info theme palette based on the given palette.
 *
 * @param p - The base palette.
 *
 * @return QPalette - The info theme palette.
 *
 * @date 31/05/2023
 */
QPalette ThemesManager::InfoTheme(QPalette p)
{
  // Set colors for info theme based on the base palette
  p.setColor(QPalette::Base, QColor(27, 27, 27));
  p.setColor(QPalette::Window, QColor(27, 27, 27));
  p.setColor(QPalette::AlternateBase, QColor(27, 27, 27));
  p.setColor(QPalette::WindowText, Qt::white);
  p.setColor(QPalette::Text, Qt::white);

  return p;
}

/*
 * @brief Returns the path for the icon with the given name based on the current theme.
 *
 * @param name - The name of the icon.
 *
 * @return QString - The path for the icon.
 *
 * @date 31/05/2023
 */
QString ThemesManager::Icons(QString name)
{
  QMetaEnum metaEnum = QMetaEnum::fromType<Theme>();
  QString strTheme = metaEnum.valueToKey(this->theme);
  return ":/" + strTheme + "/" + name;
}

/*
 * @brief Returns the color for the specified color role and color group.
 *
 * @param role - The color role.
 * @param group - The color group.
 *
 * @return QColor - The color.
 *
 * @date 31/05/2023
 */
QColor ThemesManager::Color(QPalette::ColorRole role, QPalette::ColorGroup group)
{
  return qApp->palette().color(group, role);
}