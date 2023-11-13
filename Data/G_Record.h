#ifndef G_RECORD_H
#define G_RECORD_H

#include <QObject>
#include "TmatsCode.h"

class R_Record;
class G_DataSource;

class G_Record : public QObject
{
  Q_OBJECT

  private:
    G_DataSource* Get_DataSource(QList<int> indexes);
    void ClearHash();

  public: /* Public data variables */
    QByteArray                  ProgramName;                                            // G\PN
    QByteArray                  Irig106Revision;                                        // G\106
    QByteArray                  OriginationDate;                                        // G\OD
    QByteArray                  NumberDataSources;                                      // G\DSI\N
    QHash<int, G_DataSource*>   G_DataSources;

  public:
    G_Record(QObject *parent = nullptr);
    ~G_Record();

    void Clear();

    void Decode(int code, QByteArray bytesData, QList<int> indexes);
    static G_Record* Get_G(int index, QObject *parent, QHash<int, G_Record*> &G_Records);
    static void Connect_G(QHash<int, G_Record*> &G_Records, QHash<int, R_Record*> &R_Records);
};



/* Datasource */

class G_DataSource : public QObject
{
  Q_OBJECT

  public: /* Public data variables */
    int                    DataSourceNumber = -1;                                  // G\XXX-n
    QByteArray             DataSourceID;                                           // G\DSI-n
    QByteArray             DataSourceType;                                         // G\DST-n
    R_Record              *r_record;                                               // Corresponding R record

  public:
    inline G_DataSource(QObject* parent = nullptr) : QObject(parent) {}
    inline ~G_DataSource() {}

    void Clear();

    static G_DataSource* Get_G_DataSource(int index, QObject *parent, QHash<int, G_DataSource*> &G_DataSources);
};

#endif // G_RECORD_H
