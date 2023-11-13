#include "stdafx.h"
#include "M_Record.h"
#include "B_Record.h"
#include "P_Record.h"

M_Record::M_Record(QObject *parent) : QObject(parent)
{

}

M_Record::~M_Record()
{

}

void M_Record::Decode(int code, QByteArray bytesData, QList<int> indexes)
{
  switch (code)
  {
    default:                                                                                      return;
    case M_ID:        this->DataSourceID                                          = bytesData;    return;
    case M_BSG1:      this->BasebandSignalType                                    = bytesData;    return;
    case M_BB_DLN:    this->BB_DataLinkName                                       = bytesData;    return;
  }
}

void M_Record::Clear()
{
  this->RecordNumber = -1;
  this->DataSourceID.clear();
  this->BB_DataLinkName.clear();
  this->BasebandSignalType.clear();
}

M_Record * M_Record::Get_M(int index, QObject * parent, QHash<int, M_Record*>& M_Records)
{
  if (index < 0)
    return nullptr;

  bool exist = M_Records.contains(index);

  if (exist == false)
  {
    M_Record *m_record = new M_Record(parent);
    m_record->RecordNumber = index;
    M_Records[index] = m_record;
  }

  return M_Records[index];
}

void M_Record::Connect_M(QHash<int, M_Record*>& M_Records, QHash<int, B_Record*>& B_Records, QHash<int, P_Record*>& P_Records)
{
  for (M_Record *m_record : M_Records)
  {
      for (P_Record *p_record : P_Records)
      {
        if (m_record->BB_DataLinkName == p_record->DataLinkName)
          m_record->p_record = p_record;
      }

      for (B_Record *b_record : B_Records)
      {
        if (m_record->BB_DataLinkName == b_record->DataLinkName)
          m_record->b_record = b_record;
      }
  }
}
