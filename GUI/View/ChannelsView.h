#ifndef INPUTSVIEW_H
#define INPUTSVIEW_H


#include <QtWidgets>

#include "PageView.h"
#include "SettingsManager.h"

#include "LS_X8.h"
#include "TM_Raw.h"
#include "SimIRIG.h"
#include "LS_55_45.h"
#include "Ch10Encoder.h"

#define NB_LAYOUT 3
#define MAX_LAYOUT 100
#define JSON_LAYOUT "layout"

#define LAYOUT_SIMPLE 0
#define LAYOUT_DOUBLE 1
#define LAYOUT_CUSTOM 2

/*
 * @brief The ChannelsView class represents a view for managing inputs in a GUI application.
 *
 * This class is responsible for displaying and managing the inputs in the graphical user interface. 
 * It provides functionality for manages inputs widget and data.
 *
 * @date 01/06/2023
 */
class ChannelsView : public PageView
{
  Q_OBJECT

  private:
    QMap<int, ChannelItem*> Channels; // Input item pointer linked to channel ID

    QGridLayout  *ChannelsLayout = nullptr;
    int GetSpanColumnFromIndex(int idx);

    void SetupUI();
    void SetupEvent();

    void RemoveAllChannel();
    ChannelItem* GetChannelItem(Mag200Data* data);

  protected:
    void resizeEvent(QResizeEvent *event);

  public:
      ChannelsView(QWidget *parent = nullptr);
      ~ChannelsView();

      void AddChannelItem(QWidget *widget);

      void NewStatus(int ChannelID, int Type, QVariant Value);

  public slots:
      void SendLedClicked(ChannelItem *widget, QString ledText);

  signals:
      void SendChannelLedClicked(ChannelItem *widget, QString ledText);
};

#endif // INPUTSVIEW_H
