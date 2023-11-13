#include "stdafx.h"
#include "RFView.h"

RFView::RFView(QWidget *parent) : PageView(parent)
{
  // Same order as display (used for qt translation)
  Names.append(tr("RF_STRENGTH"));
  Names.append(tr("RF_DEVIATION"));

  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();
}

RFView::~RFView()
{

}

void RFView::SetupUI()
{
  this->AddFieldView(Names);
}

void RFView::SetupEvent()
{
}

void RFView::NewSignalStrength(double Strength)
{
  const int index = 0;
  this->m_Strength = Strength;
  QString strength = StrengthToString(this->m_Strength);

  this->UpdateField(index, strength);
}

void RFView::NewSignalDeviation(double Deviation)
{
  const int index = 1;
  this->m_Deviation = Deviation;
  QString deviation = DeviationToString(this->m_Deviation);

  this->UpdateField(index, deviation);
}

void RFView::Update(ChannelItem *item)
{
  if (item == nullptr)
    return;

  LS_X8 *_LS_X8_ = static_cast<LS_X8*>(item);
  if (_LS_X8_ == nullptr)
    return;

  this->NewSignalStrength(_LS_X8_->Strength());
  this->NewSignalDeviation(_LS_X8_->Deviation());
}

QString RFView::StrengthToString(double Strength)
{
  return QString::number(Strength);
}

QString RFView::DeviationToString(double Deviation)
{
  return QString::number(Deviation);
}


