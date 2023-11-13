#ifndef P_RECORD_H
#define P_RECORD_H

#include <QObject>
#include "TmatsCode.h"

class P_SubframeID;
class P_AsyncEmbedded;
class P_SubframeLocation;
class P_SubframeDefinition;

class P_Record : public QObject
{
  Q_OBJECT

  private :
    void ClearHash();
    P_SubframeID* Get_SubframeID(QList<int> indexes);
    P_AsyncEmbedded *Get_AsyncEmbedded(QList<int> indexes);
    P_SubframeLocation* Get_SubframeLocation(QList<int> indexes);
    P_SubframeDefinition* Get_SubframeDefinition(QList<int> indexes);

  public: /* Public data variables */
    int                             RecordNumber                 = -1;     // P-x
    QByteArray                      DataLinkName;                          // P-x\DLN
    QByteArray                      PCMCode;                               // P-x\D1
    QByteArray                      BitsPerSecond;                         // P-x\D2
    QByteArray                      Polarity;                              // P-x\D4
    QByteArray                      TypeFormat;                            // P-x\TF
    QByteArray                      CommonWordLength;                      // P-x\F1
    QByteArray                      WordTransferOrder;                     // P-x\F2 most significant bit "M", least significant bit "L". default: M
    QByteArray                      ParityType;                            // P-x\F3 even "EV", odd "OD", or none "NO", default: none
    QByteArray                      ParityTransferOrder;                   // P-x\F4 leading "L", default: trailing
    QByteArray                      NumberMinorFrames;                     // P-x\MF\N
    QByteArray                      WordsInMinorFrame;                     // P-x\MF1
    QByteArray                      BitsInMinorFrame;                      // P-x\MF2
    QByteArray                      MinorFrameSyncType;                    // P-x\MF3
    QByteArray                      MinorFrameSyncPatternLength;           // P-x\MF4
    QByteArray                      MinorFrameSyncPattern;                 // P-x\MF5
    QByteArray                      InSyncCritical;                        // P-x\SYNC1
    QByteArray                      InSyncErrors;                          // P-x\SYNC2
    QByteArray                      OutSyncCritical;                       // P-x\SYNC3
    QByteArray                      OutSyncErrors;                         // P-x\SYNC4
    QByteArray                      NumberAsyncEmbedded;                   // P-x\AEF\N
    QByteArray                      NumberSubframeCounters;                // P-x\ISF\N
                          
    QHash< int, P_AsyncEmbedded* >  AsyncEmbeddeds;                        // Link to embedded stream defs
    QHash< int, P_SubframeID* >     SubframeIDs;                           // Link to Subframe ID Counter defs

  public:
    P_Record(QObject *parent = nullptr);
    ~P_Record();

    void Clear();

    void Decode(int code, QByteArray bytesData, QList<int> indexes);
    static P_Record* Get_P(int index, QObject *parent, QHash<int, P_Record*> &P_Records);
    static void Connect_P(QHash<int, P_Record*> &P_Records);
};



/* --------------------------------------------- */
/* Subframe ID Counter definitions (From 106-11) */
/* --------------------------------------------- */

/* Asynchronous Embedded Streams definitions */

class P_AsyncEmbedded : public QObject
{
  Q_OBJECT

  public: /* Public data variables */
    int                    EmbeddedStreamNumber            = -1;                   // P-x\AEF\XXX-n
    QByteArray             DataLinkName;                                           // P-x\AEF\DLN-n
    P_Record              *p_record                        = nullptr;              // Corresponding P record

  public:
    inline P_AsyncEmbedded(QObject* parent = nullptr) : QObject(parent) {}
    inline ~P_AsyncEmbedded() {}

    void Clear();

    static P_AsyncEmbedded* Get_P_AsyncEmbedded(int index, QObject *parent, QHash<int, P_AsyncEmbedded*> &AsyncEmbeddeds);
};



/* Subframe ID Counter definitions (From 106-11) */

class P_SubframeDefinition : public QObject
{
  Q_OBJECT

  private:
    void ClearHash();

  public: /* Public data variables */
    int                               SubframeDefinitionNumber     = -1;       // P-x\SFx-x-n
    QByteArray                        SubframeName;                            // P-x\SF1-x-n
    QByteArray                        SuperComPosition;                        // P-x\SF2-x-n
    QByteArray                        SuperComDefined;                         // P-x\SF3-x-n
    QHash<int, P_SubframeLocation*>   SubFrameLocations;                       // P-x\SF4-x-x-x
    QByteArray                        LocationInterval;                        // P-x\SF5-x-n
    QByteArray                        SubframeDepth;                           // P-x\SF6-x-n

  public:
    inline P_SubframeDefinition(QObject* parent = nullptr) : QObject(parent) {}
    inline ~P_SubframeDefinition() { this->ClearHash(); }

    void Clear();

    static P_SubframeDefinition* Get_P_SubframeDefinition(int index, QObject *parent, QHash<int, P_SubframeDefinition*> &SubframeDefinitions);
};

class P_SubframeLocation : public QObject
{
  Q_OBJECT

  public: /* Public data variables */
    int                    SubframeLocationNumber = -1;                            // P-x\SFx-x-x-n
    QByteArray             SubframeLocation;                                       // P-x\SF4-x-x-n

  public:
    inline P_SubframeLocation(QObject* parent = nullptr) : QObject(parent) {}
    inline ~P_SubframeLocation() {}

    void Clear();

    static P_SubframeLocation* Get_P_SubframeLocation(int index, QObject *parent, QHash<int, P_SubframeLocation*> &SubFrameLocations);
};

class P_SubframeID : public QObject
{
  Q_OBJECT

  private:
    void ClearHash();

  public: /* Public data variables */
    int                                 CounterNumber             = -1;        // P-x\ISFx-n
    QByteArray                          CounterName;                           // P-x\ISF1-n
    QByteArray                          CounterType;                           // P-x\ISF2-n
    QByteArray                          WordPosition;                          // P-x\IDC1-n
    QByteArray                          WordLength;                            // P-x\IDC2-n
    QByteArray                          BitLocation;                           // P-x\IDC3-n
    QByteArray                          CounterLength;                         // P-x\IDC4-n
    QByteArray                          Endian;                                // P-x\IDC5-n
    QByteArray                          InitValue;                             // P-x\IDC6-n
    QByteArray                          MFForInitValue;                        // P-x\IDC7-n
    QByteArray                          EndValue;                              // P-x\IDC8-n
    QByteArray                          MFForEndValue;                         // P-x\IDC9-n
    QByteArray                          CountDirection;                        // P-x\IDC10-n
    QByteArray                          NumberSubframeDefinitions;             // P-x\SF\N-n
    QHash<int, P_SubframeDefinition*>   SubframeDefinitions;

  public:
    inline P_SubframeID(QObject* parent = nullptr) : QObject(parent) {}
    inline ~P_SubframeID() { this->ClearHash(); }

    void Clear();

    static P_SubframeID* Get_P_SubframeID(int index, QObject *parent, QHash<int, P_SubframeID*> &SubframeIDs);
};

#endif // P_RECORD_H
