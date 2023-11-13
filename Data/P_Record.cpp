#include "stdafx.h"
#include "P_Record.h"

P_Record::P_Record(QObject *parent) : QObject(parent)
{

}

P_Record::~P_Record()
{
  this->ClearHash();
}

void P_Record::Decode(int code, QByteArray bytesData, QList<int> indexes)
{
  switch (code)
  {
    default:                                                                                      return;
    case P_DLN:      this->DataLinkName                                           = bytesData;    return;
    case P_D1:       this->PCMCode                                                = bytesData;    return;
    case P_D2:       this->BitsPerSecond                                          = bytesData;    return;
    case P_D4:       this->Polarity                                               = bytesData;    return;
    case P_TF:       this->TypeFormat                                             = bytesData;    return;
    case P_F1:       this->CommonWordLength                                       = bytesData;    return;
    case P_F2:       this->WordTransferOrder                                      = bytesData;    return;
    case P_F3:       this->ParityType                                             = bytesData;    return;
    case P_F4:       this->ParityTransferOrder                                    = bytesData;    return;
    case P_MF_N:     this->NumberMinorFrames                                      = bytesData;    return;
    case P_MF1:      this->WordsInMinorFrame                                      = bytesData;    return;
    case P_MF2:      this->BitsInMinorFrame                                       = bytesData;    return;
    case P_MF3:      this->MinorFrameSyncType                                     = bytesData;    return;
    case P_MF4:      this->MinorFrameSyncPatternLength                            = bytesData;    return;
    case P_MF5:      this->MinorFrameSyncPattern                                  = bytesData;    return;
    case P_SYNC1:    this->InSyncCritical                                         = bytesData;    return;
    case P_SYNC2:    this->InSyncErrors                                           = bytesData;    return;
    case P_SYNC3:    this->OutSyncCritical                                        = bytesData;    return;
    case P_SYNC4:    this->OutSyncErrors                                          = bytesData;    return;
    case P_ISF_N:    this->NumberSubframeCounters                                 = bytesData;    return;

    /* Decode SubframeID */
    case P_ISF1:     this->Get_SubframeID(indexes)->CounterName                   = bytesData;    return;
    case P_ISF2:     this->Get_SubframeID(indexes)->CounterType                   = bytesData;    return;
    case P_IDC1:     this->Get_SubframeID(indexes)->WordPosition                  = bytesData;    return;
    case P_IDC2:     this->Get_SubframeID(indexes)->WordLength                    = bytesData;    return;
    case P_IDC3:     this->Get_SubframeID(indexes)->BitLocation                   = bytesData;    return;
    case P_IDC4:     this->Get_SubframeID(indexes)->CounterLength                 = bytesData;    return;
    case P_IDC5:     this->Get_SubframeID(indexes)->Endian                        = bytesData;    return;
    case P_IDC6:     this->Get_SubframeID(indexes)->InitValue                     = bytesData;    return;
    case P_IDC7:     this->Get_SubframeID(indexes)->MFForInitValue                = bytesData;    return;
    case P_IDC8:     this->Get_SubframeID(indexes)->EndValue                      = bytesData;    return;
    case P_IDC9:     this->Get_SubframeID(indexes)->MFForEndValue                 = bytesData;    return;
    case P_IDC10:    this->Get_SubframeID(indexes)->CountDirection                = bytesData;    return;
    case P_SF_N:     this->Get_SubframeID(indexes)->NumberSubframeDefinitions     = bytesData;    return;

    /* Decode AsyncEmbedded */
    case P_AEF_N:   this->NumberAsyncEmbedded                                     = bytesData;    return;
    case P_AEF_DLN: this->Get_AsyncEmbedded(indexes)->DataLinkName                = bytesData;    return;

    /* Decode SubframeLocation */
    case P_SF4:     this->Get_SubframeLocation(indexes)->SubframeLocation         = bytesData;    return;

    /* Decode SubframeDefinition */
    case P_SF1:     this->Get_SubframeDefinition(indexes)->SubframeName           = bytesData;    return;
    case P_SF2:     this->Get_SubframeDefinition(indexes)->SuperComPosition       = bytesData;    return;
    case P_SF3:     this->Get_SubframeDefinition(indexes)->SuperComDefined        = bytesData;    return;
    case P_SF5:     this->Get_SubframeDefinition(indexes)->LocationInterval       = bytesData;    return;
    case P_SF6:     this->Get_SubframeDefinition(indexes)->SubframeDepth          = bytesData;    return;
  }
}

void P_Record::Clear()
{
  this->RecordNumber = -1;
  this->DataLinkName.clear();
  this->PCMCode.clear();
  this->BitsPerSecond.clear();
  this->Polarity.clear();
  this->TypeFormat.clear();
  this->CommonWordLength.clear();
  this->WordTransferOrder.clear();
  this->ParityType.clear();
  this->ParityTransferOrder.clear();
  this->NumberMinorFrames.clear();
  this->WordsInMinorFrame.clear();
  this->BitsInMinorFrame.clear();
  this->MinorFrameSyncType.clear();
  this->MinorFrameSyncPatternLength.clear();
  this->MinorFrameSyncPattern.clear();
  this->InSyncCritical.clear();
  this->InSyncErrors.clear();
  this->OutSyncCritical.clear();
  this->OutSyncErrors.clear();
  this->NumberAsyncEmbedded.clear();
  this->NumberSubframeCounters.clear();

  this->ClearHash();
}

void P_Record::ClearHash()
{
  qDeleteAll(this->AsyncEmbeddeds);
  qDeleteAll(this->SubframeIDs);

  this->AsyncEmbeddeds.clear();
  this->SubframeIDs.clear();
}

P_SubframeID* P_Record::Get_SubframeID(QList<int> indexes)
{
  if (indexes.size() != 1)
    return nullptr;

  P_SubframeID *p_subframeID = P_SubframeID::Get_P_SubframeID(indexes.takeFirst(), this, this->SubframeIDs);
  if (p_subframeID == nullptr)
    return nullptr;
  
  return p_subframeID;
}

P_AsyncEmbedded* P_Record::Get_AsyncEmbedded(QList<int> indexes)
{
  if (indexes.size() != 1)
    return nullptr;

  P_AsyncEmbedded *p_asyncEmbedded = P_AsyncEmbedded::Get_P_AsyncEmbedded(indexes.takeFirst(), this, this->AsyncEmbeddeds);
  if (p_asyncEmbedded == nullptr)
    return nullptr;

  return p_asyncEmbedded;
}

P_SubframeLocation* P_Record::Get_SubframeLocation(QList<int> indexes)
{
  if (indexes.size() != 3)
    return nullptr;

  P_SubframeID *p_subframeID = P_SubframeID::Get_P_SubframeID(indexes.takeFirst(), this, this->SubframeIDs);
  if (p_subframeID == nullptr)
    return nullptr;

  P_SubframeDefinition *p_subframeDefinition = P_SubframeDefinition::Get_P_SubframeDefinition(indexes.takeFirst(), p_subframeID, p_subframeID->SubframeDefinitions);
  if (p_subframeDefinition == nullptr)
    return nullptr;

  P_SubframeLocation *p_subframeLocation = P_SubframeLocation::Get_P_SubframeLocation(indexes.takeFirst(), p_subframeDefinition, p_subframeDefinition->SubFrameLocations);
  if (p_subframeLocation == nullptr)
    return nullptr;

  return p_subframeLocation;
}

P_SubframeDefinition* P_Record::Get_SubframeDefinition(QList<int> indexes)
{
  if (indexes.size() != 2)
    return nullptr;

  P_SubframeID *p_subframeID = P_SubframeID::Get_P_SubframeID(indexes.takeFirst(), this, this->SubframeIDs);
  if (p_subframeID == nullptr)
    return nullptr;

  P_SubframeDefinition *p_subframeDefinition = P_SubframeDefinition::Get_P_SubframeDefinition(indexes.takeFirst(), p_subframeID, p_subframeID->SubframeDefinitions);
  if (p_subframeDefinition == nullptr)
    return nullptr;

  return p_subframeDefinition;
}

P_Record* P_Record::Get_P(int index, QObject *parent, QHash<int, P_Record*>& P_Records)
{
  if (index < 0)
    return nullptr;

  bool exist = P_Records.contains(index);

  if (exist == false)
  {
    P_Record *p_record = new P_Record(parent);
    p_record->RecordNumber = index;
    P_Records[index] = p_record;
  }

  return P_Records[index];
}

void P_Record::Connect_P(QHash<int, P_Record*>& P_Records)
{
  for (P_Record *p_record : P_Records)
  {
    for (P_AsyncEmbedded *p_asyncEmbedded : p_record->AsyncEmbeddeds)
    {
      for (P_Record *p_embedded : P_Records)
      {
        if (p_embedded->DataLinkName == p_asyncEmbedded->DataLinkName)
          p_asyncEmbedded->p_record = p_embedded;
      }
    }
  }
}



/* --------------------------------------------- */
/* Subframe ID Counter definitions (From 106-11) */
/* --------------------------------------------- */

void P_SubframeID::Clear()
{
  this->CounterNumber = -1;
  this->CounterName.clear();
  this->CounterType.clear();
  this->WordPosition.clear();
  this->WordLength.clear();
  this->BitLocation.clear();
  this->CounterLength.clear();
  this->Endian.clear();
  this->InitValue.clear();
  this->MFForInitValue.clear();
  this->EndValue.clear();
  this->MFForEndValue.clear();
  this->CountDirection.clear();
  this->NumberSubframeDefinitions.clear();

  this->ClearHash();
}

void P_SubframeID::ClearHash()
{
  qDeleteAll(this->SubframeDefinitions);
  this->SubframeDefinitions.clear();
}

P_SubframeID * P_SubframeID::Get_P_SubframeID(int index, QObject *parent, QHash<int, P_SubframeID*> &SubframeIDs)
{
  if (index < 0)
    return nullptr;

  bool exist = SubframeIDs.contains(index);

  if (exist == false)
  {
    P_SubframeID *p_subframeID = new P_SubframeID(parent);
    p_subframeID->CounterNumber = index;
    SubframeIDs[index] = p_subframeID;
  }

  return SubframeIDs[index];
}

void P_AsyncEmbedded::Clear()
{
  this->EmbeddedStreamNumber = -1;
  this->DataLinkName.clear();
}

P_AsyncEmbedded* P_AsyncEmbedded::Get_P_AsyncEmbedded(int index, QObject *parent, QHash<int, P_AsyncEmbedded*> &AsyncEmbeddeds)
{
  if (index < 0)
    return nullptr;

  bool exist = AsyncEmbeddeds.contains(index);

  if (exist == false)
  {
    P_AsyncEmbedded *p_asyncEmbedded = new P_AsyncEmbedded(parent);
    p_asyncEmbedded->p_record = static_cast<P_Record*>(parent);
    p_asyncEmbedded->EmbeddedStreamNumber = index;
    AsyncEmbeddeds[index] = p_asyncEmbedded;
  }

  return AsyncEmbeddeds[index];
}

void P_SubframeLocation::Clear()
{
  this->SubframeLocationNumber = -1;
  this->SubframeLocation.clear();
}

P_SubframeLocation* P_SubframeLocation::Get_P_SubframeLocation(int index, QObject *parent, QHash<int, P_SubframeLocation*> &SubFrameLocations)
{
  if (index < 0)
    return nullptr;

  bool exist = SubFrameLocations.contains(index);

  if (exist == false)
  {
    P_SubframeLocation *p_subframeLocation = new P_SubframeLocation(parent);
    p_subframeLocation->SubframeLocationNumber = index;
    SubFrameLocations[index] = p_subframeLocation;
  }

  return SubFrameLocations[index];
}

void P_SubframeDefinition::Clear()
{
  this->SubframeDefinitionNumber = -1;
  this->SubframeName.clear();
  this->SuperComPosition.clear();
  this->SuperComDefined.clear();
  this->LocationInterval.clear();
  this->SubframeDepth.clear();

  this->ClearHash();
}

void P_SubframeDefinition::ClearHash()
{
  qDeleteAll(this->SubFrameLocations);
  this->SubFrameLocations.clear();
}

P_SubframeDefinition* P_SubframeDefinition::Get_P_SubframeDefinition(int index, QObject *parent, QHash<int, P_SubframeDefinition*> &SubframeDefinitions)
{
  if (index < 0)
    return nullptr;

  bool exist = SubframeDefinitions.contains(index);

  if (exist == false)
  {
    P_SubframeDefinition *p_subframeDefinition = new P_SubframeDefinition(parent);
    p_subframeDefinition->SubframeDefinitionNumber = index;
    SubframeDefinitions[index] = p_subframeDefinition;
  }

  return SubframeDefinitions[index];
}