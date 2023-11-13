#ifndef INPUTITEMVIEW_H
#define INPUTITEMVIEW_H

#include <QtWidgets>
#include <QVariant>

#include "PageView.h"

#include "LS_X8.h"
#include "TM_Raw.h"
#include "SimIRIG.h"
#include "LS_55_45.h"
#include "ChannelItem.h"

#include "RFView.h"
#include "BSView.h"
#include "FSView.h"
#include "IDView.h"

#define RF_Name "RF"
#define BS_Name "BS"
#define FS_Name "FS"
#define ID_Name "ID"

#define PAGE_RF         0
#define PAGE_BITSYNC    1
#define PAGE_FRAMESYNC  2
#define PAGE_IDENT      3

/*
 * @brief The ChannelItemManager class represents an input item view in the application.
 *
 * This class inherits from the PageView class and is responsible for displaying and managing
 * the UI elements and events specific to an input item. 
 * It provides functionality to update the data associated with the input item. 
 * The class manages 3 items :
 * - BS (BitSync)
 * - FS (FrameSync)
 * - ID (SFID)
 * The class encapsulates the functionality related to an individual input item view.
 *
 * The ChannelItemManager class collaborates with other components
 * to provide a complete user interface for managing input items. 
 * It communicates with the underlying data model to retrieve and update the input item data. 
 *
 * @date 31/05/2023
 */
class ChannelItemManager : public PageView
{
  Q_OBJECT

  private:
    int m_OldIndex = -1;

    ChannelItem* m_CurrentItem = nullptr;

    QVBoxLayout *MainLayout = nullptr;
    QStackedWidget *StackedWidget = nullptr;

    RFView *RF_View = nullptr;
    BSView *BS_View = nullptr;
    FSView *FS_View = nullptr;
    IDView *ID_View = nullptr;

    void SetupUI();
    void SetupEvent();

    void ResizeCurrentChannel();

  protected:
    void resizeEvent(QResizeEvent *event);

  public:
    ChannelItemManager(QWidget *parent = nullptr);
    ~ChannelItemManager();

    void SetChannelItem(ChannelItem *item, QString ledText);
    void SetupChannelItemEvent(ChannelItem *item);
    ChannelItem* RemoveChannelItem();

    void NewChannelData(QVector<double> ptsX, QVector<double> ptsY);

  public slots:
    void CurrentIndexChanged(int index);
    void ChannelLedClicked(ChannelItem *item, QString ledText);

  signals:
    void SendStartReadChannelData(QString channel);
    void SendStopReadChannelData();
};

#endif // INPUTITEMVIEW_H
