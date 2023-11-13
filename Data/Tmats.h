#ifndef TMATS_H
#define TMATS_H

#include <QObject>
#include "G_Record.h"
#include "R_Record.h"
#include "M_Record.h"
#include "B_Record.h"
#include "P_Record.h"

/* ------------------------------------------------------------------- */
/* Reworked from :                                                     */
/* https://github.com/atac/libirig106/blob/master/include/i106_tmats.h */
/* ------------------------------------------------------------------- */

class Tmats : public QObject
{
  Q_OBJECT

  private:
    QByteArray                  RawData;
    static QHash<QString, int>  WhiteListCodes;

    void ClearHash();
    void Initialize();
    int GetCode(QByteArray code, QList<int> &indexes);

  public: /* Public data variables */
    int                     C10Version          = -1;
    int                     ConfigChange        = -1;
    QHash< int, G_Record* >  G_Records;
    QHash< int, R_Record* >  R_Records;
    QHash< int, M_Record* >  M_Records;
    QHash< int, B_Record* >  B_Records;
    QHash< int, P_Record* >  P_Records;
    QHash< int, void*     >  T_Records;
    QHash< int, void*     >  D_Records;
    QHash< int, void*     >  S_Records;
    QHash< int, void*     >  A_Records;
    QHash< int, void*     >  C_Records;
    QHash< int, void*     >  H_Records;
    QHash< int, void*     >  V_Records;

  public:
    Tmats(QObject *parent = nullptr);
    ~Tmats();

    void Clear();

    void DecodeTmats(QString Tmats);
    void Decode_G(QByteArray bytesCode, QByteArray bytesData);
    void Decode_R(QByteArray bytesCode, QByteArray bytesData);
    void Decode_M(QByteArray bytesCode, QByteArray bytesData);
    void Decode_B(QByteArray bytesCode, QByteArray bytesData);
    void Decode_P(QByteArray bytesCode, QByteArray bytesData);
};

#endif // TMATS_DATA_H
