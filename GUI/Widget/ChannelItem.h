#ifndef CHANNELITEM_H
#define CHANNELITEM_H

#include <QtWidgets>

#include "PageDefinition.h"
#include "Mag200Data.h"
#include "Tools.h"
#include "Led.h"

// Order of the view

// DPS / TGS View
#define TITLE     0
#define RF        1
#define BS        2
#define FS        3
#define ID        4
#define BITRATE   5

// REC View
#define TITLE     0
#define PCM       1
#define CH10TIME  2

// We use "4" because this is the widest number with the given font
#define MAX_STR_WIDTH_TIME " 444-44:44:44.444" 

#define DEFAULT_PARENT "DefaultLayout"

class ChannelItem : public QFrame
{
  Q_OBJECT

  private:
    bool m_UseLine = false;
    bool m_UsePacketID = false;

    int m_PacketID = -1;
    int m_ChannelID = -1;

    Mag200Data *m_Data = nullptr;

    void SetupUI();
    void SetupEvent();

  protected:
    bool useAutoSizeFont = true;
    bool useDefaultResize = true;

    QMap<int, Led*> Leds;
    QMap<int, QLabel*> Texts;
    QHBoxLayout *MainLayout = nullptr;

    int m_LedUsedCount = 0;

    void changeEvent(QEvent * event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

  public:
    ChannelItem(Mag200Data *Data, QWidget *parent = nullptr);
    ~ChannelItem();

    int ChannelID();
    QString ChannelName();
    Mag200Data *Data() const;

    void UsePacketID(bool UsePacketID);
    void UseLine(bool UseLine);

    virtual void ResetLedFlags();
    void SetText(int index, QString text);

    void SetLedState(int index, Mag200Status state);
    virtual void NewStatus(int ChannelID, int Type, QVariant Value);
    
    QList<int> UsedLeds();
    virtual void AlignLeds(QList<int> LedsLayout);

  public slots:
    virtual void LedClicked();

  signals:
    void SendLedClicked(ChannelItem* item, QString ledText);
};

#endif // CHANNELITEM_H
