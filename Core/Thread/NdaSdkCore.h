#ifndef NDASDKCORE_H
#define NDASDKCORE_H

#include <Mag200Core.h>

#define NDA_ACQ         1000
#define NDA_STK         2000
#define NDA_TIME        3000
#define NDA_TIME_STK    4000
#define NDA_TIME_LENGTH 5000
#define NDA_SDK_CHANNEL 9999

#define NDA_SDK_LOCAL_ID 45
#define NB_READ_PTS_PER_CALL 5000

#define NDA_SDK_OK 0
#define NDA_SDK_IDLE 10
#define NDA_SDK_NO_CHANNEL -420

/*
 * @brief The NdaSdkCore class represents the core functionality of the NDA SDK.
 *
 * This class is derived from the Mag200Core class and is responsible for managing
 * the NDA SDK functionality, including initializing and terminating the SDK,
 * connecting to Magali and channels, reading time and sample data, handling errors,
 * and sending status updates.
 *
 * @date 31/05/2023
 */
class NdaSdkCore : public Mag200Core
{
  Q_OBJECT

  private:
    int m_State = 0;
    int Magali_ID = -1;
    QString CurrentChannel = "";

    bool IsChannelInit = false;
    bool IsConnectedToMagali = false;
    bool IsConnectedToChannel = false;

    bool lastCallStkStarted = true;
    bool lastCallAcqStarted = true;
    bool lastCallAcqStopped = false;

    int ReadTime();
    int ReadSample();
    int ChannelInit();
    int ConnectToMagali();
    int ConnectToChannel();

    int CheckError(int m_State);

  protected:
    void Processing();
    int Initialize();
    int Terminate();

  public:
    NdaSdkCore(QObject *parent = nullptr);
    ~NdaSdkCore();

    void SendAllStatus();
    void SetCurrentChannel(QString channel);

  signals:
    void SendNewStatus(int Channel, int Type, QVariant Value);
    void NewChannelData(QVector<double> ptsX, QVector<double> ptsY);
};

#endif // NDASDKCORE_H
