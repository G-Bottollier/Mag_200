#ifndef CH10ENCODER_H
#define CH10ENCODER_H

#include "ChannelItem.h"

class Ch10Encoder : public ChannelItem
{
  Q_OBJECT

  private:
    
    void SetupUI();
    void SetupEvent();

  protected:
    void resizeEvent(QResizeEvent * event);

  public:
      Ch10Encoder(Mag200Data *board, QWidget *parent = nullptr);
      ~Ch10Encoder();

      void ResetLedFlags();
      void AlignLeds(QList<int> LedsLayout);

  public slots:
      void NewStatus(int ChannelID, int Type, QVariant Value);
};

#endif // CH10ENCODER_H
