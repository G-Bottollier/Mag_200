#include "stdafx.h"
#include "MenuView.h"

MenuView::MenuView(QWidget *parent) : PageView(parent)
{
  SetupUI();
  SetupEvent();
}

MenuView::~MenuView()
{

}

void MenuView::SetupUI()
{
  this->UseFullPage(true);

  QWidget *MainWidget = new QWidget;

  int vSpacing = spacing;
  QMargins margins(spacing, vSpacing, spacing, vSpacing);

  MainLayout = new QVBoxLayout();
  MainLayout->setSpacing(vSpacing);
  MainLayout->setContentsMargins(margins);

  QHBoxLayout *ImportExportLayout = new QHBoxLayout();
  ImportExportLayout->setSpacing(spacing);
  ImportExportLayout->setMargin(0);

  Leds[MENU_IMPORT] = new Led("IMPORT");
  Leds[MENU_EXPORT] = new Led("EXPORT");

  Leds[MENU_IMPORT]->SetState(Mag200Status::GRAY, Color::IMPORT_01);
  Leds[MENU_EXPORT]->SetState(Mag200Status::GRAY, Color::EXPORT_01);

  QHBoxLayout *RecordReplayLayout = new QHBoxLayout();
  RecordReplayLayout->setSpacing(spacing);
  RecordReplayLayout->setMargin(0);

  Leds[MENU_RECORD] = new Led("RECORD");
  Leds[MENU_REPLAY] = new Led("REPLAY");

  Leds[MENU_RECORD]->SetState(Mag200Status::GRAY, Color::RECORD_01);
  Leds[MENU_REPLAY]->SetState(Mag200Status::GRAY, Color::REPLAY_01);

  for (Led* led : Leds)
  {
    led->UseHasButton(true);
    led->UseAutoSizeFont(true);
  }

  ImportExportLayout->addWidget(Leds[MENU_IMPORT]);
  ImportExportLayout->addWidget(Leds[MENU_EXPORT]);

  RecordReplayLayout->addWidget(Leds[MENU_RECORD]);
  RecordReplayLayout->addWidget(Leds[MENU_REPLAY]);

  MainLayout->addLayout(ImportExportLayout);
  MainLayout->addLayout(RecordReplayLayout);

  MainWidget->setLayout(MainLayout);

  this->AddMainWidget(MainWidget);
}

void MenuView::SetupEvent()
{
  for (Led* led : Leds)
    QObject::connect(led, &Led::clicked, this, &MenuView::LedClicked);
}

void MenuView::LedClicked()
{
  int index = LedIndex();

  switch (index)
  {
    default:                                                   return;
    case MENU_IMPORT:     emit SendLedClicked(PAGE_IMPORT);    return;
    case MENU_EXPORT:     emit SendLedClicked(PAGE_EXPORT);    return;
    case MENU_RECORD:     emit SendLedClicked(PAGE_RECORD);    return;
    case MENU_REPLAY:     emit SendLedClicked(PAGE_REPLAY);    return;
  }
}

void MenuView::resizeEvent(QResizeEvent * event)
{
  QSize size = event->size();
  
  int width = Tools::GetWidgetWidth(this, size, MainLayout);
  int height = Tools::GetWidgetHeight(this, size, MainLayout);

  width -= spacing;

  QSize ledSize = QSize(width * 0.5, height * 0.5);

  for (Led* led : Leds)
    led->SetSize(ledSize);
}