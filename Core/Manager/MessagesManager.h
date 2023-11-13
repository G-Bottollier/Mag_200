#ifndef MESSAGESMANAGER_H
#define MESSAGESMANAGER_H

#include <QtWidgets>
#include "Tools.h"

#include "Status.h"
#include "ScreensManager.h"
#include "Chapter6Command.h"

#if defined(CMAG200SHELLGUI_LIB)
#  define CMag200ShellGui_EXPORT Q_DECL_EXPORT
#else
#  define CMag200ShellGui_EXPORT Q_DECL_IMPORT
#endif

#define MAX_LOG_INDEX       3
#define LOG_APPLICATION     0
#define LOG_SHARED_MEMORY   1
#define LOG_NDA_SDK         2

// Convenience macro to access the MessagesManager instance
#define qMessages (static_cast<MessagesManager*>(MessagesManager::Instance()))

// Define log file path
#define LOG_FILENAME            qApp->applicationDirPath() + "/../tmp/Mag200.log"
#define LOG_HEADER_SIZE         (LOG_DATE_SIZE + LOG_FUNCTION_SIZE + 1)
#define LOG_DATE_FORMAT         QString("dd-MM-yyyy hh:mm:ss:zzz")
#define LOG_DATE_SIZE           LOG_DATE_FORMAT.length() + 2
#define LOG_FUNCTION_SIZE       75

/*
 * @brief The MessagesManager class represents the message manager of the application.
 *
 * This class is responsible for managing the display of popup messages and status information in the application.
 * It provides functionalities for showing popup messages, updating status information, and displaying progress.
 *
 * @date 31/05/2023
 */
class CMag200ShellGui_EXPORT MessagesManager : public QDialog
{
  Q_OBJECT

  private:
    // Flags
    bool bypass = false;
    bool isWaiting = false;
    bool isInitialize = false;

    // Draw
    const int borderSize = 20;
    const int borderRadius = 30;

    // UI
    QLabel *Title = nullptr;
    QLabel *Description = nullptr;
    QProgressBar *Progress = nullptr;
    QLabel *StrTime = nullptr;

    // Timer
    QTimer *Timer = nullptr;
    QElapsedTimer *ElapsedSecond = nullptr;

    void SetupUI();
    void SetupEvent();

    void UpdateElapsedTime(bool force = false);
    QString GetTimerStringFromSecond(int second);

  protected:
    void keyReleaseEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *);

  private:
    // Constructor
    explicit MessagesManager(QWidget *parent = nullptr);

  public:
    // Destructor
    ~MessagesManager();
    
    static MessagesManager * Instance();

    static const int Max = -2;
    static const int Auto = -1;

    // POP UP DISPLAY
    void InitParent(QWidget *parent);
    void Initialize();
    void Open();
    void Close();
    void EnableWaitingMode();

    void SetScrollRange(int min, int max, bool force = false);
    void SetScrollPosition(int currentPosition, bool force = false);

    void SetTitle(QString text, bool force = false);
    void SetDescription(QString text, bool force = false);

    // STATUS BAR DISPLAY
    void NewLog(int index, QStringList list);

    // STATUS OF STATUS BAR
    void NewInfo(QString info);
    void NewError(QString m_State);
    void NewWarning(QString Warning);

    // LOG TO Mag200.log
    void OpenLog();
    void Log(QString function, QString log);

  public slots:
    void TimerTimeout();

  signals:
    // LOG EVENT
    void SendNewLog(int index, QStringList list);

    // STATUS BAR EVENT
    void SendNewInfo(QString info);
    void SendNewError(QString m_State);
    void SendNewWarning(QString warning);
};

#endif // MESSAGESMANAGER_H
