#include "stdafx.h"
#include "Ch10Encoder.h"

Ch10Encoder::Ch10Encoder(Mag200Data *board, QWidget *parent) : ChannelItem(board, parent)
{
  SetupUI();
  SetupEvent();
}

Ch10Encoder::~Ch10Encoder()
{

}

void Ch10Encoder::SetupUI()
{
  QHBoxLayout* MainLayout = static_cast<QHBoxLayout*>(this->layout());

  Leds[PCM] = new Led(tr("PCM"));
  Leds[PCM]->UseBorder(false);
  Leds[PCM]->UseBackground(true);

  // Create time LED
  Leds[CH10TIME] = new Led(" TIME");
  Leds[CH10TIME]->UseBackground(false);
  Leds[CH10TIME]->UseAutoSizeFont(false);
  Leds[CH10TIME]->SetAlignement(Qt::AlignLeft | Qt::AlignVCenter);

  // Once the flags set can't change style of the Led
  //Leds[CH10TIME]->UseFixedStyle(true);

  MainLayout->addWidget(Leds[PCM]);
  MainLayout->addWidget(Leds[CH10TIME]);

  MainLayout->addStretch(0);
}

void Ch10Encoder::SetupEvent()
{
  for (Led *led : Leds)
    QObject::connect(led, &Led::clicked, this, &Ch10Encoder::LedClicked);
}

void Ch10Encoder::resizeEvent(QResizeEvent * event)
{
  this->useDefaultResize = false;
  ChannelItem::resizeEvent(event);

  // Use this flags as int to set spacing to 0 if its false
  bool IsDefault = false;
  if (this->parentWidget() != nullptr)
    if (this->parentWidget()->layout() != nullptr)
      if (this->parentWidget()->layout()->objectName() == DEFAULT_PARENT)
        IsDefault = true;

  int width = Tools::GetWidgetWidth(this, event->size(), MainLayout, false);
  int height = Tools::GetWidgetHeight(this, event->size(), MainLayout, false);

  width -= MainLayout->spacing();

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

  int allLedSize = (ledWidth * this->m_LedUsedCount) + (spacing * spacingCount);
  availableWidth = availableWidth - (ledWidth * this->m_LedUsedCount);
  titleWidth = titleWidth + availableWidth * 0.5;
  bitrateWidth = bitrateWidth + availableWidth * 0.5;

  Texts[TITLE]->setFixedWidth(titleWidth);
  Texts[TITLE]->setFont(this->font());

  if (Texts.contains(BITRATE) == true)
  {
    Texts[BITRATE]->setFixedWidth(bitrateWidth);
    Texts[BITRATE]->setFont(this->font());
  }

  Leds[PCM]->SetSize(QSize(ledWidth, ledWidth));
  Leds[PCM]->UseAutoSizeFont(true);

  availableWidth = allLedSize - ledWidth - spacing;

  Leds[CH10TIME]->SetSize(QSize(availableWidth, ledWidth));
  Leds[CH10TIME]->UseAutoSizeFont(true);
}

void Ch10Encoder::ResetLedFlags()
{
  for (Led *led : Leds)
  {
    led->UseBorder(false);
    led->UseBackground(true);
  }

  Leds[CH10TIME]->UseBorder(true);
  Leds[CH10TIME]->UseBackground(false);
}

void Ch10Encoder::AlignLeds(QList<int> LedsLayout)
{
  this->m_LedUsedCount = LedsLayout.size();
  return;
}

void Ch10Encoder::NewStatus(int ChannelID, int Type, QVariant Value)
{
  if (ChannelID != this->ChannelID())
    return;

  //Mag200Status state = static_cast<Mag200Status>(Value.toInt());

  switch (Type)
  {
  default:
    return;

  //case STAT_R1:  SetLedState(RF, state);                                       return;
  //case STAT_B:   SetLedState(BS, state);                                       return;
  //case STAT_SF:  SetLedState(FS, state);                                       return;
  //case STAT_ID:  SetLedState(ID, state);                                       return;
  //case STAT_F:   SetText(BITRATE, Tools::BitrateToString(Value.toDouble()));   return;
  }
}