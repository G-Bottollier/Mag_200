#ifndef LS_X8_H
#define LS_X8_H

#include <QtWidgets>

#include "Led.h"
#include "ChannelItem.h"

class LS_X8 : public ChannelItem
{
  Q_OBJECT

  private:
    double m_Strength = -1;
    double m_Deviation = -1;

    void SetupUI();
    void SetupEvent();

  public:
    LS_X8(Mag200Data* board, QWidget *parent = nullptr);
    ~LS_X8();

    double Strength();
    double Deviation();

    void NewStatus(int ChannelID, int Type, QVariant Value);

  signals:
    void SendNewSignalStrength(double Strength);
    void SendNewSignalDeviation(double Deviation);
};

#endif // LS_X8_H
