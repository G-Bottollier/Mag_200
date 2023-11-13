#include "stdafx.h"
#include "DatePicker.h"


#define YEAR              0
#define MONTH             1
#define DAY               2
#define HOUR              3
#define MINUTE            4
#define SECOND            5
#define ALL               0 // For readability

// Keep same order
#define MIN_ICON          0
#define MAX_ICON          1
#define MIN_VALUE         2
#define MAX_VALUE         3
#define SPLIT_DATE_1      4
#define SPLIT_DATE_2      5
#define SPLIT_DATE_TIME   6
#define SPLIT_TIME_1      7
#define SPLIT_TIME_2      8

#define SPLIT_S           SPLIT_DATE_1    // START
#define SPLIT_E           SPLIT_TIME_2    // END

DatePicker::DatePicker(int DateType, QWidget *parent) : QFrame(parent)
{
  this->m_DateType = DateType;
  SetupFormat();

  SetupUI();

  SetupEvent();

  QDateTime minDate(QDate(2000, 1, 1), QTime(0, 0, 0));
  QDateTime maxDate(QDate(2050, 12, 31), QTime(23, 59, 59));

  SetDateRange(minDate, maxDate);
}

DatePicker::~DatePicker()
{

}

void DatePicker::SetupUI()
{
  this->setFrameShape(QFrame::WinPanel);
  this->setFrameShadow(QFrame::Plain);
  this->setMidLineWidth(3);
  this->setLineWidth(3);

  QVBoxLayout *MainLayout = new QVBoxLayout();
  MainLayout->setSpacing(0);
  MainLayout->setMargin(0);

  m_DateBar = new QTabBar();
  m_DateBar->addTab(QString());
  m_DateBar->addTab(QString());
  m_DateBar->setCurrentIndex(0);

  m_DateBar->setTabIcon(0, QIcon(qThemes->Icons("SDATE_0.png")));
  m_DateBar->setTabIcon(1, QIcon(qThemes->Icons("EDATE_0.png")));

  QFrame* line = new QFrame();
  line->setLineWidth(3);
  line->setMidLineWidth(3);
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Plain);

  QHBoxLayout *datePickerLayout = new QHBoxLayout();
  datePickerLayout->setSpacing(0);
  datePickerLayout->setMargin(0);

  m_Wheels[YEAR]            = new Wheel(1970, 2050);
  m_Wheels[MONTH]           = new Wheel(1, 12);
  m_Wheels[DAY]             = new Wheel(1, 31);
  m_Wheels[HOUR]            = new Wheel(0, 23);
  m_Wheels[MINUTE]          = new Wheel(0, 59);
  m_Wheels[SECOND]          = new Wheel(0, 59);

  m_Labels[SPLIT_DATE_1]    = new QLabel(m_DateSplit[SPLIT_DATE_1]);
  m_Labels[SPLIT_DATE_2]    = new QLabel(m_DateSplit[SPLIT_DATE_2]);
  m_Labels[SPLIT_DATE_TIME] = new QLabel(m_DateSplit[SPLIT_DATE_TIME]);
  m_Labels[SPLIT_TIME_1]    = new QLabel(m_DateSplit[SPLIT_TIME_1]);
  m_Labels[SPLIT_TIME_2]    = new QLabel(m_DateSplit[SPLIT_TIME_2]);

  for (int index : m_WheelOrder)
    datePickerLayout->addWidget(m_Wheels[index]);

  int m_MaxValue = SPLIT_E - SPLIT_S;
  for (int i = m_MaxValue; i >= 0 ; i--)
  {
    QLabel *label = m_Labels[SPLIT_S + i];
    label->setAlignment(Qt::AlignCenter);
    datePickerLayout->insertWidget(i + 1, label);
  }

  MainLayout->addWidget(m_DateBar);
  MainLayout->addWidget(line);
  MainLayout->addLayout(datePickerLayout);

  this->setLayout(MainLayout);
}

void DatePicker::SetupEvent()
{
  for (Wheel *wheel : m_Wheels)
    QObject::connect(wheel, &Wheel::SendNewValue, this, &DatePicker::NewValue);

  QObject::connect(m_DateBar, &QTabBar::currentChanged, this, &DatePicker::CurrentChanged);

  if(m_BlinkTimer == nullptr)
    m_BlinkTimer = new QTimer();
  m_BlinkTimer->setInterval(BLINK_INTERVAL);
  QObject::connect(m_BlinkTimer, &QTimer::timeout, this, &DatePicker::Blink);
}

void DatePicker::SetupFormat()
{
  m_DateSplit[SPLIT_DATE_1] = "/";
  m_DateSplit[SPLIT_DATE_2] = "/";
  m_DateSplit[SPLIT_DATE_TIME] = "-";
  m_DateSplit[SPLIT_TIME_1] = ":";
  m_DateSplit[SPLIT_TIME_2] = ":";

  // QDateTime default order
  QStringList QtOrder = { "yyyy", "MM", "dd", "HH", "mm", "ss"};

  switch (this->m_DateType)
  {
    default:
    case FORMAT_DMY: m_WheelOrder.append({   DAY,     MONTH,   YEAR    });   break;
    case FORMAT_YMD: m_WheelOrder.append({   YEAR,    MONTH,   DAY     });   break;
    case FORMAT_MDY: m_WheelOrder.append({   MONTH,   DAY,     YEAR    });   break;
    case FORMAT_YDM: m_WheelOrder.append({   YEAR,    DAY,     MONTH   });   break;
  }
  m_WheelOrder.append({ HOUR, MINUTE, SECOND });

  QStringList format;
  for (int index : m_WheelOrder)
    format.append(QtOrder[index]);

  this->m_DateFormat = format.takeFirst();
  for (int i = SPLIT_S; i <= SPLIT_E; i++)
  {
    this->m_DateFormat += m_DateSplit[i] + format.takeFirst();
  }
}

void DatePicker::resizeEvent(QResizeEvent *event)
{
  QFrame::resizeEvent(event);

  if (m_DateBar == nullptr)
    return;

  QSize size = event->size();

  int width = Tools::GetWidgetWidth(this, size);
  int height = Tools::GetWidgetHeight(this, size);

  int tabBarWidth = width;
  int tabBarHeight = height * 0.25;

  m_DateBar->setFixedWidth(tabBarWidth);

  int tabCount = qMax(1, this->m_DateBar->count());
  int tabWidth = qFloor(tabBarWidth / tabCount);

  QColor highlight = this->m_UseRange == true ? qThemes->Color(QPalette::Highlight) : qThemes->Color(QPalette::Window);
  QString style = "background: rgb(%1, %2, %3, %4);";

  m_DateBar->setStyleSheet("QTabBar::tab            { width: " + QString::number(tabWidth) + "px; height: " + QString::number(tabBarHeight) + "px; } "
    + "QTabBar::tab:selected   { " + style.arg(highlight.red()).arg(highlight.green()).arg(highlight.blue()).arg(100) + " } ");

  QFont font = this->font();
  font.setBold(true);

  int iconTabWidth = tabBarWidth * 0.2;

  if (iconTabWidth > (tabBarHeight * 0.6))
    iconTabWidth = tabBarHeight * 0.6;

  // Uniform font for both type Picker & Range Date
  tabWidth = qFloor(tabBarWidth / 2);

  int availableTabWidth = tabWidth;
  qreal tabRatio = 1.0;

  int maxTab = 2;
  tabRatio -= iconTabWidth / (double)maxTab;
  availableTabWidth -= iconTabWidth;

  int labelTabFontsize = Tools::AutoFontSize(availableTabWidth, tabBarHeight, FORMAT_DATE, font);

  font.setPixelSize(labelTabFontsize);
  QFontMetrics tabMetrics(font);

  this->m_DateBar->setFont(font);
  this->m_DateBar->setIconSize(QSize(iconTabWidth, iconTabWidth));

  font.setPixelSize(labelTabFontsize * 1.4);

  for (Wheel *wheel : m_Wheels)
    wheel->setFont(font);

  for (QLabel *label : m_Labels)
    label->setFont(font);
}

void DatePicker::paintEvent(QPaintEvent * event)
{
  QFrame::paintEvent(event);

  int index = m_WheelOrder.first();

  if (m_Wheels[index] == nullptr)
    return;

  QPoint datePos = m_Wheels[index]->geometry().topLeft();
  QSize dateSize = QSize(this->width(), m_Wheels[index]->geometry().height());

  QRect HighlightRect(0, 0, this->width(), m_Wheels[index]->ItemHeight());
  QRect DateRect(datePos, dateSize);

  QPoint CenterOffset = DateRect.center() - HighlightRect.center();

  QColor Highlight = m_Wheels[index]->ItemBackground();
  if (m_DateTimes[UPPER_BOUND_DATE] <= m_DateTimes[LOWER_BOUND_DATE])
    Highlight = QColor(Color::RED_01);

  if (this->m_Blink == true)
    Highlight.setAlpha(50);

  QRect CenteredRect = HighlightRect.translated(CenterOffset);

  // DRAWING SELECTION
  QPainter painter(this);                 // Create the painter
  painter.setClipRect(CenteredRect);      // Set the region to limit
  painter.setClipping(true);              // Limit the drawing region
  painter.setPen(Qt::NoPen);              // No border
  painter.setBrush(Highlight);            // Set the fill color
  painter.drawRect(CenteredRect);         // Draw the selection rect

  // UPDATE BACKGROUND
  for (int i = SPLIT_S; i <= SPLIT_E; i++)
    m_Labels[i]->update();

  for (Wheel *wheel : m_Wheels)
    wheel->update();
}

void DatePicker::UpdateDate()
{
  int index = DatePickerType();
  m_DateTimes[index].setDate(QDate(m_Values[YEAR], m_Values[MONTH], m_Values[DAY]));
  UpdateText();
}

void DatePicker::UpdateTime()
{
  int index = DatePickerType();

  m_DateTimes[index].setTime(QTime(m_Values[HOUR], m_Values[MINUTE], m_Values[SECOND]));
  UpdateText();
}

void DatePicker::UpdateText()
{
  int index = DatePickerType();
  int tabIndex = this->m_UseRange ? index - LOWER_BOUND_DATE : START_DATE;

  m_DateBar->setTabText(tabIndex, m_DateTimes[index].toString(this->m_DateFormat));
}

void DatePicker::UpdateRange(int type, QDateTime &startTime, QDateTime &endTime, bool SetValue)
{
  int m_MinValue = 0;
  int m_MaxValue = 0;

  QDate date = QDate(m_Values[YEAR], m_Values[MONTH], m_Values[DAY]);

  switch (type)
  {
    case YEAR:
      m_MinValue = startTime.date().year();
      m_MaxValue = endTime.date().year();
      break;

    case MONTH:
      m_MinValue = (m_Values[YEAR] == startTime.date().year()) ? startTime.date().month() : 1;
      m_MaxValue = (m_Values[YEAR] == endTime.date().year()) ? endTime.date().month() : 12;
      break;

    case DAY:
      m_MinValue = (m_Values[YEAR] == startTime.date().year() && m_Values[MONTH] == startTime.date().month()) ? startTime.date().day() : 1;
      m_MaxValue = (m_Values[YEAR] == endTime.date().year() && m_Values[MONTH] == endTime.date().month()) ? endTime.date().day() : QDate(m_Values[YEAR], m_Values[MONTH], 1).daysInMonth();
      break;

    case HOUR:
      m_MinValue = (date == startTime.date()) ? startTime.time().hour() : 0;
      m_MaxValue = (date == endTime.date()) ? endTime.time().hour() : 23;
      break;

    case MINUTE:
      m_MinValue = (date == startTime.date() && m_Values[HOUR] == startTime.time().hour()) ? startTime.time().minute() : 0;
      m_MaxValue = (date == endTime.date() && m_Values[HOUR] == endTime.time().hour()) ? endTime.time().minute() : 59;
      break;

    case SECOND:
      m_MinValue = (date == startTime.date() && m_Values[HOUR] == startTime.time().hour() && m_Values[MINUTE] == startTime.time().minute()) ? startTime.time().second() : 0;
      m_MaxValue = (date == endTime.date() && m_Values[HOUR] == endTime.time().hour() && m_Values[MINUTE] == endTime.time().minute()) ? endTime.time().second() : 59;
      break;
  }

  m_Wheels[type]->SetRange(m_MinValue, m_MaxValue, SetValue);

  if (SetValue == true)
    m_Wheels[type]->SetValue(ValueToString(m_Values[type]));
}

void DatePicker::SetBoundRange(QDateTime minSeek, QDateTime maxSeek)
{
  m_DateTimes[LOWER_BOUND_DATE] = QDateTime(minSeek);
  m_DateTimes[UPPER_BOUND_DATE] = QDateTime(maxSeek);

  int index = DatePickerType();
  this->SetDate(m_DateTimes[index]);

  m_DateBar->setTabText(START_DATE, m_DateTimes[LOWER_BOUND_DATE].toString(this->m_DateFormat));
  m_DateBar->setTabText(END_DATE, m_DateTimes[UPPER_BOUND_DATE].toString(this->m_DateFormat));
}

void DatePicker::SetDateRange(QDateTime startTime, QDateTime endTime)
{
  if (endTime <= startTime)
    return;

  if (startTime == m_DateTimes[START_DATE] && endTime == m_DateTimes[END_DATE])
    return;

  int msec = startTime.time().msec();
  int span = 1000 - msec;
  startTime = startTime.addMSecs(span);

  m_DateTimes[START_DATE] = QDateTime(startTime);
  m_DateTimes[END_DATE] = QDateTime(endTime);

  m_DateTimes[LOWER_BOUND_DATE] = QDateTime(startTime);
  m_DateTimes[UPPER_BOUND_DATE] = QDateTime(endTime);
  m_DateTimes[CURRENT_DATE] = QDateTime(startTime);

  int index = DatePickerType();
  this->SetDate(m_DateTimes[index]);

  if (this->m_UseRange == true)
  {
    m_DateBar->setTabText(START_DATE, m_DateTimes[START_DATE].toString(this->m_DateFormat));
    m_DateBar->setTabText(END_DATE, m_DateTimes[END_DATE].toString(this->m_DateFormat));
  }
  else
    m_DateBar->setTabText(START_DATE, m_DateTimes[START_DATE].toString(this->m_DateFormat));
}

void DatePicker::SetDate(QDateTime date)
{
  for (Wheel *wheel : m_Wheels)
    wheel->UseAnimation(false);

  int index = DatePickerType();

  m_DateTimes[index] = QDateTime(date);

  if (m_DateTimes[index] < m_DateTimes[START_DATE])
    m_DateTimes[index] = m_DateTimes[START_DATE];

  if (m_DateTimes[index] > m_DateTimes[END_DATE])
    m_DateTimes[index] = m_DateTimes[END_DATE];

  m_Values[DAY]     = m_DateTimes[index].date().day();
  m_Values[MONTH]   = m_DateTimes[index].date().month();
  m_Values[YEAR]    = m_DateTimes[index].date().year();
  m_Values[HOUR]    = m_DateTimes[index].time().hour();
  m_Values[MINUTE]  = m_DateTimes[index].time().minute();
  m_Values[SECOND]  = m_DateTimes[index].time().second();

  for(Wheel *wheel : m_Wheels)
    wheel->blockSignals(true);

  // Update range in certain order to prevent overshooting
  for (int i = YEAR; i <= SECOND; i++)
  {
    if (m_DateTimes[index] == m_DateTimes[START_DATE])
      UpdateRange(SECOND - i, m_DateTimes[START_DATE], m_DateTimes[END_DATE], true);
    else
      UpdateRange(i, m_DateTimes[START_DATE], m_DateTimes[END_DATE], true);
  }

  for (Wheel *wheel : m_Wheels)
    wheel->blockSignals(false);

  UpdateDate();
  UpdateTime();

  for (Wheel *wheel : m_Wheels)
    wheel->UseAnimation(true);
}

QString DatePicker::ToString(int type)
{
  if(type != -1)
    return m_DateTimes[type].toString(this->m_DateFormat);

  int index = DatePickerType();
  return m_DateTimes[index].toString(this->m_DateFormat);
}

QDateTime DatePicker::ToDateTime(int type)
{
  switch (type)
  {
    default: 
      return QDateTime();

    case LOWER_BOUND_DATE:
    case UPPER_BOUND_DATE:
    case CURRENT_DATE:
      break;
  }

  return this->m_DateTimes[type];
}

qint64 DatePicker::ToSecsSinceEpoch(int type)
{
  return ToDateTime(type).toMSecsSinceEpoch();
}

bool DatePicker::IsValid()
{
  if (this->m_UseRange == true)
    return this->m_DateTimes[LOWER_BOUND_DATE] < this->m_DateTimes[UPPER_BOUND_DATE];
  else
    return this->m_DateTimes[CURRENT_DATE] >= this->m_DateTimes[START_DATE] && this->m_DateTimes[CURRENT_DATE] <= this->m_DateTimes[END_DATE];
}

void DatePicker::DoBlinkAnimation()
{
  this->m_Blink = false;
  this->m_BlinkCounter = 0;
  this->m_BlinkTimer->start();
}

void DatePicker::SetRangeMode(bool state)
{
  if (this->m_UseRange == state)
    return;

  for (int i = m_DateBar->count(); i > 0; i--)
    m_DateBar->removeTab(i - 1);

  if (state == true)
  {
    m_DateBar->addTab(QString());
    m_DateBar->addTab(QString());

    m_DateBar->setTabIcon(0, QIcon(qThemes->Icons("SDATE_0.png")));
    m_DateBar->setTabIcon(1, QIcon(qThemes->Icons("EDATE_0.png")));
  }
  else
  {
    m_DateBar->addTab(QString());
  }

  m_DateBar->setCurrentIndex(0);
  this->m_UseRange = state;
}

int DatePicker::DatePickerType()
{
  if(this->m_UseRange == false)
    return CURRENT_DATE;

  if (m_DateBar->currentIndex() == START_DATE)
    return LOWER_BOUND_DATE;

  if(m_DateBar->currentIndex() == END_DATE)
    return UPPER_BOUND_DATE;

  return CURRENT_DATE;
}

QString DatePicker::ValueToString(int value)
{
  return QString("%1").arg(value, 2, 10, QChar('0'));
}

void DatePicker::CurrentChanged(int index)
{
  if (index == START_DATE)
    this->SetDate(this->m_DateTimes[LOWER_BOUND_DATE]);

  if (index == END_DATE)
    this->SetDate(this->m_DateTimes[UPPER_BOUND_DATE]);

  UpdateDate();
  UpdateTime();
}

void DatePicker::Blink()
{
  this->m_BlinkCounter++;

  this->m_Blink = !this->m_Blink;

  this->update();

  int timeElapsed = this->m_BlinkCounter * this->m_BlinkTimer->interval();

  if (timeElapsed < BLINK_TIME)
    return;

  this->m_BlinkTimer->stop();
  this->m_BlinkCounter = 0;
  this->m_Blink = false;
}

void DatePicker::NewValue(QString strValue)
{
  // Get the wheel that triggered the slot
  Wheel* wheel = (Wheel*)QObject::sender();

  // Find the corresponding index in the m_Wheels map
  int index = m_Wheels.key(wheel);

  // Update the value in m_Values
  m_Values[index] = strValue.toInt();

  // NO BREAK ! Use the fallthrough switch property
  switch (index)
  {
    case YEAR:    UpdateRange(MONTH, m_DateTimes[START_DATE], m_DateTimes[END_DATE]);
    case MONTH:   UpdateRange(DAY, m_DateTimes[START_DATE], m_DateTimes[END_DATE]);
    case DAY:     UpdateRange(HOUR, m_DateTimes[START_DATE], m_DateTimes[END_DATE]);
    case HOUR:    UpdateRange(MINUTE, m_DateTimes[START_DATE], m_DateTimes[END_DATE]);
    case MINUTE:  UpdateRange(SECOND, m_DateTimes[START_DATE], m_DateTimes[END_DATE]);
    case SECOND:  UpdateDate(); 
                  UpdateTime();
  }
}