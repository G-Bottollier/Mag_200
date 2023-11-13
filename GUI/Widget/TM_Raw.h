#ifndef TM_RAW_H
#define TM_RAW_H

#include <QtWidgets>

#include "Led.h"
#include "SimIRIG.h"

// Using SimIRIG before coding specialization of the class
// For custom specialization subclass with ChannelItem
class TM_Raw : public SimIRIG
{
  Q_OBJECT

  public:
      TM_Raw(Mag200Data* board, QWidget *parent = nullptr);
      ~TM_Raw();
};

#endif // TM_RAW_H
