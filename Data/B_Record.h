#ifndef B_RECORD_H
#define B_RECORD_H

#include <QObject>
#include "TmatsCode.h"

class B_Record : public QObject
{
  Q_OBJECT

  public: /* Public data variables */
    int                    RecordNumber = -1;                                           // B-x
    QByteArray             DataLinkName;                                                // B-x\DLN
    QByteArray             NumberBuses;                                                 // B-x\NBS\N

  public:
    B_Record(QObject *parent = nullptr);
    ~B_Record();

    void Clear();

    void Decode(int code, QByteArray bytesData, QList<int> indexes);
    static B_Record* Get_B(int index, QObject *parent, QHash<int, B_Record*> &B_Records);
};

#endif // B_RECORD_H
