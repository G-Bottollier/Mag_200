#include "stdafx.h"
#include "ChannelsView.h"
#include "Mag200Data.h"
#include <mag200_str.h>

/*
 * @brief Constructor of the ChannelsView
 *
 * This class inherits from the PageView class and is responsible for initializing the inputs view.
 *
 * @param parent The parent widget.
 * @return void
 *
 * @date 31/05/2023
 */
ChannelsView::ChannelsView(QWidget *parent) : PageView(parent)
{
  // Set up the user interface
  SetupUI();

  // Set up event handling
  SetupEvent();
}

/*
 * @brief The destructor for the ChannelsView class.
 *
 * @return void
 *
 * @date 31/05/2023
 */
ChannelsView::~ChannelsView()
{
  // Remove the input widget
  RemoveAllChannel();
}

void ChannelsView::AddChannelItem(QWidget * widget)
{
  ChannelItem *item = static_cast<ChannelItem*>(widget);
  if (item == nullptr)
    return;

  item->UseLine(true);
  item->ResetLedFlags();

  int width = Tools::GetWidgetWidth(this, this->size());
  int height = Tools::GetWidgetHeight(this, this->size()) * 0.25;

  item->setFixedSize(width, height);

  int ChannelID = item->ChannelID();
  int index = Channels.keys().indexOf(ChannelID);
  ChannelsLayout->addWidget(item, index, 0, 1, 1);

  item->disconnect();
  QObject::connect(item, &ChannelItem::SendLedClicked, this, &ChannelsView::SendLedClicked);
}

/*
 * @brief Sets up the user interface for the ChannelsView class.
 *
 * This function creates a QGridLayout and sets its size constraint to maximum size.
 * It also sets the margin and spacing of the grid layout. Finally, it adds the grid layout
 * to the scroll view of the ChannelsView.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void ChannelsView::SetupUI()
{
  this->UseLine(true);

  this->AddHeaderView();
  this->AddHeaderTitle();
  this->SetHeaderVisible(false);
  this->SetHeaderName(tr("PAGE_CHANNELS"));

  // Create a QGridLayout for the ChannelsView
  ChannelsLayout = new QGridLayout();
  ChannelsLayout->setSizeConstraint(QLayout::SetMaximumSize);
  ChannelsLayout->setMargin(0);
  ChannelsLayout->setSpacing(2);
  ChannelsLayout->setObjectName(DEFAULT_PARENT);

  QList<int> UsedLeds;
  QMap<int, Mag200Data*> datas = qSettings->Mag200Datas();
  QList<QVector3D> layoutPositions = qSettings->LayoutPositions();
  for (int i = 0; i < datas.size(); i++)
  {
    if (i >= layoutPositions.size())
      continue;

    // Get current data of the board
    Mag200Data* data = datas.values()[i];
    int ChannelID = data->ChannelID();

    // Create a new SimIRIG widget and append it to the Channels list
    ChannelItem *channel = GetChannelItem(data);
    if (channel == nullptr)
      continue;

    // Set channel
    Channels[ChannelID] = channel;

    // Get Leds index
    UsedLeds.append(channel->UsedLeds());

    // Get the x and y coordinates from the InputsPosition
    int x = layoutPositions[i].x();
    int y = layoutPositions[i].y();

    // Calculate the span for the widget based on a condition (currently hardcoded as false)
    int span = 1;//false ? GetSpanColumnFromIndex(i) : 1;

    // Add the SimIRIG widget to the ChannelsLayout with the specified position and span
    ChannelsLayout->addWidget(Channels[ChannelID], y, x, 1, span);

    // Set the z-coordinate of the InputsPosition to true to mark it as used
    layoutPositions[i].setZ(true);
  }

  // Add the grid layout to the scroll view of the ChannelsView
  this->AddScrollView(ChannelsLayout);

  // Removes duplicates since QSet does not allow duplicates
  // Sort the list
  UsedLeds = UsedLeds.toSet().toList();
  qSort(UsedLeds);

  // Align all leds of each item
  for (ChannelItem *item : Channels)
    item->AlignLeds(UsedLeds);
}

/*
 * @brief Sets up the event handling for the ChannelsView class.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void ChannelsView::SetupEvent()
{
  for (int ChannelID : Channels.keys())
    QObject::connect(Channels[ChannelID], &ChannelItem::SendLedClicked, this, &ChannelsView::SendLedClicked);
}

void ChannelsView::resizeEvent(QResizeEvent * event)
{
  PageView::resizeEvent(event);

  int width = Tools::GetWidgetWidth(this, event->size());
  int height = Tools::GetWidgetHeight(this, event->size());

  for (ChannelItem *item : Channels)
  {
    int index = ChannelsLayout->indexOf(item);

    // Prevent moved out widget to be resized
    if (index == -1)
      continue;

    item->setFixedHeight(height * 0.25);
    item->setFixedWidth(width);
  }
}

 void ChannelsView::NewStatus(int ChannelID, int Type, QVariant Value)
 {
   QList<int> keys = Channels.keys();
   if (keys.contains(ChannelID) == false)
     return;

   ChannelItem* item = Channels[ChannelID];
   if (item == nullptr)
     return;

   item->NewStatus(ChannelID, Type, Value);
 }

/*
 * @brief Removes and deletes the input widgets from the ChannelsLayout.
 *
 * This function removes and deletes each input widget from the ChannelsLayout
 *
 * @return void
 *
 * @date 31/05/2023
 */
void ChannelsView::RemoveAllChannel()
{
  // Remove and delete each input widget
  for (int i = 0; i < Channels.size(); i++)
  {
    // Remove the input widget from the ChannelsLayout
    ChannelsLayout->removeWidget(Channels[i]);

    // Delete the input widget
    delete Channels[i];
  }

  // Clear the Channels list
  Channels.clear();
}

ChannelItem * ChannelsView::GetChannelItem(Mag200Data * data)
{
  if (data == nullptr)
    return nullptr;

  ChannelItem *item = nullptr;
  QString type = qSettings->Mag200_Type();

  int code = data->AcquisitionCode();
  switch (code)
  {
    default:                                            return item;

    /* Systeme d'acquisition TLM */
    case LSX8:          item = new LS_X8(data);         return item;
    case LS55_45:       item = new LS_55_45(data);      return item;
    case SIMIRIG:       item = new SimIRIG(data);       return item;
    case TM_RAW_CODE:   item = new TM_Raw(data);        return item;

    /* Systeme d'acquisition ETH */
    case BSC:                                           return item;
    case BSS:                                           return item;
    case RTR:                                           return item;
    case CH10:                                          return item;
    case TMOIP:                                         return item;
    case ETHERNET:                                      return item;
    case CH10_ENCODER:  
      item = new Ch10Encoder(data);
      if (type != RnR)
        item->setVisible(false);
      return item;
  }
  return item;
}

/*
 * @brief Calculates the span column value based on the index.
 *
 * This function calculates the span column value based on the InputsPosition list.
 * It checks if the new index is within the valid range and calculates the span based on the current and next position.
 *
 * If the current position is at the last column, the span is 1. 
 * If the current position and the next position are in the same row :
 *  - Then the span is the difference between the x-coordinates. 
 *  - Otherwise, the span is the remaining columns in the row plus 1.
 *
 * @param idx The index for which to calculate the span column value.
 *
 * @return int The calculated span column value.
 *
 * @date 31/05/2023
 */
int ChannelsView::GetSpanColumnFromIndex(int idx)
{
  int span = 1;
  int newIdx = idx + 1;
  int nbCol = 0;
  QList<QVector3D> layoutPositions = qSettings->LayoutPositions();

  // Check if the new index is within the valid range of InputsPosition
  if (newIdx >= 0 && newIdx < layoutPositions.size())
  {
    QPoint p1 = layoutPositions[idx].toPoint();
    QPoint p2 = layoutPositions[newIdx].toPoint();

    // Check if the current position is at the last column
    if (p1.x() == nbCol)
      return span;

    // Check if the current position and the next position are in the same row
    bool isSameRow = p1.y() == p2.y();

    // Calculate and return the span value
    if (isSameRow == true)
      return p2.x() - p1.x();
    else
      return nbCol - p1.x() + 1;
  }

  return span;
}

/*
 * @brief Handles the LedClicked signal from an SimIRIG widget.
 *
 * This function emits the SendInputLedClicked signal with the specified type and input object.
 *
 * @param type The type of the LED click event.
 * @param input The PCM_Input object associated with the clicked LED.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void ChannelsView::SendLedClicked(ChannelItem *item, QString ledText)
{
  item->disconnect();
  item->UseLine(false);
  ChannelsLayout->removeWidget(item);

  // Send the SendInputLedClicked signal
  emit SendChannelLedClicked(item, ledText);
}