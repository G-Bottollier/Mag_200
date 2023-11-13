#include "stdafx.h"
#include "REP_Toolbar.h"

REP_Toolbar::REP_Toolbar(QWidget *parent) : Toolbar(parent)
{
  SetupUI();
  SetupEvent();
}

REP_Toolbar::~REP_Toolbar()
{

}

void REP_Toolbar::SetupUI()
{
  // Create Status LEDs
  Leds[REP] = new Led("REP");
  Leds[REP]->SetIcon(qThemes->Icons("REP_0.png"), true);

  // Find the width of the Time LED
  QString timeType = this->TimeType();
  QStringList letters = timeType.split("", QString::SkipEmptyParts);

  // Create time LED
  Leds[REPLAY_TIME] = new Led("TIME");
  Leds[REPLAY_TIME]->SetInfos(letters);
  Leds[REPLAY_TIME]->SetAlignement(Qt::AlignLeft | Qt::AlignVCenter);

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

  Leds[HOME_MENU] = new Led("MENU");
  Leds[HOME_MENU]->SetState(Mag200Status::GRAY, Color::REPLAY_01);
  Leds[HOME_MENU]->SetIcon(qThemes->Icons("MENU_REPLAY_0.png"), true);

  // Add widget to the view
  this->MainLayout->addWidget(Leds[REP]);
  this->MainLayout->addWidget(Leds[REPLAY_TIME]);

  if (this->HasRF() == true)
  {
    this->MainLayout->addWidget(Leds[CH10_STK]);
    this->MainLayout->addWidget(Leds[CH10_ETH]);
  }
  else
    this->MainLayout->addWidget(Leds[STK]);

  this->MainLayout->addWidget(Leds[HOME_MENU]);
}

void REP_Toolbar::SetupEvent()
{
  for (Led* led : Leds)
    QObject::connect(led, &Led::clicked, this, &REP_Toolbar::LedClicked);
}

void REP_Toolbar::LedClicked()
{
  int index = LedIndex();

  switch (index)
  {
    default:                                                    return;
    case REP:             emit /* SendLedClicked(PAGE_); */     return;
    case REPLAY_TIME:     emit SendLedClicked(PAGE_CHANNELS);   return;
    case STK:             emit SendLedClicked(PAGE_STORAGES);        return;
    case CH10_STK:        emit SendLedClicked(PAGE_STORAGES);        return;
    case CH10_ETH:        emit SendLedClicked(PAGE_CH10);       return;
    case HOME_MENU:       emit SendLedClicked(PAGE_MENU);       return;
  }
}