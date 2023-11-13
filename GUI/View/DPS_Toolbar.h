#ifndef DPS_TOOLBAR_H
#define DPS_TOOLBAR_H

#include "Toolbar.h"

class DPS_Toolbar : public Toolbar
{
  Q_OBJECT

  private:

    void SetupUI();
    void SetupEvent();

    void LedClicked();

    void NewStatus_NDA_ACQ(bool Value);
    void NewStatus_NDA_STK(bool Value);
    void NewStatus_NDA_TIME(double Value);

    void NewStatus_CH10_ETH(Mag200Data *channel, int Value);
    void NewStatus_CH10_STK(Mag200Data *channel, int Value);
    void NewStatus_TIME(Mag200Data *channel, int Value);    

  public:
    DPS_Toolbar(QWidget *parent = nullptr);
    ~DPS_Toolbar();

  public slots:
    void NewStatus(int ChannelID, int Type, QVariant Value);
};

#endif // DPS_TOOLBAR_H
