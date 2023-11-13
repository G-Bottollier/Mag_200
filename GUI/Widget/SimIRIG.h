#ifndef INPUTITEM_H
#define INPUTITEM_H

#include <QtWidgets>

#include "Led.h"
#include "ChannelItem.h"

/*
 * @brief The SimIRIG class represents an input item in the GUI.
 *
 * It displays information about an input, such as its ID, state LEDs, and bitrate.
 * The class provides methods for setting the state of the LEDs and the input's bitrate.
 * It also handles click events on the LEDs.
 *
 * @date 31/05/2023
 */
class SimIRIG : public ChannelItem
{
  Q_OBJECT

  private:
    void SetupUI();
    void SetupEvent();

  public:
    SimIRIG(Mag200Data* board, QWidget *parent = nullptr);
    ~SimIRIG();
};

#endif // INPUTWIDGET_H
