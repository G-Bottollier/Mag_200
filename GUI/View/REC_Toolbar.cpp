#include "stdafx.h"
#include "REC_Toolbar.h"

REC_Toolbar::REC_Toolbar(QWidget *parent) : Toolbar(parent)
{
  SetupUI();
  SetupEvent();
}

REC_Toolbar::~REC_Toolbar()
{

}

void REC_Toolbar::SetupUI()
{
  // Create Status LEDs
  Leds[REC] = new Led("REC");
  Leds[REC]->UseBorder(false);
  Leds[REC]->SetIcon(qThemes->Icons("REC_0.png"), true);

  // Find the width of the Time LED
  QString timeType = this->TimeType();
  QStringList letters = timeType.split("", QString::SkipEmptyParts);

  // Create time LED
  Leds[ELAPSED_TIME] = new Led("ELAPSED_TIME");
  Leds[ELAPSED_TIME]->SetInfos(letters);
  Leds[ELAPSED_TIME]->SetAlignement(Qt::AlignLeft | Qt::AlignVCenter);

  Leds[CH10_STK] = new Led("CH10");
  Leds[CH10_STK]->SetIcon(qThemes->Icons("ECH10_0.png"), true);

  Leds[CH10_ETH] = new Led("CH10");
  Leds[CH10_ETH]->SetIcon(qThemes->Icons("CH10_0.png"), true);

  // Set global properties
  for (Led* led : Leds)
  {
    led->UseBackground(false);
    led->UseAutoSizeFont(false);
  }

  Leds[HOME_MENU] = new Led("MENU");
  Leds[HOME_MENU]->SetState(Mag200Status::GRAY, Color::RECORD_01);
  Leds[HOME_MENU]->SetIcon(qThemes->Icons("MENU_RECORD_0.png"), true);

  // Add widget to the view
  this->MainLayout->addWidget(Leds[REC]);
  this->MainLayout->addWidget(Leds[ELAPSED_TIME]);
  this->MainLayout->addWidget(Leds[CH10_STK]);

  if (this->HasRF() == true)
    this->MainLayout->addWidget(Leds[CH10_ETH]);

  this->MainLayout->addWidget(Leds[HOME_MENU]);
}

void REC_Toolbar::SetupEvent()
{
  for (Led* led : Leds)
    QObject::connect(led, &Led::clicked, this, &REC_Toolbar::LedClicked);
}

void REC_Toolbar::LedClicked()
{
  int index = LedIndex();

  switch (index)
  {
    default:                                                    return;
    case REC:             emit /* SendLedClicked(PAGE_); */     return;
    case ELAPSED_TIME:    emit /* SendLedClicked(PAGE_); */     return;
    case CH10_STK:        emit SendLedClicked(PAGE_STORAGES);        return;
    case CH10_ETH:        emit SendLedClicked(PAGE_CH10);       return;
    case HOME_MENU:       emit SendLedClicked(PAGE_MENU);       return;
  }
}

void REC_Toolbar::NewStatus(int ChannelID, int Type, QVariant Value)
{
  Mag200Data *channel = nullptr;
  QMap<int, Mag200Data*> data = qSettings->Mag200Datas();
  if (data.contains(ChannelID) == true)
    channel = data[ChannelID];

  switch (Type)
  {
    default:                                                              return;

    // NDA SDK STATUS
    case NDA_STK:           NewStatus_NDA_STK(Value.toBool());            return;
    case NDA_TIME_LENGTH:   NewStatus_NDA_TIME_LENGTH(Value.toDouble());  return;

    // SHARED MEMORY STATUS
    case STAT_CH10_ETH:  NewStatus_CH10_ETH(channel, Value.toInt());      return;
    case STAT_CH10_STK:  NewStatus_CH10_STK(channel, Value.toInt());      return;
  }
}

void REC_Toolbar::NewStatus_NDA_STK(double Value)
{
  Mag200Status status = Mag200Status::UNLOCK;
  //Color customColor = Color::NO_COLOR;

  if (Value)
  {
    status = Mag200Status::LOCK;
    //customColor = StoragesManager::AvailableSpaceColor(qSettings->App_Storage());
  }

  Leds[REC]->SetState(status/*, customColor*/);
  Leds[ELAPSED_TIME]->SetState(status/*, customColor*/);
}

void REC_Toolbar::NewStatus_NDA_TIME_LENGTH(double Value)
{
  Leds[ELAPSED_TIME]->setText(" " + Tools::ChronoToString(Value));
}



void REC_Toolbar::NewStatus_CH10_STK(Mag200Data *channel, int Value)
{
  if (channel->IsCH10() == false)
    return;

  Mag200Status status = static_cast<Mag200Status>(Value);
  Leds[CH10_ETH]->SetState(status);
}

void REC_Toolbar::NewStatus_CH10_ETH(Mag200Data *channel, int Value)
{
  if (channel->IsCH10() == false)
    return;

  Mag200Status status = static_cast<Mag200Status>(Value);
  Leds[CH10_STK]->SetState(status);
}
