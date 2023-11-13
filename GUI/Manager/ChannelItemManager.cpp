#include "stdafx.h"
#include "ChannelItemManager.h"

/*
 * @brief Constructor of the ChannelItemManager
 *
 * This class inherits from the PageView class and is responsible for displaying
 * and managing the UI elements and events specific to an input item.
 *
 * @param type The type of the input item view.
 * @param parent The parent widget.
 * @return void
 *
 * @date 31/05/2023
 */
ChannelItemManager::ChannelItemManager(QWidget *parent) : PageView(parent)
{
  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();
}

/*
 * @brief Destructor the ChannelItemManager object.
 *
 * @return void
 *
 * @date 31/05/2023
 */
ChannelItemManager::~ChannelItemManager()
{
}

/*
 * @brief Sets up the user interface (UI) for the ChannelItemManager.
 *
 * This function is responsible for setting up the UI elements, such as labels, fields, and widgets,
 * specific to the ChannelItemManager based on its type.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void ChannelItemManager::SetupUI()
{
  this->UseLine(true);
  this->AddHeaderView();

  StackedWidget = new QStackedWidget();

  // Create and initialize the various views and item views

  // PAGE 0: RFView
  RF_View = new RFView();

  // PAGE 1: BSView
  BS_View = new BSView();

  // PAGE 2: FSView
  FS_View = new FSView();

  // PAGE 3: IDView
  ID_View = new IDView();

  // Add the views and item views to the PagesManager

  StackedWidget->addWidget(RF_View);            // PAGE_RF         0
  StackedWidget->addWidget(BS_View);            // PAGE_BITSYNC    1
  StackedWidget->addWidget(FS_View);            // PAGE_FRAMESYNC  2
  StackedWidget->addWidget(ID_View);            // PAGE_IDENT      3

  this->AddMainWidget(StackedWidget);
}

void ChannelItemManager::SetupEvent()
{
  QObject::connect(StackedWidget, &QStackedWidget::currentChanged, this, &ChannelItemManager::CurrentIndexChanged);
}

void ChannelItemManager::ResizeCurrentChannel()
{
  if (m_CurrentItem == nullptr)
    return;

  QSize headerSize = this->HeaderSize();
  QSize buttonSize = this->CloseButtonSize();

  int itemWidth = headerSize.width() - buttonSize.width();
  int itemHeight = headerSize.height();

  m_CurrentItem->setFixedSize(itemWidth, itemHeight);
}

void ChannelItemManager::resizeEvent(QResizeEvent * event)
{
  PageView::resizeEvent(event);

  this->ResizeCurrentChannel();

  QSize size = event->size();
  QSize headerSize = this->HeaderSize();

  for (int i = 0; i < StackedWidget->count(); i++)
  {
    QWidget *widget = StackedWidget->widget(i);
    if (widget == nullptr)
      continue;

    widget->setFixedWidth(size.width());
    widget->setFixedHeight(size.height() - headerSize.height());
  }
}

/*
 * @brief Sets the input data for the ChannelItemManager.
 *
 * This function sets the input data for the ChannelItemManager and updates the UI based on the type of input item.
 *
 * @param input The PCM_Input object representing the input data.
 * @return void
 *
 * @date 31/05/2023
 */
void ChannelItemManager::SetChannelItem(ChannelItem *item, QString ledText)
{
  // Set the current input
  this->m_CurrentItem = item;
  this->AddHeaderWidget(item);
  this->ResizeCurrentChannel();

  SetupChannelItemEvent(item);
  CurrentIndexChanged(StackedWidget->currentIndex());
}

void ChannelItemManager::SetupChannelItemEvent(ChannelItem * item)
{
  // GENERAL EVENT
  QObject::connect(item, &ChannelItem::SendLedClicked, this, &ChannelItemManager::ChannelLedClicked);

  Mag200Data *data = item->Data();
  if (data == nullptr)
    return;

  LS_X8 *_LS_X8_ = dynamic_cast<LS_X8*>(item);
  TM_Raw *_TM_Raw_ = dynamic_cast<TM_Raw*>(item);
  SimIRIG *_SimIRIG_ = dynamic_cast<SimIRIG*>(item);
  LS_55_45 *_LS_55_45_ = dynamic_cast<LS_55_45*>(item);

  // SPECIFIC EVENT
  switch (data->AcquisitionCode())
  {
    default:
      return;

    case LSX8:
      if (_LS_X8_ == nullptr)
        return;
      RF_View->Update(item);
      QObject::connect(_LS_X8_, &LS_X8::SendNewSignalStrength, RF_View, &RFView::NewSignalStrength);
      QObject::connect(_LS_X8_, &LS_X8::SendNewSignalDeviation, RF_View, &RFView::NewSignalDeviation);
      return;

    case TM_RAW:
      if (_TM_Raw_ == nullptr)
        return;
      return;

    case SIMIRIG:
      if (_SimIRIG_ == nullptr)
        return;
      return;

    case LS55_45:
      if (_LS_55_45_ == nullptr)
        return;
      return;
  }

}

void ChannelItemManager::NewChannelData(QVector<double> ptsX, QVector<double> ptsY)
{
  ID_View->NewChannelData(ptsX, ptsY);
}

ChannelItem* ChannelItemManager::RemoveChannelItem()
{
  QWidget *widget = static_cast<QWidget*>(this->m_CurrentItem);
  ChannelItem *item = this->m_CurrentItem;

  this->RemoveHeaderWidget(widget);
  this->m_CurrentItem = nullptr;

  return item;
}

void ChannelItemManager::ChannelLedClicked(ChannelItem *, QString ledText)
{
  QStringList list = { RF_Name, BS_Name, FS_Name, ID_Name };
  int index = list.indexOf(ledText);

  if (index == -1)
    return;

  StackedWidget->setCurrentIndex(index);
}

void ChannelItemManager::CurrentIndexChanged(int index)
{
  // CLOSE OLD INDEX
  switch (m_OldIndex)
  {
    case PAGE_RF:
      // No additional actions needed for PAGE_RF
      break;
    case PAGE_BITSYNC:
      // No additional actions needed for PAGE_BITSYNC
      break;
    case PAGE_FRAMESYNC:
      // No additional actions needed for PAGE_FRAMESYNC
      break;
    case PAGE_IDENT:
      emit SendStopReadChannelData();
      break;
  }

  // OPEN NEW INDEX
  switch (index)
  {
    case PAGE_RF:
      // No additional actions needed for PAGE_RF
      break;
    case PAGE_BITSYNC:
      BS_View->Update(m_CurrentItem->Data());
      break;
    case PAGE_FRAMESYNC:
      FS_View->Update(m_CurrentItem->Data());
      break;
    case PAGE_IDENT:
      ID_View->Update(m_CurrentItem->Data());
      emit SendStartReadChannelData(m_CurrentItem->ChannelName());
      break;
  }

  // Store the current index as the old index for the next iteration
  m_OldIndex = index;
}
