#include "stdafx.h"
#include "ChannelItem.h"

ChannelItem::ChannelItem(Mag200Data *Data, QWidget *parent) : QFrame(parent)
{
  // Initializes Input Item data
  this->m_Data = Data;
  this->m_PacketID = 0; // Data->PacketID();
  this->m_ChannelID = Data->ChannelID();

  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();
}

ChannelItem::~ChannelItem()
{
  for (Led* led : Leds)
    led->disconnect();
}

void ChannelItem::SetupUI()
{
  // Setup the user interface for the SimIRIG
  this->setFrameShape(QFrame::StyledPanel);
  this->UseLine(true);

  QMargins margin(0, 0, 0, 0);
  this->MainLayout = new QHBoxLayout();
  MainLayout->setSpacing(25);
  MainLayout->setContentsMargins(margin);

  QString Name = this->m_Data->AcquisitionName();

  // Create and configure the label for displaying the input ID
  Texts[TITLE] = new QLabel(Name);
  Texts[TITLE]->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

  MainLayout->addWidget(Texts[TITLE], 1);

  this->setLayout(MainLayout);
}

void ChannelItem::SetupEvent()
{

}


/*
* @brief Handles the paint event for the SimIRIG.
*
* Draws a line at the top of the SimIRIG.
*
* @param e The QPaintEvent object.
* @return void
*
* @date 31/05/2023
*/
void ChannelItem::paintEvent(QPaintEvent *e)
{
  QWidget::paintEvent(e);

  // Return is the flag is false
  if (this->m_UseLine == false)
    return;

  // Get the top left point
  QRect rect = this->rect();
  QPoint pos = rect.topLeft();

  // Get the QPainter
  QPainter p(this);

  // Get the pen width and color from the theme
  QColor color = qThemes->Color(QPalette::Text);
  QPen pen(QBrush(color), this->height() * 0.01);
  p.setPen(pen);

  // Calculates the destination point for drawing the line
  QPoint dst = QPoint(rect.width(), pos.y());

  // Draws a line from the top-left position to the destination point
  p.drawLine(QLine(pos, dst));
}

void ChannelItem::changeEvent(QEvent * event)
{
  if (event->type() != QEvent::FontChange)
  {
    QFrame::changeEvent(event);
    return;
  }

  for (QLabel* text : Texts)
    text->setFont(this->font());

  QFrame::changeEvent(event);
}

void ChannelItem::resizeEvent(QResizeEvent * event)
{
  if (useAutoSizeFont == true)
  {
    int height = Tools::GetWidgetHeight(this, event->size(), MainLayout, false);

    qreal fontsize = height * 0.45;

    QFont font = this->font();
    font.setPixelSize(fontsize);
    font.setBold(true);
    this->setFont(font);
  }

  if(useDefaultResize == true)
  {
    // Use this flags as int to set spacing to 0 if its false
    bool IsDefault = false;
    if (this->parentWidget() != nullptr)
      if (this->parentWidget()->layout() != nullptr)
        if (this->parentWidget()->layout()->objectName() == DEFAULT_PARENT)
          IsDefault = true;

    QSize size = event->size();

    int width = Tools::GetWidgetWidth(this, size, MainLayout, false);
    int height = Tools::GetWidgetHeight(this, size, MainLayout, false);

    width -= IsDefault * MainLayout->spacing();

    qreal ratioW = 1.0;
    int availableWidth = width;

    ratioW = ratioW - (this->m_LedUsedCount * 0.15);
    qreal ratioLabel = ratioW * 0.5;

    int titleWidth = Tools::GetWidgetWidth(ratioLabel, width, availableWidth, ratioW);
    int bitrateWidth = Tools::GetWidgetWidth(ratioLabel, width, availableWidth, ratioW);

    int spacing = MainLayout->spacing();
    int spacingCount = IsDefault * qMax(0, this->m_LedUsedCount - 1);

    availableWidth = availableWidth - (spacing * spacingCount);
    int ledWidth = availableWidth / (double)this->m_LedUsedCount;

    qreal maxLedHeight = 0.85 * height;
    if (ledWidth > maxLedHeight)
      ledWidth = maxLedHeight;

    int additionalWidth = availableWidth - (ledWidth * this->m_LedUsedCount);
    titleWidth = (titleWidth + additionalWidth * 0.5);
    bitrateWidth = (bitrateWidth + additionalWidth * 0.5) - (IsDefault * spacing);

    Texts[TITLE]->setFixedWidth(titleWidth);
    Texts[TITLE]->setFont(this->font());

    if (Texts.contains(BITRATE) == true)
    {
      Texts[BITRATE]->setFixedWidth(bitrateWidth);
      Texts[BITRATE]->setFont(this->font());
    }

    for (int i = 0; i < MainLayout->count(); i++)
    {
      QLayoutItem *item = MainLayout->itemAt(i);
      QSpacerItem *spacer = dynamic_cast<QSpacerItem*>(MainLayout->itemAt(i));
      if (item == nullptr)
        continue;

      if (spacer != nullptr)
      {
        if (spacer->expandingDirections() == Qt::Vertical)
          continue;

        if (spacer->sizePolicy().horizontalPolicy() == QSizePolicy::Expanding)
          continue;

        int size = IsDefault * (ledWidth + spacing);
        spacer->changeSize(size, spacer->geometry().height());
        continue;
      }

      Led *led = dynamic_cast<Led*>(item->widget());
      if (led == nullptr)
        continue;

      led->SetSize(QSize(ledWidth, ledWidth));
      led->UseAutoSizeFont(true);
    }
  }

  QWidget::resizeEvent(event);
}

int ChannelItem::ChannelID()
{
  return this->m_ChannelID;
}

QString ChannelItem::ChannelName()
{
  if (this->m_Data == nullptr)
    return QString();
  return this->m_Data->SFID_Name();
}

Mag200Data* ChannelItem::Data() const
{
  return this->m_Data;
}

void ChannelItem::UsePacketID(bool UsePacketID)
{
  this->m_UsePacketID = UsePacketID;
}

void ChannelItem::UseLine(bool UseLine)
{
  this->m_UseLine = UseLine;
}

void ChannelItem::ResetLedFlags()
{
  for (Led *led : Leds)
  {
    led->UseBorder(false);
    led->UseBackground(true);
  }
}

void ChannelItem::SetText(int index, QString text)
{
  if (Texts.contains(index) == false)
    return;

  Texts[index]->setText(text);
}

void ChannelItem::LedClicked()
{
  Led* led = static_cast<Led*>(QObject::sender());
  if (led == nullptr)
    return;

  ResetLedFlags();

  led->UseBorder(true);
  led->UseBackground(false);

  emit SendLedClicked(this, led->text());
}

void ChannelItem::SetLedState(int index, Mag200Status state)
{
  if (Leds.contains(index) == false)
    return;

  Leds[index]->SetState(state);
}

void ChannelItem::NewStatus(int ChannelID, int Type, QVariant Value)
{
  if (ChannelID != this->ChannelID())
    return;

  Mag200Status state = static_cast<Mag200Status>(Value.toInt());

  switch (Type)
  {
    default:
      return;

    case STAT_R1:  SetLedState(RF, state);                                       return;
    case STAT_B:   SetLedState(BS, state);                                       return;
    case STAT_SF:  SetLedState(FS, state);                                       return;
    case STAT_ID:  SetLedState(ID, state);                                       return;
    case STAT_F:   SetText(BITRATE, Tools::BitrateToString(Value.toDouble()));   return;
  }
}

QList<int> ChannelItem::UsedLeds()
{
  return Leds.keys();
}

void ChannelItem::AlignLeds(QList<int> LedsLayout)
{
  this->m_LedUsedCount = LedsLayout.size();

  if (LedsLayout.size() == 0)
    return;

  if (LedsLayout == this->UsedLeds())
    return;

  // Default index
  int first = -1; 
  int layoutIndex = MainLayout->indexOf(Texts[TITLE]);

  // Insert spacing for each missing led in the Map (Leds)
  for (int i = 0; i < LedsLayout.size(); i++)
  {
    int led = LedsLayout[i];

    // If the leds exist
    if (Leds.contains(led) == true)
    {
      // Update index to the current finded led
      first = led;
      layoutIndex = MainLayout->indexOf(Leds[led]);

      // Continue, no need to add spacing
      continue;
    }

    // Calculate index and width, insert Spacing
    int index = led < first ? layoutIndex : layoutIndex + 1;
    int width = 10; // Auto sized by resizeEvent

    MainLayout->insertSpacing(index, width);
  }
}