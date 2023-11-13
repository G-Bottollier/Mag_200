#ifndef PCM_BOARD_H
#define PCM_BOARD_H

#include <QObject>
#include <QMetaObject>
#include <QMetaMethod>

#include "Tools.h"

#include <mwstrt.h>
#include <C_DEFRT.H>
#include <mag200_str.h>

/* Systeme d'acquisition TLM */
#define LSX8              0x19F00000
#define TM_RAW            TM_RAW_CODE
#define LS55_45           0x16900000
#define SIMIRIG           0x16200000

/* Systeme d'acquisition ETH */
#define BSC               0x1C600000
#define BSS               0x1A600000
#define RTR               0x1D100000
#define CH10              DATAREC3_CODE
#define TMOIP             0x18F00000
#define ETHERNET          ETH0_CODE
#define CH10_ENCODER      CH10_ENCODER_CODE

/**
* @class Mag200Data
*
* @brief Class to handle MAG200 configuration data.
*
* @details This class provides the following functionalities:
*          - Manages configuration data related to the MAG200 system, including parameters such as CardID, ChannelID, Bitrate, etc.
*          - Allows creation of an instance of the class from a MAG200_Canal structure.
*          - Provides methods to check whether the instance uses RF or CH10.
*          - Provides methods to get the acquisition type from the acquisition code.
*          - Provides a method to generate a title from the acquisition and decommutation names.
*          - Provides setters and getters for all member variables (parameters).
*
* @date 05/06/23
*/
class Mag200Data : public QObject
{
  Q_OBJECT

  private:
    // Parameters
    int m_CardID;                        // Lumistar card number
    bool m_Enabled;                      // Indicates if the driver is active in the configuration
    bool m_IrigCode;                     // IRIG time source: 0 = Internal (PC), 1 = External
    double m_Bitrate;                    // Bitrate in kb/s of the telemetry
    int m_ChannelID;                     // Channel ID, future index in T_CxtCanal in TR (unique number, not necessarily sequential)
    int m_SubCardID;                     // Sub-card number (LS45/55)
    int m_AcquisitionCode;               // Acquisition system code
    int m_DecommutationCode;             // Decommunication code
    QString m_AcquisitionName;           // Name of the acquisition system (or NewName of the node if renamed), in UTF8 and translated
    QString m_DecommutationName;         // Name of the decom (or NewName of the node if renamed), in UTF8 and translated
    int m_PulseModulationCode;           // PCM code of the telemetry

    // IRIG Parameters
    bool m_Dummy;                        // Dummy variable for alignment
    QString m_Hexa;                      // Frame sync pattern (hexadecimal)
    QString m_Binary;                    // Frame sync pattern (binary, maximum 64 bits)
    QString m_SFID_Name;                 // Name of the SFID channel (in Latin1)
    int m_FrameSyncLength;               // Frame sync length
    int m_MinorFrameLength;              // Minor frame length (short cycle) in number of bits (including variable word lengths)
    int m_MajorFrameLength;              // Major frame length
    bool m_MajorFrameLSB_ID;             // ID LSB position in the major frame
    bool m_MajorFrameType_ID;            // Major frame type: 0 = ascending, 1 = descending
    int m_MajorFramePosInWord_ID;        // ID Position in words in the major frame
    int m_MajorFrameLastValue_ID;        // ID Last value in the major frame
    int m_MajorFrameFirstValue_ID;       // ID First value in the major frame

    // CH10 Encoder Parameters
    bool m_UseChapter10File;             // Indicates if CH10 file recording is enabled
    bool m_UseChapter10Network;          // Indicates if CH10 network transmission via UDP is enabled

    // Other Parameters
    QString m_Type;                      // Card type
    QString m_Title;                     // ACQ/DECOM name


    // Private Static Function
    static QString TypeFromCodeAcq(int code);
    static QString TitleFromNames(QString ACQ_Name, QString DECOM_Name);

  public:
    Mag200Data(QObject *parent = nullptr);
    ~Mag200Data();

    // Wrap MAG200_Canal to board
    static Mag200Data* From_Mag_200_Cfg(const MAG200_Canal *canal);

    // CUSTOM
    bool IsRF() const;
    bool IsCH10() const;

  // public slots = Add all getters to the Qt Property management (QMetaObject)
  public slots:
    /****************/
    /*    GETTER    */
    /****************/
    // Parameters
    int CardID() const;
    bool Enabled() const;
    bool IrigCode() const;
    double Bitrate() const;
    int ChannelID() const;
    int SubCardID() const;
    int AcquisitionCode() const;
    int DecommutationCode() const;
    QString AcquisitionName() const;
    QString DecommutationName() const;
    int PulseModulationCode() const;

    // IRIG Parameters
    bool Dummy() const;
    QString Hexa() const;
    QString Binary() const;
    QString SFID_Name() const;
    int FrameSyncLength() const;
    int MinorFrameLength() const;
    int MajorFrameLength() const;
    bool MajorFrameLSB_ID() const;
    bool MajorFrameType_ID() const;
    int MajorFramePosInWord_ID() const;
    int MajorFrameLastValue_ID() const;
    int MajorFrameFirstValue_ID() const;

    // CH10 Encoder Parameters
    bool UseChapter10File() const;
    bool UseChapter10Network() const;

    // Other Parameters
    QString Type() const;
    QString Title() const;

    // Formatting getter
    QString BitrateText() const;

  public:
    /****************/
    /*    SETTER    */
    /****************/
    // Parameters
    void SetCardID(int CardID);
    void SetEnabled(bool Enabled);
    void SetIrigCode(bool IrigCode);
    void SetBitrate(double Bitrate);
    void SetChannelID(int ChannelID);
    void SetSubCardID(int SubCardID);
    void SetAcquisitionCode(int AcquisitionCode);
    void SetDecommutationCode(int DecommutationCode);
    void SetAcquisitionName(QString AcquisitionName);
    void SetDecommutationName(QString DecommutationName);
    void SetPulseModulationCode(int PulseModulationCode);

    // IRIG Parameters
    void SetDummy(bool Dummy);
    void SetHexa(QString Hexa);
    void SetBinary(QString Binary);
    void SetSFID_Name(QString SFID_Name);
    void SetFrameSyncLength(int FrameSyncLength);
    void SetMinorFrameLength(int MinorFrameLength);
    void SetMajorFrameLength(int MajorFrameLength);
    void SetMajorFrameLSB_ID(bool MajorFrameLSB_ID);
    void SetMajorFrameType_ID(bool MajorFrameType_ID);
    void SetMajorFramePosInWord_ID(int MajorFramePosInWord_ID);
    void SetMajorFrameLastValue_ID(int MajorFrameLastValue_ID);
    void SetMajorFrameFirstValue_ID(int MajorFrameFirstValue_ID);

    // CH10 Encoder Parameters
    void SetUseChapter10File(bool UseChapter10File);
    void SetUseChapter10Network(bool UseChapter10Network);

    // Other Parameters
    void SetType(QString Type);
    void SetTitle(QString Title);
};

#endif // PCM_BOARD_H
