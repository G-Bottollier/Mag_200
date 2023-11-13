#include "stdafx.h"
#include "NdaSdkCore.h"

#include <nda_sdk.h>

/*
 * @brief NdaSdkCore constructor.
 * @param parent The parent QObject.
 *
 * @date 31/05/2023 
 */
NdaSdkCore::NdaSdkCore(QObject *parent) : Mag200Core(parent)
{
  qRegisterMetaType<QVector<double>>();
}

/*
 * @brief NdaSdkCore destructor.
 *
 * @date 31/05/2023 
 */
NdaSdkCore::~NdaSdkCore()
{

}

/*
 * @brief Sends the status of all components.
 * Emits signals to send the state of ACQ (Acquisition) and STR (Real-time) components.
 *
 * @date 31/05/2023 
 */
void NdaSdkCore::SendAllStatus()
{
  m_Mutex.lock();

  //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_TIME, tr("STR_NO_TIME"));
  //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_ACQ, lastCallAcqStarted);
  //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_STK, lastCallStkStarted);

  m_Mutex.unlock();
}

/*
 * @brief Perform the processing tasks for the NdaSdkCore.
 * This method is called periodically every CLOCK_SPEED
 *
 * This function is responsible for performing the necessary processing tasks in the NdaSdkCore.
 * It checks for any errors from the previous iteration, connects to Magali, reads the time, initializes
 * channels, connects to channels, and reads a sample. If an m_State occurs during any of these steps, the
 * function returns early without further processing.
 *
 * After completing the processing tasks, the function updates the last m_State state to indicate success
 * (NDA_SDK_OK).
 *
 * @date 31/05/2023
 */
void NdaSdkCore::Processing()
{
  // Check for any previous errors
  m_State = CheckError(m_State);
  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
    return;

  // Connect to Magali
  m_State = ConnectToMagali();
  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
    return;

  // Read time
  m_State = ReadTime();
  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
    return;

  // Initialize channels
  m_State = ChannelInit();
  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
    return;

  // Connect to channels
  m_State = ConnectToChannel();
  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
    return;

  // Read sample
  m_State = ReadSample();
  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
    return;

  // Set the last m_State to indicate success
  this->m_LastError = NDA_SDK_OK;
}

/*
 * @brief Initializes the NDA SDK.
 * @return Returns an m_State code.
 *
 * @date 31/05/2023 
 */
int NdaSdkCore::Initialize()
{
  int m_State;

  // Initialize NDA SDK
  m_State = NDASDK_Init(NDA_SDK_LOCAL_ID);
  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
  {
    // Emit m_State signal if initialization fails
    //emit SendNewError(tr("FAIL_INIT_NDA_SDK"));
    return -1;
  }

  // Set initial states
  this->IsChannelInit = false;
  this->IsConnectedToMagali = false;
  this->IsConnectedToChannel = false;

  // Emit initial time state
  //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_TIME, tr("STR_NO_TIME"));

  // Emit initial acquisition states
  //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_ACQ, false);
  //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_STK, false);

  // Set last call status flags
  this->lastCallStkStarted = false;
  this->lastCallAcqStarted = false;
  this->lastCallAcqStopped = true;

  return m_State;
}

/*
 * @brief Terminates the NDA SDK.
 * @return Returns an m_State code.
 *
 * @date 31/05/2023 
 */
int NdaSdkCore::Terminate()
{
  int m_State = 0;

  // Stop the connection to Magali
  m_State = NDASDK_StopConnection(Magali_ID);

  // Close the NDA SDK
  m_State = NDASDK_Close();

  return m_State;
}

/*
 * @brief Reads the current time from the NDA SDK.
 * @return Returns an m_State code.
 *
 * @date 31/05/2023 
 */
int NdaSdkCore::ReadTime()
{
  int m_State = 0;
  QString strDate;
  double date = 0.0;
  double length = 0.0;
  double dateRef = 0.0;
  double GMTdateRef = 0.0;

  // Get the date reference for STR
  m_State = NDASDK_GetDateRefStk(Magali_ID, &dateRef, &GMTdateRef, &length);
  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
    return m_State;

  // Get the STK Date
  date = GMTdateRef + length;
  strDate = Tools::DateToString(date);

  // Append the date reference to the log
  this->Log.append(tr("DATE_REF_STK") + ";" + strDate);


  // Use ACQ Time if STR Time is null
  if (GMTdateRef != 0.0)
  {
    //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_TIME_STK, date);

    //if(this->lastCallStkStarted == true)
      //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_TIME_LENGTH, length * 1000);
    return m_State;
  }

  // Get the date reference for ACQ
  m_State = NDASDK_GetDateRef(Magali_ID, &dateRef, &GMTdateRef, &length);
  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
    return m_State;

  // Get the Date
  date = GMTdateRef + length;
  strDate = Tools::DateToString(date);

  // Append the date reference to the log
  this->Log.append(tr("DATE_REF") + ";" + strDate);

  // Emit the signal to send the new time
  //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_TIME, date);

  return m_State;
}

/*
 * @brief Reads a sample from the NDA SDK.
 * @return Returns an m_State code.
 *
 * @date 31/05/2023 
 */
int NdaSdkCore::ReadSample()
{
  int nb = 0;
  int m_State = 0;
  double arrX[NB_READ_PTS_PER_CALL]{ 0 };
  double arrY[NB_READ_PTS_PER_CALL]{ 0 };

  // Check if the current channel is empty
  if (this->CurrentChannel.isEmpty() == true)
  {
    return -1;
  }

  // Read all samples from the current channel
  m_State = NDASDK_ReadAllSamples(Magali_ID, CurrentChannel.toUtf8().data(), NB_READ_PTS_PER_CALL, arrX, arrY, &nb);
  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
  {
    return m_State;
  }

  // Append the number of read samples to the log
  this->Log.append(tr("NB_READ_SAMPLES") + ";" + QString::number(nb));

  // If no samples were read, return NDA_SDK_IDLE
  if (nb == 0)
  {
    return NDA_SDK_IDLE;
  }

  // Convert the sample arrays to QVector
  QVector<double> ptsX(nb);
  QVector<double> ptsY(nb);

  memcpy(ptsX.data(), arrX, sizeof(double) * nb);
  memcpy(ptsY.data(), arrY, sizeof(double) * nb);

  // Emit the signal to send the new channel data
  //emit NewChannelData(ptsX, ptsY);

  return m_State;
}

/*
 * @brief Initializes the NDA SDK channels.
 * @return Returns an m_State code.
 *
 * @date 31/05/2023 
 */
int NdaSdkCore::ChannelInit()
{
  int m_State = 0;

  // Check if the channel is already initialized
  if (this->IsChannelInit == true)
    return m_State;

  // Set X RELATIVE INC Mode
  m_State = NDASDK_SetXMode(Magali_ID, NDASDK_X_RELATIVE_INC);
  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
    return m_State;

  // Query Channels to enable data readable
  int nb;
  char **channels;
  m_State = NDASDK_QueryChannels(Magali_ID, &channels, &nb);
  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
    return m_State;

  // Update the channel initialization status and append the information to the log
  this->IsChannelInit = true;
  this->Log.append(tr("IS_CHANNEL_INIT") + ";" + (IsChannelInit ? "true" : "false"));
  this->Log.append(tr("NB_CHANNEL") + ";" + QString::number(nb));

  return m_State;
}

/*
 * @brief Connects to Magali in the NDA SDK.
 * @return Returns NDA_SDK_IDLE if the connection was successful, or an m_State code if there was an m_State.
 *
 * @date 31/05/2023 
 */
int NdaSdkCore::ConnectToMagali()
{
  int isOK = -1;
  int m_State = 0;

  // If already connected, check status and emit signals
  if (this->IsConnectedToMagali == true)
  {
    // Check STK status
    isOK = -1;
    m_State = NDASDK_IsStkStarted(Magali_ID, &isOK);
    if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
      return m_State;

    bool isStkStarted = isOK == 1;
    this->Log.append(tr("IS_STK_STARTED") + ";" + (isStkStarted ? "true" : "false"));

    if (isStkStarted != lastCallStkStarted)
    {
      this->lastCallStkStarted = isStkStarted;
      //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_STK, isStkStarted);
    }

    // Check ACQ status
    isOK = -1;
    m_State = NDASDK_IsStarted(Magali_ID, &isOK);
    if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
      return m_State;

    bool isStarted = isOK == 1;
    this->Log.append(tr("IS_ACQ_STARTED") + ";" + (isStarted ? "true" : "false"));

    if (isStarted != lastCallAcqStarted)
    {
      this->lastCallAcqStarted = isStarted;
      //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_ACQ, isStarted);
    }

    if (isStarted == true)
      return m_State;

    // Check ACQ !status
    isOK = -1;
    m_State = NDASDK_IsStopped(Magali_ID, &isOK);
    if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
      return m_State;

    bool isStopped = isOK == 1;
    this->Log.append(tr("IS_ACQ_STOPPED") + ";" + (isStopped ? "true" : "false"));

    if (isStopped != lastCallAcqStopped)
    {
      this->lastCallAcqStopped = isStopped;
      //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_ACQ, !isStopped);
    }

    if (isStopped == true)
      return NDA_SDK_IDLE;
  }

  // Attempt to connect to Magali
  //emit StartProcessing(tr("CONNECT_TO_MAGALI"), "");

  // Get Connection ID
  m_State = NDASDK_AddLocalConnection(&Magali_ID);
  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
    return m_State;

  this->Log.append(tr("MAGALI_ID") + ";" + QString::number(Magali_ID));

  // Start Connection
  m_State = NDASDK_StartConnection(Magali_ID);
  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
    return m_State;

  // Set flag to true
  this->IsConnectedToMagali = true;
  this->Log.append(tr("IS_CONNECTED") + ";" + (IsConnectedToMagali ? "true" : "false"));

  // Connected to Magali
  //emit EndedProcessing();

  // Wait for real-time startup
  return NDA_SDK_IDLE;
}

/*
 * @brief Connects to the NDA SDK channel.
 * @return Returns an m_State code.
 *
 * @date 31/05/2023 
 */
int NdaSdkCore::ConnectToChannel()
{
  int m_State = 0;

  // Check if already connected to the channel
  if (this->IsConnectedToChannel == true)
    return m_State;

  // Check if the current channel is empty
  if (this->CurrentChannel.isEmpty() == true)
    return NDA_SDK_IDLE;

  // Convert current channel to QByteArray
  QByteArray byteChannel = this->CurrentChannel.toUtf8();

  // Connect to the channel
  m_State = NDASDK_ConnectChannel(Magali_ID, byteChannel.data(), NB_READ_PTS_PER_CALL);
  if (m_State == T_NDA_ERROR::NDASDK_COMMAND_FAILED)
    return m_State;

  if (m_State != T_NDA_ERROR::NDASDK_SUCCESS)
    return m_State;

  // Update the connected channel status and append the information to the log
  this->IsConnectedToChannel = true;
  this->Log.append(tr("CHANNEL_NAME") + ";" + CurrentChannel);
  this->Log.append(tr("CHANNEL_CONNECTED") + ";" + (IsConnectedToChannel ? "true" : "false"));

  return m_State;
}

/*
 * @brief Sets the current channel in the NDA SDK.
 * @param channel The new channel to set.
 *
 * @date 31/05/2023 
 */
void NdaSdkCore::SetCurrentChannel(QString channel)
{
  // Lock the mutex to ensure thread safety
  m_Mutex.lock();

  // Disconnect from the current channel if already connected
  if (IsConnectedToChannel == true)
    NDASDK_DisconnectChannel(Magali_ID, CurrentChannel.toUtf8().data());

  // Set the IsConnectedToChannel flag to false
  IsConnectedToChannel = false;

  // Set the new channel as the current channel
  CurrentChannel = channel;

  // Unlock the mutex
  m_Mutex.unlock();
}

/*
 * @brief Checks for any errors in the NDA SDK.
 * @param m_State The current m_State code.
 * @return Returns the updated m_State code.
 *
 * @date 31/05/2023 
 */
int NdaSdkCore::CheckError(int m_State)
{
  this->Log.append(tr("LAST_ERROR") + ";" + QString::number(m_State));

  // Check if there is no m_State
  if (m_State == 0)
  {
    // Update the last m_State if it's different
    if (this->m_LastError != m_State)
      this->m_LastError = m_State;

    return NDA_SDK_OK;
  }

  // Void loop until there is something to do
  if (m_State == NDA_SDK_IDLE)
    return NDA_SDK_OK;

  QString strError = "";
  int checkError = NDA_SDK_OK;

  bool isDisplayable = this->m_LastError != m_State;
  if (isDisplayable == true)
    this->m_LastError = m_State;
  int chk = 0;
  switch (m_State)
  {
    // Handle specific m_State codes
  case NDASDK_TIMEOUT:
  case NDASDK_COMMAND_FAILED:
    return NDA_SDK_OK;

  case NDASDK_UNAVAILABLE_ID:
    //emit UpdateProcessing(tr("NDA_SDK_UNAVAILABLE_ID"), tr("RETRY_5_SEC"));
    QThread::sleep(5);
    return NDA_SDK_OK;

  case NDASDK_NO_CONNECTION_AVAILABLE:
    strError = tr("NDA_SDK_NO_CONNECTION_AVAILABLE");

  case NDASDK_SOCKET_ERROR:
    strError = tr("SOCKET_ERROR");
    //emit UpdateProcessing(strError, tr("RETRY_5_SEC"));
    QThread::sleep(5);

  case NDASDK_NO_CONNECTION:
    if (this->Magali_ID >= 0)
      checkError = NDASDK_StopConnection(this->Magali_ID);

    // Close the NDA SDK
    chk = NDASDK_Close();

    // Initialize NDA SDK
    chk = NDASDK_Init(NDA_SDK_LOCAL_ID);

    this->Magali_ID = -1;

    this->IsChannelInit = false;
    this->IsConnectedToMagali = false;
    this->IsConnectedToChannel = false;

    //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_TIME, tr("STR_NO_TIME"));
    //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_ACQ, false);
    //emit SendNewStatus(NDA_SDK_CHANNEL, NDA_STK, false);

    this->lastCallStkStarted = false;
    this->lastCallAcqStarted = false;
    this->lastCallAcqStopped = true;

    return checkError;

  case NDA_SDK_NO_CHANNEL:
    if (isDisplayable == false)
      return NDA_SDK_OK;

    //emit SendNewError(tr("NDA_SDK_NO_CHANNEL"));
    return NDA_SDK_OK;

    // Handle various m_State codes
  case NDASDK_NO_ID_SELECTED:
  case NDASDK_ITEM_ALREADY_EXISTS:
  case NDASDK_ITEM_DOES_NOT_EXISTS:
  case NDASDK_CHANNEL_DOES_NOT_EXISTS:
  case NDASDK_FILE_OPEN_ERROR:
  case NDASDK_NET_SEND_ERROR:
  case NDASDK_NET_RECV_ERROR:
  case NDASDK_NDA_MSG_ERROR:
  case NDASDK_CANCELLED:
  case NDASDK_ERROR_ON_ARGUMENT:
  case NDASDK_MEMORY_ALLOC_ERROR:
  case NDASDK_COMMAND_EXECUTION_ERROR:
  case NDASDK_ERROR_SYSTEM:
  case NDASDK_INVALID_DESC:
  case NDASDK_INVALID_PARAMETER:
  case NDASDK_INVALID_DEVICE:
  case NDASDK_CHANNEL_VALUE_NOT_FOUND:
  case NDASDK_ALREADY_INITIALIZED:
  case NDASDK_NOT_INITIALIZED:
  case NDASDK_OVERFLOW:
  case NDASDK_INSUFFICIENT_RESS:
  case NDASDK_NO_AVAILABLE_NETWORK:
  case NDASDK_CHANNEL_CREATE_FAILED:
  case NDASDK_CHANNEL_ALREADY_EXISTS:
  case NDASDK_SRC_TA_ERROR:
  case NDASDK_SRC_UNIT_ERROR:
  case NDASDK_SRC_LEVEL1_ERROR:
  case NDASDK_SRC_LEVEL2_ERROR:
  case NDASDK_DEST_UNIT_ERROR:
  case NDASDK_DEST_LEVEL1_ERROR:
  case NDASDK_DEST_LEVEL2_ERROR:
  case NDASDK_DESTINATION_EXIST:
  case NDASDK_SRC_TA_LOCKED:
  case NDASDK_PP_NO_ALLOWED:
    //int a = 0;
    break;
  }

  return NDA_SDK_OK;
}