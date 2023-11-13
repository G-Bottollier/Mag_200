#ifndef DATEPICKER_H
#define DATEPICKER_H

#include <QFrame>

#include "Wheel.h"
#include "Status.h"
#include "Tools.h"

#define FORMAT_DMY        0
#define FORMAT_YMD        1
#define FORMAT_MDY        2
#define FORMAT_YDM        3

#define START_DATE        0
#define END_DATE          1
#define LOWER_BOUND_DATE  2
#define UPPER_BOUND_DATE  3
#define CURRENT_DATE      4

#define BLINK_TIME        560
#define BLINK_INTERVAL    80

#define FORMAT_DATE       "00/00/0000-00:00:00"

class DatePicker : public QFrame
{
  Q_OBJECT

  private:
    QTabBar *m_DateBar = nullptr;

    bool m_Blink = false;
    int m_BlinkCounter = 0;
    QTimer *m_BlinkTimer = nullptr;

    int m_DateType;
    QString m_DateFormat;
    QList<int> m_WheelOrder;

    bool m_UseRange = true;

    QMap<int, int> m_Values;
    QMap<int, Wheel*> m_Wheels;
    QMap<int, QLabel*> m_Labels;
    QMap<int, QString> m_DateSplit;
    QMap<int, QDateTime> m_DateTimes;
    
    void SetupUI();
    void SetupEvent();
    void SetupFormat();

    void UpdateDate();
    void UpdateTime();
    void UpdateText();
    void UpdateRange(int type, QDateTime &startTime, QDateTime &endTime, bool SetValue = false);

    int DatePickerType();
    QString ValueToString(int value);

  protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

  public:
    DatePicker(int DateType = FORMAT_DMY, QWidget *parent = nullptr);
    ~DatePicker();

    void SetDate(QDateTime date);
    void SetBoundRange(QDateTime lowerBound, QDateTime upperBound);
    void SetDateRange(QDateTime startTime, QDateTime endTime);

    QDateTime ToDateTime(int type);
    QString ToString(int type = -1);
    qint64 ToSecsSinceEpoch(int type);

    bool IsValid();
    void DoBlinkAnimation();
    void SetRangeMode(bool state);

  public slots:
    void NewValue(QString strValue);
    void CurrentChanged(int index);
    void Blink();
};

#endif // DATEPICKER_H
