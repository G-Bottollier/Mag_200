#ifndef LS_55_45_H
#define LS_55_45_H

#include <QtWidgets>

#include "Led.h"
#include "SimIRIG.h"

// Using SimIRIG before coding specialization of the class
// For custom specialization subclass with ChannelItem
class LS_55_45 : public SimIRIG
{
  Q_OBJECT

  public:
    LS_55_45(Mag200Data* board, QWidget *parent = nullptr);
    ~LS_55_45();
};

#endif // LS_55_45_H
