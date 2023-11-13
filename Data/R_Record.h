#ifndef R_RECORD_H
#define R_RECORD_H

#include <QObject>
#include "TmatsCode.h"

class M_Record;
class P_Record;
class R_DataSource;

class R_Record : public QObject
{
  Q_OBJECT

  private:
    R_DataSource* Get_DataSource(QList<int> indexes);

  public: /* Public data variables */
    int                         RecordNumber;              // R-x
    QByteArray                  DataSourceID;              // R-x\ID
    QByteArray                  NumberDataSources;         // R-x\N
    QByteArray                  RawIndexEnabled;           // R-x\IDX\E
    int                         IndexEnabled        = -1;  // Only valid if RawIndexEnabled != NULL
    QByteArray                  RawEventsEnabled;          // R-x\EVE\E
    int                         EventsEnabled       = -1;  // Only valid if RawEventsEnabled != NULL
    QHash<int, R_DataSource*>   R_DataSources;

  public:
    R_Record(QObject *parent = nullptr);
    ~R_Record();

    void Clear();
    void ClearHash();

    void Decode(int code, QByteArray bytesData, QList<int> indexes);

    static R_Record* Get_R(int index, QObject *parent, QHash<int, R_Record*> &R_Records);
    static void Connect_R(QHash<int, R_Record*> &R_Records, QHash<int, M_Record*> &M_Records, QHash<int, P_Record*> &P_Records);
};



/* Datasource */

class R_DataSource : public QObject
{
  Q_OBJECT

  public: /* Public data variables */
    int            DataSourceNumber       = -1;                                        // R-x\XXX-n
    QByteArray     DataSourceID;                                                       // R-x\DSI-n
    QByteArray     ChannelDataType;                                                    // R-x\CDT-n
    QByteArray     RawTrackNumber;                                                     // R-x\TK1-n
    int            TrackNumber            = -1;                                        // Only valid if RawTrackNumber != NULL
    QByteArray     RawEnabled;                                                         // R-x\CHE-n
    int            Enabled                = -1;                                        // Only valid if RawEnabled != NULL
    QByteArray     PCMDataLinkName;                                                    // R-x\PDLN-n (-04, -05)
    QByteArray     BusDataLinkName;                                                    // R-x\BDLN-n (-04, -05)
    QByteArray     ChannelDataLinkName;                                                // R-x\CDLN-n (-07, -09)

    /* Video channel attributes */
    QByteArray     VideoDataType;                                                      // (R-x\VTF-n)
    QByteArray     VideoEncodeType;                                                    // (R-x\VXF-n)
    QByteArray     VideoSignalType;                                                    // (R-x\VST-n)
    QByteArray     VideoSignalFormat;                                                  // (R-x\VSF-n)
    QByteArray     VideoConstBitRate;                                                  // (R-x\CBR-n)
    QByteArray     VideoVarPeakBitRate;                                                // (R-x\VBR-n)
    QByteArray     VideoEncodingDelay;                                                 // (R-x\VED-n)

    /* PCM channel attributes */
    QByteArray     PCMDataTypeFormat;                                                  // (R-x\PDTF-n)
    QByteArray     PCMDataPacking;                                                     // (R-x\PDP-n)
    QByteArray     PCMInputClockEdge;                                                  // (R-x\ICE-n)
    QByteArray     PCMInputSignalType;                                                 // (R-x\IST-n)
    QByteArray     PCMInputThreshold;                                                  // (R-x\ITH-n)
    QByteArray     PCMInputTermination;                                                // (R-x\ITM-n)
    QByteArray     PCMVideoTypeFormat;                                                 // (R-x\PTF-n)

    /* Analog channel attributes */
    QByteArray     AnalogChannelsPerPacket;                                            // (R-1\ACH\N-n)
    QByteArray     AnalogSampleRate;                                                   // (R-1\ASR-n)
    QByteArray     AnalogDataPacking;                                                  // (R-1\ADP-n)

    M_Record      *m_record                               = nullptr;                   // Corresponding M record
    P_Record      *p_record                               = nullptr;                   // Corresponding P record

  public:
    inline R_DataSource(QObject* parent = nullptr) : QObject(parent) {}
    inline ~R_DataSource() {}

    void Clear();

    static R_DataSource* Get_R_DataSource(int index, QObject *parent, QHash<int, R_DataSource*> &R_DataSources);
};

#endif // R_RECORD_H
