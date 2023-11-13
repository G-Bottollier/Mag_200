#include "stdafx.h"
#include "G_Record.h"
#include "R_Record.h"

G_Record::G_Record(QObject *parent) : QObject(parent)
{

}

G_Record::~G_Record()
{
  this->ClearHash();
}

void G_Record::Decode(int code, QByteArray bytesData, QList<int> indexes)
{
  switch (code)
  {
    default:                                                                                      return;
    case G_PN:      this->ProgramName                                             = bytesData;    return;
    case G_106:     this->Irig106Revision                                         = bytesData;    return;
    case G_OD:      this->OriginationDate                                         = bytesData;    return;
    case G_DSI_N:   this->NumberDataSources                                       = bytesData;    return;
    case G_DSI:     this->Get_DataSource(indexes)->DataSourceID                   = bytesData;    return;
    case G_DST:     this->Get_DataSource(indexes)->DataSourceType                 = bytesData;    return;
  }
}

void G_Record::Clear()
{
  this->ProgramName.clear();
  this->Irig106Revision.clear();
  this->OriginationDate.clear();
  this->NumberDataSources.clear();

  this->ClearHash();
}

void G_Record::ClearHash()
{
  qDeleteAll(this->G_DataSources);
  this->G_DataSources.clear();
}

G_DataSource* G_Record::Get_DataSource(QList<int> indexes)
{
  if (indexes.size() != 1)
    return nullptr;

  G_DataSource *g_dataSource = G_DataSource::Get_G_DataSource(indexes.takeFirst(), this, this->G_DataSources);
  if (g_dataSource == nullptr)
    return nullptr;

  return g_dataSource;
}

G_Record * G_Record::Get_G(int index, QObject * parent, QHash<int, G_Record*>& G_Records)
{
  if (index < 0)
    return nullptr;

  bool exist = G_Records.contains(index);

  if (exist == false)
    G_Records[index] = new G_Record(parent);

  return G_Records[index];
}

void G_Record::Connect_G(QHash<int, G_Record*>& G_Records, QHash<int, R_Record*>& R_Records)
{
  for (G_Record *g_record : G_Records)
  {
    for (G_DataSource *g_datasource : g_record->G_DataSources)
    {
      for (R_Record *r_record : R_Records)
      {
        if (g_datasource->DataSourceID == r_record->DataSourceID)
          g_datasource->r_record = r_record;
      }
    }
  }
}

void G_DataSource::Clear()
{
  this->DataSourceNumber = -1;
  this->DataSourceID.clear();
  this->DataSourceType.clear();
}

G_DataSource * G_DataSource::Get_G_DataSource(int index, QObject * parent, QHash<int, G_DataSource*>& G_DataSources)
{
  if (index < 0)
    return nullptr;

  bool exist = G_DataSources.contains(index);

  if (exist == false)
  {
    G_DataSource *g_dataSource = new G_DataSource(parent);
    g_dataSource->DataSourceNumber = index;
    G_DataSources[index] = g_dataSource;
  }

  return G_DataSources[index];
}


