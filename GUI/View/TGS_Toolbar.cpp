#include "stdafx.h"
#include "TGS_Toolbar.h"

TGS_Toolbar::TGS_Toolbar(QWidget *parent) : DPS_Toolbar(parent)
{
  SetupUI();
  SetupEvent();
}

TGS_Toolbar::~TGS_Toolbar()
{

}

void TGS_Toolbar::SetupUI()
{
  // Subclass DPSView for use the same behavior
  // For add custom behavior subclass toolbar!
}

void TGS_Toolbar::SetupEvent()
{

}
