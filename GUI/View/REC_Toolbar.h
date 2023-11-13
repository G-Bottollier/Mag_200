#ifndef REC_TOOLBAR_H
#define REC_TOOLBAR_H

#include "Toolbar.h"

class REC_Toolbar : public Toolbar
{
  Q_OBJECT

  private:
    void SetupUI();
    void SetupEvent();

    void LedClicked();

    void NewStatus_NDA_STK(double Value);
    void NewStatus_NDA_TIME_LENGTH(double Value);

    void NewStatus_CH10_STK(Mag200Data *channel, int Value);
    void NewStatus_CH10_ETH(Mag200Data *channel, int Value);

  public:
    REC_Toolbar(QWidget *parent = nullptr);
    ~REC_Toolbar();

  public slots:
    void NewStatus(int ChannelID, int Type, QVariant Value);
};

#endif // REC_TOOLBAR_H
