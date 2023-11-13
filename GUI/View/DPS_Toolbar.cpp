#include "stdafx.h"
#include "DPS_Toolbar.h"

DPS_Toolbar::DPS_Toolbar(QWidget *parent) : Toolbar(parent)
{
  SetupUI();
  SetupEvent();
}

DPS_Toolbar::~DPS_Toolbar()
{

}

void DPS_Toolbar::SetupUI()
{
  // Create Status LEDs
  Leds[ACQ] = new Led("ACQ");
  Leds[ACQ]->SetIcon(qThemes->Icons("ACQ_0.png"), true);

  // Find the width of the Time LED
  QString timeType = this->TimeType();
  QStringList letters = timeType.split("", QString::SkipEmptyParts);

  // Create time LED
  Leds[TIME] = new Led("TIME");
  Leds[TIME]->SetInfos(letters);
  Leds[TIME]->SetAlignement(Qt::AlignLeft | Qt::AlignVCenter);

  Leds[STK] = new Led("STK");
  Leds[STK]->SetIcon(qThemes->Icons("STK_0.png"), true);

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

  // Add widget to the view
  this->MainLayout->addWidget(Leds[ACQ]);
  this->MainLayout->addWidget(Leds[TIME]);
  if (this->HasRF() == true)
  {
    this->MainLayout->addWidget(Leds[CH10_STK]);
    this->MainLayout->addWidget(Leds[CH10_ETH]);
  }
  else
    this->MainLayout->addWidget(Leds[STK]);
}

void DPS_Toolbar::SetupEvent()
{
  for (Led* led : Leds)
    QObject::connect(led, &Led::clicked, this, &DPS_Toolbar::LedClicked);
}

void DPS_Toolbar::LedClicked()
{
  int index = LedIndex();

  switch (index)
  {
    default:                                                return;
    case ACQ:         emit /* SendLedClicked(PAGE_ACQ); */  return;
    case TIME:        emit /* SendLedClicked(PAGE_);    */  return;
    case STK:         emit SendLedClicked(PAGE_STORAGES);        return;
    case CH10_STK:    emit SendLedClicked(PAGE_STORAGES);        return;
    case CH10_ETH:    emit SendLedClicked(PAGE_CH10);       return;
  }
}

void DPS_Toolbar::NewStatus(int ChannelID, int Type, QVariant Value)
{
  Mag200Data *channel = nullptr;
  QMap<int, Mag200Data*> data = qSettings->Mag200Datas();
  if (data.contains(ChannelID) == true)
    channel = data[ChannelID];

  switch (Type)
  {
    default:                                                              return;

    // NDA SDK STATUS
    case NDA_ACQ:        NewStatus_NDA_ACQ(Value.toBool());               return;
    case NDA_STK:        NewStatus_NDA_STK(Value.toBool());               return;
    case NDA_TIME:       NewStatus_NDA_TIME(Value.toDouble());            return;
    case NDA_TIME_STK:   NewStatus_NDA_TIME(Value.toDouble());            return;

    // SHARED MEMORY STATUS
    case STAT_CH10_CPT:                                                   return;
    case STAT_CH10_ETH:  NewStatus_CH10_ETH(channel, Value.toInt());      return;
    case STAT_CH10_STK:  NewStatus_CH10_STK(channel, Value.toInt());      return;
    case STAT_T:         NewStatus_TIME(channel, Value.toInt());          return;
  }
}


void DPS_Toolbar::NewStatus_NDA_ACQ(bool Value)
{
  Mag200Status status = Value == true ? Mag200Status::LOCK : Mag200Status::UNLOCK;

  // Show appropriate message based on the state
  if (Value == true)
    qMessages->NewInfo(tr("STATUS_RUN"));
  else
    qMessages->NewInfo(tr("STATUS_READY"));

  // Update the state of the ACQ LED
  Leds[ACQ]->SetState(status);
}

void DPS_Toolbar::NewStatus_NDA_STK(bool Value)
{
  Mag200Status status = Mag200Status::UNLOCK;
  Color customColor = Color::NO_COLOR;

  if (Value)
  {
    status = Mag200Status::LOCK;
    customColor = StoragesManager::AvailableSpaceColor(qSettings->App_Storage());
  }

  Leds[STK]->SetState(status, customColor);
}

void DPS_Toolbar::NewStatus_NDA_TIME(double Value)
{
  QString strDate = Tools::DateToString(Value);
  Leds[TIME]->setText(" " + strDate);
}

void DPS_Toolbar::NewStatus_CH10_ETH(Mag200Data *channel, int Value)
{
  if (channel->IsCH10() == false)
    return;

  Mag200Status status = static_cast<Mag200Status>(Value);
  Leds[CH10_ETH]->SetState(status);
}

void DPS_Toolbar::NewStatus_CH10_STK(Mag200Data *channel, int Value)
{
  if (channel->IsCH10() == false)
    return;

  Mag200Status status = static_cast<Mag200Status>(Value);
  Leds[CH10_STK]->SetState(status);
}

void DPS_Toolbar::NewStatus_TIME(Mag200Data *channel, int Value)
{
  int ChannelID = channel->ChannelID();
  Times[ChannelID] = Value;

  QList<int> values = Times.values();
  values.removeAll(Mag200Status::LOCK);
  qSort(values);

  Mag200Status status;
  if (values.size() == 0)
    status = Mag200Status::LOCK;
  else
    status = static_cast<Mag200Status>(values.first());

  if (status == Mag200Status::UNLOCK)
    qMessages->NewWarning(tr("STATUS_WAIT"));

  Leds[TIME]->SetState(status);
}

