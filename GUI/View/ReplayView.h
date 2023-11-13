#ifndef REPLAYVIEW_H
#define REPLAYVIEW_H

#include "PageView.h"

#include "ReplayBar.h"

#define STOP      0
#define PLAY      1
#define LOOP      2
#define SEARCH    3
#define RANGE     4

class ReplayView : public PageView
{
  Q_OBJECT

  private:
    ReplayBar *replayBar = nullptr;
    QLabel *time = nullptr;

    QDateTime m_StartDate;
    QDateTime m_EndDate;
    QString m_Dateformat = "(yy) - hh:mm:ss.zzz";

    void SetupUI();
    void SetupEvent();

    void LedClicked();
    QString DateToString(QDateTime date);

  protected:
    void resizeEvent(QResizeEvent *event);
     
  public:
    ReplayView(QWidget * parent = Q_NULLPTR);
    ~ReplayView();

    QDateTime Date();

    QDateTime StartDate();
    QDateTime EndDate();

    QDateTime LowerBoundDate();
    QDateTime UpperBoundDate();

    void NewPercentage(int percentage);
    void NewPlayAcknowledge(int error);
    void NewStopAcknowledge(int error);
    void NewFilesAcknowledge(QDateTime fileStart, QDateTime fileEnd);

    void SetBoundRange(QDateTime lowerBoundDate, QDateTime upperBoundDate);
    void SetDate(QDateTime date);

  public slots:
    void NewValue(qint64 value);

    void LedSearchClicked();
    void LedRangeClicked();
};

#endif // REPLAYVIEW_H
