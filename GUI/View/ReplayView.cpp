#include "stdafx.h"
#include "ReplayView.h"

ReplayView::ReplayView(QWidget *parent) : PageView(parent)
{
  this->m_StartDate = QDateTime(QDate(2023, 1, 1), QTime(0, 0, 0), Qt::UTC);
  this->m_EndDate = QDateTime(QDate(2023, 12, 31), QTime(23, 59, 59), Qt::UTC);

  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();
}

ReplayView::~ReplayView()
{

}

QDateTime ReplayView::Date()
{
  return this->replayBar->Date();
}

/*
* @brief Sets up the user interface for ReplayView.
*
* Sets the header name to "PAGE_REPLAY".
*
* @return void
*
* @date 31/05/2023
*/
void ReplayView::SetupUI()
{
  this->UseLine(true);

  QVBoxLayout *layout = new QVBoxLayout();
  layout->setMargin(0);
  layout->setSpacing(0);

  time = new QLabel(this->DateToString(m_StartDate));
  time->setAlignment(Qt::AlignCenter);

  replayBar = new ReplayBar();
  
  layout->addWidget(time);
  layout->addWidget(replayBar, 1);
  
  Leds[STOP] = new Led("stop");
  Leds[PLAY] = new Led("play");
  Leds[LOOP] = new Led("loop");
  Leds[SEARCH] = new Led("search");
  Leds[RANGE] = new Led("range");

  Leds[STOP]->SetIcon(qThemes->Icons("STOP_0.png"), false);
  Leds[PLAY]->SetIcon(qThemes->Icons("PLAY_0.png"), false);
  Leds[LOOP]->SetIcon(qThemes->Icons("LOOP_0.png"), true);
  Leds[SEARCH]->SetIcon(qThemes->Icons("DATE_PICKER_0.png"), false);
  Leds[RANGE]->SetIcon(qThemes->Icons("RANGE_PICKER_0.png"), false);
 
  // Set global properties
  QHBoxLayout *ledLayout = new QHBoxLayout();
  ledLayout->setSpacing(0);
  ledLayout->setMargin(0);

  for (Led* led : Leds)
  {
    led->SetState(Mag200Status::GRAY);
    led->UseHasButton(true);
    led->UseBackground(true);
    ledLayout->addWidget(led);
  }

  Leds[LOOP]->setCheckable(true);

  layout->addLayout(ledLayout);

  QWidget *w = new QWidget();
  w->setLayout(layout);
  this->AddMainWidget(w);

  replayBar->SetDateRange(m_StartDate, m_EndDate);

  QObject::connect(replayBar, &ReplayBar::SendNewValue, this, &ReplayView::NewValue);
}

/*
* @brief Sets up event connections for Chapter10View.
*
* @return void
*
* @date 31/05/2023
*/
void ReplayView::SetupEvent()
{
  for (Led* led : Leds)
    QObject::connect(led, &Led::clicked, this, &ReplayView::LedClicked);

  // Chapter 6
  QObject::connect(qCommands, &CommandsManager::SendPlayAcknowledge, this, &ReplayView::NewPlayAcknowledge);
  QObject::connect(qCommands, &CommandsManager::SendStopAcknowledge, this, &ReplayView::NewStopAcknowledge);
  QObject::connect(qCommands, &CommandsManager::SendNewFileDate, this, &ReplayView::NewFilesAcknowledge);
  QObject::connect(qCommands, &CommandsManager::SendNewPercentage, this, &ReplayView::NewPercentage);
  
}

void ReplayView::LedClicked()
{
  int index = LedIndex();

  switch (index)
  {
    default:                                                  return;
    case STOP:      qCommands->NewCommand(CMD_STOP);          return;
    case PLAY:      qCommands->NewCommand(CMD_PLAY);          return;

    case LOOP:                                                return;
    case SEARCH:    emit SendLedClicked(PAGE_DATE_PICKER);    return;
    case RANGE:     emit SendLedClicked(PAGE_RANGE_PICKER);   return;
  }
}

void ReplayView::resizeEvent(QResizeEvent * event)
{
  PageView::resizeEvent(event);

  QSize size = event->size();

  int headerSize = this->HeaderSize().height();
  int height = Tools::GetWidgetHeight(this, size);

  if (height == headerSize)
    return;

  height -= headerSize;

  qreal ratio = 1.0;

  qreal timeRatio = 0.20;
  qreal buttonRatio = 0.25;
  qreal replayBarRatio = 0.5;

  // Calculate widgets height
  int availableHeight = height;
  int timeHeight = Tools::GetWidgetHeight(timeRatio, height, availableHeight, ratio);
  int buttonHeight = Tools::GetWidgetHeight(buttonRatio, height, availableHeight, ratio);
  int replayBarHeight = Tools::GetWidgetHeight(replayBarRatio, height, availableHeight, ratio);

  for (int i = 0; i < Leds.size(); i++)
  {
    Leds[i]->SetSize(QSize(buttonHeight, buttonHeight));
  }

  QFont font = this->font();
  font.setBold(true);

  int fontsize = Tools::AutoFontSize(size.width(), timeHeight, m_Dateformat, font);
  font.setPixelSize(fontsize);
  time->setFont(font);

  time->setFixedHeight(timeHeight);
  replayBar->setFixedHeight(replayBarHeight);
}

QDateTime ReplayView::StartDate()
{
  return m_StartDate;
}

QDateTime ReplayView::EndDate()
{
  return m_EndDate;
}

QDateTime ReplayView::LowerBoundDate()
{
  return replayBar->LowerBoundDate();
}

QDateTime ReplayView::UpperBoundDate()
{
  return replayBar->UpperBoundDate();
}

void ReplayView::NewPercentage(int percentage)
{
  qint64 timeSpan = this->m_EndDate.toMSecsSinceEpoch() - this->m_StartDate.toMSecsSinceEpoch();

  qint64 offset = (timeSpan * percentage) / 100;
  QDateTime newDate = this->m_StartDate.addMSecs(offset);

  this->replayBar->SetDate(newDate);
}

void ReplayView::NewPlayAcknowledge(int error)
{
  if (Leds[PLAY]->IsChecked() == true)
    return;

  if (error == -1)
  {
    Leds[PLAY]->Toggled(false);
    return;
  }

  // Start play
  Leds[PLAY]->Toggled(true);
}

void ReplayView::NewStopAcknowledge(int error)
{
  if (error == -1)
    return;

  // End play
  Leds[PLAY]->Toggled(false);
}

void ReplayView::NewFilesAcknowledge(QDateTime fileStart, QDateTime fileEnd)
{
  this->m_StartDate = fileStart;
  this->m_EndDate = fileEnd;

  replayBar->SetDateRange(m_StartDate, m_EndDate);

  this->SetBoundRange(this->m_StartDate, this->m_EndDate);
  this->SetDate(this->m_StartDate);
}

void ReplayView::SetBoundRange(QDateTime lowerBoundDate, QDateTime upperBoundDate)
{
  replayBar->SetBoundRange(lowerBoundDate, upperBoundDate);
}

void ReplayView::SetDate(QDateTime date)
{
  replayBar->SetDate(date);
}

QString ReplayView::DateToString(QDateTime date)
{
  QString dayOfYear = QString("%1").arg(date.date().dayOfYear(), 3, 10, QChar('0'));
  QString format = date.toString(m_Dateformat);

  return dayOfYear + " " + format;
}

void ReplayView::NewValue(qint64 msecs)
{
  QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(msecs);
  time->setText(this->DateToString(dateTime));
}

void ReplayView::LedSearchClicked() { emit this->SendLedClicked(PAGE_DATE_PICKER);  }
void ReplayView::LedRangeClicked()  { emit this->SendLedClicked(PAGE_RANGE_PICKER); }
