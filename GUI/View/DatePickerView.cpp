#include "stdafx.h"
#include "DatePickerView.h"

DatePickerView::DatePickerView(int type, QWidget *parent) : PageView(parent)
{
  this->m_Type = type;

  SetupUI();

  SetupEvent();
}

DatePickerView::~DatePickerView()
{

}

void DatePickerView::SetupUI()
{
  this->UseLine(true);
  this->UseFullPage(true);

  QString headerName;
  bool isDate = this->m_Type == DATE_PICKER;
  bool isRange = this->m_Type == RANGE_PICKER;
  headerName = isDate ? tr("PAGE_DATE_PICKER") : headerName;
  headerName = isRange ? tr("PAGE_RANGE_PICKER") : headerName;

  this->AddHeaderView();
  this->AddHeaderTitle();
  this->SetHeaderName(headerName);

  QFont font = this->font();
  font.setFamily("Dubai");
  font.setBold(true);

  m_Pickers[this->m_Type] = new DatePicker();
  m_Pickers[this->m_Type]->SetRangeMode(isRange);
  m_Pickers[this->m_Type]->setFont(font);
  
  this->AddMainWidget(m_Pickers[this->m_Type]);
}

void DatePickerView::SetupEvent()
{

}

void DatePickerView::resizeEvent(QResizeEvent * event)
{
  PageView::resizeEvent(event);

  QSize size = event->size();

  int headerSize = this->HeaderSize().height();

  int width = Tools::GetWidgetWidth(this, size);
  int height = Tools::GetWidgetHeight(this, size) - headerSize;

  if (height == headerSize)
    return;

  m_Pickers[this->m_Type]->setFixedSize(QSize(width, height));
}

void DatePickerView::SetDate(QDateTime date)
{
  m_Pickers[this->m_Type]->SetDate(date);
}

void DatePickerView::SetDateRange(QDateTime startTime, QDateTime endTime)
{
  m_Pickers[this->m_Type]->SetDateRange(startTime, endTime);
}

void DatePickerView::SetBoundRange(QDateTime lowerBound, QDateTime upperBound)
{
  m_Pickers[this->m_Type]->SetBoundRange(lowerBound, upperBound);
}

QString DatePickerView::StartDate()
{
  if (this->m_Type != RANGE_PICKER)
    return QString();

  return m_Pickers[this->m_Type]->ToString(LOWER_BOUND_DATE);
}

QString DatePickerView::EndDate()
{
  if (this->m_Type != RANGE_PICKER)
    return QString();

  return m_Pickers[this->m_Type]->ToString(UPPER_BOUND_DATE);
}

QDateTime DatePickerView::Date()
{
  return m_Pickers[this->m_Type]->ToDateTime(CURRENT_DATE);
}

QDateTime DatePickerView::LowerBoundDate()
{
  return m_Pickers[this->m_Type]->ToDateTime(LOWER_BOUND_DATE);
}

QDateTime DatePickerView::UpperBoundDate()
{
  return m_Pickers[this->m_Type]->ToDateTime(UPPER_BOUND_DATE);
}

void DatePickerView::CloseClicked()
{
  if (m_Pickers[this->m_Type]->IsValid() == true)
    PageView::CloseClicked();
  else
    m_Pickers[this->m_Type]->DoBlinkAnimation();
}