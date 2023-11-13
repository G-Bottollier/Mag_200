#include "stdafx.h"
#include "Mag200Controller.h"

/*
* @brief The Mag200Controller class represents the controller for the MAG200 application.
*
* This class is responsible for managing the core components of the MAG200 application, including
* the NdaSdkCore and SharedMemoryCore, and handling the communication between them and the GUI.
*
* @date 31/05/2023 
*/
Mag200Controller::Mag200Controller(QObject *parent) : QObject(parent)
{
  // Setup  
  Setup();

  // Setup event handlers
  SetupEvent();
}

/*
 * @brief Destructor for the Mag200Controller class.
 *
 * The destructor cleans up and ensures the proper termination of the NdaSdkThread and SharedMemoryThread.
 * It requests the threads to exit and waits for them to finish before the Mag200Controller object is destroyed.
 *
 * @date 31/05/2023
 */
Mag200Controller::~Mag200Controller()
{
  // Stop all threads
  this->StopMag200Core();

  // Delete Threads
  m_NdaSdkThread->deleteLater();
  m_SharedMemoryThread->deleteLater();
  m_Chapter6Thread->deleteLater();
  m_FileCopyThread->deleteLater();
}

/*
 * @brief Initializes the core components of the MAG200 application.
 *
 * This function creates instances of NdaSdkCore and SharedMemoryCore, and their respective threads.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::Setup()
{
  // Create an instance of NdaSdkCore and its own QThread
  m_NdaSdkCore = new NdaSdkCore();
  m_NdaSdkThread = new QThread;

  // Create an instance of SharedMemoryCore and its own QThread
  m_SharedMemoryCore = new SharedMemoryCore();
  m_SharedMemoryThread = new QThread;

  m_Chapter6Core = new Chapter6Core();
  m_Chapter6Thread = new QThread;

  m_FileCopyCore = new FileCopyCore();
  m_FileCopyThread = new QThread;
}

/*
 * @brief Sets up the event connections between the components of the MAG200 application.
 *
 * This function establishes the connections between the threads, cores, and the controller.
 * It connects the signals and slots to enable communication between the components.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::SetupEvent()
{
  // Thread
  QObject::connect(m_NdaSdkThread, &QThread::finished, m_NdaSdkThread, &QObject::deleteLater);
  QObject::connect(m_NdaSdkThread, &QThread::started, m_NdaSdkCore, &Mag200Core::StartMag200Core);

  QObject::connect(m_SharedMemoryThread, &QThread::finished, m_SharedMemoryThread, &QObject::deleteLater);
  QObject::connect(m_SharedMemoryThread, &QThread::started, m_SharedMemoryCore, &Mag200Core::StartMag200Core);

  QObject::connect(m_Chapter6Thread, &QThread::finished, m_Chapter6Thread, &QObject::deleteLater);
  QObject::connect(m_Chapter6Thread, &QThread::started, m_Chapter6Core, &Mag200Core::StartMag200Core);

  QObject::connect(m_FileCopyThread, &QThread::started, m_FileCopyCore, &Mag200Core::StartMag200Core);

  // Core
  QObject::connect(m_NdaSdkCore, &Mag200Core::StartProcessing, this, &Mag200Controller::StartProcessing);
  QObject::connect(m_NdaSdkCore, &Mag200Core::EndedProcessing, this, &Mag200Controller::EndedProcessing);
  QObject::connect(m_NdaSdkCore, &Mag200Core::UpdateProcessing, this, &Mag200Controller::UpdateProcessing);

  QObject::connect(m_SharedMemoryCore, &Mag200Core::StartProcessing, this, &Mag200Controller::StartProcessing);
  QObject::connect(m_SharedMemoryCore, &Mag200Core::EndedProcessing, this, &Mag200Controller::EndedProcessing);
  QObject::connect(m_SharedMemoryCore, &Mag200Core::UpdateProcessing, this, &Mag200Controller::UpdateProcessing);

  QObject::connect(m_Chapter6Core, &Mag200Core::StartProcessing, this, &Mag200Controller::StartProcessing);
  QObject::connect(m_Chapter6Core, &Mag200Core::EndedProcessing, this, &Mag200Controller::EndedProcessing);
  QObject::connect(m_Chapter6Core, &Mag200Core::UpdateProcessing, this, &Mag200Controller::UpdateProcessing);

  // NDASDK
  QObject::connect(m_NdaSdkCore, &NdaSdkCore::SendNewStatus, this, &Mag200Controller::NewStatus);

  QObject::connect(m_NdaSdkCore, &Mag200Core::SendNewLog, this, &Mag200Controller::NewLogNdaSdk);

  QObject::connect(m_NdaSdkCore, &Mag200Core::SendNewInfo, this, &Mag200Controller::NewInfo);
  QObject::connect(m_NdaSdkCore, &Mag200Core::SendNewError, this, &Mag200Controller::NewError);
  QObject::connect(m_NdaSdkCore, &Mag200Core::SendNewWarning, this, &Mag200Controller::NewWarning);
  QObject::connect(m_NdaSdkCore, &NdaSdkCore::NewChannelData, this, &Mag200Controller::NewChannelData);

  // SHARED MEMORY
  QObject::connect(m_SharedMemoryCore, &SharedMemoryCore::SendNewTestArea, this, &Mag200Controller::NewTestArea);
  QObject::connect(m_SharedMemoryCore, &SharedMemoryCore::SendNewStatus, this, &Mag200Controller::NewStatus);

  QObject::connect(m_SharedMemoryCore, &Mag200Core::SendNewLog, this, &Mag200Controller::NewLogSharedMemory);

  QObject::connect(m_SharedMemoryCore, &Mag200Core::SendNewInfo, this, &Mag200Controller::NewInfo);
  QObject::connect(m_SharedMemoryCore, &Mag200Core::SendNewError, this, &Mag200Controller::NewError);
  QObject::connect(m_SharedMemoryCore, &Mag200Core::SendNewWarning, this, &Mag200Controller::NewWarning);

  // FILE COPY
  QObject::connect(m_FileCopyCore, &FileCopyCore::SendCopyProgress, this, &Mag200Controller::NewCopyProgress);
  QObject::connect(m_FileCopyCore, &Mag200Core::finished, this, &Mag200Controller::ResetCopyThread);
  
  // CHAPTER 6
  QObject::connect(qCommands, &CommandsManager::SendNewCommand, this, &Mag200Controller::NewCommand);
  QObject::connect(m_Chapter6Core, &Chapter6Core::SendConnected, qCommands, &CommandsManager::Connected, Qt::QueuedConnection);
  QObject::connect(m_Chapter6Core, &Chapter6Core::SendDisconnected, qCommands, &CommandsManager::Disconnected, Qt::QueuedConnection);
  QObject::connect(m_Chapter6Core, &Chapter6Core::SendNewAcknowledge, qCommands, &CommandsManager::NewAcknowledge, Qt::QueuedConnection);
}

/*
 * @brief Starts the MAG200 core components.
 *
 * This function moves the NdaSdkCore and SharedMemoryCore objects to their respective threads
 * and starts the threads.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::StartMag200Core()
{
  // Move NdaSdkCore to its dedicated QThread and run it
  //m_NdaSdkCore->moveToThread(m_NdaSdkThread);
  //m_NdaSdkThread->start();

  // Move SharedMemoryCore to its dedicated QThread and run it
  m_SharedMemoryCore->moveToThread(m_SharedMemoryThread);
  m_SharedMemoryThread->start();

  // Move Chapter6Core to its dedicated QThread and run it
  m_Chapter6Core->moveToThread(m_Chapter6Thread);
  m_Chapter6Thread->start();
}

/*
 * @brief Stops the MAG200 core components.
 *
 * This function stops the NdaSdkCore and SharedMemoryCore components.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::StopMag200Core()
{
  // Stop all Mag200Core Thread
  m_NdaSdkCore->StopMag200Core();
  m_SharedMemoryCore->StopMag200Core();
  m_Chapter6Core->StopMag200Core();
}

/*
 * @brief Pauses the MAG200 core components.
 *
 * This function pauses the NdaSdkCore and SharedMemoryCore components.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::PauseMag200Core()
{
  // Pause all Mag200Core Thread
  m_NdaSdkCore->PauseMag200Core();
  m_SharedMemoryCore->PauseMag200Core();
  m_Chapter6Core->PauseMag200Core();
}

/*
 * @brief Unpauses the MAG200 core components.
 *
 * This function unpauses the NdaSdkCore and SharedMemoryCore components.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::UnPauseMag200Core()
{
  // UnPause all Mag200Core Thread
  m_NdaSdkCore->UnPauseMag200Core();
  m_SharedMemoryCore->UnPauseMag200Core();
  m_Chapter6Core->UnPauseMag200Core();
}

/*
 * @brief Sends the status of all components.
 *
 * This function sends the status of the SharedMemoryCore and NdaSdkCore components.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::SendAllStatus()
{
  // Send all status of each Mag200Core Thread
  m_SharedMemoryCore->SendAllStatus();
  m_NdaSdkCore->SendAllStatus();
  m_Chapter6Core->SendAllStatus();
}

/*
 * @brief Starts the processing.
 *
 * This function opens the processing dialog with the given title and description.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::StartProcessing(QString title, QString description)
{
  // Set the scroll range of qMessages
  qMessages->SetScrollRange(0, 10);

  // Set the title and description of qMessages
  qMessages->SetTitle(title);
  qMessages->SetDescription(description);

  // Enable the waiting mode of qMessages
  qMessages->EnableWaitingMode();

  // Open qMessages
  qMessages->Open();
}

/*
 * @brief Updates the processing dialog.
 *
 * This function updates the processing dialog with the given title and description.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::UpdateProcessing(QString title, QString description)
{
  // Set the title and description of qMessages
  qMessages->SetTitle(title);
  qMessages->SetDescription(description);
}

/*
 * @brief Ends the processing.
 *
 * This function closes the processing dialog.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::EndedProcessing()
{
  // Close the qMessages dialog
  qMessages->Close();
}

/*
 * @brief Handles new log entries from the NdaSdkCore.
 *
 * This function forwards the new log entries to the message dialog for display.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::NewLogNdaSdk(QStringList list)
{
  // Pass the log list to qMessages for NDA SDK logs
  qMessages->NewLog(LOG_NDA_SDK, list);
}

/*
 * @brief Handles new log entries from the SharedMemoryCore.
 *
 * This function forwards the new log entries to the message dialog for display.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::NewLogSharedMemory(QStringList list)
{
  // Pass the log list to qMessages for Shared Memory logs
  qMessages->NewLog(LOG_SHARED_MEMORY, list);
}

/*
 * @brief Handles new info messages.
 *
 * This function forwards the new info messages to the message dialog for display.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::NewInfo(QString strInfo)
{
  // Check if the new info is the same as the last status
  if (this->LastStatus == strInfo)
    return;

  // Update the last status with the new info
  this->LastStatus = strInfo;

  // Pass the new info to qMessages for display
  qMessages->NewInfo(strInfo);
}

/*
 * @brief Handles new m_State messages.
 *
 * This function forwards the new m_State messages to the message dialog for display.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::NewError(QString strError)
{
  // Check if the new m_State is the same as the last status
  if (this->LastStatus == strError)
    return;

  // Update the last status with the new m_State
  this->LastStatus = strError;

  // Pass the new m_State to qMessages for display
  qMessages->NewError(strError);
}

/*
 * @brief Handles new warning messages.
 *
 * This function forwards the new warning messages to the message dialog for display.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::NewWarning(QString strWarning)
{
  // Check if the new warning is the same as the last status
  if (this->LastStatus == strWarning)
    return;

  // Update the last status with the new warning
  this->LastStatus = strWarning;

  // Pass the new warning to qMessages for display
  qMessages->NewWarning(strWarning);
}

/*******************/
/*  RESEND SIGNAL  */
/*******************/
void Mag200Controller::NewTime(QString date)                                      { emit SendNewTime(date);                     }
void Mag200Controller::NewState_ACQ(bool state)                                   { emit SendNewState_ACQ(state);               }
void Mag200Controller::NewState_STK(bool state)                                   { emit SendNewState_STR(state);               }

void Mag200Controller::NewTestArea(QString testArea)                              { emit SendNewTestArea(testArea);             }
void Mag200Controller::NewStatus(int ChannelID, int Type, QVariant Value)         { emit SendNewStatus(ChannelID, Type, Value); }

void Mag200Controller::NewChannelData(QVector<double> ptsX, QVector<double> ptsY) { emit SendNewChannelData(ptsX, ptsY);        }

void Mag200Controller::NewCopyProgress(qint64 progress)                           { emit SendCopyProgress(progress);            }


/*******************/
/*      SLOTS      */
/*******************/

/*
 * @brief Sends the status from a specific index to the SharedMemoryCore.
 * @param index The index from which the status will be sent.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::SendStatusFromIndex(int index)
{
  // Forward the request to the SharedMemoryCore to send status from a specific index
  m_SharedMemoryCore->SendStatus(index);
}

/*
 * @brief Starts reading logs by calling the corresponding methods in NdaSdkCore and SharedMemoryCore.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::StartReadLog()
{
  // Start reading logs by calling the corresponding methods in both NdaSdkCore and SharedMemoryCore
  m_NdaSdkCore->StartReadLog();
  m_SharedMemoryCore->StartReadLog();
}

/*
 * @brief Stops reading logs by calling the corresponding methods in NdaSdkCore and SharedMemoryCore.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::StopReadLog()
{
  // Stop reading logs by calling the corresponding methods in both NdaSdkCore and SharedMemoryCore
  m_NdaSdkCore->StopReadLog();
  m_SharedMemoryCore->StopReadLog();
}

/*
 * @brief Starts reading channel data for a specific channel in NdaSdkCore.
 * @param channel The channel for which the data will be read.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::StartReadChannelData(QString channel)
{
  // Set the current channel for reading in NdaSdkCore
  m_NdaSdkCore->SetCurrentChannel(channel);
}

/*
 * @brief Stops reading channel data by clearing the current channel for reading in NdaSdkCore.
 *
 * @date 31/05/2023 
 */
void Mag200Controller::StopReadChannelData()
{
  // Clear the current channel for reading in NdaSdkCore
  m_NdaSdkCore->SetCurrentChannel(QString());
}

void Mag200Controller::NewCommand(int code, QStringList args)
{
  m_Chapter6Core->NewCommand(code, args);
}

void Mag200Controller::NewCopyFile(QString src, QString dst)
{
  m_FileCopyCore->InitFileCopy(src, dst);

  if(m_FileCopyCore->thread() != m_FileCopyThread)
    m_FileCopyCore->moveToThread(m_FileCopyThread);
  m_FileCopyThread->start();
}

void Mag200Controller::NewCancelCopy()
{
  m_FileCopyCore->StopMag200Core();
}

void Mag200Controller::ResetCopyThread()
{
  // Create a new thread and move the FileCopyCore object to that thread by invoking QObject::moveToThread() from its current thread
  QThread *newThread = new QThread();
  m_FileCopyCore->InvokeMoveToThread(newThread);

  m_FileCopyThread->quit();
  m_FileCopyThread->wait();
  m_FileCopyCore->disconnect(m_FileCopyThread);

  delete  m_FileCopyThread;
  m_FileCopyThread = nullptr;

  m_FileCopyThread = newThread;

  QObject::connect(m_FileCopyThread, &QThread::finished, m_FileCopyThread, &QObject::deleteLater);
  QObject::connect(m_FileCopyThread, &QThread::started, m_FileCopyCore, &Mag200Core::StartMag200Core);
}
