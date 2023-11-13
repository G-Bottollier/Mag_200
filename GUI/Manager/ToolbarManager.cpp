#include "stdafx.h"
#include "ToolbarManager.h"

ToolbarManager::ToolbarManager(QWidget *parent) : QStackedWidget(parent)
{
  SetupUI();
  SetupEvent();
}

ToolbarManager::~ToolbarManager()
{

}

void ToolbarManager::SetupUI()
{
  Toolbars[TOOLBAR_DPS] = new DPS_Toolbar();
  Toolbars[TOOLBAR_TGS] = new TGS_Toolbar();
  Toolbars[TOOLBAR_IMP] = new IMP_Toolbar();
  Toolbars[TOOLBAR_EXP] = new EXP_Toolbar();
  Toolbars[TOOLBAR_REC] = new REC_Toolbar();
  Toolbars[TOOLBAR_REP] = new REP_Toolbar();

  for (Toolbar* toolbar : Toolbars)
    this->addWidget(toolbar);

  const QStringList types = qSettings->Mag200_Types();
  QString type = qSettings->Mag200_Type();

  int index = types.indexOf(type);
  if (index == -1)
    return;

  this->setCurrentIndex(index);
}

void ToolbarManager::SetupEvent()
{
  for (Toolbar* toolbar : Toolbars)
    QObject::connect(toolbar, &Toolbar::SendLedClicked, this, &ToolbarManager::LedClicked);

  IMP_Toolbar *toolbar = static_cast<IMP_Toolbar*>(Toolbars[TOOLBAR_IMP]);
  QObject::connect(toolbar, &IMP_Toolbar::SendOpenParentFolder, this, &ToolbarManager::NewOpenParentFolder);
}

void ToolbarManager::resizeEvent(QResizeEvent * event)
{
  for (Toolbar* toolbar : Toolbars)
    toolbar->setFixedHeight(event->size().height());
}

void ToolbarManager::CurrentIndexChanged(int index)
{
  // CLOSE OLD INDEX
  switch (m_OldIndex)
  {
    case TOOLBAR_DPS:
      // No additional actions needed for TOOLBAR_DPS
      break;
    case TOOLBAR_TGS:
      // No additional actions needed for TOOLBAR_TGS
      break;
    case TOOLBAR_REC:
      // No additional actions needed for TOOLBAR_REC
      break;
    case TOOLBAR_REP:
      // No additional actions needed for TOOLBAR_REP
      break;
  }

  // OPEN NEW INDEX
  switch (index)
  {
    case TOOLBAR_DPS:
      // No additional actions needed for TOOLBAR_DPS
      break;
    case TOOLBAR_TGS:
      // No additional actions needed for TOOLBAR_TGS
      break;
    case TOOLBAR_REC:
      // No additional actions needed for TOOLBAR_REC
      break;
    case TOOLBAR_REP:
      // No additional actions needed for TOOLBAR_REP
      break;
  }

  // Store the current index as the old index for the next iteration
  m_OldIndex = index;
}

void ToolbarManager::NewToolbarIndex(int index)
{
  this->setCurrentIndex(index);
}

void ToolbarManager::NewStatus(int ChannelID, int Type, QVariant Value)
{
  int index = this->currentIndex();
  if (index == -1)
    return;

  if (Toolbars[index]->IsValidStatus(ChannelID, Type) == false)
    return;

  Toolbars[index]->NewStatus(ChannelID, Type, Value); 
}

void ToolbarManager::NewDrivesOpen()
{
  IMP_Toolbar *toolbar = static_cast<IMP_Toolbar*>(Toolbars[TOOLBAR_IMP]);
  toolbar->setFolderName(tr("ALL_DRIVES"));
}

void ToolbarManager::NewFolderOpen(QFileInfo info)
{
  IMP_Toolbar *toolbar = static_cast<IMP_Toolbar*>(Toolbars[TOOLBAR_IMP]);
  
  QFileInfo entry(info.canonicalFilePath());
  QString name = entry.fileName();

  if (name.isEmpty() == true)
    name = info.absoluteFilePath();

  name.remove("/");

  toolbar->setFolderName(name);
}

/*******************/
/** RESEND SIGNAL **/
/*******************/
void ToolbarManager::LedClicked(int type)   { emit this->SendLedClicked(type);    }
void ToolbarManager::NewOpenParentFolder()  { emit this->SendOpenParentFolder();  }