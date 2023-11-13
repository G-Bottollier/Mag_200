#include "stdafx.h"
#include "FSView.h"

FSView::FSView(QWidget *parent) : PageView(parent)
{
  // Same order as display (used for qt translation)
  Names.append(tr("FS_BINARY"));
  Names.append(tr("FS_BIT_RATE"));
  Names.append(tr("FS_DIRECTION"));
  Names.append(tr("FS_FIRST_ID_VALUE"));
  Names.append(tr("FS_HEXA"));
  Names.append(tr("FS_ID_BIT_LSB"));
  Names.append(tr("FS_ID_WORD_LOCATION"));
  Names.append(tr("FS_LAST_ID_VALUE"));
  Names.append(tr("FS_MINOR_FRAMES"));
  Names.append(tr("FS_SYNCHRO_LENGTH"));

  // Same order as Names
  Methods.append(META_METHOD(Mag200Data::Binary));
  Methods.append(META_METHOD(Mag200Data::BitrateText));
  Methods.append(META_METHOD(Mag200Data::MajorFrameType_ID));
  Methods.append(META_METHOD(Mag200Data::MajorFrameFirstValue_ID));
  Methods.append(META_METHOD(Mag200Data::Hexa));
  Methods.append(META_METHOD(Mag200Data::MajorFrameLSB_ID));
  Methods.append(META_METHOD(Mag200Data::MajorFramePosInWord_ID));
  Methods.append(META_METHOD(Mag200Data::MajorFrameLastValue_ID));
  Methods.append(META_METHOD(Mag200Data::MajorFrameLength));
  Methods.append(META_METHOD(Mag200Data::FrameSyncLength));

  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();

  if (Names.size() == Methods.size())
    return;

  qFatal("INCOHERENT CODING : Size Missmatch !");
}

FSView::~FSView()
{
}

void FSView::SetupUI()
{
  this->AddFieldView(this->Names);
}

void FSView::SetupEvent()
{

}

void FSView::Update(Mag200Data * data)
{
  this->UpdateFieldView(data, this->Methods);
}
