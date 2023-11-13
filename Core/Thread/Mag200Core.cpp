#include "stdafx.h"
#include "Mag200Core.h"

/*
* @brief Constructor for the Mag200Core class.
*
* This constructor creates an instance of the Mag200Core class.
*
* @param parent: The parent QObject.
*
* @date 31/05/2023 
*/
Mag200Core::Mag200Core(QObject *parent) : QObject(parent)
{

}

/*
* @brief Destructor for the Mag200Core class.
*
* This destructor cleans up any resources used by the Mag200Core class.
*
* @date 31/05/2023 
*/
Mag200Core::~Mag200Core()
{

}

/*
* @brief Slot function to start the Mag200Core.
*
* This slot function is called to start the Mag200Core execution.
* It enters a loop and repeatedly calls the Processing function until the stop flag is set.
*
* @date 31/05/2023 
*/
void Mag200Core::StartMag200Core()
{
  emit started();

  this->m_HasToStop = false;
  this->m_HasToPause = false;

  // Initialize the Mag200Core
  if (this->Initialize() != 0)
    this->m_HasToStop = true;

  while (true)
  {
    // Clear the log
    //this->LogClear();

    // Delay execution for the specified clock speed
    if(this->m_UseSleep)
    QThread::msleep(CLOCK_SPEED);

    // Check if the process has to stop
    if (this->m_HasToStop == true)
      break;

    // Check if the process has to pause
    if (this->m_HasToPause == true)
      continue;

    // Perform the processing
    this->Processing();

    // Log the current time
    //this->LogTime();
  }

  // Terminate the Mag200Core
  this->Terminate();

  emit finished();
}

void Mag200Core::MoveToThread(QThread * thread)
{
  this->moveToThread(thread);
}

/*
* @brief Function to clear the log.
*
* This function clears the log data and restarts the timer.
*
* @date 31/05/2023 
*/
void Mag200Core::LogClear()
{
  // Clear and restart the timer
  timer.restart();
  this->Log.clear();
}

/*
* @brief Function to log the time.
*
* This function updates the time-related variables and adds relevant information to the log.
*
* @date 31/05/2023 
*/
void Mag200Core::LogTime()
{
  // Update values for the Log
  this->m_TotalTime += this->timer.elapsed() - CLOCK_SPEED;
  this->m_TimeElapsed = this->timer.elapsed() - CLOCK_SPEED;
  this->m_AverageTime = this->m_TotalTime / this->m_NbProcessed;
  this->m_NbProcessed++;

  // Calculate the time from the start of the thread
  QString chrono = Tools::ChronoToString(this->m_NbProcessed * CLOCK_SPEED + this->m_TotalTime);

  // Append values to the Log
  this->Log.append(tr("CORE_TOTAL_TIME") + ";" + chrono);
  this->Log.append(tr("CORE_ELAPSED_TIME") + ";" + QString::number(this->m_TimeElapsed));
  this->Log.append(tr("CORE_AVERAGE_TIME") + ";" + QString::number(this->m_AverageTime));
  this->Log.append(tr("CORE_NB_PROCESSED") + ";" + QLocale().toString(this->m_NbProcessed));

  // Check if the log needs to be sent
  if (this->m_HasToSendLog == true)
    emit SendNewLog(Log);
}

/*
* @brief Slot function to stop the Mag200Core.
*
* This slot function is called to stop the Mag200Core execution.
* It sets the stop flag, which will cause the Processing loop to exit.
*
* @date 31/05/2023 
*/
void Mag200Core::StopMag200Core()
{
  m_Mutex.lock();

  // Set the flag to stop the Mag200Core processing
  m_HasToStop = true;

  m_Mutex.unlock();
}

/*
* @brief Function to stop reading log.
*
* This function sets the flag to stop sending log data.
*
* @date 31/05/2023 
*/
void Mag200Core::StopReadLog()
{
  m_Mutex.lock();

  // Set the flag to stop sending log
  m_HasToSendLog = false;

  m_Mutex.unlock();
}

/*
* @brief Function to start reading log.
*
* This function sets the flag to start sending log data.
*
* @date 31/05/2023 
*/
void Mag200Core::StartReadLog()
{
  m_Mutex.lock();

  // Set the flag to start sending log
  m_HasToSendLog = true;

  m_Mutex.unlock();
}

/*
* @brief Function to pause Mag200Core processing.
*
* This function sets the flag to pause the Mag200Core processing loop.
*
* @date 31/05/2023 
*/
void Mag200Core::PauseMag200Core()
{
  m_Mutex.lock();

  // Set the flag to pause the Mag200Core processing
  m_HasToPause = true;

  m_Mutex.unlock();
}

/*
* @brief Function to resume Mag200Core processing.
*
* This function sets the flag to resume the Mag200Core processing loop.
*
* @date 31/05/2023 
*/
void Mag200Core::UnPauseMag200Core()
{
  m_Mutex.lock();
  
  // Set the flag to resume the Mag200Core processing
  m_HasToPause = false;

  m_Mutex.unlock();
}

void Mag200Core::InvokeMoveToThread(QThread * thread)
{
  // Ensure that the Mag200Core's moveToThread is called in the correct thread
  QMetaObject::invokeMethod(this, "MoveToThread", Qt::BlockingQueuedConnection, Q_ARG(QThread*, thread));
}

/*
* @brief Function to send all status information.
*
* This function is responsible for sending all status information to connected components.
*
* @date 31/05/2023 
*/
void Mag200Core::SendAllStatus()
{

}

/*
* @brief Virtual function for processing.
*
* This function is called during each iteration of the Mag200Core processing loop.
* Subclasses can override this function to provide their specific processing logic.
*
* @date 31/05/2023 
*/
void Mag200Core::Processing()
{

}

/*
* @brief Virtual function for initialization.
*
* This function is called at the beginning of Mag200Core execution to perform any necessary initialization.
* Subclasses can override this function to provide their specific initialization logic.
*
* @return Returns 0 if initialization is successful, otherwise returns an m_State code.
*
* @date 31/05/2023 
*/
int Mag200Core::Initialize()
{
  // Reset Log Value
  this->m_NbProcessed = 1;
  this->m_TimeElapsed = 0;
  this->m_AverageTime = 0;
  this->m_TotalTime = 0;

  return 0;
}

/*
* @brief Virtual function for termination.
*
* This function is called at the end of Mag200Core execution to perform any necessary termination.
* Subclasses can override this function to provide their specific termination logic.
*
* @return Returns 0 if termination is successful, otherwise returns an m_State code.
*
* @date 31/05/2023 
*/
int Mag200Core::Terminate()
{
  return 0;
}