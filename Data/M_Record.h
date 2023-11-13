#ifndef M_RECORD_H
#define M_RECORD_H

#include <QObject>
#include "TmatsCode.h"

class P_Record;
class B_Record;

class M_Record : public QObject
{
  Q_OBJECT

  public: /* Public data variables */
    int                    RecordNumber         = -1;           // M-x
    QByteArray             DataSourceID;                        // M-x\ID
    QByteArray             BB_DataLinkName;                     // M-x\BB\DLN
    QByteArray             BasebandSignalType;                  // M-x\BSG1
    P_Record              *p_record             = nullptr;      // Corresponding P record
    B_Record              *b_record             = nullptr;      // Corresponding B record

  public:
    M_Record(QObject *parent = nullptr);
    ~M_Record();

    void Clear();

    void Decode(int code, QByteArray bytesData, QList<int> indexes);
    static M_Record* Get_M(int index, QObject *parent, QHash<int, M_Record*> &M_Records);
    static void Connect_M(QHash<int, M_Record*> &M_Records, QHash<int, B_Record*> &B_Records, QHash<int, P_Record*> &P_Records);
};

#endif // M_RECORD_H
