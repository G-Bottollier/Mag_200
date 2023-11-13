#include "stdafx.h"
#include "SharedMemoryCore.h"

/*
 * @brief The SharedMemoryCore class represents the core functionality for managing shared memory.
 *
 * This class is responsible for initializing shared memory, reading and updating status information,
 * and managing the overall state of the application.
 *
 * @date 31/05/2023
 */
SharedMemoryCore::SharedMemoryCore(QObject *parent) : Mag200Core(parent)
{
  // Register the custom type Mag200Status for signal-slot communication
  qRegisterMetaType<Mag200Status>();

  /* Systeme d'acquisition TLM */
  m_StatusACQ[LSX8]         = { STAT_B, STAT_SF, STAT_ID, STAT_T, STAT_R1, STAT_R1_S, STAT_R1_D };
  m_StatusACQ[TM_RAW]       = { STAT_B, STAT_SF, STAT_ID, STAT_T, STAT_F                        };
  m_StatusACQ[LS55_45]      = { STAT_B, STAT_SF, STAT_ID, STAT_T, STAT_F                        };
  m_StatusACQ[SIMIRIG]      = { STAT_B, STAT_SF, STAT_ID, STAT_T, STAT_F                        };

  /* Systeme d'acquisition ETH */
  m_StatusACQ[BSC]          = { STAT_B, STAT_SF, STAT_ID, STAT_T, STAT_F                        };
  m_StatusACQ[BSS]          = { STAT_B, STAT_SF, STAT_ID, STAT_T, STAT_F                        };
  m_StatusACQ[RTR]          = { STAT_B, STAT_SF, STAT_ID, STAT_T, STAT_F                        };
  m_StatusACQ[CH10]         = { STAT_CH10_STK, STAT_CH10_ETH                                    };
  m_StatusACQ[TMOIP]        = { STAT_B, STAT_SF, STAT_ID, STAT_T, STAT_F                        };
  m_StatusACQ[ETHERNET]     = { STAT_B, STAT_SF, STAT_ID, STAT_T, STAT_F                        };
  m_StatusACQ[CH10_ENCODER] = { STAT_CH10_STK, STAT_CH10_ETH                                    };
}

/*
 * @brief Destructor for the SharedMemoryCore class.
 *
 * Cleans up any allocated resources and performs necessary cleanup operations.
 *
 * @date 31/05/2023
 */
SharedMemoryCore::~SharedMemoryCore()
{

}

void SharedMemoryCore::SendAllStatus()
{
  for (int ChannelId : this->m_Status.keys())
    SendStatus(ChannelId);
}

void SharedMemoryCore::SendStatus(int ChannelID)
{
  // Check if the channel ID is valid
  if (this->m_Status.contains(ChannelID) == false)
    return;

  // Get the current status values for the given channel
  QVector<int> status = this->m_Status[ChannelID];

  // Loop over all status values
  for (int i = 0; i < status.size(); ++i)
  {
    if (status[i] == -1)
      continue;

    // Emit a signal with the current status value
    //emit SendNewStatus(ChannelID, i, status[i]);
  }
}

/*
 * @brief Perform the processing tasks for the SharedMemoryCore.
 * This method is called periodically every CLOCK_SPEED
 * 
 * This function is responsible for performing the necessary processing tasks in the SharedMemoryCore.
 * It checks for any errors from the previous iteration, reads the test area information, and reads the
 * status information from the shared memory. If an m_State occurs during any of these steps, the function
 * returns early without further processing.
 *
 * After completing the processing tasks, the function updates the last m_State state to indicate success
 * (SHARED_MEMORY_OK).
 *
 * @date 31/05/2023
 */
void SharedMemoryCore::Processing()
{
  // Check if there was an m_State in the previous iteration
  m_State = CheckError(m_State);
  if (m_State != SHARED_MEMORY_OK)
    return;

  // Read the test area information
  m_State = ReadTestArea();

  // Read the status information
  m_State = ReadStatus();
  if (m_State != SHARED_MEMORY_OK)
    return;

  // Update the last m_State state
  this->m_LastError = SHARED_MEMORY_OK;
}


/*
 * @brief Initialize the shared memory core.
 *
 * This method initializes the shared memory core by opening the shared memory and initializing member variables.
 *
 * @return An integer representing the m_State status of the initialization.
 *
 * @date 31/05/2023 
 */
int SharedMemoryCore::Initialize()
{
  // Call the base class's Initialize function
  Mag200Core::Initialize();

  int m_State = 0;

  // Open the shared memory for status
  m_State = Status_ShmOpen(FALSE);
  if (m_State != SHM_NONE)
  {
    // Emit an m_State signal if failed to initialize shared memory
    //emit SendNewError(tr("FAIL_INIT_SHARED_MEMORY"));
    return -1;
  }

  // Initialize member variables
  this->m_TestArea = "";

  return m_State;
}

/*
 * @brief Terminate the shared memory core.
 *
 * This method terminates the shared memory core by closing the shared memory.
 *
 * @return An integer representing the m_State status of the termination.
 *
 * @date 31/05/2023 
 */
int SharedMemoryCore::Terminate()
{
  // Call the base class's Terminate function
  Mag200Core::Terminate();

  int m_State = 0;

  // Close the shared memory status
  Status_ShmClose();

  return m_State;
}

/*
 * @brief Read the test area from shared memory.
 *
 * This method reads the test area information from shared memory and updates the corresponding member variable.
 *
 * @return An integer representing the m_State status of the operation.
 *
 * @date 31/05/2023 
 */
int SharedMemoryCore::ReadTestArea()
{
  // Read the test area name from the shared memory
  QString testArea = QString::fromLatin1(Get_wa_name());

  // If Work Area is disconnected
  if (testArea.isEmpty())
    testArea = tr("NO_WORK_AREA");

  // Append the test area name to the log
  this->Log.append(tr("TEST_AREA") + ";" + testArea);

  // Check if the test area name has changed
  if (testArea != m_TestArea)
  {
    // Update the test area name
    m_TestArea = testArea;

    this->m_Status.clear();

    // Emit a signal to notify the change in the test area name
    //emit SendNewTestArea(m_TestArea);
  }

  return 0;
}

/*
 * @brief Read the status information from shared memory.
 *
 * This method reads the status information from shared memory and performs necessary checks and updates.
 *
 * @return An integer representing the m_State status of the operation.
 *
 * @date 31/05/2023 
 */
int SharedMemoryCore::ReadStatus()
{
  int m_State = 0;
  int NbStatusCptr = 0;

  // Send status of channels identified by their index
  for (int i = 0; i < MAX_STATUS; i++)
  {
    int idCanal = i;
    int localId = ShmComm_GetLocalId();
    int index = Status_GetCanalIndex(localId, idCanal) - 1;

    if (index < 0)
      continue;

    // Get Status
    T_Status status;
    Status_Get(localId, idCanal, &status);

    // Check if channel status changed
    CheckStatus(idCanal, status);

    NbStatusCptr++;
  }

  // If no status read, return SHARED_MEMORY_NO_STATUS
  if (NbStatusCptr == 0)
    return SHARED_MEMORY_NO_STATUS;

  return m_State;
}

void SharedMemoryCore::ResetAllStatus()
{
  // Loop over all channels
  for (int ChannelId : this->m_Status.keys())
  {
    // Get the current status values for the given channel
    QVector<int> status = this->m_Status[ChannelId];

    // Loop over all status values
    for (int i = 0; i < status.size(); ++i)
    {
      // If the current status value is unused
      if (status[i] == -1)
        continue;

      // Reset value
      switch (i)
      {
        default:
          continue;
       
        // Status
        case STAT_B:
        case STAT_SF:
        case STAT_ID:
        case STAT_T:
        case STAT_R1:
          status[i] = Mag200Status::UNLOCK;
          break;

        // Data
        case STAT_F:
        case STAT_R1_S:
        case STAT_R1_D:
          status[i] = 0;
          break;
      }

      //emit SendNewStatus(ChannelId, i, status[i]);
    }

    this->m_Status[ChannelId] = status;
  }
}

void SharedMemoryCore::CheckStatus(int ChannelID, T_Status T_Status)
{
  // If the current status values for the given channel are not defined, initialize them
  if (this->m_Status.contains(ChannelID) == false)
    this->m_Status[ChannelID] = QVector<int>(MAX_ACQ, -1);

  // Get the current saved status values for the given channel
  QVector<int> status = this->m_Status[ChannelID];

  // Get the list of status codes that the current acquisition system can handle
  QVector<int> statusIndex = this->m_StatusACQ[T_Status.uiCodeAcq];

  // Loop over all status codes that the current acquisition system can handle
  for (int index : statusIndex)
  {
    // Get the current status value
    int value = T_Status.acq[index];

    // If the STAT_F_LOW flag is low, convert the value from bps to kbps
    if (index == STAT_F)
      if (T_Status.acq[STAT_F_LOW] == 0)
        value *= 1000;

    // If the current status value is different from the old status value, update it and emit a signal
    if (value == status[index])
      continue;

    status[index] = value;
    emit SendNewStatus(ChannelID, index, value);
  }

  // Update the current status values for the given channel
  this->m_Status[ChannelID] = status;
}

/*
 * @brief Check and handle the m_State based on the provided m_State code.
 *
 * This method checks and handles the m_State based on the provided m_State code, and updates the last m_State accordingly.
 *
 * @param m_State The m_State code to check and handle.
 * @return An integer representing the status after m_State handling.
 *
 * @date 31/05/2023 
 */
int SharedMemoryCore::CheckError(int m_State)
{
  // Append the m_State information to the log
  this->Log.append(tr("LAST_ERROR") + ";" + QString::number(m_State));

  // No m_State
  if (m_State == 0)
  {
    // Check if the last m_State is different and update it if necessary
    if (this->m_LastError != m_State)
      this->m_LastError = m_State;
    return SHARED_MEMORY_OK;
  }

  // Void loop until something to do
  if (m_State == SHARED_MEMORY_IDLE)
    return SHARED_MEMORY_OK;

  QString strError = "";
  //int checkError = SHARED_MEMORY_OK;

  bool isDisplayable = this->m_LastError != m_State;
  if (isDisplayable == true)
    this->m_LastError = m_State;

  // Handle specific m_State cases
  switch (m_State)
  {
  case SHARED_MEMORY_NO_STATUS:
    // If the m_State is not displayable, return without taking any action
    if (isDisplayable == false)
      return SHARED_MEMORY_OK;
    // Reset all status in case of SHARED_MEMORY_NO_STATUS m_State
    ResetAllStatus();
    break;
  }

  // Error processed, return OK
  return SHARED_MEMORY_OK;
}