#include "stdafx.h"
#include "EXP_Toolbar.h"

EXP_Toolbar::EXP_Toolbar(QWidget *parent) : Toolbar(parent)
{
  SetupUI();
  SetupEvent();
}

EXP_Toolbar::~EXP_Toolbar()
{

}

void EXP_Toolbar::SetupUI()
{
  Leds[HOME_MENU] = new Led("MENU");
  Leds[HOME_MENU]->SetState(Mag200Status::GRAY, Color::EXPORT_01);
  Leds[HOME_MENU]->SetIcon(qThemes->Icons("MENU_EXPORT_0.png"), true);

  // Add widget to the view
  this->MainLayout->addWidget(Leds[HOME_MENU]);
}

void EXP_Toolbar::SetupEvent()
{
  QObject::connect(Leds[HOME_MENU], &Led::clicked, this, &EXP_Toolbar::LedClicked);
}

void EXP_Toolbar::LedClicked()
{
  int index = LedIndex();

  switch (index)
  {
    default:                                                    return;
    case HOME_MENU:       emit SendLedClicked(PAGE_MENU);       return;
  }
}