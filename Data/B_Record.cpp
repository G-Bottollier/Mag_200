#include "stdafx.h"
#include "B_Record.h"

B_Record::B_Record(QObject *parent) : QObject(parent)
{

}

B_Record::~B_Record()
{

}

void B_Record::Decode(int code, QByteArray bytesData, QList<int> indexes)
{
  switch (code)
  {
    default:                                                                                      return;
    case B_DLN:      this->DataLinkName                                           = bytesData;    return;
    case B_NBS_N:    this->NumberBuses                                            = bytesData;    return;
  }
}

void B_Record::Clear()
{
  this->RecordNumber = -1;
  this->DataLinkName.clear();
  this->NumberBuses.clear();
}

B_Record* B_Record::Get_B(int index, QObject *parent, QHash<int, B_Record*> &B_Records)
{
  if (index < 0)
    return nullptr;

  bool exist = B_Records.contains(index);

  if (exist == false)
  {
    B_Record *b_record = new B_Record(parent);
    b_record->RecordNumber = index;
    B_Records[index] = b_record;
  }

  return B_Records[index];
}