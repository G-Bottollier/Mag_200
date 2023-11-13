#ifndef CMAG200GUILIB_H
#define CMAG200GUILIB_H

#include <QtWidgets>

// Thread
#include "Mag200Controller.h"

// Manager
#include "MessagesManager.h"
#include "SettingsManager.h"
#include "StoragesManager.h"

// GUI
#include "ToolbarManager.h"
#include "PagesManager.h"
#include "StatusBar.h"

#if defined(CMAG200SHELLGUI_LIB)
#  define CMag200ShellGui_EXPORT Q_DECL_EXPORT
#else
#  define CMag200ShellGui_EXPORT Q_DECL_IMPORT
#endif

/*
 * @brief The CMag200ShellGui class represents the main GUI application for CMAG200.
 *
 * class and is responsible for initializing the GUI, setting up event connections, 
 * and managing the interaction between various components of the application.
 * 
 * @date 31/05/2023
 */
class CMag200ShellGui_EXPORT CMag200ShellGui : public QMainWindow
{
	Q_OBJECT

  private:
    // GUI
    ToolbarManager *m_ToolbarManager = nullptr;
    PagesManager *m_PageManager = nullptr;
    StatusBar * m_StatusBar = nullptr;

    QVBoxLayout *MainLayout = nullptr;

    // THREAD
    Mag200Controller *m_Mag200Controller = nullptr;

    quint16 fileChecksum = 0;
    QScreen *m_Mag200Screen = nullptr;
    bool HasUniqueConnection = false;

    void SetupUI();
    void SetupEvent();

  protected:
    void resizeEvent(QResizeEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

  public:
	  CMag200ShellGui(QWidget *parent = nullptr);
	  ~CMag200ShellGui();

    void Initialize();

    void DeleteCentralWidget();


  public slots:
    void Reload();
    void ResizePageManager(bool UseFullPage);
};
#endif // CMAG200GUILIB_H
