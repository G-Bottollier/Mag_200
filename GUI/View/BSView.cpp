#include "stdafx.h"
#include "BSView.h"

BSView::BSView(QWidget *parent) : PageView(parent)
{
  // Same order as display (used for qt translation)
  Names.append(tr("BS_BIT_RATE"));
  Names.append(tr("BS_PCM_CODE"));

  // Same order as Names
  Methods.append(META_METHOD(Mag200Data::BitrateText));
  Methods.append(META_METHOD(Mag200Data::PulseModulationCode));

  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();

  if (Names.size() == Methods.size())
    return;

  qFatal("INCOHERENT CODING : Size Missmatch !");
}

BSView::~BSView()
{

}

void BSView::SetupUI()
{
  this->AddFieldView(this->Names);
}

void BSView::SetupEvent()
{

}

void BSView::Update(Mag200Data * data)
{
  this->UpdateFieldView(data, this->Methods);
}
