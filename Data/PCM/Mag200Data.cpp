/* (EVOL_MAG200_CFG) JMB 05/06/23 : Fichier de configuration global dans lca mc_mag200.cfg */

#include "stdafx.h"
#include "Mag200Data.h"

/**
* @brief Constructor for the Mag200Data class.
*
* @date 05/06/23
*/
Mag200Data::Mag200Data(QObject *parent) : QObject(parent)
{
}

/**
* @brief Destructor for the Mag200Data class.
*
* @date 05/06/23
*/
Mag200Data::~Mag200Data()
{
  this->disconnect();
}

/**
* @brief Creates an instance of Mag200Data from a MAG200_Canal structure.
*
* @details This function initializes a new instance of the Mag200Data class
*          using the provided MAG200_Canal structure. The structure's fields
*          are copied to the corresponding member variables of the new Mag200Data instance.
*
* @param canal Pointer to the MAG200_Canal structure.
*
* @return A new instance of the Mag200Data class.
*
* @date 05/06/23
*
* @par Revision History :
*       - 
*/
Mag200Data* Mag200Data::From_Mag_200_Cfg(const MAG200_Canal *canal)
{
  Mag200Data* board = new Mag200Data();

  // Parameters
  board->m_CardID                   = canal->nMainId;
  board->m_Enabled                  = (canal->enabled != 0);
  board->m_IrigCode                 = (canal->sourceIrig != 0);
  board->m_Bitrate                  = canal->freqbit * 1000;
  board->m_ChannelID                = canal->canal;
  board->m_SubCardID                = canal->nSubId;
  board->m_AcquisitionCode          = canal->codeAcq;
  board->m_DecommutationCode        = canal->codeDcm;
  board->m_AcquisitionName          = QString::fromUtf8(canal->nomAcq);
  board->m_DecommutationName        = QString::fromUtf8(canal->nomDcm);
  board->m_PulseModulationCode      = canal->pcm_code;

  // IRIG Parameters
  board->m_Dummy                    = (canal->dummy[0] != 0);
  board->m_Hexa                     = QString::fromLatin1(canal->pattern_syncCC);
  board->m_Binary                   = QString::fromLatin1(canal->sync_CC);
  board->m_SFID_Name                = QString::fromLatin1(canal->szSFID);
  board->m_FrameSyncLength          = canal->l_syncCC_b;
  board->m_MinorFrameLength         = canal->l_CC_b;
  board->m_MajorFrameLength         = canal->nbCC_CL;
  board->m_MajorFrameLSB_ID         = (canal->bit_lsb_IDCL != 0);
  board->m_MajorFrameType_ID        = (canal->type_IDCL != 0);
  board->m_MajorFramePosInWord_ID   = canal->pos_IDCL_W;
  board->m_MajorFrameLastValue_ID   = canal->n_last_IDCL;
  board->m_MajorFrameFirstValue_ID  = canal->n_first_IDCL;

  // CH10 Encoder Parameters
  board->m_UseChapter10File         = (canal->ch10_file != 0);
  board->m_UseChapter10Network      = (canal->ch10_udp != 0);

  QString Title = Mag200Data::TitleFromNames(board->AcquisitionName(), board->DecommutationName());
  QString Type = Mag200Data::TypeFromCodeAcq(board->AcquisitionCode());

  board->m_Title                    = Title;
  board->m_Type                     = Title;

  return board;
}

/**
* @brief Checks if the current instance uses CH10.
*
* @details This function checks if the current instance uses CH10
*          by checking the UseChapter10File and UseChapter10Network flags
*          along with the AcquisitionCode.
*
* @return True if the current instance uses CH10, False otherwise.
*
* @date 05/06/23
*
* @par Revision History :
*       - 
*/
bool Mag200Data::IsCH10() const
{
  bool isCh10 = false;

  bool isCh10File = this->m_UseChapter10File == true;
  bool isCh10Network = this->m_UseChapter10Network == true;
  bool isCh10Encoder = this->m_AcquisitionCode == CH10_ENCODER_CODE;

  isCh10 |= isCh10File && isCh10Encoder;
  isCh10 |= isCh10Network && isCh10Encoder;

  return isCh10;
}

/**
* @brief Checks if the current instance uses RF.
*
* @details This function checks if the acquisition code corresponds to RF usage.
*
* @return True if the current instance uses RF, False otherwise.
*
* @date 05/06/23
*
* @par Revision History :
*       -
*/
bool Mag200Data::IsRF() const
{
  switch (this->m_AcquisitionCode)
  {
    default:
      return false;
    case LSX8:
      return true;
  }
}

/**
* @brief Gets acquisition type from the acquisition code.
*
* @details This function maps the provided acquisition code to the corresponding
*          acquisition type and returns the type as a string.
*
* @param code The acquisition code.
*
* @return String representing the acquisition type.
*
* @date 05/06/23
*
* @par Revision History :
*       - (EVOL_MAG200_CFG)
*/
QString Mag200Data::TypeFromCodeAcq(int code)
{
  // Checking code
  switch (code)
  {
    //Systeme d'acquisition TLM
  case LSX8:          return QString("lsx8");
  case LS55_45:       return QString("ls55-45");
  case SIMIRIG:       return QString("simirig");
  case TM_RAW_CODE:   return QString("tmraw");

    /* Systeme d'acquisition ETH */
  case BSC:           return QString("bsc");
  case BSS:           return QString("bss");
  case RTR:           return QString("rtr");
  case CH10:          return QString("ch10");
  case TMOIP:         return QString("tmoip");
  case ETHERNET:      return QString("ethernet");
  case CH10_ENCODER:  return QString("ch10encoder");
  default:                return QString("");
  }
}

/**
* @brief Gets the title from the acquisition and decommutation names.
*
* @details This function combines the acquisition name and decommutation name
*          into a single string that serves as the title. If either name is empty,
*          the other is returned as the title. If both are empty, an empty string is returned.
*
* @param ACQ_Name The acquisition name.
* @param DECOM_Name The decommutation name.
*
* @return String representing the title.
*
* @date 05/06/23
*
* @par Revision History :
*       - (EVOL_MAG200_CFG)
*/
QString Mag200Data::TitleFromNames(QString ACQ_Name, QString DECOM_Name)
{
  if (ACQ_Name.isEmpty() && DECOM_Name.isEmpty())
    return QString();

  if (DECOM_Name.isEmpty())
    return QString(ACQ_Name);

  if (ACQ_Name.isEmpty())
    return QString(DECOM_Name);

  return QString(ACQ_Name + "\\" + DECOM_Name);
}

/****************/
/*    GETTER    */
/****************/

// Parameters
int Mag200Data::CardID()                                                   const { return this->m_CardID;                          }
bool Mag200Data::Enabled()                                                 const { return this->m_Enabled;                         }
bool Mag200Data::IrigCode()                                                const { return this->m_IrigCode;                        }
double Mag200Data::Bitrate()                                               const { return this->m_Bitrate;                         }
int Mag200Data::ChannelID()                                                const { return this->m_ChannelID;                       }
int Mag200Data::SubCardID()                                                const { return this->m_SubCardID;                       }
int Mag200Data::AcquisitionCode()                                          const { return this->m_AcquisitionCode;                 }
int Mag200Data::DecommutationCode()                                        const { return this->m_DecommutationCode;               }
QString Mag200Data::AcquisitionName()                                      const { return this->m_AcquisitionName;                 }
QString Mag200Data::DecommutationName()                                    const { return this->m_DecommutationName;               }
int Mag200Data::PulseModulationCode()                                      const { return this->m_PulseModulationCode;             }
                                                                                                                                   
// IRIG Parameters                                                                                                                 
bool Mag200Data::Dummy()                                                   const { return this->m_Dummy;                           }
QString Mag200Data::Hexa()                                                 const { return this->m_Hexa;                            }
QString Mag200Data::Binary()                                               const { return this->m_Binary;                          }
QString Mag200Data::SFID_Name()                                            const { return this->m_SFID_Name;                       }
int Mag200Data::FrameSyncLength()                                          const { return this->m_FrameSyncLength;                 }
int Mag200Data::MinorFrameLength()                                         const { return this->m_MinorFrameLength;                }
int Mag200Data::MajorFrameLength()                                         const { return this->m_MajorFrameLength;                }
bool Mag200Data::MajorFrameLSB_ID()                                        const { return this->m_MajorFrameLSB_ID;                }    
bool Mag200Data::MajorFrameType_ID()                                       const { return this->m_MajorFrameType_ID;               }
int Mag200Data::MajorFramePosInWord_ID()                                   const { return this->m_MajorFramePosInWord_ID;          }
int Mag200Data::MajorFrameLastValue_ID()                                   const { return this->m_MajorFrameLastValue_ID;          }
int Mag200Data::MajorFrameFirstValue_ID()                                  const { return this->m_MajorFrameFirstValue_ID;         }
                                                                                                                                   
// CH10 Encoder Parameters                                                                                                         
bool Mag200Data::UseChapter10File()                                        const { return this->m_UseChapter10File;                }
bool Mag200Data::UseChapter10Network()                                     const { return this->m_UseChapter10Network;             }
                                                                                                                                   
// Other Parameters                                                                                                                
QString Mag200Data::Type()                                                 const { return this->m_Type;                            }
QString Mag200Data::Title()                                                const { return this->m_Title;                           }
QString Mag200Data::BitrateText()                                          const { return Tools::BitrateToString(this->m_Bitrate); }

/****************/
/*    SETTER    */
/****************/

// Parameters                                                                                                                      
void Mag200Data::SetCardID(int CardID)                                     { m_CardID = CardID;                                    }
void Mag200Data::SetEnabled(bool Enabled)                                  { m_Enabled = Enabled;                                  }
void Mag200Data::SetIrigCode(bool IrigCode)                                { m_IrigCode = IrigCode;                                }
void Mag200Data::SetBitrate(double Bitrate)                                { m_Bitrate = Bitrate;                                  }
void Mag200Data::SetChannelID(int ChannelID)                               { m_ChannelID = ChannelID;                              }
void Mag200Data::SetSubCardID(int SubCardID)                               { m_SubCardID = SubCardID;                              }
void Mag200Data::SetAcquisitionCode(int AcquisitionCode)                   { m_AcquisitionCode = AcquisitionCode;                  }
void Mag200Data::SetDecommutationCode(int DecommutationCode)               { m_DecommutationCode = DecommutationCode;              }
void Mag200Data::SetAcquisitionName(QString AcquisitionName)               { m_AcquisitionName = AcquisitionName;                  }
void Mag200Data::SetDecommutationName(QString DecommutationName)           { m_DecommutationName = DecommutationName;              }
void Mag200Data::SetPulseModulationCode(int PulseModulationCode)           { m_PulseModulationCode = PulseModulationCode;          }
                                                                                                                                   
// IRIG Parameters                                                                                                                 
void Mag200Data::SetDummy(bool Dummy)                                      { m_Dummy = Dummy;                                      }
void Mag200Data::SetHexa(QString Hexa)                                     { m_Hexa = Hexa;                                        }
void Mag200Data::SetBinary(QString Binary)                                 { m_Binary = Binary;                                    }
void Mag200Data::SetSFID_Name(QString SFID_Name)                           { m_SFID_Name = SFID_Name;                              }
void Mag200Data::SetFrameSyncLength(int FrameSyncLength)                   { m_FrameSyncLength = FrameSyncLength;                  }
void Mag200Data::SetMinorFrameLength(int MinorFrameLength)                 { m_MinorFrameLength = MinorFrameLength;                }
void Mag200Data::SetMajorFrameLength(int MajorFrameLength)                 { m_MajorFrameLength = MajorFrameLength;                }
void Mag200Data::SetMajorFrameLSB_ID(bool MajorFrameLSB_ID)                { m_MajorFrameLSB_ID = MajorFrameLSB_ID;                }
void Mag200Data::SetMajorFrameType_ID(bool MajorFrameType_ID)              { m_MajorFrameType_ID = MajorFrameType_ID;              }
void Mag200Data::SetMajorFramePosInWord_ID(int MajorFramePosInWord_ID)     { m_MajorFramePosInWord_ID = MajorFramePosInWord_ID;    }
void Mag200Data::SetMajorFrameLastValue_ID(int MajorFrameLastValue_ID)     { m_MajorFrameLastValue_ID = MajorFrameLastValue_ID;    }
void Mag200Data::SetMajorFrameFirstValue_ID(int MajorFrameFirstValue_ID)   { m_MajorFrameFirstValue_ID = MajorFrameFirstValue_ID;  }
                                                                                                                                   
// CH10 Encoder Parameters                                                                                                         
void Mag200Data::SetUseChapter10File(bool UseChapter10File)                { m_UseChapter10File = UseChapter10File;                }
void Mag200Data::SetUseChapter10Network(bool UseChapter10Network)          { m_UseChapter10Network = UseChapter10Network;          }
                                                                                                                                   
// Other Parameters                                                                                                                
void Mag200Data::SetType(QString Type)                                     { m_Type = Type;                                        }
void Mag200Data::SetTitle(QString Title)                                   { m_Title = Title;                                      }