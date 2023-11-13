#ifndef TOOLS_H
#define TOOLS_H

#include <QtWidgets>

#include "ThemesManager.h"
#include "Settings.h"

#define LOG                                 QString::number(__LINE__)
#define TYPE_TO_STRING(Type)                (void(sizeof(Type)),#Type)
#define VARIABLE_TO_STRING(Variable)        (void(Variable),#Variable)
#define FUNCTION_TO_STRING(Function)        (void(&Function),#Function)
#define METHOD_TO_STRING(ClassName,Method)  (void(&ClassName::Method),#Method)

// Callable via MyClass::MyMethod
#define META_METHOD(Method) (void(&Method), QString(#Method).section("::", 1))

#define LOG_FUNCTION(Function)              QString::number(__LINE__).rightJustified(5, '0') + " " #Function
#define LOG_METHOD(ClassName,Method)        QString::number(__LINE__).rightJustified(5, '0') + " " #ClassName "::" #Method

/*
 * @brief The Tools class represents a collection of utility functions.
 *
 * This class is responsible for providing various utility functions
 * It serves as a toolbox for common operations in the application.
 *
 * @return void
 *
 * @date 31/05/2023
 */
class Tools : public QObject
{
  Q_OBJECT

  private:
    // Constructor
    Tools(QObject *parent = nullptr);

  public:
    // Destructor
    ~Tools();

  public:
    static QString DateToString(double date);
    static QString ChronoToString(qint64 milliseconds, bool useDays = true);
    static QString BitrateToString(double bitrate, bool isKbps = true, int nbDecimal = 2);
    
    static QList<QScreen*> GetScreens();
    static QStringList GetScreensNames();

    static int GetWidgetWidth(QWidget *widget, QSize size, QBoxLayout *mainLayout = nullptr, bool useSpacing = true);
    static int GetWidgetHeight(QWidget *widget, QSize size, QBoxLayout *mainLayout = nullptr, bool useSpacing = true);
    
    static int GetWidgetWidth(qreal ratio, int &width, int &availableWidth, qreal &availableRatio);
    static int GetWidgetHeight(qreal ratio, int &height, int &availableHeight, qreal &availableRatio);

    static int AutoFontSize(int maxWidth, int maxHeight, QString text, QFont defaultFont = QFont());

    static bool CompareFileInfo(const QFileInfo &a, const QFileInfo &b);
    static QString NearestParentPath(QString path);

    static bool IsJumboPixmap(QPixmap &pixmap);
    
};

#endif // TOOLS_H
