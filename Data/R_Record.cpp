#include "stdafx.h"
#include "R_Record.h"
#include "M_Record.h"
#include "P_Record.h"

R_Record::R_Record(QObject *parent) : QObject(parent)
{

}

R_Record::~R_Record()
{
this->ClearHash();
}

void R_Record::Decode(int code, QByteArray bytesData, QList<int> indexes)
{
switch (code)
{
default:return;
case R_ID:this->DataSourceID= bytesData;return;
case R_N: this->NumberDataSources = bytesData;return;
case R_IDX_E: this->RawIndexEnabled = bytesData;
this->IndexEnabled= this->RawIndexEnabled == "T"; return;
case R_EV_E:this->RawEventsEnabled= bytesData;
this->EventsEnabled = this->RawEventsEnabled == "T";return;
case R_DSI: this->Get_DataSource(indexes)->DataSourceID = bytesData;return;
case R_CDT: this->Get_DataSource(indexes)->ChannelDataType= bytesData;return;
case R_DST: this->Get_DataSource(indexes)->ChannelDataType= bytesData;return;
case R_TK1: this->Get_DataSource(indexes)->RawTrackNumber = bytesData;
this->Get_DataSource(indexes)->TrackNumber= bytesData.toInt();return;
case R_CHE: this->Get_DataSource(indexes)->RawEnabled = bytesData;
this->Get_DataSource(indexes)->Enabled= this->Get_DataSource(indexes)->RawEnabled == "T";
return;
case R_BDLN:this->Get_DataSource(indexes)->BusDataLinkName= bytesData;return;
case R_PDLN:this->Get_DataSource(indexes)->PCMDataLinkName= bytesData;return;
case R_CDLN:this->Get_DataSource(indexes)->ChannelDataLinkName= bytesData;return;

/* Video attributes*/
case R_VTF:this->Get_DataSource(indexes)->VideoDataType = bytesData;return;
case R_VXF:this->Get_DataSource(indexes)->VideoEncodeType = bytesData;return;
case R_VST:this->Get_DataSource(indexes)->VideoSignalType = bytesData;return;
case R_VSF:this->Get_DataSource(indexes)->VideoSignalFormat = bytesData;return;
case R_CBR:this->Get_DataSource(indexes)->VideoConstBitRate = bytesData;return;
case R_VBR:this->Get_DataSource(indexes)->VideoVarPeakBitRate = bytesData;return;
case R_VED:this->Get_DataSource(indexes)->VideoEncodingDelay= bytesData;return;

/* PCM Attributes*/
case R_PDTF: this->Get_DataSource(indexes)->PCMDataTypeFormat = bytesData;return;
case R_PDP:this->Get_DataSource(indexes)->PCMDataPacking= bytesData;return;
case R_ICE:this->Get_DataSource(indexes)->PCMInputClockEdge = bytesData;return;
case R_IST:this->Get_DataSource(indexes)->PCMInputSignalType= bytesData;return;
case R_ITH:this->Get_DataSource(indexes)->PCMInputThreshold = bytesData;return;
case R_ITM:this->Get_DataSource(indexes)->PCMInputTermination = bytesData;return;
case R_PTF:this->Get_DataSource(indexes)->PCMVideoTypeFormat= bytesData;return;

/* Analog Attributes */
case R_ACH_N:this->Get_DataSource(indexes)->AnalogChannelsPerPacket = bytesData;return;
case R_ASR:this->Get_DataSource(indexes)->AnalogSampleRate= bytesData;return;
case R_ADP:this->Get_DataSource(indexes)->AnalogDataPacking = bytesData;return;
}
}

void R_Record::Clear()
{
  this->RecordNumber = -1;
  this->DataSourceID.clear();
  this->NumberDataSources.clear();
  this->RawIndexEnabled.clear();
  this->IndexEnabled = -1;
  this->RawEventsEnabled.clear();
  this->EventsEnabled = -1;

  this->ClearHash();
}

void R_Record::ClearHash()
{
  qDeleteAll(this->R_DataSources);
  this->R_DataSources.clear();
}

void R_Record::Connect_R(QHash<int, R_Record*> &R_Records, QHash<int, M_Record*> &M_Records, QHash<int, P_Record*> &P_Records)
{
  for (R_Record *r_record : R_Records)
  {
    for (R_DataSource *r_datasource : r_record->R_DataSources)
    {
      for (M_Record *m_record : M_Records)
      {
        if (r_datasource->DataSourceID == m_record->DataSourceID)
          r_datasource->m_record = m_record;
      }

      for (P_Record *p_record : P_Records)
      {
        // TMATS 04 and 05
        if (r_datasource->DataSourceID == p_record->DataLinkName)
          r_datasource->p_record = p_record;

        // TMATS 07 and 09 and later
        if (r_datasource->ChannelDataLinkName == p_record->DataLinkName)
          r_datasource->p_record = p_record;
      }
    }
  }
}

R_DataSource* R_Record::Get_DataSource(QList<int> indexes)
{
if (indexes.size() != 1)
return nullptr;

R_DataSource *r_dataSource = R_DataSource::Get_R_DataSource(indexes.takeFirst(), this, this->R_DataSources);
if (r_dataSource == nullptr)
return nullptr;

return r_dataSource;
}

R_Record * R_Record::Get_R(int index, QObject * parent, QHash<int, R_Record*>& R_Records)
{
if (index < 0)
return nullptr;

bool exist = R_Records.contains(index);

if (exist == false)
{
R_Record *r_record = new R_Record(parent);
r_record->RecordNumber = index;
R_Records[index] = r_record;
}

return R_Records[index];
}

void R_DataSource::Clear()
{
  this->DataSourceNumber = -1;
  this->DataSourceID.clear();
  this->ChannelDataType.clear();
  this->RawTrackNumber.clear();
  this->TrackNumber = -1;
  this->RawEnabled.clear();
  this->Enabled = -1;
  this->PCMDataLinkName.clear();
  this->BusDataLinkName.clear();
  this->ChannelDataLinkName.clear();

  /* Video channel attributes */
  this->VideoDataType.clear();
  this->VideoEncodeType.clear();
  this->VideoSignalType.clear();
  this->VideoSignalFormat.clear();
  this->VideoConstBitRate.clear();
  this->VideoVarPeakBitRate.clear();
  this->VideoEncodingDelay.clear();

  /* PCM channel attributes */
  this->PCMDataTypeFormat.clear();
  this->PCMDataPacking.clear();
  this->PCMInputClockEdge.clear();
  this->PCMInputSignalType.clear();
  this->PCMInputThreshold.clear();
  this->PCMInputTermination.clear();
  this->PCMVideoTypeFormat.clear();

  /* Analog channel attributes */
  this->AnalogChannelsPerPacket.clear();
  this->AnalogSampleRate.clear();
  this->AnalogDataPacking.clear();
}

R_DataSource* R_DataSource::Get_R_DataSource(int index, QObject *parent, QHash<int, R_DataSource*> &R_DataSources)
{
if (index < 0)
return nullptr;

bool exist = R_DataSources.contains(index);

if (exist == false)
{
R_DataSource *r_dataSource = new R_DataSource(parent);
r_dataSource->DataSourceNumber = index;
R_DataSources[index] = r_dataSource;
}

return R_DataSources[index];
}