#include "stdafx.h"
#include "FileCopyCore.h"

const qint64 INITIAL_BUFFER_SIZE = 1024 * 1024;     // 1MB
const qint64 MAX_BUFFER_SIZE = 1024 * 1024 * 10;    // 10MB
const qint64 MIN_BUFFER_SIZE = 1024 * 512;          // 512KB

FileCopyCore::FileCopyCore(QObject *parent) : Mag200Core(parent)
{
  // We don't need sleep because we are copying file !
  this->m_UseSleep = false;
}

FileCopyCore::~FileCopyCore()
{

}

void FileCopyCore::InitFileCopy(QString src, QString dst)
{
  this->src = src;
  this->dst = dst;
}

int FileCopyCore::Initialize()
{
  if (QFileInfo::exists(src) == false)
    return -1;

  if (QFileInfo::exists(dst) == true)
    QFile::remove(dst);

  srcFile.setFileName(this->src);
  dstFile.setFileName(this->dst);

  if (srcFile.open(QIODevice::ReadOnly) == false)
    return -1;

  if (dstFile.open(QIODevice::ReadWrite) == false)
    return -1;

  dstFile.resize(srcFile.size());

  this->bufferSize = MAX_BUFFER_SIZE;
  this->totalBytesCopied = 0;
  this->lastChunkTime = 0;
  return 0;
}

void FileCopyCore::Processing()
{
  if (srcFile.size() == this->totalBytesCopied)
  {
    emit SendCopyProgress(srcFile.size());
    this->StopMag200Core();
    return;
  }

  if (srcFile.atEnd() == true)
    return;

  qint64 chunkSize = qMin(this->bufferSize, srcFile.size() - this->totalBytesCopied);

  uchar *sourceMap = srcFile.map(this->totalBytesCopied, chunkSize);
  uchar *destinationMap = dstFile.map(this->totalBytesCopied, chunkSize, QFileDevice::MapPrivateOption);

  if (sourceMap && destinationMap)
  {
    // Memory mapping copy
    memcpy(destinationMap, sourceMap, chunkSize);

    srcFile.unmap(sourceMap);
    dstFile.unmap(destinationMap);
  }
  else
  {
    // Regular copy
    QByteArray buffer = srcFile.read(chunkSize);
    dstFile.write(buffer);
  }

  this->totalBytesCopied += chunkSize;
  emit SendCopyProgress(this->totalBytesCopied);

  return;
}

int FileCopyCore::Terminate()
{
  isReady = false;
  this->src = QString();
  this->dst = QString();

  srcFile.close();
  dstFile.close();

  if (srcFile.size() != this->totalBytesCopied)
  {
    dstFile.remove();
    emit SendCopyProgress(0);
  }

  return 0;
}