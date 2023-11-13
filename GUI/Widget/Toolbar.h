#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QtWidgets>

#include "Led.h"
#include "Tools.h"
#include "PageDefinition.h"
#include "MessagesManager.h"
#include "SettingsManager.h"

#include "NdaSdkCore.h"

// Order of the view
#define ACQ           0
#define REC           1
#define REP           2
#define TIME          3
#define REPLAY_TIME   4
#define ELAPSED_TIME  5
#define STK           6
#define CH10_STK      7
#define CH10_ETH      8
#define HOME_MENU     9
#define PARENT_DIR    10

#define INTERN        "INT"
#define EXTERN        "EXT"
#define FLYWHEEL      "FLY"
#define DO_NOT_EXIST  "DNE"

/*
 * @brief The Toolbar class represents a custom toolbar widget.
 *
 * This class is responsible for creating and managing the toolbar's user interface.
 * The toolbar displays various LEDs and updates their states based on the provided data.
 *
 * @date 31/05/2023
 */
class Toolbar : public QFrame
{
  Q_OBJECT

  private:
    QString m_TimeType;
    int m_HasRF = false;

    void SetupUI();
    void Initialize();
    virtual void SetupEvent();

  protected:
    void resizeEvent(QResizeEvent *event);

  protected :
    QMap<int, int> Times;
    QMap<int, Led*> Leds;

    QHBoxLayout *MainLayout = nullptr;

    int LedIndex();
    int LedFontSize(int width, int height, QStringList letters);

  public:
    Toolbar(QWidget *parent = nullptr);
    ~Toolbar();

    bool IsValidStatus(int ChannelID, int Type);
    QString TimeType();
    bool HasRF();

  public slots:
    virtual void NewStatus(int ChannelID, int Type, QVariant Value);
    
  signals:
    void SendLedClicked(int Page);
};

#endif // STATUSWIDGET_H
