#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QtWidgets>

#include "Led.h"
#include "Tools.h"
#include "MessagesManager.h"
#include "CommandsManager.h"
#include "SettingsManager.h"

/*
 * @brief The StatusBar class represents a status bar widget.
 *
 * The StatusBar class inherits from QFrame.
 * Provides functionality for displaying status information : 
 *  - Info
 *  - Warning
 *  - Error
 * Update the Theme of the bar according to the current status
 *
 * @date 31/05/2023
 */
class StatusBar : public QFrame
{
  Q_OBJECT

  private:
    int current = -1;
    QLabel *Status = nullptr;
    QLabel *TestArea = nullptr;
    QHBoxLayout *MainLayout = nullptr;

    void SetupUI();
    void SetupEvent();

  protected:
    void resizeEvent(QResizeEvent *event);

  public:
    StatusBar(QWidget *parent = nullptr);
    ~StatusBar();

  public slots:
    void AddInfo(QString info);
    void AddError(QString m_State);
    void AddWarning(QString warning);
    void NewTestArea(QString testArea);

    void NewStatusAcknowledge(int status);

  signals:
    void SendLedClicked_LOG();
};

#endif // STATUSBAR_H
