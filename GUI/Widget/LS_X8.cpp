#include "stdafx.h"
#include "LS_X8.h"

LS_X8::LS_X8(Mag200Data *board, QWidget *parent) : ChannelItem(board, parent)
{
  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();
}

LS_X8::~LS_X8()
{

}

void LS_X8::SetupUI()
{
  QHBoxLayout *MainLayout = static_cast<QHBoxLayout*>(this->layout());

  // Create the LEDs for displaying the input state
  this->Leds[RF] = new Led(tr("RF"));
  this->Leds[BS] = new Led(tr("BS"));
  this->Leds[FS] = new Led(tr("FS"));
  this->Leds[ID] = new Led(tr("ID"));

  for (Led *led : Leds)
  {
    led->UseBorder(false);
    led->UseBackground(true);
    MainLayout->addWidget(led);
  }

  MainLayout->addStretch(0);
}

void LS_X8::SetupEvent()
{
  for (Led *led : Leds)
    QObject::connect(led, &Led::clicked, this, &LS_X8::LedClicked);
}

double LS_X8::Strength()
{
  return this->m_Strength;
}

double LS_X8::Deviation()
{
  return this->m_Deviation;
}

void LS_X8::NewStatus(int ChannelID, int Type, QVariant Value)
{
  ChannelItem::NewStatus(ChannelID, Type, Value);

  if (ChannelID != this->ChannelID())
    return;

  switch (Type)
  {
    default: 
      return;

    case STAT_R1_S : 
      this->m_Strength = Value.toDouble();
      emit SendNewSignalStrength(this->m_Strength);
      break;

    case STAT_R1_D :
      this->m_Deviation = Value.toDouble();
      emit SendNewSignalDeviation(this->m_Deviation);
      break;
  }
}
