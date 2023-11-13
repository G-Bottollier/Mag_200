#ifndef CHAPTER6COMMAND_H
#define CHAPTER6COMMAND_H

#include <QtWidgets>
#include <QtNetwork>

#define MAX_SIZE 1024 * 1024

class Chapter6Command : public QObject
{
  Q_OBJECT

  private:
    QString command;

    static QHash<int, QString> m_Codes;
    QList<QStringList> m_Lines;

    QByteArray data;

    bool hasAcknowledge = false;

    static bool IsInitialized;
    static void InitializeCodes();
    bool IsAcknowledge();
    bool ReadData();

  public:
    Chapter6Command(QObject *parent = nullptr);
    ~Chapter6Command();

    QList<QStringList> Lines();

    void clear();
    bool SendCommand(QTcpSocket *socket);
    bool ReadAcknowledge(QTcpSocket *socket);

    bool DataOverflow();
    bool HasAcknowledge();
    bool MakeCommand(QPair<int, QStringList> command);
};

// Generated Unique identifier from qChecksum with the define name as String Value
// Allow theses identifier to do a Switch Case from a QString to Checksum !
// displayed and sorted as integer to prevent collisions (duplicates)
// 
// Contains all ASCII Commands from Chapter 6
//
// Qt Using CRC16_X_25
// Calculator :
// http://www.sunshine2k.de/coding/javascript/crc/crc_js.html

// COMMANDS
#define CMD_PLAY              6887     // PLAY
#define CMD_FILES             8460     // FILES
#define CMD_RCC_106           8703     // RCC-106
#define CMD_TIME              8955     // TIME
#define CMD_PUBLISH_FILE      10485    // PUBLISH_FILE
#define CMD_EVENT             16785    // EVENT
#define CMD_PUBLISH           20939    // PUBLISH
#define CMD_DISMOUNT          20974    // DISMOUNT
#define CMD_ASSIGN            21372    // ASSIGN
#define CMD_VERBOSE           23688    // VERBOSE
#define CMD_DATE              25873    // DATE
#define CMD_PUBLISH_TCP       26571    // PUBLISH_TCP
#define CMD_DUB               28546    // DUB
#define CMD_PUBLISH_CFG       29917    // PUBLISH_CFG
#define CMD_ERASE             29949    // ERASE
#define CMD_STOP              31660    // STOP
#define CMD_TMATS             33310    // TMATS
#define CMD_CRITICAL          34790    // CRITICAL
#define CMD_HELP              36153    // HELP
#define CMD_QUEUE             36157    // QUEUE
#define CMD_RESET             38021    // RESET
#define CMD_CONFIG            40290    // CONFIG
#define CMD_COPY              40870    // COPY
#define CMD_BBREAD            41623    // BBREAD
#define CMD_MOUNT             42214    // MOUNT
#define CMD_LOOP              42695    // LOOP
#define CMD_MEDIA             44325    // MEDIA
#define CMD_IRIG106           44493    // IRIG106
#define CMD_TCPPORTS          47059    // TCPPORTS
#define CMD_REPLAY            48887    // REPLAY
#define CMD_SETUP             49265    // SETUP
#define CMD_SANITIZE          49776    // SANITIZE
#define CMD_RECORD            50018    // RECORD
#define CMD_STATUS            50238    // STATUS
#define CMD_OUT_CRATE         50447    // OUT_CRATE
#define CMD_DRIVE             51139    // DRIVE
#define CMD_BBSECURE          51952    // BBSECURE
#define CMD_BBLIST            56439    // BBLIST
#define CMD_VOLUME            56480    // VOLUME
#define CMD_HEALTH            57458    // HEALTH
#define CMD_IRIG_106          58128    // IRIG-106
#define CMD_FIND              58317    // FIND
#define CMD_PAUSE             58993    // PAUSE
#define CMD_ETOELOOP          61037    // ETOELOOP
#define CMD_BIT               61661    // BIT
#define CMD_RESUME            61948    // RESUME
#define CMD_STREAM            62997    // STREAM

// TMATS SUB COMMANDS
#define CMD_TMATS_DELETE      7666     // TMATS DELETE
#define CMD_TMATS_CHECKSUM    12683    // TMATS CHECKSUM
#define CMD_TMATS_WRITE       24063    // TMATS WRITE
#define CMD_TMATS_GET         24768    // TMATS GET
#define CMD_TMATS_READ        29931    // TMATS READ
#define CMD_TMATS_SAVE        49697    // TMATS SAVE
#define CMD_TMATS_VERSION     55209    // TMATS VERSION

// MAGALI SUB COMMANDS
#define CMD_CONFIG_STATUS     44125    // CONFIG STATUS
#define CMD_CONFIG_MODE       41243    // CONFIG MODE 0: DPS, 1:TGS, 2:Record, 3:Replay, 4:Import, 5:Export

// MAG 200 STATES
#define MODE_DPS              0        // DPS
#define MODE_TGS              1        // TGS
#define MODE_RECORD           2        // R&R (default)
#define MODE_REPLAY           3        // R&R
#define MODE_IMPORT           4        // R&R
#define MODE_EXPORT           5        // R&R

// R&R RECORDER STATES
#define STATE_FAIL            0
#define STATE_IDLE            1
#define STATE_BIT             2
#define STATE_ERASE           3
#define STATE_DECLASSIFY      4
#define STATE_RECORD          5
#define STATE_PLAY            6
#define STATE_RECORD_N_PLAY   7
#define STATE_FIND            8
#define STATE_BUSY            9
#define STATE_ERROR           10

#endif // CHAPTER6COMMAND_H
