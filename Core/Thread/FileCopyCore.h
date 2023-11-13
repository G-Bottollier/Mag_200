#ifndef FILECOPYCORE_H
#define FILECOPYCORE_H

#include "Mag200Core.h"

class FileCopyCore : public Mag200Core
{
  Q_OBJECT

  private:
    QString src;
    QString dst;
    QFile srcFile;
    QFile dstFile;

    bool isReady = false;

    qint64 bufferSize = 0;
    qint64 lastChunkTime = 0;
    qint64 totalBytesCopied = 0;

  protected:
    void Processing();
    int Initialize();
    int Terminate();

  public:
    FileCopyCore(QObject *parent = nullptr);
    ~FileCopyCore();

    void InitFileCopy(QString src, QString dst);

  signals:
    void SendCopyProgress(qint64 bytesCopied);
};

#endif // FILECOPYCORE_H
