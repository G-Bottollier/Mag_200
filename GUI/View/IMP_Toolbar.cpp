#include "stdafx.h"
#include "IMP_Toolbar.h"

IMP_Toolbar::IMP_Toolbar(QWidget *parent) : Toolbar(parent)
{
  SetupUI();
  SetupEvent();
}

IMP_Toolbar::~IMP_Toolbar()
{

}

void IMP_Toolbar::SetupUI()
{
  Leds[PARENT_DIR] = new Led("PARENT_DIR");
  Leds[PARENT_DIR]->UseHasButton(true);
  Leds[PARENT_DIR]->SetState(Mag200Status::GRAY);
  Leds[PARENT_DIR]->SetIcon(qThemes->Icons("BACK_0.png"), true);

  labelFolder = new QLabel("Current");
  labelFolder->setAlignment(Qt::AlignCenter);

  Leds[HOME_MENU] = new Led("MENU");
  Leds[HOME_MENU]->SetState(Mag200Status::GRAY, Color::IMPORT_01);
  Leds[HOME_MENU]->SetIcon(qThemes->Icons("MENU_IMPORT_0.png"), true);

  // Add widget to the view
  this->MainLayout->addWidget(Leds[PARENT_DIR]);
  this->MainLayout->addWidget(labelFolder, 1);
  this->MainLayout->addWidget(Leds[HOME_MENU]);
}

void IMP_Toolbar::SetupEvent()
{
  for (Led* led : Leds)
    QObject::connect(led, &Led::clicked, this, &IMP_Toolbar::LedClicked);
}

void IMP_Toolbar::setFolderName(QString name)
{
  labelFolder->setText(name);
}

void IMP_Toolbar::LedClicked()
{
  int index = LedIndex();

  switch (index)
  {
    default:                                                    return;
    case HOME_MENU:       emit SendLedClicked(PAGE_MENU);       return;
    case PARENT_DIR:      emit SendOpenParentFolder();          return;
  }
}