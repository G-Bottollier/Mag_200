#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QtWidgets>

#include "Tools.h"

#define NB_RECT       101 // There is 101 rect displayed from 0 to 100 inclusive
#define NB_VALUE      100 // There is 100 values
#define MAX_RATIO     1.5

#define TIME_ABS 0
#define TIME_REL 1

class ReplayBar :public QWidget
{
  Q_OBJECT

private:

  // Value
  qint64 m_Value = 0;
  qint64 m_OldValue = -1;

  // Min and Max date
  qint64 m_StartTime = 0;
  qint64 m_EndTime = 100;

  // Seek can be moved to change replay bound
  qint64 m_LowerBound = 0;
  qint64 m_UpperBound = 100;

  int m_MarginH = 0;
  int m_MarginV = 0;

  QRectF m_TimelineRect = QRectF();
  QRectF m_LowerBoundRect = QRectF();
  QRectF m_UpperBoundRect = QRectF();
  QRectF m_AnimatedRect = QRectF();

  double m_AnimatedFactor = 1.0;
  QTimer *m_AnimatedTimer;

  bool m_IsTimelinePressed = false;
  bool m_IsLowerBoundPressed = false;
  bool m_IsUpperBoundPressed = false;

  void drawRects(QPainter &painter, int start, int end, QColor color, double widthFactor = 1.0);

  double TimelineWidth();
  double WidthPerRect();
  double TimePerRect();
  double LowerBoundIndex();
  double UpperBoundIndex();
  double IndexFromPosition(int x);

  double IndexToValue(double index);
  double ValueToIndex(double value);

protected:
  void paintEvent(QPaintEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

public:
  ReplayBar(QWidget *parent = nullptr);
  ~ReplayBar();

  void SetDate(QDateTime date);
  void SetDateRange(QDateTime startTime, QDateTime endTime);
  void SetBoundRange(QDateTime lowerBound, QDateTime upperBound);

  // Getter
  QDateTime Date();
  QDateTime LowerBoundDate();
  QDateTime UpperBoundDate();

  double StartTime(int ref = TIME_ABS);
  double EndTime(int ref = TIME_ABS);

  double LowerBound(int ref = TIME_ABS);
  double UpperBound(int ref = TIME_ABS);

  double TimeRange();
  double BoundRange();

  public slots:
    void Animate();

  signals:
    void SendNewValue(qint64);
};

#endif // WAVEFORM_H
