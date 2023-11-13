#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

#include <QStackedWidget>

#include "SettingsManager.h"

#include "DPS_Toolbar.h"
#include "TGS_Toolbar.h"
#include "IMP_Toolbar.h"
#include "EXP_Toolbar.h"
#include "REP_Toolbar.h"
#include "REC_Toolbar.h"

#define TOOLBAR_DPS 0
#define TOOLBAR_TGS 1
#define TOOLBAR_EXP 2
#define TOOLBAR_IMP 3
#define TOOLBAR_REC 4
#define TOOLBAR_REP 5

class ToolbarManager : public QStackedWidget
{
  Q_OBJECT

  private:
    int m_OldIndex = -1;

    QMap<int, Toolbar*> Toolbars;
    
    void SetupUI();
    void SetupEvent();

  protected:
    void resizeEvent(QResizeEvent *event);

  public:
    ToolbarManager(QWidget *parent = nullptr);
    ~ToolbarManager();

  public slots:
    void NewStatus(int ChannelID, int Type, QVariant Value);
    void CurrentIndexChanged(int index);
    void NewToolbarIndex(int index);
    void LedClicked(int type);

    void NewDrivesOpen();
    void NewOpenParentFolder();
    void NewFolderOpen(QFileInfo info);

  signals:
    void SendOpenParentFolder();
    void SendLedClicked(int type);
};

#endif // TOOLBARMANAGER_H
